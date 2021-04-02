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

#ifndef LED_IO_H
#define LED_IO_H

/* Select one of the following defines. Note that you don't have to
change the target device. */

	#if !defined(RSK_RX130) && !defined(TB_RX130)

		//#define RSK_RX130		/* Renesas Starter Kit for RX130-512KB */
		#define TB_RX130		/* Target Board for RX130 */

	#endif

/* But, in case of GNURX project, if you change the target device,
please take care of the following three KEEP() statements in the
src/linker_script.ld file by your hand. */

/*
	.text 0xFFF80000: AT(0xFFF80000)
	{
		*(.text)
		KEEP(*(.text.*ISR))
		KEEP(*(.text.*_isr))
		KEEP(*(.text.*_interrupt))
		*(.text.*)
		*(P)
		etext = .;
	} > ROM
*/

/* Board support settings. */

	#ifdef RSK_RX130

		/* https://www.renesas.com/products/software-tools/boards-and-kits/starter-kits/renesas-starter-kit-for-rx130-512kb.html */

		/* R5F51308ADFP 100-pin LFQFP */

		/* General Values */
		#define LED_ON					(0)
		#define LED_OFF 				(1)
		#define SET_BIT_HIGH			(1)
		#define SET_BIT_LOW 			(0)

		/* Switches */
		#define SW1 					(PORT3.PIDR.BIT.B1)
		#define SW2 					(PORT3.PIDR.BIT.B2)
		#define SW3 					(PORT1.PIDR.BIT.B6)
		#define U_GPIO_PIN_SW1			(GPIO_PORT_3_PIN_1)
		#define U_GPIO_PIN_SW2			(GPIO_PORT_3_PIN_2)
		#define U_GPIO_PIN_SW3			(GPIO_PORT_1_PIN_6)

		/* LED port settings */
		#define LED0					(PORTD.PODR.BIT.B3)
		#define LED1					(PORTD.PODR.BIT.B4)
		#define LED2					(PORTE.PODR.BIT.B6)
		#define LED3					(PORTE.PODR.BIT.B7)
		#define U_GPIO_PIN_LED0 		(GPIO_PORT_D_PIN_3)
		#define U_GPIO_PIN_LED1 		(GPIO_PORT_D_PIN_4)
		#define U_GPIO_PIN_LED2 		(GPIO_PORT_E_PIN_6)
		#define U_GPIO_PIN_LED3 		(GPIO_PORT_E_PIN_7)

		/* FreeRTOS CLI Command Console */
		#define U_SCI_UART_CLI_PINSET()	R_SCI_PinSet_SCI1()
		#define U_SCI_UART_CLI_SCI_CH	(SCI_CH1)
		#define U_DTC_UART_CLI_TX_ACT	((dtc_activation_source_t)VECT(SCI1,TXI1))
		#define U_DTC_UART_CLI_TX_DR	(SCI1.TDR)

	#endif /* RSK_RX130 */

	#ifdef TB_RX130

		/* https://www.renesas.com/products/software-tools/boards-and-kits/eval-kits/rx-family-target-board.html */

		/* R5F51308ADFP 100-pin LFQFP */

		/* General Values */
		#define LED_ON					(0)
		#define LED_OFF 				(1)
		#define SW_PUSH 				(0)
		#define SW_RELEASE				(1)

		/* Switches */
		#define SW1						(PORTB.PIDR.BIT.B1)
		#define U_GPIO_PIN_SW1			(GPIO_PORT_B_PIN_1)

		/* LED port settings */
		#define LED0					(PORTD.PODR.BIT.B6)
		#define LED1					(PORTD.PODR.BIT.B7)
		#define U_GPIO_PIN_LED0			(GPIO_PORT_D_PIN_6)
		#define U_GPIO_PIN_LED1			(GPIO_PORT_D_PIN_7)

		/* FreeRTOS CLI Command Console */
		#define U_SCI_UART_CLI_PINSET()	R_SCI_PinSet_SCI1()
		#define U_SCI_UART_CLI_SCI_CH	(SCI_CH1)
		#define U_SCI_UART_CLI_RX_PU	(GPIO_PORT_3_PIN_0)
		#define U_DTC_UART_CLI_TX_ACT	((dtc_activation_source_t)VECT(SCI1,TXI1))
		#define U_DTC_UART_CLI_TX_DR	(SCI1.TDR)

	#endif /* TB_RX130 */

	#ifndef LED0
		#error The hardware platform is not defined
	#endif

/* Board Support Data Structures. */

#include "r_sci_rx_if.h"
#include "r_dtc_rx_if.h"

extern sci_hdl_t xSerialSciHandle;
extern dtc_transfer_data_t xSerialTxDtcInfo;

/* Board Support Callback Functions. */

extern void vSerialSciCallback( void *pvArgs );

#endif /* LED_IO_H */

