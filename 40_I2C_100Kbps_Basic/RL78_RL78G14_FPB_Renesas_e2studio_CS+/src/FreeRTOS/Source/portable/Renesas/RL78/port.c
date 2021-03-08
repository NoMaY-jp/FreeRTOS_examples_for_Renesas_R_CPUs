/*
 * FreeRTOS Kernel V10.4.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Miscellaneous checks for this port. */
#if ( defined( configKERNEL_INTERRUPT_PRIORITY ) && !defined( configMAX_SYSCALL_INTERRUPT_PRIORITY ) ) || \
    ( !defined( configKERNEL_INTERRUPT_PRIORITY ) && defined( configMAX_SYSCALL_INTERRUPT_PRIORITY ) ) || \
    ( defined( configKERNEL_INTERRUPT_PRIORITY ) && ( configKERNEL_INTERRUPT_PRIORITY != 3 ) ) || \
    ( defined( configMAX_SYSCALL_INTERRUPT_PRIORITY ) && ( configMAX_SYSCALL_INTERRUPT_PRIORITY != 3 ) )
    #error Invalid configKERNEL_INTERRUPT_PRIORITY and/or configMAX_SYSCALL_INTERRUPT_PRIORITY setting - both must be set to 3, or left undefined.
#endif

/* The critical nesting value is initialised to a non zero value to ensure
interrupts don't accidentally become enabled before the scheduler is started. */
#define portINITIAL_CRITICAL_NESTING  ( ( uint16_t ) 10 )

/* Initial PSW value allocated to a newly created task.
 *   11000110
 *   ||||||||-------------- Carry Flag cleared
 *   |||||||--------------- In-service priority 0 Flags set to low priority
 *   |||||----------------- In-service priority 1 Flags set to low priority
 *   ||||------------------ Register bank Select 0 Flag cleared
 *   |||------------------- Auxiliary Carry Flag cleared
 *   ||-------------------- Register bank Select 1 Flag cleared
 *   |--------------------- Zero Flag set
 *   ---------------------- Global Interrupt Flag set (enabled)
 */
#define portPSW		  ( 0xc6UL )

/* PSW's ISP bits values related to SYSCALL interrupts. */
#define portPSW_ISP_SYSCALL_INTERRUPT_ENABLE     ( 3 << 1 )
#define portPSW_ISP_SYSCALL_INTERRUPT_DISABLE    ( 2 << 1 )
#define portPSW_ISP_SYSCALL_INTERRUPT_EXECUTING  ( 2 << 1 )

/* Macros to get/set PSW's ISP bits. (Operator '&' or '|' may change Zero Flag.) */
#define portPSW_REG           ( * ( /* volatile is unnecessary in the use case here */ uint8_t * ) 0xfffa )
#define portGET_PSW_ISP()     ( portPSW_REG & 0x06 /* 0b00000110 */ )
#define portSET_PSW_ISP(val)  ( portPSW_REG = ( portPSW_REG & 0xF9 /* 0b11111001 */ ) | ( val ) )

/* The address of the pxCurrentTCB variable, but don't know or need to know its
type. */
typedef void TCB_t;
extern volatile TCB_t * volatile pxCurrentTCB;

/* Each task maintains a count of the critical section nesting depth.  Each time
a critical section is entered the count is incremented.  Each time a critical
section is exited the count is decremented - with interrupts only being
re-enabled if the count is zero.

usCriticalNesting will get set to zero when the scheduler starts, but must
not be initialised to zero as that could cause problems during the startup
sequence. */
volatile uint16_t usCriticalNesting = portINITIAL_CRITICAL_NESTING;

/* Each ISR which uses interrupt dedicated stack maintains a count of the interrupt
nesting depth.  Each time an ISR is entered the count is incremented.  Each time
an ISR is exited the count is decremented.  The stack is switched to interrupt stack
from task stacks when the count changes from zero to one.  The stack is switched
back to task stacks from interrupt stack when the count changes from one to zero.
The count is held in the ucInterruptStackNesting variable.  The stack pointer value
of interrupted task stack is held in the pxInterruptedTaskStack variable.  The value
is saved to the variable when the stack is switched to interrupt stack from task
stacks and restored from the variable when the stack is switched back to task stacks
from interrupt stack. */
volatile uint8_t ucInterruptStackNesting = 0;
volatile StackType_t * pxInterruptedTaskStack = NULL;

/*-----------------------------------------------------------*/

/*
 * Sets up the periodic ISR used for the RTOS tick using the interval timer.
 * The application writer can define configSETUP_TICK_INTERRUPT() (in
 * FreeRTOSConfig.h) such that their own tick interrupt configuration is used
 * in place of prvSetupTimerInterrupt().
 */
#ifndef configSETUP_TICK_INTERRUPT
	/* The user has not provided their own tick interrupt configuration so use
	the definition in this file (which uses the interval timer). */
	static void prvSetupTimerInterrupt( void );
#endif

/*
 * Defined in portasm.asm, this function starts the scheduler by loading the
 * context of the first task to run.
 */
extern void vPortStartFirstTask( void );

/*
 * Used to catch tasks that attempt to return from their implementing function.
 */
static void prvTaskExitError( void );

/*-----------------------------------------------------------*/

/*
 * Initialise the stack of a task to look exactly as if a call to
 * portSAVE_CONTEXT had been called.
 *
 * See the header file portable.h.
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
uint32_t *pulLocal;

	/* With large code sizeof( TaskFunction_t ) == 4.  With small code
	sizeof( TaskFunction_t ) == 2. */

	/* The return address, leaving space for the first two bytes of	the
	32-bit value.  See the comments above the prvTaskExitError() prototype
	at the top of this file. */
	pxTopOfStack--;
	pulLocal = ( uint32_t * ) pxTopOfStack;
	*pulLocal = ( uint32_t ) prvTaskExitError;
	pxTopOfStack--;

	/* Task function.  Again as it is written as a 32-bit value a space is
	left on the stack for the second two bytes. */
	pxTopOfStack--;

	/* Task function start address combined with the PSW. */
	pulLocal = ( uint32_t * ) pxTopOfStack;
#ifdef __RL78_SMALL__
	/* Cast from 16bits pointer to 32bits value is potentially not safe. */
	*pulLocal = ( ( ( uint32_t ) ( uint16_t ) pxCode ) | ( portPSW << 24UL ) );
#else /* __RL78_MEDIUM__ */
	/* Cast from 32bits (actually 24bits) pointer to 32bits value is safe. */
	*pulLocal = ( ( ( uint32_t ) pxCode ) | ( portPSW << 24UL ) );
#endif
	pxTopOfStack--;

	/* The parameter is passed in AX. */
	*pxTopOfStack = ( StackType_t ) pvParameters;
	pxTopOfStack--;

	/* The initial value for the general purpose register BC, DE and HL. */
	*pxTopOfStack = ( StackType_t ) 0xBCBC;
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0xDEDE;
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x2222;
	pxTopOfStack--;

	/* ES and CS registers. */
	*pxTopOfStack = ( StackType_t ) 0x000F;
	pxTopOfStack--;

	/* Finally the critical section nesting count is set to zero when the task
	first starts. */
	*pxTopOfStack = ( StackType_t ) portNO_CRITICAL_SECTION_NESTING;

	/* Return a pointer to the top of the stack that has been generated so it
	can	be stored in the task control block for the task. */
	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

static void prvTaskExitError( void )
{
	/* A function that implements a task must not exit or attempt to return to
	its caller as there is nothing to return to.  If a task wants to exit it
	should instead call vTaskDelete( NULL ).

	Artificially force an assert() to be triggered if configASSERT() is
	defined, then stop here so application writers can catch the error. */
	configASSERT( usCriticalNesting == ~0U );
	portDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
	/* Setup the hardware to generate the tick.  Interrupts are disabled when
	this function is called. */
#ifdef configSETUP_TICK_INTERRUPT
	configSETUP_TICK_INTERRUPT();
#else
	prvSetupTimerInterrupt();
#endif

	/* Restore the context of the first task that is going to run. */
	vPortStartFirstTask();

	return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the RL78 port will get stopped. */
}
/*-----------------------------------------------------------*/

#ifndef configSETUP_TICK_INTERRUPT
static void prvSetupTimerInterrupt( void )
{
const uint16_t usClockHz = 15000UL; /* Internal clock. */
const uint16_t usCompareMatch = ( usClockHz / configTICK_RATE_HZ ) - 1UL;

	/* Use the internal 15K clock. */
	OSMC = ( uint8_t ) 0x16;

	#if INTIT == 0x38
	{
		/* Supply the interval timer clock. */
		RTCEN = ( uint8_t ) 1U;

		/* Disable INTIT interrupt. */
		ITMK = ( uint8_t ) 1;

		/* Disable ITMC operation. */
		ITMC = ( uint8_t ) 0x0000;

		/* Clear INIT interrupt. */
		ITIF = ( uint8_t ) 0;

		/* Set interval and enable interrupt operation. */
		ITMC = usCompareMatch | 0x8000U;

		/* Enable INTIT interrupt. */
		ITMK = ( uint8_t ) 0;
	}
	#endif

	#if INTIT == 0x3C
	{
		/* Supply the interval timer clock. */
		TMKAEN = ( uint8_t ) 1U;

		/* Disable INTIT interrupt. */
		TMKAMK = ( uint8_t ) 1;

		/* Disable ITMC operation. */
		ITMC = ( uint8_t ) 0x0000;

		/* Clear INIT interrupt. */
		TMKAIF = ( uint8_t ) 0;

		/* Set interval and enable interrupt operation. */
		ITMC = usCompareMatch | 0x8000U;

		/* Enable INTIT interrupt. */
		TMKAMK = ( uint8_t ) 0;
	}
	#endif
}
#endif /* ifndef configSETUP_TICK_INTERRUPT */
/*-----------------------------------------------------------*/

#define portRESET_ISP() \
	do \
	{ \
		portSET_PSW_ISP( portPSW_ISP_SYSCALL_INTERRUPT_ENABLE ); \
	} while(0)
/*-----------------------------------------------------------*/

#if( configASSERT_DEFINED == 1 )
#define portRAISE_ISP() \
	do \
	{ \
		if( portGET_PSW_ISP() == portPSW_ISP_SYSCALL_INTERRUPT_ENABLE) \
		{ \
			portSET_PSW_ISP( portPSW_ISP_SYSCALL_INTERRUPT_DISABLE ); \
		} \
	} while(0)
#else
#define portRAISE_ISP() \
	do \
	{ \
		portSET_PSW_ISP( portPSW_ISP_SYSCALL_INTERRUPT_DISABLE ); \
	} while(0)
#endif
/*-----------------------------------------------------------*/

void vPortResetISP( void )
{
	portRESET_ISP();
}
/*-----------------------------------------------------------*/

void vPortRaiseISP( void )
{
	portRAISE_ISP();
}
/*-----------------------------------------------------------*/

#if( configASSERT_DEFINED == 1 )
void vPortValidateInterruptPriority( void )
{
	configASSERT( portGET_PSW_ISP() == portPSW_ISP_SYSCALL_INTERRUPT_EXECUTING );
}
#endif
/*-----------------------------------------------------------*/

void vPortEnterCritical( void )
{
	portRAISE_ISP();

	/* Now interrupts are disabled ulCriticalNesting can be accessed
	 * directly.  Increment ulCriticalNesting to keep a count of how many
	 * times portENTER_CRITICAL() has been called. */
	usCriticalNesting++;
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void )
{
	if( usCriticalNesting > portNO_CRITICAL_SECTION_NESTING )
	{
		/* Decrement the nesting count as we are leaving a critical section. */
		usCriticalNesting--;

		/* If the nesting level has reached zero then interrupts should be
		 * re-enabled. */
		if( usCriticalNesting == portNO_CRITICAL_SECTION_NESTING )
		{
			portRESET_ISP();
		}
	}
}
/*-----------------------------------------------------------*/

