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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#if defined(__CCRL__)
	#include "iodefine.h"
#elif defined(__GNUC__) && !defined(__ASSEMBLER__)
	#include "iodefine.h"
	#include "iodefine_ext.h"
#elif defined(__ICCRL78__) ||  defined(__IASMRL78__)
	#include "ior5f104ml.h"
	#include "ior5f104ml_ext.h"
#endif

#define configUSE_PREEMPTION			1
#define configUSE_TIME_SLICING			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
#define configTICK_RATE_HZ				(( TickType_t ) 100)
#define configMINIMAL_STACK_SIZE		(( unsigned short ) 0)
#define configTOTAL_HEAP_SIZE			( ( size_t ) 0 )
#define configMAX_TASK_NAME_LEN			(12)
#define configUSE_TRACE_FACILITY		0
#define configUSE_16_BIT_TICKS			1
#define configIDLE_SHOULD_YIELD			0
#define configUSE_CO_ROUTINES 			0
#define configUSE_MUTEXES				1
#define configGENERATE_RUN_TIME_STATS	0
#define configCHECK_FOR_STACK_OVERFLOW	0
#define configASSERT_DEFINED			0
#define configUSE_RECURSIVE_MUTEXES		0
#define configQUEUE_REGISTRY_SIZE		0
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_QUEUE_SETS			0
#define configUSE_COUNTING_SEMAPHORES	0
#define configMAX_PRIORITIES			(4)
#define configMAX_CO_ROUTINE_PRIORITIES (2)
#define configUSE_TASK_NOTIFICATIONS     1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   1
#define configRECORD_STACK_HIGH_ADDRESS  0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0

/* Dynamic allocation and static allocation. */
#define configSUPPORT_DYNAMIC_ALLOCATION        0
#define configSUPPORT_STATIC_ALLOCATION         1

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	0

/* Software timer definitions. */
#define configUSE_TIMERS				0
#define configTIMER_TASK_PRIORITY		(3)
#define configTIMER_QUEUE_LENGTH		5
#define configTIMER_TASK_STACK_DEPTH	0

/* The interrupt priority used by the kernel itself for the tick interrupt and
the yield interrupt (in case of Renesas RL78, the BRK software interrupt).  This
would normally be the lowest priority.  In case of Renesas RL78 port, the value
is limited to the lowest priority '3' only. */
#define configKERNEL_INTERRUPT_PRIORITY         3

/* The highest interrupt priority from which interrupt safe FreeRTOS API calls
can be made.  Interrupts that use a priority above this (in case of Renesas RL78,
lower numeric values) will not be effected by anything the kernel is doing.  In
case of Renesas RL78 port, the value is limited to the lowest priority '3' only.
DO NOT CALL INTERRUPT SAFE FREERTOS API FROM ANY INTERRUPT THAT USE A PRIORITY
ABOVE THIS! (higher priorities are lower numeric values.) */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    3

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet			0
#define INCLUDE_uxTaskPriorityGet			0
#define INCLUDE_vTaskDelete					0
#define INCLUDE_vTaskCleanUpResources		0
#define INCLUDE_vTaskSuspend				1
#define INCLUDE_xTaskDelayUntil				0
#define INCLUDE_vTaskDelay					1
#define INCLUDE_uxTaskGetStackHighWaterMark	0
#define INCLUDE_xTaskGetSchedulerState		0
#define INCLUDE_eTaskGetState				0
#define INCLUDE_xTaskGetCurrentTaskHandle	1
#define INCLUDE_xTimerPendFunctionCall		0

#if (configUSE_16_BIT_TICKS == 1)
#define pdMS_TO_TICKS( xTimeInMs ) ( ( TickType_t ) ( ( ( uint32_t ) ( xTimeInMs ) * ( uint32_t ) configTICK_RATE_HZ ) / ( uint32_t ) 1000 ) )
#endif

#if defined(__CCRL__) || (defined(__GNUC__) && !defined(__ASSEMBLER__)) || defined(__ICCRL78__)

void vApplicationSetupTimerInterrupt( void );
#define configSETUP_TICK_INTERRUPT() vApplicationSetupTimerInterrupt()

#if (configASSERT_DEFINED == 1)
void vAssertCalled( void );
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled()
#endif

/* Run time stats gathering definitions. */
unsigned long ulGetRunTimeCounterValue( void );
void vConfigureTimerForRunTimeStats( void );
#define configGENERATE_RUN_TIME_STATS    0
//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    vConfigureTimerForRunTimeStats()
//#define portGET_RUN_TIME_COUNTER_VALUE()            ulGetRunTimeCounterValue()

#endif

#define NDEBUG  /* Defined: Release build, Not defined: Debug build */

#if !defined(NDEBUG)

#if defined(configSUPPORT_DYNAMIC_ALLOCATION)
#undef configUSE_MALLOC_FAILED_HOOK
#endif
#undef configCHECK_FOR_STACK_OVERFLOW
#undef configASSERT_DEFINED

#if defined(configSUPPORT_DYNAMIC_ALLOCATION)
#define configUSE_MALLOC_FAILED_HOOK	1
#endif
#define configCHECK_FOR_STACK_OVERFLOW	2
#define configASSERT_DEFINED			1

#if defined(__CCRL__) || (defined(__GNUC__) && !defined(__ASSEMBLER__)) || defined(__ICCRL78__)
#if !defined(configASSERT)
void vAssertCalled( void );
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled()
#endif
#endif

#endif /*  !defined(NDEBUG) */

#endif /* FREERTOS_CONFIG_H */
