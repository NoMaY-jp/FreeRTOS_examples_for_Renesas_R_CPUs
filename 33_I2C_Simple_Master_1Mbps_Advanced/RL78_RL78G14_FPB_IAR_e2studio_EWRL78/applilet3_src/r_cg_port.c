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
* File Name    : r_cg_port.c
* Version      : Applilet3 for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for PORT module.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

#if defined(RENESAS_SIMULATOR_DEBUGGING)
/* Workaround for a problem that the Renesas RL78 simulator does not work expectedly
 * for a wiring such as "N-ch Open Drain Output Port <--> LED <--> R <--> VDD" */
void R_PORT_Create_org(void);
void R_PORT_Create_simulator_patch(void);
void R_PORT_Create(void)
{
    R_PORT_Create_org();
    R_PORT_Create_simulator_patch();
}
#define R_PORT_Create R_PORT_Create_org
#endif

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    P4 = _08_Pn3_OUTPUT_1 | _10_Pn4_OUTPUT_1;
    POM1 = _10_POMn4_NCH_ON | _20_POMn5_NCH_ON;
    POM3 = _01_POMn0_NCH_ON;
    POM4 = _08_POMn3_NCH_ON | _10_POMn4_NCH_ON;
    POM5 = _01_POMn0_NCH_ON;
    PM4 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
          _20_PMn5_NOT_USE | _C0_PM4_DEFAULT;
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name: R_PORT_Create_simulator_patch
* Description  : This function patches R_PORT_Create() for the Renesas RL78 Simulator
* Arguments    : None
* Return Value : None
******************************************************************************/
#if defined(RENESAS_SIMULATOR_DEBUGGING)
/* Workaround for a problem that the Renesas RL78 simulator does not work expectedly
 * for a wiring such as "N-ch Open Drain Output Port <--> LED <--> R <--> VDD" */
void R_PORT_Create_simulator_patch(void)
{
    if (IsRenesasSimDebugMode())
    {
        POM4 &= ~(_08_POMn3_NCH_ON | _10_POMn4_NCH_ON);
    }
}
#endif

/* End user code. Do not edit comment generated here */
