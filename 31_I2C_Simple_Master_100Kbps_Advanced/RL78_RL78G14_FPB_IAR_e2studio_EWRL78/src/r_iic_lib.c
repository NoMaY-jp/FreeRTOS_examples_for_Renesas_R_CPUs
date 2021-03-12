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

/********************************************************************************
* File Name    : r_iic_lib.c
* Version      : 2.00
* Device(s)    : R5F100LE
* Tool-Chain   : CCRL
* Description  : This file implements IIC control functions.
* Creation Date: 2015/09/28
* Modified Date: 2015/10/06 unify inteface
* Modified Date: 2016/01/06 support read mode
* Modified Date: 2016/06/06 add retry function and porting to CC-RL
* Modified Date: 2017/12/18 delete unused POM1 handling from IIC_TM03_init
* Modified Date: 2021/03/10 copy some definitions from r_cg_userdefine.h
* Modified Date: 2021/03/10 add support of callback for FreeRTOS
* Modified Date: 2021/03/12 add support of GNURL78 and ICCRL78
********************************************************************************/

/********************************************************************************
Includes
********************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_iic_lib.h"

void u_iic00_callback_master_common(MD_STATUS flag);    /* for FreeRTOS */

/********************************************************************************
Pragma directive
********************************************************************************/
#if defined(__CCRL__)
#pragma interrupt r_iic00_interrupt(vect=INTIIC00,bank=RB1)
#pragma interrupt r_tau0_channel3_interrupt(vect=INTTM03,bank=RB2)
#endif

/********************************************************************************
Global variables
********************************************************************************/
/* add constant data define as following :@2016/01/06   */

#define     IIC_ERROR       ( 0xFFU )   /* IIC operation error          */
#define     IIC00_TRG       ( 0x0001U ) /* IIC00 and SO trigger data    */
#define     IIC00_CK        ( 0x0100U ) /* IIC00 CK trigger data        */
#define     DUMMY_DATA      ( 0xFFU )   /* dummy data for receive       */
#define     IIC_WAIT_TIME   ( 4U )      /* wait after INTIIC is 5*4=20us*/

volatile uint8_t   g_iic00_status;      /* iic00 master operationflag   */
/*  00 : operation end witout error(IIC_SUCCESS)
    81 : slave address transmit for transmit mode(IIC_TX_SVADDR)
    01 : data transmit mode(IIC_USING)
    C1 : slave address transmit for receive mode(IIC_RX_SVADDR)
    41 : data receive mode(IIC_RX_MODE)
    84 : no ACK from slave for data(NO_ACK)
    85 : no ACK for slave address(NO_SLAVE)
*/
volatile uint8_t * gp_iic00_rx_addr;    /* iic00 receive buffer address */
volatile uint16_t  g_iic00_rx_len;      /* iic00 receive data length    */
volatile uint16_t  g_iic00_rx_cnt;      /* iic00 receive data count     */
volatile uint8_t * gp_iic00_tx_addr;    /* iic00 send buffer address    */
volatile uint16_t  g_iic00_tx_cnt;      /* iic00 send data count        */

/* copy following 4 + 1 definitions from r_cg_userdefine.h :@2021/03/10 */
#define     TRUTH           ( 1U )
#define     FALSE           ( 0U )
#define     INT_MASK        ( 1U )      /* mask interrupt request flag  */
#define     INT_ENABLE      ( 0U )      /* for interrupt mask clear     */

/* add one definition for TM03 :2016/01/06  */
#define     TM03_trigger    ( 0x08U )   /* TM03 start trigger data      */

volatile uint8_t   g_TM03flag;          /* TM03 count end flag          */
volatile uint8_t   g_TM03cnt;           /* TM03 counter                 */

/********************************************************************************
Local functions
********************************************************************************/

void r_iic00_startcondition(void);     /* issue START condition        */
void r_iic00_stop(void);                /* stop IIC00 operation         */
void wait_5us(void);                    /* wait for 5us SCL width       */

/*******************************************************************************
* Function Name: IIC_TM03_init
* Description  : disable external edge and start TM03 and enable IIC00 interrupt
* Arguments    : none
* Return Value : none
********************************************************************************/
void IIC_TM03_init(void)
{
    TMR03 = 0x09;                       /* disable edge detection   */
    TS0 = TM03_trigger;                 /* enable TM03              */
    TMMK03 = INT_MASK;                  /* mask TM03 interrupt      */
    g_TM03flag = FALSE;                 /* clear TM03 flag          */
    g_TM03cnt = 0x00;                   /* clear 5us counter        */
    IICMK00 = INT_ENABLE;               /* enable IIC00 interrupt   */
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
* Description  : This function wait time
* Arguments    : none
* Return Value : none
********************************************************************************/
void wait_20us(void)
{
    uint8_t cnt;
    for ( cnt = 4 ; cnt > 0 ; cnt-- )
    {
        TMIF03 = FALSE;                 /* clear INTTM03 request flag   */
        TS0 = TM03_trigger;             /* trigger TM03 count           */
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
    TS0 = TM03_trigger;                 /* trigger TM03                 */
    while ( FALSE == TMIF03 )           /* wait for TM03 count up       */
    {
        NOP();
    }
}


/*******************************************************************************
* Function Name: R_IIC_Master_Send
* Description  : This function starts IIC00 in transmision mode.
* Arguments    : adr -
*                    slave address
*                tx_buf -
*                    transfer data pointer
*                tx_num -
*                    transmit data number
* Return Value : MD_OK
********************************************************************************/
uint8_t R_IIC_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{

    /* copy transfer parameters */
    g_iic00_tx_cnt = tx_num;            /* set data number to transmit  */
    gp_iic00_tx_addr = tx_buf;          /* set data pointer             */

    /* start to access IIC bus  */

    r_iic00_startcondition();           /* issue start condition        */

    g_iic00_status = IIC_TX_SVADDR;     /* set execution flag           */

    SIO00 = adr & 0xFE;                 /* transmit slave address       */
    g_TM03flag = FALSE;                 /* clear enable next operation  */

    return( MD_OK );

}

/*******************************************************************************
* Function Name: R_IIC_Master_Receive
* Description  : This function starts IIC00 in receive mode.
* Arguments    : adr -
*                    slave address
*                rx_buf -
*                    received data buffer pointer
*                rx_num -
*                    receive data size+1
* Return Value : MD_OK
********************************************************************************/
uint8_t R_IIC_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{

    /* copy parameters  */

    g_iic00_rx_len = rx_num;            /* set data number to transmit  */
    g_iic00_rx_cnt = 0x00;
    gp_iic00_rx_addr = rx_buf;          /* set data pointer             */

    /* start to access IIC bus  */

    r_iic00_startcondition();          /* issue start condition        */

    g_iic00_status = IIC_RX_SVADDR;     /* set RX execution flag        */

    SIO00 = adr | 0x01;                 /* transmit slave address       */
    g_TM03flag = FALSE;                 /* clear enable next operation  */

    return( MD_OK );

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

    return( g_iic00_status );

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

    while( IIC_USING == (g_iic00_status & IIC_STS_MASK) )
    {
        HALT();                         /* wait for communication end   */
    }

    status = g_iic00_status;            /* set result status            */

    if ( IIC_SUCCESS != status )
    {
        R_IIC_StopCondition();          /* issue STOP conditiopn        */
        wait_time(3);                   /* wait for 60us                */
    }

    return( status );
}

/*******************************************************************************
* Function Name: R_IIC_StopCondition
* Description  : This function issue start condition
* Arguments    : none
* Return Value : MD_OK
********************************************************************************/
uint8_t R_IIC_StopCondition(void)
{

    wait_time(1);                       /* dummy wait 20us              */

    r_iic00_stop();                     /* stop IIC00 operation         */

    SO0 &= (~IIC00_TRG);                /* fall down SDA signal         */

    wait_5us();                         /* for command interval         */

    SO0 |= IIC00_CK;                    /* rise up SCL signal           */

    wait_5us();                         /* for command interval         */

    SO0 |= IIC00_TRG;                   /* rise up SDA signal           */

    wait_time(1);

    SCR00 &= (~0xC000);                  /* clear TX and RX enable        */

    g_TM03flag = FALSE;

    return( MD_OK );

}


/*******************************************************************************
* Function Name: r_iic00_startcondition
* Description  : This function issue start condition
* Arguments    : none
* Return Value : none
********************************************************************************/
void r_iic00_startcondition(void)
{

    r_iic00_stop();                     /* stop IIC00 operation         */

    wait_5us();                         /* wait for bus free time       */

    if ( FALSE  == (SO0 & IIC00_TRG) )
    {
        SO0 |= IIC00_TRG;               /* rise SO00 bit                */
        wait_5us();                     /* wait for bus free time       */
    }

    if ( FALSE == (SO0 & IIC00_CK) )
    {
        SO0 |= IIC00_CK;                /* rise up SCL signal           */
        wait_5us();
    }
    
    SO0 &= (~IIC00_TRG);                /* fall down SDA when in SCL=H  */

    wait_5us();                         /* wait for bus setup time      */

    SO0 &= (~IIC00_CK);                   /* fall down SCL after SDA =L   */

    SOE0 |= IIC00_TRG;                  /* enable channel00 output      */
    SCR00 |= 0x8000;                    /* set transmit enable          */
    SS0 = IIC00_TRG;                    /* enable channel00 operation   */

    wait_5us();                         /* for command interval         */

}

/*******************************************************************************
* Function Name: r_iic00_stop
* Description  : This function stop IIC00 operation
* Arguments    : none
* Return Value : none
********************************************************************************/
void r_iic00_stop(void)
{

    uint8_t work;

    wait_time(1);                       /* dummy wait 20us              */

    work = 0x40 & SSR00L;
    if ( FALSE != work )
    {
        while ( FALSE != work )         /* wait for previous execution  */
        {
            work = SSR00L;              /* get IIC00 status             */
            work &= 0x40;               /* get transfer status (TSF)    */
        }
        wait_5us();
    }

    SOE0 &= (~IIC00_TRG);               /* disable IIC00 output         */
    ST0L = IIC00_TRG;                   /* stop channel 0 operation     */

}

/********************************************************************************
* Function Name: r_iic00_interrupt
* Description  : This function is IIC interrupt service routine.
                ( only start TM03 operation. )
* Arguments    : None
* Return Value : None
*********************************************************************************/
#if defined(__CCRL__)
static void r_iic00_interrupt(void)
#elif defined(__ICCRL78__)
#pragma vector = INTIIC00_vect
#pragma bank=1
static __interrupt void r_iic00_interrupt(void)
#elif defined(__GNUC__)
void r_iic00_interrupt(void)
#endif
{

    TMIF03 = FALSE;                     /* clear TM03 interrupt flag    */
    TS0 = TM03_trigger;                 /* start TM03                   */
    g_TM03cnt = (IIC_WAIT_TIME-1);      /* set 5us * 4 = 20us           */
    TMMK03 = INT_ENABLE;                /* enable TM03 vector interrupt */
    g_TM03flag = TRUTH;
}

/********************************************************************************
* Function Name: r_tau0_channel3_interrupt
* Description  : This function is TM03 interrupt service routine.
                ( actual procedure of IIC01 interrupt. )
* Arguments    : None
* Return Value : None
*********************************************************************************/
#if defined(__CCRL__)
static void r_tau0_channel3_interrupt(void)
#elif defined(__ICCRL78__)
#pragma vector = INTTM03_vect
#pragma bank=2
static __interrupt void r_tau0_channel3_interrupt(void)
#elif defined(__GNUC__)
void r_tau0_channel3_interrupt(void)
#endif
{

    uint8_t work;                       /* work register            */

  if ( 0 == g_TM03cnt )
    {
        if( TRUTH == g_TM03flag )
        {
            g_TM03flag = FALSE;         /* clear TM03 count flag    */

            TMMK03 = INT_MASK;          /* mask TM03 interrupt      */

            if ( IIC_RX_MODE == g_iic00_status )
            {                           /* IIC00 is working         */
/*--------------------------------------------------------------
        data receive operation

    store received data
----------------------------------------------------------------*/
                (*gp_iic00_rx_addr) = SIO00;    /* store RX data    */

                g_iic00_rx_cnt++;       /* count up received data   */

                if ( g_iic00_rx_cnt < g_iic00_rx_len )
                {
/*-------------------------------------------------------------------
        more data to receive is exist

    change parameters
---------------------------------------------------------------------*/

                    gp_iic00_rx_addr++;

                    if ( g_iic00_rx_cnt == ( g_iic00_rx_len -1) )
                    {
/*-------------------------------------------------------------------
    next data is last data to receive. then NACK response
---------------------------------------------------------------------*/

                        SOE0 &= (~IIC00_TRG);
                    }
                    else
                    {
                        /* do nothing */
                    }

/* start to receive next data   */
                    SIO00 = DUMMY_DATA;
                }
                else                /* data receive complete    */
                {
/*-------------------------------------------------------------------
    complete receive data
---------------------------------------------------------------------*/
                    g_iic00_status = IIC_SUCCESS;  /* set success flag */
                    u_iic00_callback_master_common( IIC_SUCCESS );
                }
            }
            else
            {
/*--------------------------------------------------------------
        IIC00 data transfer end

    check ACK from slave 

----------------------------------------------------------------*/

                work = SSR00L;          /* read result falg         */
                SIR00L = work;          /* clear error flag         */

                if( work & 0x02 )
                {

/*  NACK from slave          */

                    if ( (g_iic00_status & 0x80) )
                    {                   /* address transmit         */
                        g_iic00_status = NO_SLAVE;
                        u_iic00_callback_master_common( NO_SLAVE );
                    }
                    else
                    {                   /* NACK for data            */
                        g_iic00_status = NO_ACK;
                        u_iic00_callback_master_common( NO_ACK );
                    }
                }
                else                    /* ACK from slave           */
                {
                    g_iic00_status &= 0x7F; /* clear address bit    */

                    if( IIC_USING == g_iic00_status ) /* transmit data operation */
                    {
/*--------------------------------------------------------------
        data is transmitted 
----------------------------------------------------------------*/

                        if ( g_iic00_tx_cnt > 0 ) /* more data to transmit */
                        {               /* set transmit data        */
                            SIO00 = (*gp_iic00_tx_addr);
                            gp_iic00_tx_addr++;
                            g_iic00_tx_cnt--;
                        }
                        else            /* data transmit complete   */
                        {
                            g_iic00_status = IIC_SUCCESS; /* set data transfer end    */
                            u_iic00_callback_master_common( IIC_SUCCESS );
                        }
                    }
                    else 

/*===================================================================

    slave address for receive operation

    g_iic00_status = 0xC1 -> 0x41 : slave address is transmitted

=====================================================================*/
                    {

/*-------------------------------------------------------------------
    slave address end and start receive
---------------------------------------------------------------------*/
                        ST0 = IIC00_TRG;/* stop IIC00 operaion      */
                        SCR00 ^= 0xC000;/* TX disable & enable RX   */
                        SS0 = IIC00_TRG;/* start IIC00 operation    */
                        SIO00 = DUMMY_DATA;/* trigger RX operation     */
                        g_iic00_status = IIC_RX_MODE;/* change status */
                    }
                }
            }

        }
    }
    else
    {
        g_TM03cnt--;
        TS0 = TM03_trigger;             /* start TM03                   */
    }

}

