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

	#if !defined(RSK_RX72N) && !defined(RPB_RX72N)

		//#define RSK_RX72N		/* Renesas Starter Kit+ for RX72N */
		#define RPB_RX72N		/* Renesas Envision Kit RPBRX72N */

	#endif

/* But, in case of GNURX project, if you change the target device,
please take care of the following three KEEP() statements in the
src/linker_script.ld file by your hand. */

/*
	.text 0xFFC00000: AT(0xFFC00000)
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

	#ifdef RSK_RX72N

		/* https://www.renesas.com/jp/ja/products/software-tools/boards-and-kits/starter-kits/renesas-starter-kitplus-for-rx72n.html */

		/* R5F572NNDDBD (or R5F572NNHDBD) 224-pin LFBGA */

		/* General Values */
		#define LED_ON					(0)
		#define LED_OFF 				(1)
		#define SET_BIT_HIGH			(1)
		#define SET_BIT_LOW 			(0)

		/* Switches */
		#define SW1 					(PORT4.PIDR.BIT.B5)
		#define SW2 					(PORT4.PIDR.BIT.B4)
		#define SW3 					(PORT0.PIDR.BIT.B7)
		#define U_GPIO_PIN_SW1			(GPIO_PORT_4_PIN_5)
		#define U_GPIO_PIN_SW2			(GPIO_PORT_4_PIN_4)
		#define U_GPIO_PIN_SW3			(GPIO_PORT_0_PIN_7)

		/* LED port settings */
		#define LED0					(PORT7.PODR.BIT.B1)
		#define LED1					(PORTH.PODR.BIT.B6)
		#define LED2					(PORTL.PODR.BIT.B7)
		#define LED3					(PORTL.PODR.BIT.B6)
		#define U_GPIO_PIN_LED0 		(GPIO_PORT_7_PIN_1)
		#define U_GPIO_PIN_LED1 		(GPIO_PORT_H_PIN_6)
		#define U_GPIO_PIN_LED2 		(GPIO_PORT_L_PIN_7)
		#define U_GPIO_PIN_LED3 		(GPIO_PORT_L_PIN_6)

		/* FreeRTOS CLI Command Console */
		extern void U_SCI_PinSet_SCI9_RSK_RX72N(void);
		#define U_SCI_UART_CLI_PINSET()	U_SCI_PinSet_SCI9_RSK_RX72N()
		#define U_SCI_UART_CLI_SCI_CH	(SCI_CH9)
		#define U_DTC_UART_CLI_TX_ACT	((dtc_activation_source_t)VECT(SCI9,TXI9))
		#define U_DTC_UART_CLI_TX_DR	(SCI9.TDR)

	#endif /* RSK_RX72N */

	#ifdef RPB_RX72N

		/* https://www.renesas.com/jp/ja/products/software-tools/boards-and-kits/eval-kits/rx72n-envision-kit.html */

		/* R5F572NDHDFB 144-pin LFQFP */

		/* General Values */
		#define LED_ON					(0)
		#define LED_OFF 				(1)
		#define SW_PUSH 				(0)
		#define SW_RELEASE				(1)

		/* Switches (and its notation in the User's Manual) */
		#define SW1/*(SW2)*/				(PORT0.PIDR.BIT.B7)
		#define U_GPIO_PIN_SW1/*(SW2)*/ 	(GPIO_PORT_0_PIN_7)

		/* LED port settings (and its notation in the User's Manual) */
		#define LED0/*(LED2)*/				(PORT4.PODR.BIT.B0)
		#define U_GPIO_PIN_LED0/*(LED2)*/	(GPIO_PORT_4_PIN_0)

		/* FreeRTOS CLI Command Console */
		#define U_SCI_UART_CLI_PINSET()	R_SCI_PinSet_SCI2()
		#define U_SCI_UART_CLI_SCI_CH	(SCI_CH2)
		#define U_DTC_UART_CLI_TX_ACT	((dtc_activation_source_t)VECT(SCI2,TXI2))
		#define U_DTC_UART_CLI_TX_DR	(SCI2.TDR)

	#endif /* RPB_RX72N */

	#ifndef LED0
		#error The hardware platform is not defined
	#endif

#endif /* LED_IO_H */

