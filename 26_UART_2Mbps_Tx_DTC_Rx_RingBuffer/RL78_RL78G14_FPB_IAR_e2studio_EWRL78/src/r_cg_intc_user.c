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
* File Name    : r_cg_intc_user.c
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for INTC module.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
#include "freertos_start.h"
#include "freertos_isr.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern TaskHandle_t g_intc0_task;      /* intc0 event notify task */

#define r_intc0_interrupt R_CG_FREERTOS_INTERRUPT_EI(r_intc0_interrupt)
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc0_interrupt
* Description  : This function is INTP0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTP0_vect
__interrupt static void r_intc0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */

    vTaskNotifyGiveFromISR_R_Helper( &g_intc0_task );

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
