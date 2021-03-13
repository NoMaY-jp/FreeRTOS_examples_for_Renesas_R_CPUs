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
* Copyright (C) 2011, 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_iic_lib.c
* Version      : 2.00
* Device(s)    : R5F100LE
* Tool-Chain   : CCRL
* Description  : This file implements IIC control functions.
* Creation Date: 2015/12/26
* Modified Date: 2016/06/07
* Modified Date: 2021/03/13 copy some definitions from r_cg_userdefine.h
* Modified Date: 2021/03/13 add support of callback for FreeRTOS
* Modified Date: 2021/03/13 add support of GNURL78 and ICCRL78
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_iic_lib.h"

void u_iica0_callback_master_common(MD_STATUS flag);    /* for FreeRTOS */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#if defined(__CCRL__)
#pragma interrupt r_iica0_interrupt(vect=INTIICA0,bank=RB1)
#endif

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/

#define     WAITTIME    ( 1000U )       /* limit of start condition     */
#define     STS_MASK    ( 0x3FU )       /* IICA0 status mask data       */
#define     ON_COMMU    ( 0x01U )       /* on communication             */
#define     DUMMY_DATA  ( 0xFFU )       /* dummy data for receive       */

volatile uint8_t   g_iica0_status;      /* iica0 master operationflag   */
/*  00 : operation end witout error(IIC_SUCCESS)
    01 : data transmit mode(IIC_USING)
    82 : I2C bus is busy(MD_ERROR1)
    84 : no ACK from slave for data(NO_ACK)
    85 : no ACK for slave address(NO_SLAVE)
*/
extern volatile uint8_t * gp_iica0_rx_address;  /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_len;       /* iica0 receive data length    */
extern volatile uint16_t  g_iica0_rx_cnt;       /* iica0 receive data count     */
extern volatile uint8_t * gp_iica0_tx_address;  /* iica0 send buffer address    */
extern volatile uint16_t  g_iica0_tx_cnt;       /* iica0 send data count        */

/* copy following 4 definitions from r_cg_userdefine.h :@2021/03/13 */
#define     TRUTH           ( 1U )
#define     FALSE           ( 0U )
#define     INT_MASK        ( 1U )      /* mask interrupt request flag  */
#define     INT_ENABLE      ( 0U )      /* for interrupt mask clear     */

/***********************************************************************************************************************
Local functions
***********************************************************************************************************************/

uint8_t R_IICA0_bus_check(void);        /* check I2C bus status         */

/*******************************************************************************
* Function Name: IIC_TM03_init
* Description  : disable external edge and start TM03
* Arguments    : none
* Return Value : none
********************************************************************************/
void IIC_TM03_init(void)
{
    TMR03 = 0x09;                       /* disable edge detection   */
    TS0 = 0x08;                         /* enable TM03              */
    TMMK03 = INT_MASK;                  /* mask TM03 interrupt      */
}
/*******************************************************************************
* Function Name: wait_time
* Description  : This function wait time
* Arguments    : wait time by 20us
* Return Value : none
********************************************************************************/
void wait_time(uint8_t time)
{
    uint8_t work;
    for ( work = time ; work > 0 ; work-- )
    {
        wait_20us();                    /* wait 20us                    */
    }
}

/*******************************************************************************
* Function Name: wait_20us
* Description  : This function wait 5us
* Arguments    : none
* Return Value : none
********************************************************************************/
void wait_20us(void)
{
    uint8_t cnt;
    for ( cnt = 4 ; cnt > 0 ; cnt-- )
    {
        TMIF03 = FALSE;                 /* clear INTTM03 request flag   */
        TS0 = 0x08;                     /* trigger TM03 count           */
        while ( FALSE == TMIF03 )       /* wait for 5us delay time      */
        {
            NOP();
        }
    }
}

/*******************************************************************************
* Function Name: wait_5us
* Description  : This function wait 5us
* Arguments    : none
* Return Value : none
********************************************************************************/
void wait_5us(void)
{

    TMIF03 = FALSE;
    TS0 = 0x08;                         /* trigger TM03                 */
    while ( FALSE == TMIF03 )           /* wait for TM03 count up       */
    {
        NOP();
    }
}

/*******************************************************************************
* Function Name: R_IIC_Master_Send
* Description  : This function starts iica0 in transmision mode.
* Arguments    : adr -
*                    slave address
*                tx_buf -
*                    transfer data pointer
*                tx_num -
*                    transmit data number
* Return Value : none
*                    IIC_SUCCESS: get I2C bus and transmit slave address
*                    MD_ERROR1: I2C bus is busy
********************************************************************************/
uint8_t R_IIC_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{

    uint8_t status;

    status = R_IICA0_bus_check();       /* check I2C bus status         */

    if ( IIC_SUCCESS == status )
    {
        g_iica0_tx_cnt = tx_num;        /* set data number to transmit  */
        gp_iica0_tx_address = tx_buf;   /* set data pointer             */

    /* get I2C bus and start to access I2C bus  */

        IICA0 = adr & 0xFE;             /* transmit slave address       */

    }

    return (status);

}

/*******************************************************************************
* Function Name: R_IIC_Master_Receive
* Description  : This function starts iica0 in receive mode.
* Arguments    : adr -
*                    slave address
*                rx_buf -
*                    received data buffer pointer
*                rx_num -
*                    receive data size+1
* Return Value : status -
*                    IIC_SUCCESS: get I2C bus and transmit slave address
*                    MD_ERROR1: I2C bus is busy
********************************************************************************/
uint8_t R_IIC_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{

    MD_STATUS status;

    status = R_IICA0_bus_check();       /* check I2C bus status         */

    if ( IIC_SUCCESS == status )
    {

    /* get I2C bus and set access parameters    */

        g_iica0_rx_len = rx_num;        /* set data number to transmit  */
        g_iica0_rx_cnt = 0U;
        gp_iica0_rx_address = rx_buf;   /* set data pointer             */

    /* start to access I2C bus  */

        IICA0 = adr | 0x01;             /* transmit slave address       */

    }

    return (status);
}

/*******************************************************************************
* Function Name: R_IIC_check_comstate
* Description  : This function get communication status
* Arguments    : none
* Return Value : result status
                0x00 : success(IIC_SUCCESS)
                0x81 : transmit slave address for transmit(IIC_TX_SVADDR)
                0x01 : transmit operating(IIC_USING)
                0xC1 : transmit slave address for receive(IIC_RX_SVADDR)
                0x41 : receive operating(IIC_RX_MODE)
                0x84 : no ACK from slave for data(NO_ACK)
                0x85 : no ACK for slave address(NO_SLAVE)
********************************************************************************/
uint8_t R_IIC_check_comstate(void)
{

    return( g_iica0_status );

}

/*******************************************************************************
* Function Name: R_IIC_wait_comend
* Description  : This function wait for communication end
* Arguments    : none
* Return Value : result status
                0x00 : success
                other : error
********************************************************************************/
uint8_t R_IIC_wait_comend(void)
{
    uint8_t status;

    while( TRUTH == ( g_iica0_status & STS_MASK ) )
    {
        HALT();                         /* wait for communication end   */
    }

    status = g_iica0_status;            /* set result status            */

    if ( IIC_SUCCESS != status )
    {
        R_IIC_StopCondition();          /* issue STOP conditiopn        */
        wait_time(3);                   /* wait for 20us*3=60us         */
    }

    return( status );
}

/*******************************************************************************
* Function Name: R_IIC_StopCondition
* Description  : This function issue start condition
* Arguments    : none
* Return Value : result
                0x00 : success
                other : error
********************************************************************************/
uint8_t R_IIC_StopCondition(void)
{

    uint8_t status;
    uint16_t wait;

    status = IIC_SUCCESS;               /* set dummy status "success"   */

    SPT0 = TRUTH;                       /* issue stop condition to stop */

    for( wait = WAITTIME ; ( wait > 0 ) && ( FALSE == SPD0 ) ; wait-- )
    {
        NOP();                          /* wait for detection           */
    }

    if ( FALSE == SPD0 )
    {             
       status = MD_ERROR1;              /* not detect stop condition    */
    }

    return (status);
}

/*******************************************************************************
* Function Name: R_IICA0_bus_check
* Description  : This function check I2C bus and issue start condition
* Arguments    : none
* Return Value : status -
*                    IIC_SUCCESS: get I2C bus
*                    MD_ERROR1: I2C bus is busy
********************************************************************************/
uint8_t R_IICA0_bus_check(void)
{
    uint8_t status;
    uint16_t wait;

    /* Check I2C bus is used or in waiting state    */

    status = MD_ERROR1;                 /* set dummy error flag         */


    if ( ( FALSE == IICBSY0 ) || ( TRUTH == MSTS0 ))    /* Check I2C bus */
    {

    /* I2C bus is usable, then issue start condition    */

        STT0 = TRUTH;                   /* issue IICA0 start condition  */

    /* Wait for start condition is detected */

        for( wait = WAITTIME ; ( wait > 0 )&&( FALSE == STD0 ) ; wait--)
        {
            NOP();
        }

        if ( TRUTH == MSTS0 )           /* success to get I2C bus       */
        {
            status = IIC_SUCCESS;       /* set status OK                */
            g_iica0_status = ON_COMMU;  /* set communication flag       */
        }

    }

    return (status);
}

/********************************************************************************
* Function Name: r_iica0_interrupt
* Description  : This function is INTIICA0 interrupt service routine.
* Arguments    : None
* Return Value : None
*********************************************************************************/
#if defined(__CCRL__)
static void r_iica0_interrupt(void)
#elif defined(__ICCRL78__)
#pragma vector = INTIICA0_vect
#pragma bank=1
static __interrupt void r_iica0_interrupt(void)
#elif defined(__GNUC__)
void r_iica0_interrupt(void)
#endif
{

    if ( TRUTH == MSTS0 )               /* check slave mode operation ? */
    {

    /* master mode operation    */

        if ( TRUTH == STD0 )            /* slave address transmit end   */
        {
            if ( TRUTH == ACKD0 )
            {

/*----------------------------------------------
    ACK response and start data transfe
----------------------------------------------*/


                if ( TRUTH == TRC0 )

/*	data transmit mode operation	*/
                {
                    if ( g_iica0_tx_cnt > 0 )
                    {
                        IICA0 = *gp_iica0_tx_address;   /* set transmit data        */
                        gp_iica0_tx_address++;          /* move pointer to next data*/
                        g_iica0_tx_cnt--;               /* count down data number   */
                    }
                    else
                    {
                        g_iica0_status = IIC_SUCCESS;   /* set data transfer end    */
                        u_iica0_callback_master_common( IIC_SUCCESS );
                    }
                }
                else
                {
/*	data receive mode operation	*/

                    WTIM0 = FALSE;              /* set 8bit wait mode           */
                    ACKE0 = TRUTH;
                    
                    IICA0 = DUMMY_DATA;         /* canceling wait and start Rx  */
                }
            }
            else
            {
                g_iica0_status = NO_SLAVE;      /* set communication error flag */
                u_iica0_callback_master_common( NO_SLAVE );
            }

        }
        else
        {

/*----------------------------------------------
    data transfer stage
----------------------------------------------*/
            if ( TRUTH == TRC0 )
            {

    /* transmit operation mode  */

                if ( TRUTH == ACKD0 )
                {

    /* success data transfer and check remaining data   */

                    if ( g_iica0_tx_cnt > 0 )   /* check transmit data number   */
                    {

    /* prepare next transmit data   */

                        IICA0 = *gp_iica0_tx_address;   /* transmit data            */
                        gp_iica0_tx_address++;          /* move pointer to next data*/
                        g_iica0_tx_cnt--;               /* count down data number   */
                    }
                    else
                    {

    /* data transmit complete, set end flag and exit    */

                        g_iica0_status = IIC_SUCCESS;   /* set data transfer end    */
                        u_iica0_callback_master_common( IIC_SUCCESS );

                    }
                }
                else
                {

    /* NACK error   */

                    g_iica0_status = NO_ACK;            /* set communication error flag */
                    u_iica0_callback_master_common( NO_ACK );
                }
            }
            else
            {

    /* receive operation mode   */

                if ( g_iica0_rx_cnt < g_iica0_rx_len )
                {

    /* store recieved data  */

                    *gp_iica0_rx_address = IICA0;   /* store received data  */
                    gp_iica0_rx_address++;          /* move store pointer   */
                    g_iica0_rx_cnt++;               /* countup data number  */
                    if ( g_iica0_rx_cnt == g_iica0_rx_len )
                    {

    /* receive last data operation (send NACK and change to 9 clock wait    */

                        ACKE0 = FALSE;              /* NACK for last data   */
                        WTIM0 = TRUTH;              /* change to 9clock wait*/
                        WREL0 = TRUTH;              /* canceling wait to end*/
                    }
                    else
                    {
                        WREL0 = TRUTH;              /* start next reception */
                    }
                }
                else
                {

    /* receive operation complete at 9th clock  */

                    g_iica0_status = IIC_SUCCESS;   /* set data receive end */
                    u_iica0_callback_master_common( IIC_SUCCESS );


                }

            }

        }

    /* master transfer operation end    */

    }

}


