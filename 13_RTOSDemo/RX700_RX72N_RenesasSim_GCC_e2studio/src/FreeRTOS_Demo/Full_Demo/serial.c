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

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Demo program includes. */
#include "serial.h"

/* Renesas includes. */
#include "platform.h"
#include "u_bsp_lowlvl_ext.h"

/* Eval board specific definitions. */
#include "demo_specific_io.h"

/* Characters received from the debug console are stored in this queue, ready
to be received by the application.  ***NOTE*** Using a queue in this way is very
convenient, but also very inefficient.  It can be used here because characters
will only arrive slowly.  In a higher bandwidth system a circular RAM buffer or
DMA should be used in place of this queue. */
static QueueHandle_t xRxQueue = NULL;

/* When a task calls vSerialPutString() its handle is stored in xSendingTask,
before being placed into the Blocked state (so does not use any CPU time) to
wait for the transmission to end.  The task handle is then used from tick hook
at the end of the transmission to remove the task from the Blocked state. */
static TaskHandle_t xSendingTask = NULL;

/* Working variables used in vSerialPortTickHook() to send the string passed
to vSerialPutString(). */
static const signed char * volatile pcStringToSend;
static uint16_t  volatile usStringLengthToSend;

/* Tick hook which is called from vApplicationTickHook() for the Command Console
which uses the debug console instead of the UART. */
void vSerialPortTickHook( uint16_t *pusTicksCmdConsoleInUse );

void vSerialPortTickHook( uint16_t *pusTicksCmdConsoleInUse )
{
    /* Check input from the Debug Console. */
    if( is_charget_ready() )
    {
        char cCharReceived = charget();

        /* Ensure the queue is ready to be received by the application. */
        if( xRxQueue != NULL )
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;

            /* Characters received from the UART are stored in this queue, ready to be
            received by the application.  ***NOTE*** Using a queue in this way is very
            convenient, but also very inefficient.  It can be used here because
            characters will only arrive slowly.  In a higher bandwidth system a circular
            RAM buffer or DMA should be used in place of this queue. */
            xQueueSendFromISR( xRxQueue, &cCharReceived, &xHigherPriorityTaskWoken );

            /* See http://www.freertos.org/xQueueOverwriteFromISR.html for information
            on the semantics of this ISR. */
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }

        /* Lock the Debug Console for the Command Console for a while. */
        if( *pusTicksCmdConsoleInUse == 0 )
        {
            *pusTicksCmdConsoleInUse = 1;
        }
    }

    /* Check output to the Debug Console. */
    while( is_charput_ready() && 0U < usStringLengthToSend )
    {
        charput( *pcStringToSend );
        pcStringToSend++;
        usStringLengthToSend--;

        if( 0U == usStringLengthToSend )
        {
            /* Ensure the task is waiting for the notofication. */
            if( xSendingTask != NULL )
            {
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;

                /* A task is waiting for the end of the Tx, unblock it now.
                http://www.freertos.org/vTaskNotifyGiveFromISR.html */
                vTaskNotifyGiveFromISR( xSendingTask, &xHigherPriorityTaskWoken );
                xSendingTask = NULL;

                portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
            }
        }

        /* Lock the Debug Console for the Command Console for a while. */
        if( *pusTicksCmdConsoleInUse == 0 )
        {
            *pusTicksCmdConsoleInUse = 1;
        }
    }
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    ( void ) ulWantedBaud;
    ( void ) uxQueueLength;

    /* Characters received from the UART are stored in this queue, ready to be
    received by the application.  ***NOTE*** Using a queue in this way is very
    convenient, but also very inefficient.  It can be used here because
    characters will only arrive slowly.  In a higher bandwidth system a circular
    RAM buffer or DMA should be used in place of this queue. */
    xRxQueue = xQueueCreate( uxQueueLength, sizeof( char ) );
    configASSERT( xRxQueue );

    /* Only one UART is supported, so it doesn't matter what is returned
    here. */
    return 0;
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 5000 );

    /* Only one port is supported. */
    ( void ) pxPort;

    /* Don't send the string unless the previous string has been sent. */
    if( ( xSendingTask == NULL ) && ( usStringLength > 0 ) )
    {
        /* Ensure the calling task's notification state is not already
        pending. */
        xTaskNotifyStateClear( NULL );

        /* Store the handle of the transmitting task.  This is used to unblock
        the task when the transmission has completed. */
        xSendingTask = xTaskGetCurrentTaskHandle();

        /* Store the string and the string length for the transmission. */
        pcStringToSend =pcString;
        usStringLengthToSend = usStringLength;

        /* Wait in the Blocked state (so not using any CPU time) until the
        transmission has completed. */
        ulTaskNotifyTake( pdTRUE, xMaxBlockTime );

        /* A breakpoint can be set here for debugging. */
        nop();
    }
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
    /* Only one UART is supported. */
    ( void ) pxPort;

    /* Return a received character, if any are available.  Otherwise block to
    wait for a character. */
    return xQueueReceive( xRxQueue, pcRxedChar, xBlockTime );
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
    /* Just mapped to vSerialPutString() so the block time is not used. */
    ( void ) xBlockTime;

    vSerialPutString( pxPort, &cOutChar, sizeof( cOutChar ) );
    return pdPASS;
}
