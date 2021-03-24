#include "platform.h"
#include <stdlib.h>

/* Replacement to be thread-safe (in case of other than using heap_3.c). */
void *malloc( size_t xWantedSize )
{
#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    return pvPortMalloc( xWantedSize );
#else
    INTERNAL_NOT_USED( xWantedSize );

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );

    return NULL;
#endif
}

/* Replacement to be thread-safe (in case of other than using heap_3.c). */
void free( void *pv )
{
#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    vPortFree( pv );
#else
    INTERNAL_NOT_USED( pv );

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );
#endif
}

#if defined(__GNUC__)

int8_t *sbrk( size_t size );

/* Maybe not called but necessary for linking without an undefined error. */
int8_t *sbrk( size_t size )
{
    INTERNAL_NOT_USED( size );

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );

    return (int8_t *)-1;
}

#endif /* defined(__GNUC__) */

/******************************************************************************
* Function Name: vTaskNotifyGiveFromISR_R_Helper
* Description  : Helper function for vTaskNotifyGiveFromISR().
* Arguments    : pxTask -
*                    Pointer to task handler
* Return value : None.
******************************************************************************/
void vTaskNotifyGiveFromISR_R_Helper(TaskHandle_t *pxTask)
{
    if (NULL != *pxTask)
    {
        BaseType_t sHigherPriorityTaskWoken = pdFALSE;

        /* Notify the task that the interrupt/callback is complete. */
        vTaskNotifyGiveFromISR( *pxTask, &sHigherPriorityTaskWoken );

        /* There are no interrupt/callback in progress, so no tasks to notify. */
        *pxTask = NULL;

        portYIELD_FROM_ISR( sHigherPriorityTaskWoken );
    }

} /* End of function vTaskNotifyGiveFromISR_R_Helper() */

/******************************************************************************
* Function Name: xTaskNotifyFromISR_R_Helper
* Description  : Helper function for xTaskNotifyFromISR().
* Arguments    : pxTask -
*                    Pointer to task handler
*                ulValue -
*                    Notification value
* Return value : None. (This is NOT the same as xTaskNotifyFromISR().)
******************************************************************************/
void xTaskNotifyFromISR_R_Helper(TaskHandle_t *pxTask, uint32_t ulValue)
{
    if (NULL != *pxTask)
    {
        BaseType_t sHigherPriorityTaskWoken = pdFALSE;

        /* Notify the task that the interrupt/callback is complete. */
        xTaskNotifyFromISR( *pxTask, ulValue, eSetValueWithOverwrite, &sHigherPriorityTaskWoken );

        /* There are no interrupt/callback in progress, so no tasks to notify. */
        *pxTask = NULL;

        portYIELD_FROM_ISR( sHigherPriorityTaskWoken );
    }

} /* End of function xTaskNotifyFromISR_R_Helper() */

/******************************************************************************
* Function Name: ulTaskNotifyTake_R_Helper
* Description  : Helper function for ulTaskNotifyTake().
* Arguments    : xTicksToWait -
*                    Ticks to wait
* Return value : The same return value from ulTaskNotifyTake().
******************************************************************************/
uint32_t ulTaskNotifyTake_R_Helper(TickType_t xTicksToWait)
{
    /* Wait to be notified that the interrupt/callback is complete. */
    return ulTaskNotifyTake( pdTRUE, xTicksToWait );

} /* End of function ulTaskNotifyTake_R_Helper() */

/******************************************************************************
* Function Name: ulTaskNotifyTake_R_Helper_Ex2__helper
* Description  : Helper function for ulTaskNotifyTake_R_Helper_Ex2().
* Arguments    : pxTask -
*                    Pointer to variable of task handler
*                xStatus -
*                    Status of setup of interrupt/callback
*                xTicksToWait -
*                    Ticks to wait
* Return value : The same return value from ulTaskNotifyTake().
* Note         : Internal use only.
******************************************************************************/
uint32_t ulTaskNotifyTake_R_Helper_Ex2__helper(TaskHandle_t *pxTask, MD_STATUS xStatus, TickType_t xTicksToWait)
{
    /* Check whether the setup of the interrupt/callback was succeeded. */
    if (MD_OK != xStatus)
    {
        /* There are no interrupt/callback in progress, so ensure no tasks to notify. */
        *pxTask = NULL;

        /* Return zero which is the same value when ulTaskNotifyTake() is timed out. */
        return 0U;
    }

    /* Wait to be notified that the interrupt/callback is complete. */
    return ulTaskNotifyTake( pdTRUE, xTicksToWait );

} /* End of function ulTaskNotifyTake_R_Helper_Ex2__helper() */

/******************************************************************************
* Function Name: ulTaskNotifyTake_R_Abort_Helper
* Description  : Helper function for ulTaskNotifyTake().
* Arguments    : pxTask -
*                    Pointer to variable of task handler
* Return value : None. (This is NOT the same as ulTaskNotifyTake().)
* Note         : Internal use only.
******************************************************************************/
void ulTaskNotifyTake_R_Abort_Helper(TaskHandle_t *pxTask)
{
    /* There are no interrupt/callback in progress, so ensure no tasks to notify. */
    *pxTask = NULL;

    /* Clear an unhandled notification from timeout till abort */
    ulTaskNotifyTake( pdTRUE, 0 );

} /* End of function ulTaskNotifyTake_R_Abort_Helper() */

/******************************************************************************
* Function Name: xTaskGetCurrentTaskHandle_R_Helper
* Description  : Helper function for xTaskGetCurrentTaskHandle().
* Arguments    : None.
* Return value : The same return value from xTaskGetCurrentTaskHandle().
******************************************************************************/
TaskHandle_t xTaskGetCurrentTaskHandle_R_Helper(void)
{
    /* Ensure the calling task does not already have a notification pending. */
    ulTaskNotifyTake( pdTRUE, 0 );

    /* Return the handle of the calling task. */
    return xTaskGetCurrentTaskHandle();

} /* End of function xTaskGetCurrentTaskHandle_R_Helper() */
