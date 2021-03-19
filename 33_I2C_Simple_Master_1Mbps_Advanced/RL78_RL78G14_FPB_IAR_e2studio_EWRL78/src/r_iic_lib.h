/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2011, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_iic_lib.h
* Version      : 2.00
* Device(s)    : R5F100LE
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Serial module.
* Creation Date: 2015/09/28
* Modified Date: 2015/10/06
* Modified Date: 2016/01/06
* Modified Date: 2016/06/06
* Modified Date: 2020/12/28
***********************************************************************************************************************/

/* add constant data define as following :@2016/06/06   */
/* fix constant data define as following :@2021/12/28   */
#define     IIC_STS_MASK    ( 0x03 )    /* mask data for get status     */
#define     IIC_SUCCESS     ( 0x00 )    /* communication is success     */
#define     IIC_TX_SVADDR   ( 0x81 )    /* Tx slave address in TX mode  */
#define     IIC_USING       ( 0x01 )    /* IIC bus is using (transmit)  */
#define     IIC_RX_SVADDR   ( 0xC1 )    /* Tx slave address in RX mode  */
#define     IIC_RX_MODE     ( 0x41 )    /* IIC is in data receive mode  */
#define     NO_ACK          ( 0x84 )    /* no ACK from slave for data   */
#define     NO_SLAVE        ( 0x8C )    /* no ACK for slave address     */
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
uint8_t R_IIC_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num);
                                        /* transmit data to slave       */
uint8_t R_IIC_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num);
                                        /* recieve data from slave      */
uint8_t R_IIC_wait_comend(void);      /* wait for transfer end        */
uint8_t R_IIC_check_comstate(void);   /* check transfer status        */
/*  00 : operation end witout error(IIC_SUCCESS)
    81 : slave address transmit for transmit(IIC_TX_SVADDR)
    01 : transmit data operation mode(IIC_USING)
    C1 : slave address transmit for receive mode(IIC_RX_SVADDR)
    41 : receive data operation mode(IIC_RX_MODE)
    84 : no ACK from slave for data(NO_ACK)
    85 : no ACK for slave address(NO_SLAVE)
*/
uint8_t R_IIC_StopCondition(void);    /* issue STOP condition         */

void IIC_TM03_init(void);               /* change Timer03 mode & start  */
void wait_time(uint8_t time);           /* wait for 20us * arguments    */
void wait_20us(void);                   /* wait 20us using TM03         */
void wait_5us(void);                    /* wait 5us using TM03          */

/***********************************************************************************************************************
Global variable
***********************************************************************************************************************/
//extern  uint8_t   g_iic00_status;       /* iic00 master operationflag   */
/*  00 : operation end witout error(IIC_SUCCESS)
    81 : slave address transmit for transmit(IIC_TX_SVADDR)
    01 : transmit data operation mode(IIC_USING)
    C1 : slave address transmit for receive mode(IIC_RX_SVADDR)
    41 : receive data operation mode(IIC_RX_MODE)
    84 : no ACK from slave for data(NO_ACK)
    85 : no ACK for slave address(NO_SLAVE)
*/

