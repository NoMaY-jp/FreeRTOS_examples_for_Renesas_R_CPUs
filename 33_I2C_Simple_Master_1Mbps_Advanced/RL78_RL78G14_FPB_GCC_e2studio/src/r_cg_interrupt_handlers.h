/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_interrupt_handlers.h
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : GCCRL78
* Description  : This file declares interrupt handlers.
* Creation Date: 
***********************************************************************************************************************/

#ifndef INTERRUPT_HANDLERS_H_H
#define INTERRUPT_HANDLERS_H_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
//0x8
void r_intc0_interrupt(void) __attribute__ ((interrupt));
//0x1E
void r_iic00_interrupt(void) __attribute__ ((interrupt));
//0x2A
void r_iica0_interrupt(void) __attribute__ ((interrupt));
//0x32
void r_tau0_channel3_interrupt(void) __attribute__ ((interrupt));
//Hardware Vectors
//0x0
void PowerON_Reset(void) __attribute__ ((interrupt));
//idle Vectors
void R_Dummy(void) __attribute__ ((interrupt));

#endif
