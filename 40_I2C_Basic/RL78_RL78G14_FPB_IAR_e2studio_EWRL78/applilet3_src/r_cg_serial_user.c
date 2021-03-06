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
* Version      : Applilet3 for RL78/G14 V2.05.05.01 [25 Nov 2020]
* Device(s)    : R5F104ML
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for Serial module.
* Creation Date: 
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */

#include "freertos_start.h"

#include "r_cg_userdefine.h"
#if 0
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */
extern uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
extern uint8_t   g_iica1_master_status_flag;  /* iica1 master flag */
extern uint8_t   g_iica1_slave_status_flag;   /* iica1 slave flag */
extern uint8_t * gp_iica1_rx_address;         /* iica1 receive buffer address */
extern uint16_t  g_iica1_rx_cnt;              /* iica1 receive data length */
extern uint16_t  g_iica1_rx_len;              /* iica1 receive data count */
extern uint8_t * gp_iica1_tx_address;         /* iica1 send buffer address */
extern uint16_t  g_iica1_tx_cnt;              /* iica1 send data count */
/* Start user code for global. Do not edit comment generated here */
#endif /* #if 0 */

extern volatile uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */ 
extern volatile uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern volatile uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern volatile uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern volatile uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern volatile uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
extern volatile uint8_t   g_iica1_master_status_flag;  /* iica1 master flag */ 
extern volatile uint8_t   g_iica1_slave_status_flag;   /* iica1 slave flag */
extern volatile uint8_t * gp_iica1_rx_address;         /* iica1 receive buffer address */
extern volatile uint16_t  g_iica1_rx_cnt;              /* iica1 receive data length */
extern volatile uint16_t  g_iica1_rx_len;              /* iica1 receive data count */
extern volatile uint8_t * gp_iica1_tx_address;         /* iica1 send buffer address */
extern volatile uint16_t  g_iica1_tx_cnt;              /* iica1 send data count */

extern TaskHandle_t       g_iica0_master_task;         /* iica0 master task */
extern TaskHandle_t       g_iica1_slave_task;          /* iica1 slave task */

static volatile uint32_t  g_iica1_slave_notification;  /* iica1 slave notification */

extern void U_IICA0_Master_Send_Receive_Stop(void);    /* for internal use */
extern void U_IICA1_Slave_Send_Receive_Stop(void);     /* for internal use */

static void u_iica0_callback_master_common(MD_STATUS flag);    /* iica0 master common callback */
static void u_iica1_callback_slave_common(MD_STATUS flag);     /* iica1 slave common callback */
static void u_wdt_request_interrupt(void);             /* wdt interrupt (as a software intetrrupt) request */

static inline void WTIM1_controle_patch(uint8_t val)
{
    if (1U == val) { WTIM1 = 1U; } else { WTIM1 = 0U; }
}
static volatile uint8_t WTIM1_controle_ignore;
#undef WTIM1
#define WTIM1 WTIM1_controle_patch(TRC1); WTIM1_controle_ignore

#define r_iica0_interrupt R_CG_FREERTOS_INTERRUPT_EI(r_iica0_interrupt)
#define r_iica1_interrupt R_CG_INTERRUPT_EI(r_iica1_interrupt)
#define u_wdt_interrupt R_CG_FREERTOS_INTERRUPT_EI(u_wdt_interrupt)

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : This function is INTIICA0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTIICA0_vect
__interrupt static void r_iica0_interrupt(void)
{
    if ((IICS0 & _80_IICA_STATUS_MASTER) == 0x80U)
    {
        iica0_master_handler();
    }
}

/***********************************************************************************************************************
* Function Name: iica0_master_handler
* Description  : This function is IICA0 master handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica0_master_handler(void)
{
    /* Control for communication */
    if ((0U == IICBSY0) && (g_iica0_tx_cnt != 0U))
    {
        r_iica0_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if ((g_iica0_master_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (1U == ACKD0)
            {
                g_iica0_master_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                
                if (1U == TRC0)
                {
                    WTIM0 = 1U;
                    
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE0 = 1U;
                    WTIM0 = 0U;
                    WREL0 = 1U;
                }
            }
            else
            {
                r_iica0_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (1U == TRC0)
            {
                if ((0U == ACKD0) && (g_iica0_tx_cnt != 0U))
                {
                    r_iica0_callback_master_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;
                    
                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        ACKE0 = 0U;
                        WTIM0 = 1U;
                        WREL0 = 1U;
                    }
                    else
                    {
                        WREL0 = 1U;
                    }
                }
                else
                {
                    r_iica0_callback_master_receiveend();
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_error
* Description  : This function is a callback function when IICA0 master error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */

    u_iica0_callback_master_common( flag );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_receiveend
* Description  : This function is a callback function when IICA0 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_receiveend(void)
{
    SPT0 = 1U;
    /* Start user code. Do not edit comment generated here */

    u_iica0_callback_master_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_sendend
* Description  : This function is a callback function when IICA0 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_sendend(void)
{
    SPT0 = 1U;
    /* Start user code. Do not edit comment generated here */

    u_iica0_callback_master_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica1_interrupt
* Description  : This function is INTIICA1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTIICA1_vect
__interrupt static void r_iica1_interrupt(void)
{
    if ((IICS1 & _80_IICA_STATUS_MASTER) == 0U)
    {
        iica1_slave_handler();
    }
}

/***********************************************************************************************************************
* Function Name: iica1_slave_handler
* Description  : This function is IICA1 slave handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica1_slave_handler(void)
{
    /* Control for stop condition */
    if (1U == SPD1)
    {    
        /* Get stop condition */
        SPIE1 = 0U;
        g_iica1_slave_status_flag = 1U;
    }
    else
    {
        if ((g_iica1_slave_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (1U == COI1)
            {
                SPIE1 = 1U;
                g_iica1_slave_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                
                if (1U == TRC1)
                {
                    WTIM1 = 1U;
                    
                    if (g_iica1_tx_cnt > 0U)
                    {
                        IICA1 = *gp_iica1_tx_address;
                        gp_iica1_tx_address++;
                        g_iica1_tx_cnt--;
                    }
                    else
                    {
                        r_iica1_callback_slave_sendend();
                        WREL1 = 1U;
                    }
                }
                else
                {
                    ACKE1 = 1U;
                    WTIM1 = 0U;
                    WREL1 = 1U;
                }
            }
            else
            {
                r_iica1_callback_slave_error(MD_ERROR);
            }
        }
        else
        {
            if (1U == TRC1)
            {
                if ((0U == ACKD1) && (g_iica1_tx_cnt != 0U))
                {
                    r_iica1_callback_slave_error(MD_NACK);
                }
                else
                {
                    if (g_iica1_tx_cnt > 0U)
                    {
                        IICA1 = *gp_iica1_tx_address;
                        gp_iica1_tx_address++;
                        g_iica1_tx_cnt--;
                    }
                    else
                    {
                        r_iica1_callback_slave_sendend();
                        WREL1 = 1U;
                    }
                }
            }
            else
            {
                if (g_iica1_rx_cnt < g_iica1_rx_len)
                {
                    *gp_iica1_rx_address = IICA1;
                    gp_iica1_rx_address++;
                    g_iica1_rx_cnt++;
                    
                    if (g_iica1_rx_cnt == g_iica1_rx_len)
                    {
                        WTIM1 = 1U;
                        WREL1 = 1U;
                        r_iica1_callback_slave_receiveend();
                    }
                    else
                    {
                        WREL1 = 1U;
                    }
                }
                else
                {
                    WREL1 = 1U;
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iica1_callback_slave_error
* Description  : This function is a callback function when IICA1 slave error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica1_callback_slave_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */

    u_iica1_callback_slave_common( flag );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica1_callback_slave_receiveend
* Description  : This function is a callback function when IICA1 finishes slave reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica1_callback_slave_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_iica1_callback_slave_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica1_callback_slave_sendend
* Description  : This function is a callback function when IICA1 finishes slave transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica1_callback_slave_sendend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_iica1_callback_slave_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name: u_iica0_callback_master_common
* Description  : This function is a callback function when IICA0 finishes
*                master transmission/reception or IICA0 master error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
******************************************************************************/
static void u_iica0_callback_master_common(MD_STATUS flag)
{
    U_IICA0_Master_Send_Receive_Stop();

    xTaskNotifyFromISR_R_Helper( &g_iica0_master_task, 0x10000 | flag );
}

/******************************************************************************
* Function Name: u_iica1_callback_slave_common
* Description  : This function is a callback function when IICA1 finishes
*                slave transmission/reception or IICA1 slave error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
******************************************************************************/
static void u_iica1_callback_slave_common(MD_STATUS flag)
{
    U_IICA1_Slave_Send_Receive_Stop();

    /* If there are no tasks waiting for a notification or a notification was already
     * sent (or is going to be sent), i.e. 
     * when g_iica1_slave_task == NULL || g_iica1_slave_notification != 0U,
     * a notification isn't sent or is skipped.
     */
    if (NULL != g_iica1_slave_task && 0U == g_iica1_slave_notification)
    {
        /* Generate INTWDTI interrupt manually as a software intetrrupt. The interrupt
         * priority of INTIICA1 is configured higher than the SYSCALL interrupt priority.
         * On the other hand, the interrupt priority of INTWDTI is configured within
         * the SYSCALL interrupt priority.
         */
        g_iica1_slave_notification = 0x10000 | flag;
        u_wdt_request_interrupt();
    }
}

/******************************************************************************
* Function Name: u_wdt_request_interrupt
* Description  : This function requests INTWDT interrupt (as a software intetrrupt).
* Arguments    : None
* Return Value : None
******************************************************************************/
static void u_wdt_request_interrupt(void)
{
    WDTIIF = 1U;   /* set INTWDTI interrupt flag (as a software interrupt) */
    WDTIMK = 0U;   /* enable INTWDTI interrupt (as a software interrupt) */
}

/******************************************************************************
* Function Name: u_wdt_interrupt
* Description  : This function is INTWDT interrupt (as a software intetrrupt) service routine.
* Arguments    : None
* Return Value : None
******************************************************************************/
#pragma vector = INTWDTI_vect
__interrupt static void u_wdt_interrupt(void)
{
    /* Note that g_iica1_slave_task is automatically set to NULL after posting a notification,
     * but g_iica1_slave_notification isn't automatically set to 0, so is manually set to 0.
     */
    xTaskNotifyFromISR_R_Helper( &g_iica1_slave_task, g_iica1_slave_notification );
    g_iica1_slave_notification = 0U;
}

/* End user code. Do not edit comment generated here */
