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
extern uint8_t   g_iic00_master_status_flag;  /* iic00 start flag for send address check by master mode */
extern uint8_t * gp_iic00_tx_address;         /* iic00 send data pointer by master mode */
extern uint16_t  g_iic00_tx_count;            /* iic00 send data size by master mode */
extern uint8_t * gp_iic00_rx_address;         /* iic00 receive data pointer by master mode */
extern uint16_t  g_iic00_rx_count;            /* iic00 receive data size by master mode */
extern uint16_t  g_iic00_rx_length;           /* iic00 receive data length by master mode */
extern uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */
extern uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
#endif /* #if 0 */

extern volatile uint8_t   g_iic00_master_status_flag;  /* iic00 start flag for send address check by master mode */
extern volatile uint8_t * gp_iic00_tx_address;         /* iic00 send data pointer by master mode */
extern volatile uint16_t  g_iic00_tx_count;            /* iic00 send data size by master mode */
extern volatile uint8_t * gp_iic00_rx_address;         /* iic00 receive data pointer by master mode */
extern volatile uint16_t  g_iic00_rx_count;            /* iic00 receive data size by master mode */
extern volatile uint16_t  g_iic00_rx_length;           /* iic00 receive data length by master mode */
extern volatile uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */ 
extern volatile uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern volatile uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern volatile uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern volatile uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern volatile uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */

extern TaskHandle_t       g_iic00_master_task;         /* iic00 master task */
extern TaskHandle_t       g_iica0_slave_task;          /* iica0 slave task */

static volatile uint32_t  g_iica0_slave_notification;  /* iica0 slave notification */

extern void U_IIC00_Master_Send_Receive_Stop(void);    /* for internal use */
extern void U_IICA0_Slave_Send_Receive_Stop(void);     /* for internal use */

static void u_iic00_callback_master_common(MD_STATUS flag);    /* iic00 master common callback */
static void u_iica0_callback_slave_common(MD_STATUS flag);     /* iica0 slave common callback */
static void u_wdt_request_interrupt(void);             /* wdt interrupt (as a software intetrrupt) request */

static void u_cg_task_notify_request_from_high_priority_isr(TaskHandle_t xTask, volatile uint32_t *pulNotifyVariable, uint32_t ulNotifyValue);
static void u_cg_task_notify_requested_from_high_priority_isr(TaskHandle_t *pxTask, volatile uint32_t *pulNotifyVariable);

static inline void WTIM0_controle_patch(uint8_t val)
{
    if (1U == val) { WTIM0 = 1U; } else { WTIM0 = 0U; }
}
static volatile uint8_t WTIM0_controle_ignore;
#undef WTIM0
#define WTIM0 WTIM0_controle_patch(1U); WTIM0_controle_ignore

#define r_iic00_interrupt R_CG_FREERTOS_INTERRUPT_EI(r_iic00_interrupt)
#define r_iica0_interrupt R_CG_INTERRUPT_EI(r_iica0_interrupt)
#define u_wdt_interrupt R_CG_FREERTOS_INTERRUPT_EI(u_wdt_interrupt)

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_iic00_interrupt
* Description  : This function is INTIIC00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTIIC00_vect
__interrupt static void r_iic00_interrupt(void)
{
    volatile uint16_t w_count;

    for (w_count = 0U; w_count <= IIC00_WAITTIME_2; w_count++)
    {
        NOP();
    }

    if (((SSR00 & _0002_SAU_PARITY_ERROR) == 0x0002U) && (g_iic00_tx_count != 0U))
    {
        r_iic00_callback_master_error(MD_NACK);
    }
    else if(((SSR00 & _0001_SAU_OVERRUN_ERROR) == 0x0001U) && (g_iic00_tx_count != 0U))
    {
        r_iic00_callback_master_error(MD_OVERRUN);
    }
    else
    {
        /* Control for master send */
        if ((g_iic00_master_status_flag & _01_SAU_IIC_SEND_FLAG) == 1U)
        {
            if (g_iic00_tx_count > 0U)
            {
                SIO00 = *gp_iic00_tx_address;
                gp_iic00_tx_address++;
                g_iic00_tx_count--;
            }
            else
            {
                /* IIC master transmission finishes and a callback function can be called here. */
                r_iic00_callback_master_sendend();
            }
        }
        /* Control for master receive */
        else 
        {
            if ((g_iic00_master_status_flag & _04_SAU_IIC_SENDED_ADDRESS_FLAG) == 0U)
            {
                ST0 |= _0001_SAU_CH0_STOP_TRG_ON;
                SCR00 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
                SCR00 |= _4000_SAU_RECEPTION;
                SS0 |= _0001_SAU_CH0_START_TRG_ON;
                g_iic00_master_status_flag |= _04_SAU_IIC_SENDED_ADDRESS_FLAG;
                
                if (g_iic00_rx_length == 1U)
                {
                    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable IIC00 out */
                }
                
                SIO00 = 0xFFU;
            }
            else
            {
                if (g_iic00_rx_count < g_iic00_rx_length)
                {
                    *gp_iic00_rx_address = SIO00;
                    gp_iic00_rx_address++;
                    g_iic00_rx_count++;
                    
                    if (g_iic00_rx_count == (g_iic00_rx_length - 1U))
                    {
                        SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable IIC00 out */
                        SIO00 = 0xFFU;
                    }
                    else if (g_iic00_rx_count == g_iic00_rx_length)
                    {
                        /* IIC master reception finishes and a callback function can be called here. */
                        r_iic00_callback_master_receiveend();
                    }
                    else
                    {
                        SIO00 = 0xFFU;
                    }
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iic00_callback_master_error
* Description  : This function is a callback function when IIC00 master error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
***********************************************************************************************************************/
static void r_iic00_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */

    u_iic00_callback_master_common( flag );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic00_callback_master_receiveend
* Description  : This function is a callback function when IIC00 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic00_callback_master_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_iic00_callback_master_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic00_callback_master_sendend
* Description  : This function is a callback function when IIC00 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic00_callback_master_sendend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_iic00_callback_master_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : This function is INTIICA0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTIICA0_vect
__interrupt static void r_iica0_interrupt(void)
{
    if ((IICS0 & _80_IICA_STATUS_MASTER) == 0U)
    {
        iica0_slave_handler();
    }
}

/***********************************************************************************************************************
* Function Name: iica0_slave_handler
* Description  : This function is IICA0 slave handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica0_slave_handler(void)
{
    /* Control for stop condition */
    if (1U == SPD0)
    {    
        /* Get stop condition */
        SPIE0 = 0U;
        g_iica0_slave_status_flag = 1U;
    }
    else
    {
        if ((g_iica0_slave_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (1U == COI0)
            {
                SPIE0 = 1U;
                g_iica0_slave_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                
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
                        r_iica0_callback_slave_sendend();
                        WREL0 = 1U;
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
                r_iica0_callback_slave_error(MD_ERROR);
            }
        }
        else
        {
            if (1U == TRC0)
            {
                if ((0U == ACKD0) && (g_iica0_tx_cnt != 0U))
                {
                    r_iica0_callback_slave_error(MD_NACK);
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
                        r_iica0_callback_slave_sendend();
                        WREL0 = 1U;
                    }
                }
            }
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;
                    
                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        WTIM0 = 1U;
                        WREL0 = 1U;
                        r_iica0_callback_slave_receiveend();
                    }
                    else
                    {
                        WREL0 = 1U;
                    }
                }
                else
                {
                    WREL0 = 1U;
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_error
* Description  : This function is a callback function when IICA0 slave error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_slave_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */

    u_iica0_callback_slave_common( flag );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_receiveend
* Description  : This function is a callback function when IICA0 finishes slave reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_slave_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_iica0_callback_slave_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_sendend
* Description  : This function is a callback function when IICA0 finishes slave transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_slave_sendend(void)
{
    /* Start user code. Do not edit comment generated here */

    u_iica0_callback_slave_common( MD_OK );

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name: u_iic00_callback_master_common
* Description  : This function is a callback function when IIC00 finishes
*                master transmission/reception or IIC00 master error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
******************************************************************************/
static void u_iic00_callback_master_common(MD_STATUS flag)
{
    U_IIC00_Master_Send_Receive_Stop();

    xTaskNotifyFromISR_R_Helper( &g_iic00_master_task, 0x10000 | flag );
}

/******************************************************************************
* Function Name: u_iica0_callback_slave_common
* Description  : This function is a callback function when IICA0 finishes
*                slave transmission/reception or IICA0 slave error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
******************************************************************************/
static void u_iica0_callback_slave_common(MD_STATUS flag)
{
    U_IICA0_Slave_Send_Receive_Stop();

    u_cg_task_notify_request_from_high_priority_isr( g_iica0_slave_task, &g_iica0_slave_notification, 0x10000 | flag );
}

/******************************************************************************
* Function Name: u_cg_task_notify_request_from_high_priority_isr
* Description  : This function helps a task notification from high priority ISR to task.
* Arguments    : xTask -
*                    Task handler
*                pulNotifyVariable -
*                    Pointer to variable of notification value
*                ulNotifyValue -
*                    Notification value
* Return Value : None
******************************************************************************/
static void u_cg_task_notify_request_from_high_priority_isr(TaskHandle_t xTask, volatile uint32_t *pulNotifyVariable, uint32_t ulNotifyValue)
{
    /* If there are no tasks waiting for a notification or a notification was already
     * sent (or is going to be sent), a notification isn't sent or is skipped.
     */
    if (NULL != xTask && 0U == *pulNotifyVariable)
    {
        /* Generate INTWDTI interrupt manually as a software intetrrupt. The interrupt
         * priority of the ISR is configured higher than the SYSCALL interrupt priority.
         * On the other hand, the interrupt priority of INTWDTI is configured within
         * the SYSCALL interrupt priority.
         */
        *pulNotifyVariable = ulNotifyValue;
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
    u_cg_task_notify_requested_from_high_priority_isr( &g_iica0_slave_task, &g_iica0_slave_notification );
}

/******************************************************************************
* Function Name: u_cg_task_notify_requested_from_high_priority_isr
* Description  : This function helps a task notification from high priority ISR to task.
* Arguments    : xTask -
*                    Task handler
*                pulNotifyVariable -
*                    Pointer to variable of notification value
* Return Value : None
******************************************************************************/
static void u_cg_task_notify_requested_from_high_priority_isr(TaskHandle_t *pxTask, volatile uint32_t *pulNotifyVariable)
{
    uint32_t ulNotifyValue;

    /* If the task had been already notified or isn't waiting for any notification,
     * or if the notification value isn't set,
     * i.e. when NULL == *pxTask || 0U == *pulNotifyVariable,
     * actually the task will not be notified.
     */
    if (NULL != *pxTask && 0U != *pulNotifyVariable)
    {
        /* Note that *pxTask is automatically set to NULL by xTaskNotifyFromISR_R_Helper()
         * when posting a notification. On the other hand, *pulNotifyVariable isn't
         * set to 0 automatically therefore it is set to 0 manually.
         */
        ulNotifyValue = *pulNotifyVariable;
        *pulNotifyVariable = 0U;
        xTaskNotifyFromISR_R_Helper( pxTask, ulNotifyValue );
    }
}

/* End user code. Do not edit comment generated here */
