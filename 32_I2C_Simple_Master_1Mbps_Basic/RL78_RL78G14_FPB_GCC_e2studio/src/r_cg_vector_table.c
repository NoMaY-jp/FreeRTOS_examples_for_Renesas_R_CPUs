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
* File Name    : r_cg_vector_table.c
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : GCCRL78
* Description  : This file implements interrupt vector.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_vector_table.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Set option bytes */
#define OPTION_SECT          __attribute__ ((section (".option_bytes")))
const uint8_t Option_Bytes[] OPTION_SECT = 
    {0xEFU, 0xFFU, 0xF8U, 0x85U};

/* Set security ID */
#define SECURITYID_SECT          __attribute__ ((section (".security_id")))
const uint8_t Security_Id[] SECURITYID_SECT = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

#define VEC          __attribute__ ((section (".vec")))
const void *HardwareVectors[] VEC  = {
	// Address 0x0
	PowerON_Reset,
	// Secure for Debugging
    (void*)0xFFFF
};

#define VECT_SECT          __attribute__ ((section (".vects")))
const void *Vectors[] VECT_SECT  = {
	// Address 0x4
	R_Vector_0x4,
	// Address 0x6
	R_Vector_0x6,
	// Address 0x8
	r_intc0_interrupt,
	// Address 0xA
	R_Vector_0xA,
	// Address 0xC
	R_Vector_0xC,
	// Address 0xE
	R_Vector_0xE,
	// Address 0x10
	R_Vector_0x10,
	// Address 0x12
	R_Vector_0x12,
	// Address 0x14
	R_Vector_0x14,
	// Address 0x16
	R_Vector_0x16,
	// Address 0x18
	R_Vector_0x18,
	// Address 0x1A
	R_Vector_0x1A,
	// Address 0x1C
	R_Vector_0x1C,
	// Address 0x1E
	r_iic00_interrupt,
	// Address 0x20
	R_Vector_0x20,
	// Address 0x22
	R_Vector_0x22,
	// Address 0x24
	R_Vector_0x24,
	// Address 0x26
	R_Vector_0x26,
	// Address 0x28
	R_Vector_0x28,
	// Address 0x2A
	r_iica0_interrupt,
	// Address 0x2C
	R_Vector_0x2C,
	// Address 0x2E
	R_Vector_0x2E,
	// Address 0x30
	R_Vector_0x30,
	// Address 0x32
	R_Vector_0x32,
	// Address 0x34
	R_Vector_0x34,
	// Address 0x36
	R_Vector_0x36,
	// Address 0x38
	R_Vector_0x38,
	// Address 0x3A
	R_Vector_0x3A,
	// Address 0x3C
	R_Vector_0x3C,
	// Address 0x3E
	R_Vector_0x3E,
	// Address 0x40
	R_Vector_0x40,
	// Address 0x42
	R_Vector_0x42,
	// Address 0x44
	R_Vector_0x44,
	// Address 0x46
	R_Vector_0x46,
	// Address 0x48
	R_Vector_0x48,
	// Address 0x4A
	R_Vector_0x4A,
	// Address 0x4C
	R_Vector_0x4C,
	// Address 0x4E
	R_Vector_0x4E,
	// Address 0x50
	R_Vector_0x50,
	// Address 0x52
	R_Vector_0x52,
	// Address 0x54
	R_Vector_0x54,
	// Address 0x56
	R_Vector_0x56,
	// Address 0x58
	R_Vector_0x58,
	// Address 0x5A
	R_Vector_0x5A,
	// Address 0x5C
	R_Vector_0x5C,
	// Address 0x5E
	R_Vector_0x5E,
	// Address 0x60
	R_Vector_0x60,
	// Address 0x62
	R_Vector_0x62,
	// Address 0x64
	R_Vector_0x64,
	// Address 0x66
	R_Vector_0x66,
	// Address 0x68
	R_Vector_0x68,
	// Address 0x6A
	R_Vector_0x6A,
	// Address 0x6C
	R_Vector_0x6C,
	// Address 0x6E
	R_Vector_0x6E,
	// Address 0x70
	R_Vector_0x70,
	// Address 0x72
	R_Vector_0x72,
	// Address 0x74
	R_Vector_0x74,
	// Address 0x76
	R_Vector_0x76,
	// Address 0x78
	R_Vector_0x78,
	// Address 0x7A
	R_Vector_0x7A,
	// Address 0x7C
	R_Vector_0x7C,
	// Address 0x7E
	R_Vector_0x7E,
};
/***********************************************************************************************************************
* Function Name: R_Dummy
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Dummy(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

#ifndef VECTOR_TABLE_H
#error The r_cg_vector_table.c has not been modified by the r_cg_vector_table.js yet.
#error Please touch the r_cg_vector_table.c (without anything other than saving it).
#endif

/* End user code. Do not edit comment generated here */
