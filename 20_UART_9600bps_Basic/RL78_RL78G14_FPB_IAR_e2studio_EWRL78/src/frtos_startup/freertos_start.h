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
* File Name    : freertos_start.h
* Version      : 1.0
* Description  : FreeRTOS's user-defined functions header file.
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
******************************************************************************/

#ifndef FREERTOS_START_H_
#define FREERTOS_START_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#define STACK_BUFF_BYTES(task, interruptstub, mergin) ((task)+(interruptstub)+(mergin))

#if defined(__CCRL__)

/* The stack usage analysis tool 'Call Walker' provides detail stack usage information.
 * The following table is based on the result of analysis 10-Jan-2021 12:09:59, 21:14:06.
 *
 * The stack space for interrupt stub is calculated as follows:
 *
 * interrupt stub size = MAX( space for task context, space for nested interrupt )
 *                     = MAX( (PC + PSW + REGS + usCriticalNesting), (PC + PSW + REGS) )
 *                     = MAX( (3 + 1 + 10 + 2), (3 + 1 + 10) )
 *                     = MAX( 16, 14 )
 *                     = 16
 */

#if 1

/* The following table is for the case of using compile optimization of CC-RL. */

#define task_LED0_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  52, 16, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  52, 16, 32 ) )
#define task_CONIO_STACK_BUFF_DEPTH pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  72, 16, 32 ) )
#define main_task_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(   4, 16, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  64, 16, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(   0,  0,  0 ) )

#else

/* The following table is for the case of NOT using compile optimization of CC-RL. */

#define task_LED0_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  78, 16, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  78, 16, 32 ) )
#define task_CONIO_STACK_BUFF_DEPTH pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES( 110, 16, 32 ) )
#define main_task_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(   6, 16, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(  90, 16, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( STACK_BUFF_BYTES(   0,  0,  0 ) )

#endif

#elif defined(__GNUC__) ||  defined(__ICCRL78__)

/* There are no detail stack usage information. */

#define task_LED0_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( 256 )
#define task_LED1_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( 256 )
#define task_CONIO_STACK_BUFF_DEPTH pdBYTES_TO_STACK_DEPTH( 256 )
#define main_task_STACK_BUFF_DEPTH  pdBYTES_TO_STACK_DEPTH( 128 )
#define IdleTask_STACK_BUFF_DEPTH   pdBYTES_TO_STACK_DEPTH( 256 )

#endif

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/
/************* semaphore handler *****************/



/************* mutex handler *********************/


/************** queues handler *******************/


/************** event groups handler *************/


/************** mailbox handler ******************/


/************** memory pool handler **************/


/************** task handler *********************/


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
extern void Processing_Before_Start_Kernel(void);
extern void Kernel_Object_init(void);
extern void vPrintString(const char *pcMessage);
extern void vTaskNotifyGiveFromISR_R_Helper(TaskHandle_t *pxTask);
extern void xTaskNotifyFromISR_R_Helper(TaskHandle_t *pxTask, uint32_t ulValue);
extern uint32_t ulTaskNotifyTake_R_Helper(TickType_t xTicksToWait);
extern TaskHandle_t xTaskGetCurrentTaskHandle_R_Helper(void);

#define xTaskCreateStatic_R_Helper(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask) \
do{ \
    static StaticTask_t pxTaskCode##_xTCBBuffer; \
    static StackType_t  pxTaskCode##_xStackBuffer[usStackDepth]; \
    TaskHandle_t xCreatedTask; \
    xCreatedTask = xTaskCreateStatic( pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxTaskCode##_xStackBuffer, &pxTaskCode##_xTCBBuffer ); \
    if (NULL != (TaskHandle_t *)pxCreatedTask) \
    { \
        *(TaskHandle_t *)pxCreatedTask = xCreatedTask; \
    } \
}while (0)

#define vTaskNotifyGiveFromISR_R_Helper(pxTask) \
do{ \
    if (NULL != *pxTask) \
    { \
        BaseType_t sHigherPriorityTaskWoken = pdFALSE; \
\
        /* Notify the task that the interrupt/callback is complete. */ \
        vTaskNotifyGiveFromISR( *pxTask, &sHigherPriorityTaskWoken ); \
\
        /* There are no interrupt/callback in progress, so no tasks to notify. */ \
        *pxTask = NULL; \
\
        portYIELD_FROM_ISR( sHigherPriorityTaskWoken ); \
    } \
}while (0)

#define xTaskNotifyFromISR_R_Helper(pxTask, ulValue) \
do{ \
    if (NULL != *pxTask) \
    { \
        BaseType_t sHigherPriorityTaskWoken = pdFALSE; \
\
        /* Notify the task that the interrupt/callback is complete. */ \
        xTaskNotifyFromISR( *pxTask, ulValue, eSetValueWithOverwrite, &sHigherPriorityTaskWoken ); \
\
        /* There are no interrupt/callback in progress, so no tasks to notify. */ \
        *pxTask = NULL; \
\
        portYIELD_FROM_ISR( sHigherPriorityTaskWoken ); \
    } \
}while (0)

#define ulTaskNotifyTake_R_Helper(xTicksToWait) \
( \
    /* Wait to be notified that the interrupt/callback is complete. */\
    ulTaskNotifyTake( pdTRUE, xTicksToWait ) \
)

#define xTaskGetCurrentTaskHandle_R_Helper() \
( \
    /* Ensure the calling task does not already have a notification pending. */ \
    ulTaskNotifyTake( pdTRUE, 0 ), \
\
    /* Return the handle of the calling task. */ \
    xTaskGetCurrentTaskHandle() \
)

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_START_H_ */
