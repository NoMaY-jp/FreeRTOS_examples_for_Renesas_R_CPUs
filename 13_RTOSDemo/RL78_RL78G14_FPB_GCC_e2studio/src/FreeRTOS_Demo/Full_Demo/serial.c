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
#include "r_cg_serial.h"

/* Eval board specific definitions. */
#include "demo_specific_io.h"

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    ( void ) ulWantedBaud;
    ( void ) uxQueueLength;

    U_UART3_Start();

    /* Only one UART is supported, so it doesn't matter what is returned
    here. */
    return 0;
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
unsigned short usTxStringIndex;
unsigned short usTxStringCount;

    /* Only one port is supported. */
    ( void ) pxPort;

    for( usTxStringIndex = 0; usTxStringIndex < usStringLength; usTxStringIndex += usTxStringCount )
    {
        if( usStringLength - usTxStringIndex > 256 )
        {
            usTxStringCount = 256;
        }
        else
        {
            usTxStringCount = usStringLength - usTxStringIndex;
        }

        U_UART3_Send_Wait( ( uint8_t * ) (pcString + usTxStringIndex), usTxStringCount );

        /* A breakpoint can be set here for debugging. */
        nop();
    }
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
MD_STATUS status;
uint8_t err_type;
signed portBASE_TYPE xReturn;

    /* Only one UART is supported. */
    ( void ) pxPort;

    status = U_UART3_Receive_Wait( (uint8_t *) pcRxedChar, 1, &err_type, xBlockTime );

    /* A breakpoint can be set here for debugging. */
    nop();

    if (status == MD_OK)
    {
        xReturn = pdPASS;
    }
    else
    {
        xReturn = pdFAIL;

        U_UART3_Receive_ClearError();
    }

    return xReturn;
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
