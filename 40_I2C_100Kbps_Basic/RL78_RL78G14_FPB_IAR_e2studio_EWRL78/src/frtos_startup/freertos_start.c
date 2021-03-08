/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : freertos_start.c
* Version      : 1.0
* Description  : Contains FreeRTOS user-defined functions.
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "freertos_start.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#define NL "\n"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/************* semaphore initialization *****************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/* FreeRTOS's system timer. */
#ifndef configSETUP_TICK_INTERRUPT
void vApplicationSetupTimerInterrupt(void);
#endif /* configSETUP_TICK_INTERRUPT */

/* Hook functions used by FreeRTOS. */
void vAssertCalled(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* FreeRTOS's processing before start the kernel. */
void Processing_Before_Start_Kernel(void);

/* Main task. */
extern void main_task(void *pvParameters);

/******************************************************************************
* Function Name: vApplicationSetupTimerInterrupt
* Description  : Initialize system timer for FreeRTOS with tick interrupt 1ms.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
#ifdef configSETUP_TICK_INTERRUPT
void vApplicationSetupTimerInterrupt(void)
{
    const uint16_t usClockHz = 15000UL; /* Internal clock. */
    const uint16_t usCompareMatch = ( usClockHz / configTICK_RATE_HZ ) - 1UL;

    /* Use the internal 15K clock. */
    OSMC = ( uint8_t ) 0x16;

    #if ( defined( INTIT_vect ) && INTIT_vect == 0x38 ) || ( defined( INTIT ) && INTIT == 0x38 )
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
    #elif ( defined( INTIT_vect ) && INTIT_vect == 0x3C ) || ( defined( INTIT ) && INTIT == 0x3C )
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
    #else

        #error Neither RTC nor TMKA is available for the tick interrupt.

    #endif
}
#endif /* ifdef configSETUP_TICK_INTERRUPT */

/******************************************************************************
* Function Name: vAssertCalled
* Description  : This function is used to validate the input parameters.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
#if( configASSERT_DEFINED == 1 )
void vAssertCalled(void)
{
    static volatile unsigned long ul;

    taskENTER_CRITICAL();
    {
        vPrintString( NL "Assertion failed!" NL );

        /* Use the debugger to set ul to a non-zero value in order to step out
        of this function to determine why it was called. */
        ul = 0;
        while( 0 == ul )
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();

} /* End of function vAssertCalled() */
#endif /* configASSERT_DEFINED == 1 */

/******************************************************************************
* Function Name: vApplicationIdleHook
* Description  : This function will be called on each cycle of the idle task.
*                NOTE: vApplicationIdleHook() MUST NOT CALL A FUNCTION
*                      THAT MIGHT BLOCK UNDER ANY CIRCUMSTANCES.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
#if( configUSE_IDLE_HOOK == 1 )
void vApplicationIdleHook(void)
{
    /* Implement user-code for user own purpose. */

} /* End of function vApplicationIdleHook() */
#endif /* configUSE_IDLE_HOOK == 1 */

/******************************************************************************
* Function Name: vApplicationTickHook
* Description  : This function will be called every tick interrupt.
*                NOTE: vApplicationTickHook() EXECUTES FROM WITHIN AN ISR,
*                      SO MUST BE VERY SHORT AND NOT USE MUCH STACK.
*                      IN ADDITION, NOT CALL ANY APIs WITHOUT "FromISR" OR
*                      "FROM_ISR" AT THE END.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
#if( configUSE_TICK_HOOK == 1 )
void vApplicationTickHook(void)
{
    /* Implement user-code for user own purpose. */

} /* End of function vApplicationTickHook() */
#endif /* configUSE_TICK_HOOK == 1 */

/******************************************************************************
* Function Name: vApplicationMallocFailedHook
* Description  : This function is to capture the failure while
*                memory allocation.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
#if( configUSE_MALLOC_FAILED_HOOK == 1 )
void vApplicationMallocFailedHook(void)
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

    taskENTER_CRITICAL();

    vPrintString( NL "Insufficient heap memory!" NL );

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );

    for( ; ; )
    {
        /* Loop here */
    };

} /* End of function vApplicationMallocFailedHook() */
#endif /* configUSE_MALLOC_FAILED_HOOK == 1 */

/******************************************************************************
* Function Name: vApplicationStackOverflowHook
* Description  : Hook function is to capture the failure when the stack size
*                is insufficient for processing.
* Arguments    : pxTask -
*                    Task handler
*                pcTaskName -
*                    Pointer of where to store the task's name
* Return Value : None.
******************************************************************************/
#if( configCHECK_FOR_STACK_OVERFLOW != 0 )
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */

    taskENTER_CRITICAL();

    vPrintString( NL "Stack Overflow!" NL );

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );

    for( ; ; )
    {
        /* Loop here */
    };

} /* End of function vApplicationStackOverflowHook() */
#endif /* configCHECK_FOR_STACK_OVERFLOW != 0 */

/******************************************************************************
* Function Name: vApplicationGetIdleTaskMemory
* Description  : This function will be called when the idle task will be created
*                using user provided RAM.
* Arguments    : ppxIdleTaskTCBBuffer -
*                    Pointer of where to store the Idle Task's TCB Buffer address
*                ppxIdleTaskStackBuffer -
*                    Pointer of where to store the Idle Task's Stack Buffer address
*                pulIdleTaskStackSize -
*                    Pointer of where to store the Idle Task's Stack Buffer size
* Return Value : None.
******************************************************************************/
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCBBuffer;
    static StackType_t  xIdleTaskStackBuffer[IdleTask_STACK_BUFF_DEPTH];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = xIdleTaskStackBuffer;
    *pulIdleTaskStackSize = IdleTask_STACK_BUFF_DEPTH;

} /* End of vApplicationGetIdleTaskMemory() */
#endif /* configSUPPORT_STATIC_ALLOCATION == 1 */

/******************************************************************************
* Function Name: vApplicationGetTimerTaskMemory
* Description  : This function will be called when the scheduler is started if
*                configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are set to 1.
* Arguments    : ppxTimerTaskTCBBuffer -
*                    Pointer of where to store the Timer Task's TCB Buffer address
*                ppxTimerTaskStackBuffer -
*                    Pointer of where to store the Timer Task's Stack Buffer address
*                pulTimerTaskStackSize -
*                    Pointer of where to store the Timer Task's Stack Buffer size
* Return Value : None.
******************************************************************************/
#if( configSUPPORT_STATIC_ALLOCATION == 1 && configUSE_TIMERS == 1 )
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
    static StaticTask_t xTimerTaskTCBBuffer;
    static StackType_t  xTimerTaskStackBuffer[TimerTask_STACK_BUFF_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = xTimerTaskStackBuffer;
    *pulTimerTaskStackSize = TimerTask_STACK_BUFF_DEPTH;

} /* End of vApplicationGetTimerTaskMemory() */
#endif /* configSUPPORT_STATIC_ALLOCATION == 1 && configUSE_TIMERS == 1 */

/******************************************************************************
* Function Name: vPrintString
* Description  : This function is used to write a string to a debug console.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
void vPrintString(const char *pcMessage)
{
    /* Write the string to a debug console, using a critical section
    as a crude method of mutual exclusion. */

    extern void sim_debugger_console( const char *message );

    sim_debugger_console( pcMessage );

} /* End of function vPrintString() */

/******************************************************************************
* Function Name : Processing_Before_Start_Kernel
* Description   : Create a main task, FreeRTOS's objects (e.g. mailbox, task,
*                 semaphore, mutex...) if required.
* Arguments     : None.
* Return value  : None.
* Note          : Be aware that auto variables created on the stack in this
*                 function will be discarded after returning from this function.
*                 Therefore don't pass the address of auto variables to tasks.
*                 (Moreover, the stack used before starting scheduler will be
*                 re-used as interrupt dedicated stack after scheduler started.)
******************************************************************************/
void Processing_Before_Start_Kernel(void)
{
#if 0
    BaseType_t ret;
#endif /* #if 0 */

    /************** semaphore creation ***********************/



    /************** mutex creation ***************************/


    /************** queues creation **************************/


    /************** event groups creation ********************/


    /************** mailbox creation *************************/


    /************** memory pool creation *********************/


    Kernel_Object_init();

#if 0
    /************** task creation ****************************/
    /* Main task. */
    ret = xTaskCreate(main_task, "MAIN_TASK", 512, NULL, 3, NULL);
    if (pdPASS != ret)
    {
        while(1)
        {
            /* Failed! Task can not be created. */
        }
    }
#endif /* #if 0 */

    vPrintString( NL );

} /* End of function Processing_Before_Start_Kernel() */
