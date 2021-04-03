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

#ifndef DEMO_MAIN_H
#define DEMO_MAIN_H

/* ATTENTION : 2Mbps UART is used by also the command console demo of the full
demo, the comprehensive test and demo which is one of the two standard RTOS Demos,
for the RL78/G14 Fast Prototyping Board, instead of 115.2Kbps UART used by the
same demo for the RX72N Envision Kit. */

/* Set mainCREATE_NON_STANDARD_RTOS_DEMO to one to run the following non standard
RTOS Demo, or 0 to run the standard RTOS Demo which is the simple blinky demo or
the more comprehensive test and demo application.

The non standard RTOS Demo:

(1)   Toggle LED0 every 1000ms.
(2)   Toggle LED1 when SW_USER is pushed.
(3-1) Echo every three characters through the UART3 using the following sockets.
        RxD3 : J7-D0
        TxD3 : J7-D1
        GND  : J8-G
        Baud rate : 2Mbps
(3-2) Blink LED1 faster if parity error, framing error, overrun error or timeout
occurs. The error state is cleared when SW_USER is pushed. */
#define mainCREATE_NON_STANDARD_RTOS_DEMO	0

/* Set mainCREATE_SIMPLE_BLINKY_DEMO_ONLY to one to run the simple blinky demo,
or 0 to run the more comprehensive test and demo application. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	0

/* The following demo_main() is placed in the src/frtos_startup/freertos_start.c
and it calls main_blinky() or main_full() according to the macro setting. */
extern void demo_main( void );

/* The following functions are placed in the src/frtos_startup/freertos_start.c
and these are called from the src/FreeRTOS_Demo/Blinky_Demo/main_blinky.c or the
src/FreeRTOS_Demo/Full_Demo/main_full.c according to the macro setting. */
extern void vToggleLED( void );
extern void vSendString( const char * const pcString );

#endif
