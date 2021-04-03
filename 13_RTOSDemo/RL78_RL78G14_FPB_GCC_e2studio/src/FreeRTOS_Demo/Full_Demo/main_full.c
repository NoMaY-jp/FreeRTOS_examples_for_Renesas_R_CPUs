/*
 * FreeRTOS V202012.00
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * NOTE 1:  This project provides two demo applications.  A simple blinky style
 * project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the comprehensive test and demo version.
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * full demo.  Generic functions, such FreeRTOS hook functions, and functions
 * required to configure the hardware, are defined in main.c.
 *
 ******************************************************************************
 *
 * main_full() creates all the demo application tasks and software timers, then
 * starts the scheduler.  The web documentation provides more details of the
 * standard demo application tasks, which provide no particular functionality,
 * but do provide a good example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Reg test" tasks - These fill both the core and floating point registers with
 * known values, then check that each register maintains its expected value for
 * the lifetime of the task.  Each task uses a different set of values.  The reg
 * test tasks execute with a very low priority, so get preempted very
 * frequently.  A register containing an unexpected value is indicative of an
 * error in the context switching mechanism.
 *
 * "Check" task - The check executes every three seconds.  It checks that all
 * the standard demo tasks, and the register check tasks, are not only still
 * executing, but are executing without reporting any errors.  The check task
 * toggles the LED every three seconds if all the standard demo tasks are
 * executing as expected, or every 500ms if a potential error is discovered in
 * any task.  And the check task writes the pass message or the fail status
 * message to the UART and/or something like a dedicated debug console (thease
 * are used in place of the LED to allow easy execution in simulator such as
 * QEMU or debugger).
 *
 * "FreeRTOS+CLI command console" -  The command console uses SCIx for its
 * input and output.  The baud rate is set to 115200.  Type "help" to see a list
 * of registered commands.  The FreeRTOS+CLI license is different to the
 * FreeRTOS license, see http://www.FreeRTOS.org/cli for license and usage
 * details.
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Standard demo application includes. */
#include "flop.h"
#include "semtest.h"
#include "dynamic.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "countsem.h"
#include "GenQTest.h"
#include "recmutex.h"
#include "death.h"
#include "partest.h"
#include "comtest2.h"
#include "serial.h"
#include "TimerDemo.h"
#include "QueueOverwrite.h"
#include "IntQueue.h"
#include "EventGroupsDemo.h"
#include "TaskNotify.h"
#include "TaskNotifyArray.h"
#include "IntSemTest.h"

/* Renesas includes. */
#include "platform.h"

/* Eval board specific definitions. */
#include "demo_main.h"
#include "demo_specific_io.h"

/* Priorities for the demo application tasks. */
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2UL )
#define mainCREATOR_TASK_PRIORITY			( tskIDLE_PRIORITY + 3UL )
#define mainFLOP_TASK_PRIORITY				( tskIDLE_PRIORITY )
#define mainUART_COMMAND_CONSOLE_STACK_SIZE	( configMINIMAL_STACK_SIZE * 3UL )
#define mainCHECK_TASK_PRIORITY				( configMAX_PRIORITIES - 1 )
#define mainQUEUE_OVERWRITE_PRIORITY		( tskIDLE_PRIORITY )

/* The priority used by the UART command console task. */
#define mainUART_COMMAND_CONSOLE_TASK_PRIORITY	( configMAX_PRIORITIES - 2 )

/* The period of the check task, in ms, converted to ticks using the
pdMS_TO_TICKS() macro.  mainNO_ERROR_CHECK_TASK_PERIOD is used if no errors have
been found, mainERROR_CHECK_TASK_PERIOD is used if an error has been found. */
#define mainNO_ERROR_CHECK_TASK_PERIOD		pdMS_TO_TICKS( 3000UL )
#define mainERROR_CHECK_TASK_PERIOD 		pdMS_TO_TICKS( 500UL )

/* Parameters that are passed into the register check tasks solely for the
purpose of ensuring parameters are passed into tasks correctly. */
#if defined(__ICCRL78__)
	#if __DATA_MODEL__ == __DATA_MODEL_FAR__
		#define mainREG_TEST_TASK_1_PARAMETER		( ( void * ) 0x12345678 )
		#define mainREG_TEST_TASK_2_PARAMETER		( ( void * ) 0x87654321 )
	#else
		#define mainREG_TEST_TASK_1_PARAMETER		( ( void * ) 0x1234 )
		#define mainREG_TEST_TASK_2_PARAMETER		( ( void * ) 0x5678 )
	#endif
#else /*  defined(__ICCRL78__) */
	#define mainREG_TEST_TASK_1_PARAMETER		( ( void * ) 0x1234 )
	#define mainREG_TEST_TASK_2_PARAMETER		( ( void * ) 0x5678 )
#endif /*  defined(__ICCRL78__) */

/* The base period used by the timer test tasks. */
#define mainTIMER_TEST_PERIOD				( 50 )

/* The size of the stack allocated to the check task (as described in the
comments at the top of this file. */
#define mainCHECK_TASK_STACK_SIZE_WORDS 100

/* Size of the stacks to allocated for the register check tasks. */
#define mainREG_TEST_STACK_SIZE_WORDS 80

/*-----------------------------------------------------------*/

/*
 * Called by demo_main() to run the full demo (as opposed to the blinky demo)
 * when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 0.
 */
void main_full( void );

/*
 * The check task, as described at the top of this file.
 */
static void prvCheckTask( void *pvParameters );

/*
 * Register check tasks as described at the top of this file.  The nature of
 * these files necessitates that they are written in an assembly file, but the
 * entry points are kept in the C file for the convenience of checking the task
 * parameter.
 */
static void prvRegTestTaskEntry1( void *pvParameters );
extern void vRegTest1Task( void );
static void prvRegTestTaskEntry2( void *pvParameters );
extern void vRegTest2Task( void );
void vRegTestError( void );

/*
 * Tick hook used by the full demo, which includes code that interacts with
 * some of the tests.
 */
void vFullDemoTickHook( void );

/*
 * A high priority task that does nothing other than execute at a pseudo random
 * time to ensure the other test tasks don't just execute in a repeating
 * pattern.
 */
static void prvPseudoRandomiser( void *pvParameters );

/*
 * Register commands that can be used with FreeRTOS+CLI.  The commands are
 * defined in CLI-Commands.c and File-Related-CLI-Command.c respectively.
 */
extern void vRegisterSampleCLICommands( void );

/*
 * The task that manages the FreeRTOS+CLI input and output.
 */
extern void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );

/*-----------------------------------------------------------*/

/* The following two variables are used to communicate the status of the
register check tasks to the check task.  If the variables keep incrementing,
then the register check tasks have not discovered any errors.  If a variable
stops incrementing, then an error has been found. */
volatile uint16_t usRegTest1LoopCounter = 0U, usRegTest2LoopCounter = 0U;

/*-----------------------------------------------------------*/

void main_full( void )
{
	/* Start all the other standard demo/test tasks.  They have no particular
	functionality, but do demonstrate how to use the FreeRTOS API and test the
	kernel port. */
#if 0 /* This demo is not yet implemented. */
	vStartInterruptQueueTasks();
#endif
	vStartDynamicPriorityTasks();
	vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
	vCreateBlockTimeTasks();
#if !defined( EWRL78_16K_KICKSTART_EDITION )
	vStartCountingSemaphoreTasks();
	vStartGenericQueueTasks( tskIDLE_PRIORITY );
	vStartRecursiveMutexTasks();
	vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
#if 0 /* This demo is too heavy for RL78. */
	vStartMathTasks( mainFLOP_TASK_PRIORITY );
#endif
	vStartTimerDemoTask( mainTIMER_TEST_PERIOD );
	vStartQueueOverwriteTask( mainQUEUE_OVERWRITE_PRIORITY );
	vStartEventGroupTasks();
	vStartTaskNotifyTask();
	vStartTaskNotifyArrayTask();
	vStartInterruptSemaphoreTasks();
#endif /* !defined( EWRL78_16K_KICKSTART_EDITION ) */

	/* Create the register check tasks, as described at the top of this	file.
	Use xTaskCreateStatic() to create a task using only statically allocated
	memory. */
	xTaskCreate( prvRegTestTaskEntry1, 			/* The function that implements the task. */
				 "Reg1", 						/* The name of the task. */
				 mainREG_TEST_STACK_SIZE_WORDS, /* Size of stack to allocate for the task - in words not bytes!. */
				 mainREG_TEST_TASK_1_PARAMETER, /* Parameter passed into the task. */
				 tskIDLE_PRIORITY, 				/* Priority of the task. */
				 NULL );						/* Can be used to pass out a handle to the created task. */
	xTaskCreate( prvRegTestTaskEntry2, "Reg2", mainREG_TEST_STACK_SIZE_WORDS, mainREG_TEST_TASK_2_PARAMETER, tskIDLE_PRIORITY, NULL );

	/* Create the task that just adds a little random behaviour. */
	xTaskCreate( prvPseudoRandomiser, "Rnd", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, NULL );

	/* Start the tasks that implements the command console on the UART, as
	described above. */
	vUARTCommandConsoleStart( mainUART_COMMAND_CONSOLE_STACK_SIZE, mainUART_COMMAND_CONSOLE_TASK_PRIORITY );

	/* Register the standard CLI commands. */
	vRegisterSampleCLICommands();

	/* Create the task that performs the 'check' functionality,	as described at
	the top of this file. */
	xTaskCreate( prvCheckTask, "Check", mainCHECK_TASK_STACK_SIZE_WORDS, NULL, mainCHECK_TASK_PRIORITY, NULL );

	/* The set of tasks created by the following function call have to be
	created last as they keep account of the number of tasks they expect to see
	running. */
	vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the Idle and/or
	timer tasks to be created.  See the memory management section on the
	FreeRTOS web site for more details on the FreeRTOS heap
	http://www.freertos.org/a00111.html. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvCheckTask( void *pvParameters )
{
TickType_t xDelayPeriod = mainNO_ERROR_CHECK_TASK_PERIOD;
TickType_t xLastExecutionTime;
uint16_t usLastRegTest1Value = 0, usLastRegTest2Value = 0;
char * const pcPassMessage = ".";
char * pcStatusMessage = pcPassMessage;

	/* Just to stop compiler warnings. */
	( void ) pvParameters;

	/* Initialise xLastExecutionTime so the first call to vTaskDelayUntil()
	works correctly. */
	xLastExecutionTime = xTaskGetTickCount();

	/* Cycle for ever, delaying then checking all the other tasks are still
	operating without error.  The onboard LED is toggled on each iteration.
	If an error is detected then the delay period is decreased from
	mainNO_ERROR_CHECK_TASK_PERIOD to mainERROR_CHECK_TASK_PERIOD.  This has the
	effect of increasing the rate at which the onboard LED toggles, and in so
	doing gives visual feedback of the system status. */
	for( ;; )
	{
		/* Delay until it is time to execute again. */
		vTaskDelayUntil( &xLastExecutionTime, xDelayPeriod );

		/* Check all the demo tasks (other than the flash tasks) to ensure
		that they are all still running, and that none have detected an error. */
#if 0 /* This demo is not yet implemented. */
		if( xAreIntQueueTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Queue interrupt demo/tests.\r\n";
		}
#endif

#if 0 /* This demo is too heavy for RL78. */
		if( xAreMathsTaskStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Flop demo/tests.\r\n";
		}
#endif

		if( xAreDynamicPriorityTasksStillRunning() == pdFALSE )
		{
			pcStatusMessage = "ERROR: Dynamic priority demo/tests.\r\n";
		}

		if( xAreBlockingQueuesStillRunning() == pdFALSE )
		{
			pcStatusMessage = "ERROR: Blocking queue demo/tests.\r\n";
		}

		if( xAreBlockTimeTestTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Block time demo/tests.\r\n";
		}

#if !defined( EWRL78_16K_KICKSTART_EDITION )

		if ( xAreGenericQueueTasksStillRunning() == pdFALSE )
		{
			pcStatusMessage = "ERROR: Generic queue demo/tests.\r\n";
		}

		if ( xAreRecursiveMutexTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Recursive mutex demo/tests.\r\n";
		}

		if( xIsCreateTaskStillRunning() == pdFALSE )
		{
			pcStatusMessage = "ERROR: Suicide task demo/tests.\r\n";
		}

		if( xAreSemaphoreTasksStillRunning() == pdFALSE )
		{
			pcStatusMessage = "ERROR: Semaphore demo/tests.\r\n";
		}

		if( xAreTimerDemoTasksStillRunning( ( TickType_t ) xDelayPeriod ) == pdFAIL )
		{
			pcStatusMessage = "ERROR: Timer demo/tests.\r\n";
		}

		if( xAreCountingSemaphoreTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Counting semaphore demo/tests.\r\n";
		}

		if( xIsQueueOverwriteTaskStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Queue overwrite demo/tests.\r\n";
		}

		if( xAreEventGroupTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Event group demo/tests.\r\n";
		}

		if( xAreTaskNotificationTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Task notification demo/tests.\r\n";
		}

		if( xAreTaskNotificationArrayTasksStillRunning() == pdFAIL )
		{
			pcStatusMessage = "ERROR: Task notification array demo/tests.\r\n";
		}

		if( xAreInterruptSemaphoreTasksStillRunning() == pdFALSE )
		{
			pcStatusMessage = "ERROR: Interrupt semaphore demo/tests.\r\n";
		}

#endif /* !defined( EWRL78_16K_KICKSTART_EDITION ) */

		/* Check that the register test 1 task is still running. */
		if( usLastRegTest1Value == usRegTest1LoopCounter )
		{
			pcStatusMessage = "ERROR: Register test 1.\r\n";
		}
		usLastRegTest1Value = usRegTest1LoopCounter;

		/* Check that the register test 2 task is still running. */
		if( usLastRegTest2Value == usRegTest2LoopCounter )
		{
			pcStatusMessage = "ERROR: Register test 2.\r\n";
		}
		usLastRegTest2Value = usRegTest2LoopCounter;

		/* Write the status message to the UART and toggle the LED to show the
		system status if the UART is not connected. */
		if( pcStatusMessage == pcPassMessage )
		{
			/* Write the pass message to a dedicated debug console. */
			vSendString( pcPassMessage );
		}
		else
		{
			if( xDelayPeriod == mainNO_ERROR_CHECK_TASK_PERIOD )
			{
				/* Write the fail status message to a dedicated debug console. */
				vSendString( "\r\n" );
				vSendString( pcStatusMessage );
			}
		}
		vToggleLED();

		/* If an error has been found then increase the LED toggle rate by
		increasing the cycle frequency. */
		if( pcStatusMessage != pcPassMessage )
		{
			/* An error has been detected in one of the tasks - flash the LED
			at a higher frequency to give visible feedback that something has
			gone wrong (it might just be that the loop back connector required
			by the comtest tasks has not been fitted). */
			xDelayPeriod = mainERROR_CHECK_TASK_PERIOD;
		}
	}
}
/*-----------------------------------------------------------*/

static void prvPseudoRandomiser( void *pvParameters )
{
const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL, ulMinDelay = pdMS_TO_TICKS( 35 );
volatile uint32_t ulNextRand = ( uint32_t ) (size_t) &pvParameters, ulValue;

	/* This task does nothing other than ensure there is a little bit of
	disruption in the scheduling pattern of the other tasks.  Normally this is
	done by generating interrupts at pseudo random times. */
	for( ;; )
	{
		ulNextRand = ( ulMultiplier * ulNextRand ) + ulIncrement;
		ulValue = ( ulNextRand >> 16UL ) & 0xffUL;

		if( ulValue < ulMinDelay )
		{
			ulValue = ulMinDelay;
		}

		vTaskDelay( ulValue );

		while( ulValue > 0 )
		{
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();
			nop();

			ulValue--;
		}
	}
}
/*-----------------------------------------------------------*/

void vFullDemoTickHook( void )
{
	/* Called from vApplicationTickHook() when the project is configured to
	build the full test/demo applications. */

#if !defined( EWRL78_16K_KICKSTART_EDITION )

	/* The full demo includes a software timer demo/test that requires
	prodding periodically from the tick interrupt. */
	vTimerPeriodicISRTests();

	/* Call the periodic queue overwrite from ISR demo. */
	vQueueOverwritePeriodicISRDemo();

	/* Call the periodic event group from ISR demo. */
	vPeriodicEventGroupsProcessing();

	/* Use task notifications from an interrupt. */
	xNotifyTaskFromISR();
	xNotifyArrayTaskFromISR();

	/* Use mutexes from interrupts. */
	vInterruptSemaphorePeriodicTest();

#endif /* !defined( EWRL78_16K_KICKSTART_EDITION ) */
}
/*-----------------------------------------------------------*/

static void prvRegTestTaskEntry1( void *pvParameters )
{
	/* Although the regtest task is written in assembler, its entry point is
	written in C for convenience of checking the task parameter is being passed
	in correctly. */
	if( pvParameters == mainREG_TEST_TASK_1_PARAMETER )
	{
		/* Start the part of the test that is written in assembler. */
		vRegTest1Task();
	}

	/* The following line will only execute if the task parameter is found to
	be incorrect.  The check task will detect that the regtest loop counter is
	not being incremented and flag an error. */
	vTaskDelete( NULL );
}
/*-----------------------------------------------------------*/

static void prvRegTestTaskEntry2( void *pvParameters )
{
	/* Although the regtest task is written in assembler, its entry point is
	written in C for convenience of checking the task parameter is being passed
	in correctly. */
	if( pvParameters == mainREG_TEST_TASK_2_PARAMETER )
	{
		/* Start the part of the test that is written in assembler. */
		vRegTest2Task();
	}

	/* The following line will only execute if the task parameter is found to
	be incorrect.  The check task will detect that the regtest loop counter is
	not being incremented and flag an error. */
	vTaskDelete( NULL );
}
/*-----------------------------------------------------------*/

void vRegTestError( void )
{
	/* Called by both reg test tasks if an error is found.  There is no way out
	of this function so the loop counter of the calling task will stop
	incrementing, which will result in the check timer signaling an error. */
	for( ;; );
}
/*-----------------------------------------------------------*/

