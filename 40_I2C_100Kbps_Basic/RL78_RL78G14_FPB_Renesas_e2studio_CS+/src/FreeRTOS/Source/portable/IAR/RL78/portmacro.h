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

#ifndef PORTMACRO_H
#define PORTMACRO_H

/* Hardware specifics. */
#include <intrinsics.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

#if __DATA_MODEL__ == __DATA_MODEL_FAR__ && __CODE_MODEL__ == __CODE_MODEL_NEAR__
	#warning This port has not been tested with your selected memory model combination. If a far data model is required it is recommended to also use a far code model.
#endif

#if __DATA_MODEL__ == __DATA_MODEL_NEAR__ && __CODE_MODEL__ == __CODE_MODEL_FAR__
	#warning This port has not been tested with your selected memory model combination. If a far code model is required it is recommended to also use a far data model.
#endif

/* Type definitions. */

#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  uint16_t
#define portBASE_TYPE   short

typedef portSTACK_TYPE StackType_t;
typedef short BaseType_t;
typedef unsigned short UBaseType_t;


#if __DATA_MODEL__ == __DATA_MODEL_FAR__
	#define portPOINTER_SIZE_TYPE uint32_t
#else
	#define portPOINTER_SIZE_TYPE uint16_t
#endif


#if ( configUSE_16_BIT_TICKS == 1 )
	typedef unsigned int TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

/* Interrupt control macros. */

/* These macros should not be called directly, but through the
 * taskENTER_CRITICAL() and taskEXIT_CRITICAL() macros.  A check is performed
 * if configASSERT() is defined to ensure the ISP value was found to be 2
 * when an ISR safe FreeRTOS API function was executed.  ISR safe FreeRTOS API
 * functions are those that end in FromISR.  FreeRTOS maintains a separate
 * interrupt API to ensure API function and interrupt entry is as fast and as
 * simple as possible. */

void vPortRaiseISP( void );
#define portDISABLE_INTERRUPTS()  vPortRaiseISP()

void vPortResetISP( void );
#define portENABLE_INTERRUPTS()   vPortResetISP()

#ifdef configASSERT
	void vPortValidateInterruptPriority( void );
	#define portASSERT_IF_INTERRUPT_PRIORITY_INVALID()  vPortValidateInterruptPriority()
#endif
/*-----------------------------------------------------------*/

/* Critical section control macros. */
#define portNO_CRITICAL_SECTION_NESTING		( ( uint16_t ) 0 )

void vPortEnterCritical( void );
#define portENTER_CRITICAL()  vPortEnterCritical()

void vPortExitCritical( void );
#define portEXIT_CRITICAL()   vPortExitCritical()
/*-----------------------------------------------------------*/

/* Task utilities. */
#define portYIELD()	__asm( "BRK" )
#define portYIELD_FROM_ISR( xHigherPriorityTaskWoken ) if( xHigherPriorityTaskWoken ) vTaskSwitchContext()
#define portNOP()	__asm( "NOP" )
/*-----------------------------------------------------------*/

/* Hardwware specifics. */
#define portBYTE_ALIGNMENT	2
#define portSTACK_GROWTH	( -1 )
#define portTICK_PERIOD_MS	( ( TickType_t ) 1000 / configTICK_RATE_HZ )
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )
/*-----------------------------------------------------------*/

/* Prevent warnings of undefined behaviour: the order of volatile accesses is
 * undefined - all warnings have been manually checked and are not an issue, and
 * the warnings cannot be prevent by code changes without undesirable effects. */
#pragma diag_suppress=Pa082

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

