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
* File Name    : r_cg_dtc.h
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for DTC module.
* Creation Date: 
***********************************************************************************************************************/

#ifndef DTC_H
#define DTC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/* 
    Peripheral Enable Register 1 (PER1) 
*/
/* Control of D/A converter input clock (DTCEN) */
#define _00_DTC_CLOCK_STOP                  (0x00U) /* stop supply of input clock */
#define _08_DTC_CLOCK_SUPPLY                (0x08U) /* supplie input clock */

/*
    DTC Control Register j (DTCCRj) (j = 0 to 23)
*/
/* Data size selection (SZ) */
#define _00_DTC_DATA_SIZE_8BITS             (0x00U) /* 8 bits */
#define _40_DTC_DATA_SIZE_16BITS            (0x40U) /* 16 bits */
/* Repeat mode interrupt enable bit (RPTINT) */
#define _00_DTC_REPEAT_INT_DISABLE          (0x00U) /* disable interrupt generation */
#define _20_DTC_REPEAT_INT_ENABLE           (0x20U) /* enable interrupt generation */
/* Chain transfer enable bit (CHNE) */
#define _00_DTC_CHAIN_TRANSFER_DISABLE      (0x00U) /* disable chain transfers */
#define _10_DTC_CHAIN_TRANSFER_ENABLE       (0x10U) /* enable chain transfers */
/* Destination address control bit (DAMOD) */
#define _00_DTC_DEST_ADDR_FIXED             (0x00U) /* destination address fixed */
#define _08_DTC_DEST_ADDR_INCREMENTED       (0x08U) /* destination address incremented */
/* Source address control bit (SAMOD) */
#define _00_DTC_SOURCE_ADDR_FIXED           (0x00U) /* source address fixed */
#define _04_DTC_SOURCE_ADDR_INCREMENTED     (0x04U) /* source address incremented */
/* Repeat area select bit (RPTSEL) */
#define _00_DTC_REPEAT_AREA_DEST            (0x00U) /* transfer destination is the repeat area */
#define _02_DTC_REPEAT_AREA_SOURCE          (0x02U) /* transfer source is the repeat area */
/* Transfer mode select bit (MODE) */
#define _00_DTC_TRANSFER_MODE_NORMAL        (0x00U) /* normal mode */
#define _01_DTC_TRANSFER_MODE_REPEAT        (0x01U) /* repeat mode */

/*  
    DTC Activation Enable Register 0 (DTCEN0)
*/
/* DTC activation enable bit (DTCEN07 - DTCEN00) */
#define _00_DTC_INT0_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT0) */
#define _40_DTC_INT0_ACTIVATION_ENABLE      (0x40U) /* enable activation (INT0) */
#define _00_DTC_INT1_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT1) */
#define _20_DTC_INT1_ACTIVATION_ENABLE      (0x20U) /* enable activation (INT1) */
#define _00_DTC_INT2_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT2) */
#define _10_DTC_INT2_ACTIVATION_ENABLE      (0x10U) /* enable activation (INT2) */
#define _00_DTC_INT3_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT3) */
#define _08_DTC_INT3_ACTIVATION_ENABLE      (0x08U) /* enable activation (INT3) */
#define _00_DTC_INT4_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT4) */
#define _04_DTC_INT4_ACTIVATION_ENABLE      (0x04U) /* enable activation (INT4) */
#define _00_DTC_INT5_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT5) */
#define _02_DTC_INT5_ACTIVATION_ENABLE      (0x02U) /* enable activation (INT5) */
#define _00_DTC_INT6_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT6) */
#define _01_DTC_INT6_ACTIVATION_ENABLE      (0x01U) /* enable activation (INT6) */

/*  
    DTC Activation Enable Register 1 (DTCEN1)
*/
/* DTC activation enable bit (DTCEN17 - DTCEN10) */
#define _00_DTC_INT7_ACTIVATION_DISABLE     (0x00U) /* disable activation (INT7) */
#define _80_DTC_INT7_ACTIVATION_ENABLE      (0x80U) /* enable activation (INT7) */
#define _00_DTC_KEY_ACTIVATION_DISABLE      (0x00U) /* disable activation (KEY) */
#define _40_DTC_KEY_ACTIVATION_ENABLE       (0x40U) /* enable activation (KEY) */
#define _00_DTC_AD_ACTIVATION_DISABLE       (0x00U) /* disable activation (AD) */
#define _20_DTC_AD_ACTIVATION_ENABLE        (0x20U) /* enable activation (AD) */
#define _00_DTC_UART0R_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART0 reception/CSI01/IIC01 transfer) */
#define _10_DTC_UART0R_ACTIVATION_ENABLE    (0x10U) /* enable activation (UART0 reception/CSI01/IIC01 transfer) */
#define _00_DTC_UART0T_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART0 transmission/CSI00/IIC00 transfer) */
#define _08_DTC_UART0T_ACTIVATION_ENABLE    (0x08U) /* enable activation (UART0 transmission/CSI00/IIC00 transfer) */
#define _00_DTC_UART1R_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART1 reception/CSI11/IIC11 transfer) */
#define _04_DTC_UART1R_ACTIVATION_ENABLE    (0x04U) /* enable activation (UART1 reception/CSI11/IIC11 transfer) */
#define _00_DTC_UART1T_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART1 transmission/CSI10/IIC10 transfer) */
#define _02_DTC_UART1T_ACTIVATION_ENABLE    (0x02U) /* enable activation (UART1 transmission/CSI10/IIC10 transfer) */
#define _00_DTC_UART2R_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART2 reception/CSI21/IIC21 transfer) */
#define _01_DTC_UART2R_ACTIVATION_ENABLE    (0x01U) /* enable activation (UART2 reception/CSI21/IIC21 transfer) */

/*  
    DTC Activation Enable Register 2 (DTCEN2)
*/
/* DTC activation enable bit (DTCEN27 - DTCEN20) */
#define _00_DTC_UART2T_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART2 transmission/CSI20/IIC20 transfer) */
#define _80_DTC_UART2T_ACTIVATION_ENABLE    (0x80U) /* enable activation (UART2 transmission/CSI20/IIC20 transfer) */
#define _00_DTC_UART3R_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART3 reception/CSI31/IIC31 transfer) */
#define _40_DTC_UART3R_ACTIVATION_ENABLE    (0x40U) /* enable activation (UART3 reception/CSI31/IIC31 transfer) */
#define _00_DTC_UART3T_ACTIVATION_DISABLE   (0x00U) /* disable activation (UART3 transmission/CSI30/IIC30 transfer) */
#define _20_DTC_UART3T_ACTIVATION_ENABLE    (0x20U) /* enable activation (UART3 transmission/CSI30/IIC30 transfer) */
#define _00_DTC_TAU00_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU00) */
#define _10_DTC_TAU00_ACTIVATION_ENABLE     (0x10U) /* enable activation (TAU00) */
#define _00_DTC_TAU01_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU01) */
#define _08_DTC_TAU01_ACTIVATION_ENABLE     (0x08U) /* enable activation (TAU01) */
#define _00_DTC_TAU02_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU02) */
#define _04_DTC_TAU02_ACTIVATION_ENABLE     (0x04U) /* enable activation (TAU02) */
#define _00_DTC_TAU03_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU03) */
#define _02_DTC_TAU03_ACTIVATION_ENABLE     (0x02U) /* enable activation (TAU03) */
#define _00_DTC_TAU10_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU10) */
#define _01_DTC_TAU10_ACTIVATION_ENABLE     (0x01U) /* enable activation (TAU10) */
/*  
    DTC Activation Enable Register 3 (DTCEN3)
*/
/* DTC activation enable bit (DTCEN37 - DTCEN30) */
#define _00_DTC_TAU11_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU11) */
#define _80_DTC_TAU11_ACTIVATION_ENABLE     (0x80U) /* enable activation (TAU11) */
#define _00_DTC_TAU12_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU12) */
#define _40_DTC_TAU12_ACTIVATION_ENABLE     (0x40U) /* enable activation (TAU12) */
#define _00_DTC_TAU13_ACTIVATION_DISABLE    (0x00U) /* disable activation (TAU13) */
#define _20_DTC_TAU13_ACTIVATION_ENABLE     (0x20U) /* enable activation (TAU13) */
#define _00_DTC_TMRDA0_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match A0) */
#define _10_DTC_TMRDA0_ACTIVATION_ENABLE    (0x10U) /* enable activation (Timer RD compare match A0) */
#define _00_DTC_TMRDB0_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match B0) */
#define _08_DTC_TMRDB0_ACTIVATION_ENABLE    (0x08U) /* enable activation (Timer RD compare match B0) */
#define _00_DTC_TMRDC0_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match C0) */
#define _04_DTC_TMRDC0_ACTIVATION_ENABLE    (0x04U) /* enable activation (Timer RD compare match C0) */
#define _00_DTC_TMRDD0_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match D0) */
#define _02_DTC_TMRDD0_ACTIVATION_ENABLE    (0x02U) /* enable activation (Timer RD compare match D0) */
#define _00_DTC_TMRDA1_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match A1) */
#define _01_DTC_TMRDA1_ACTIVATION_ENABLE    (0x01U) /* enable activation (Timer RD compare match A1) */

/*  
    DTC Activation Enable Register 4 (DTCEN4)
*/
/* DTC activation enable bit (DTCEN47 - DTCEN40) */
#define _00_DTC_TMRDB1_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match B1) */
#define _80_DTC_TMRDB1_ACTIVATION_ENABLE    (0x80U) /* enable activation (Timer RD compare match B1) */
#define _00_DTC_TMRDC1_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match C1) */
#define _40_DTC_TMRDC1_ACTIVATION_ENABLE    (0x40U) /* enable activation (Timer RD compare match C1) */
#define _00_DTC_TMRDD1_ACTIVATION_DISABLE   (0x00U) /* disable activation (Timer RD compare match D1) */
#define _20_DTC_TMRDD1_ACTIVATION_ENABLE    (0x20U) /* enable activation (Timer RD compare match D1) */
#define _00_DTC_TMRGA_ACTIVATION_DISABLE    (0x00U) /* disable activation (Timer RG compare match A) */
#define _10_DTC_TMRGA_ACTIVATION_ENABLE     (0x10U) /* enable activation (Timer RG compare match A) */
#define _00_DTC_TMRGB_ACTIVATION_DISABLE    (0x00U) /* disable activation (Timer RG compare match B) */
#define _08_DTC_TMRGB_ACTIVATION_ENABLE     (0x08U) /* enable activation (Timer RG compare match B) */
#define _00_DTC_TMRJ0_ACTIVATION_DISABLE    (0x00U) /* disable activation (Timer RJ0 underflow) */
#define _04_DTC_TMRJ0_ACTIVATION_ENABLE     (0x04U) /* enable activation (Timer RJ0 underflow) */
#define _00_DTC_CMP0_ACTIVATION_DISABLE     (0x00U) /* disable activation (Comparator detection 0) */
#define _02_DTC_CMP0_ACTIVATION_ENABLE      (0x02U) /* enable activation (Comparator detection 0) */
#define _00_DTC_CMP1_ACTIVATION_DISABLE     (0x00U) /* disable activation (Comparator detection 1) */
#define _01_DTC_CMP1_ACTIVATION_ENABLE      (0x01U) /* enable activation (Comparator detection 1) */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _FEDF_DTCD0_SRC_ADDRESS             (0xFEDFU)
#define _FFFF_DTCD0_DEST_ADDRESS            (0xFFFFU)
#define _01_DTCD0_TRANSFER_BYTE             (0x01U)
#define _01_DTCD0_TRANSFER_BLOCKSIZE        (0x01U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct
{
    uint8_t  dtccr;
    uint8_t  dtbls;
    uint8_t  dtcct;
    uint8_t  dtrld;
    uint16_t dtsar;
    uint16_t dtdar;
}st_dtc_data;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_DTC_Create(void);
void R_DTCD0_Start(void);
void R_DTCD0_Stop(void);

/* Start user code for function. Do not edit comment generated here */
void U_DTCD0_UserInit(volatile uint8_t * tx_buf, uint16_t tx_num);
/* End user code. Do not edit comment generated here */
#endif
