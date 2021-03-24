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
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for Serial module.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart3_tx_address;         /* uart3 send buffer address */
extern volatile uint16_t  g_uart3_tx_count;            /* uart3 send data number */
extern volatile uint8_t * gp_uart3_rx_address;         /* uart3 receive buffer address */
extern volatile uint16_t  g_uart3_rx_count;            /* uart3 receive data number */
extern volatile uint16_t  g_uart3_rx_length;           /* uart3 receive data length */
/* Start user code for global. Do not edit comment generated here */

extern TaskHandle_t       g_uart3_tx_task;             /* uart3 send task */
extern volatile bool      g_uart3_tx_ready_flag;       /* uart3 send end flag */
extern TaskHandle_t       g_uart3_rx_task;             /* uart3 receive task */

extern void U_UART3_Receive_Stop(void);                /* for internal use */
extern void U_UART3_Send_Stop(void);                   /* for internal use */

static void u_uart3_rx_callback_common(uint16_t stat); /* uart3 receive common callback */

#define r_uart3_interrupt_receive R_CG_FREERTOS_INTERRUPT_EI(r_uart3_interrupt_receive)
#define r_uart3_interrupt_send R_CG_FREERTOS_INTERRUPT_EI(r_uart3_interrupt_send)

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart3_interrupt_receive
* Description  : This function is INTSR3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart3_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR13 & 0x0007U);
    SIR13 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart3_callback_error(err_type);
    }
    
    rx_data = RXD3;

    if (g_uart3_rx_length > g_uart3_rx_count)
    {
        *gp_uart3_rx_address = rx_data;
        gp_uart3_rx_address++;
        g_uart3_rx_count++;

        if (g_uart3_rx_length == g_uart3_rx_count)
        {
            r_uart3_callback_receiveend();
        }
    }
    else
    {
        r_uart3_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart3_interrupt_send
* Description  : This function is INTST3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart3_interrupt_send(void)
{
    if (g_uart3_tx_count > 0U)
    {
        TXD3 = *gp_uart3_tx_address;
        gp_uart3_tx_address++;
        g_uart3_tx_count--;
    }
    else
    {
        r_uart3_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart3_callback_receiveend
* Description  : This function is a callback function when UART3 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart3_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_uart3_rx_callback_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart3_callback_softwareoverrun
* Description  : This function is a callback function when UART3 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart3_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */

    INTERNAL_NOT_USED( rx_data );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart3_callback_sendend
* Description  : This function is a callback function when UART3 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart3_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */

    U_UART3_Send_Stop();

    g_uart3_tx_ready_flag = true;

    vTaskNotifyGiveFromISR_R_Helper( &g_uart3_tx_task );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart3_callback_error
* Description  : This function is a callback function when UART3 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart3_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */

    u_uart3_rx_callback_common( (err_type << 8) | MD_RECV_ERROR );

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: u_uart3_rx_callback_common
* Description  : This function is a callback function when UART3 finishes transmission or UART3 reception error occurs.
* Arguments    : stat -
*                    status value (lower 8bits) with error type value (upper 8bits)
* Return Value : None
***********************************************************************************************************************/
static void u_uart3_rx_callback_common(uint16_t stat)
{
    U_UART3_Receive_Stop();

    xTaskNotifyFromISR_R_Helper( &g_uart3_rx_task, 0x10000 | stat );
}

/* End user code. Do not edit comment generated here */
