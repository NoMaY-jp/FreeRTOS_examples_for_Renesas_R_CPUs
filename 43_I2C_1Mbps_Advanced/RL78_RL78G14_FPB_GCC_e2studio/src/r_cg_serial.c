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
* File Name    : r_cg_serial.c
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
volatile uint8_t   g_iica0_master_status_flag; /* iica0 master flag */
volatile uint8_t   g_iica0_slave_status_flag;  /* iica0 slave flag */
volatile uint8_t * gp_iica0_rx_address;        /* iica0 receive buffer address */
volatile uint16_t  g_iica0_rx_len;             /* iica0 receive data length */
volatile uint16_t  g_iica0_rx_cnt;             /* iica0 receive data count */
volatile uint8_t * gp_iica0_tx_address;        /* iica0 send buffer address */
volatile uint16_t  g_iica0_tx_cnt;             /* iica0 send data count */
volatile uint8_t   g_iica1_master_status_flag; /* iica1 master flag */
volatile uint8_t   g_iica1_slave_status_flag;  /* iica1 slave flag */
volatile uint8_t * gp_iica1_rx_address;        /* iica1 receive buffer address */
volatile uint16_t  g_iica1_rx_len;             /* iica1 receive data length */
volatile uint16_t  g_iica1_rx_cnt;             /* iica1 receive data count */
volatile uint8_t * gp_iica1_tx_address;        /* iica1 send buffer address */
volatile uint16_t  g_iica1_tx_cnt;             /* iica1 send data count */
/* Start user code for global. Do not edit comment generated here */

TaskHandle_t       g_iica0_master_task;        /* iica0 master task */
TaskHandle_t       g_iica1_slave_task;         /* iica1 slave task */
MD_STATUS       (* g_iica1_slave_cb_user)(MD_STATUS);  /* iica1 slave callback er function */

static QueueHandle_t g_iica0_master_mutex;     /* iica0 master mutex */

void U_IICA1_Slave_Send_Receive_Stop(void);    /* for internal use */

static void U_IICA1_Slave_Operation(void (*cb_user)(void));

#if defined(RENESAS_SIMULATOR_DEBUGGING)
/* Workaround for the limitation that the Renesas RL78 simulator doesn't support
 * the port redirection featue. */
void R_IICA0_Create_org(void);
void R_IICA0_Create_simulator_patch(void);
void R_IICA0_Create(void)
{
    if (IsRenesasSimDebugMode())
    {
        /* Reset port redirection registers before starting the IICA0 module. */
        PIOR0 = 0x00U;
        PIOR1 = 0x00U;
    }
    R_IICA0_Create_org();
    R_IICA0_Create_simulator_patch();
}
#define R_IICA0_Create R_IICA0_Create_org
#endif

#define R_IICA0_Master_Send R_CG_API_DO_NOT_USE(R_IICA0_Master_Send)
#define R_IICA0_Master_Receive R_CG_API_DO_NOT_USE(R_IICA0_Master_Receive)
#define R_IICA0_StopCondition R_CG_API_DO_NOT_USE(R_IICA0_StopCondition)

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_IICA0_Create
* Description  : This function initializes the IICA0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Create(void)
{
    IICA0EN = 1U; /* supply IICA0 clock */
    IICE0 = 0U; /* disable IICA0 operation */
    IICAMK0 = 1U; /* disable INTIICA0 interrupt */
    IICAIF0 = 0U; /* clear INTIICA0 interrupt flag */
    /* Set INTIICA0 level 2 priority */
    IICAPR10 = 1U;
    IICAPR00 = 0U;
    /* Set SCLA0, SDAA0 pin */
    POM1 |= 0x30U;
    P1 &= 0xCFU;
    PM1 |= 0x30U;
    SMC0 = 1U;
    IICWL0 = _09_IICA0_IICWL_VALUE;
    IICWH0 = _08_IICA0_IICWH_VALUE;
    DFC0 = 1U; /* digital filter on */
    IICCTL01 |= _01_IICA_fCLK_HALF;
    SVA0 = _12_IICA0_MASTERADDRESS;
    STCEN0 = 1U;
    IICRSV0 = 1U;
    SPIE0 = 0U;
    WTIM0 = 1U;
    ACKE0 = 1U;
    IICAMK0 = 0U;
    IICE0 = 1U;
    LREL0 = 1U;
    /* Set SCLA0, SDAA0 pin */
    PM1 &= 0xCFU;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Stop
* Description  : This function stops IICA0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Stop(void)
{
    IICE0 = 0U;    /* disable IICA0 operation */
}

/***********************************************************************************************************************
* Function Name: R_IICA0_StopCondition
* Description  : This function sets IICA0 stop condition flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_StopCondition(void)
{
    SPT0 = 1U;     /* set stop condition flag */
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Send
* Description  : This function starts to send data as master mode.
* Arguments    : adr -
*                    transfer address
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_IICA0_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    IICAMK0 = 1U;  /* disable INTIICA0 interrupt */
    
    if ((1U == IICBSY0) && (0U == MSTS0))
    {
        /* Check bus busy */
        IICAMK0 = 0U;  /* enable INTIICA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        STT0 = 1U; /* send IICA0 start condition */
        IICAMK0 = 0U;  /* enable INTIICA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == STD0)
        {
            status = MD_ERROR2;
        }
		
        /* Set parameter */
        g_iica0_tx_cnt = tx_num;
        gp_iica0_tx_address = tx_buf;
        g_iica0_master_status_flag = _00_IICA_MASTER_FLAG_CLEAR;
        adr &= (uint8_t)~0x01U; /* set send mode */
        IICA0 = adr; /* send address */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Receive
* Description  : This function starts to receive IICA0 data as master mode.
* Arguments    : adr -
*                    receive address
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_IICA0_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    IICAMK0 = 1U;  /* disable INTIIA0 interrupt */
    
    if ((1U == IICBSY0) && (0U == MSTS0))
    {
        /* Check bus busy */
        IICAMK0 = 0U;  /* enable INTIIA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        STT0 = 1U; /* set IICA0 start condition */
        IICAMK0 = 0U;  /* enable INTIIA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == STD0)
        {
            status = MD_ERROR2;
        }
		
        /* Set parameter */
        g_iica0_rx_len = rx_num;
        g_iica0_rx_cnt = 0U;
        gp_iica0_rx_address = rx_buf;
        g_iica0_master_status_flag  = _00_IICA_MASTER_FLAG_CLEAR;
        adr |= 0x01U; /* set receive mode */
        IICA0 = adr; /* receive address */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_IICA1_Create
* Description  : This function initializes the IICA1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA1_Create(void)
{
    IICA1EN = 1U; /* supply IICA1 clock */
    IICE1 = 0U; /* disable IICA1 operation */
    IICAMK1 = 1U; /* disable INTIICA1 interrupt */
    IICAIF1 = 0U; /* clear INTIICA1 interrupt flag */
    /* Set INTIICA1 level 1 priority */
    IICAPR11 = 0U;
    IICAPR01 = 1U;
    /* Set SCLA1, SDAA1 pin */
    P6 &= 0xF3U;
    PM6 |= 0x0CU;
    SMC1 = 1U;
    IICWL1 = _15_IICA1_IICWL_VALUE;
    IICWH1 = _14_IICA1_IICWH_VALUE;
    DFC1 = 1U; /* digital filter on */
    IICCTL11 |= _01_IICA_fCLK_HALF;
    SVA1 = _10_IICA1_SLAVEADDRESS;
    STCEN1 = 1U;
    IICRSV1 = 1U;
    SPIE1 = 0U;
    WTIM1 = 1U;
    ACKE1 = 1U;
    IICAMK1 = 0U;
    IICE1 = 1U;
    LREL1 = 1U;
    /* Set SCLA1, SDAA1 pin */
    PM6 &= 0xF3U;
}

/***********************************************************************************************************************
* Function Name: R_IICA1_Stop
* Description  : This function stops IICA1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA1_Stop(void)
{
    IICE1 = 0U;    /* disable IICA1 operation */
}

/***********************************************************************************************************************
* Function Name: R_IICA1_Slave_Send
* Description  : This function sends data as slave mode.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IICA1_Slave_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iica1_tx_cnt = tx_num;
    gp_iica1_tx_address = tx_buf;
    g_iica1_slave_status_flag = 0U;
}

/***********************************************************************************************************************
* Function Name: R_IICA1_Slave_Receive
* Description  : This function receives data as slave mode.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IICA1_Slave_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iica1_rx_len = rx_num;
    g_iica1_rx_cnt = 0U;
    gp_iica1_rx_address = rx_buf; 
    g_iica1_slave_status_flag  = 0U;
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name: R_IICA0_Create_simulator_patch
* Description  : This function patches R_IICA0_Create() for the Renesas RL78 Simulator
* Arguments    : None
* Return Value : None
******************************************************************************/
#if defined(RENESAS_SIMULATOR_DEBUGGING)
/* Workaround for the limitation that the Renesas RL78 simulator doesn't support
 * the port redirection featue. */
void R_IICA0_Create_simulator_patch(void)
{
    if (IsRenesasSimDebugMode())
    {
        /* Reset SCLA0, SDAA0 pin */
        PM1 &= ~0x30U;
        P1 |= ~0xCFU;
        POM1 &= ~0x30U;
        /* Reset SCLA0, SDAA0 pin */
        PM1 &= 0xCFU;

        /* Set SCLA0, SDAA0 pin */
        P6 &= 0xFCU;
        PM6 |= 0x03U;
        /* Set SCLA0, SDAA0 pin */
        PM6 &= 0xFCU;
    }
}
#endif

/******************************************************************************
* Function Name: U_IICA0_Master_init
* Description  : This function does additional initialization for IICA0 operation.
* Arguments    : None
* Return Value : None
******************************************************************************/
void U_IICA0_Master_init(void)
{
    IIC_TM03_init();

    xSemaphoreCreateMutexStatic_R_Helper(&g_iica0_master_mutex);
}

/******************************************************************************
* Function Name: U_IICA0_Master_Lock
* Description  : This function locks IICA0 module for exclusive operation.
* Arguments    : None
* Return Value : None
******************************************************************************/
void U_IICA0_Master_Lock(void)
{
    xSemaphoreTake(g_iica0_master_mutex, portMAX_DELAY);
}

/******************************************************************************
* Function Name: U_IICA0_Master_Unlock
* Description  : This function unlocks IICA0 module which was locked for exclusive operation.
* Arguments    : None
* Return Value : None
******************************************************************************/
void U_IICA0_Master_Unlock(void)
{
    xSemaphoreGive(g_iica0_master_mutex);
}

/******************************************************************************
* Function Name: U_IICA0_Master_Send_Wait
* Description  : This function starts transferring data for IICA0 in master mode.
* Arguments    : adr7 -
*                    slave device '7-bit' address (Not '8-bit' address)
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK(IIC_SUCCESS) or MD_ERROR1 or NO_SLAVE or NO_ACK
******************************************************************************/
MD_STATUS U_IICA0_Master_Send_Wait(uint8_t adr7, const uint8_t *tx_buf, uint16_t tx_num)
{
    MD_STATUS status;
    uint32_t value;

    /* Wait for a notification from the interrupt/callback */
    value = ulTaskNotifyTake_R_Helper_Ex2( &g_iica0_master_task, status = R_IIC_Master_Send( (adr7 << 1), (uint8_t *)tx_buf, tx_num ), portMAX_DELAY );
    if (MD_OK == status)
    {
        R_IIC_StopCondition();

        status = value & 0xFFFFU;
    }

    return status;
}

/******************************************************************************
* Function Name: U_IICA0_Master_Receive_Wait
* Description  : This function starts receiving data for IICA0 in master mode.
* Arguments    : adr7 -
*                    slave device '7-bit' address (Not '8-bit' address)
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK(IIC_SUCCESS) or MD_ERROR1 or NO_SLAVE or NO_ACK
******************************************************************************/
MD_STATUS U_IICA0_Master_Receive_Wait(uint8_t adr7, uint8_t *rx_buf, uint16_t rx_num)
{
    MD_STATUS status;
    uint32_t value;

    /* Wait for a notification from the interrupt/callback */
    value = ulTaskNotifyTake_R_Helper_Ex2( &g_iica0_master_task, status = R_IIC_Master_Receive( (adr7 << 1), rx_buf, rx_num ), portMAX_DELAY );
    if (MD_OK == status)
    {
        R_IIC_StopCondition();

        status = value & 0xFFFFU;
    }

    return status;
}

/******************************************************************************
* Function Name: U_IICA1_Slave_Operation_Wait
* Description  : This function starts IICA0 operation as slave mode.
* Arguments    : cb_user -
*                    callback functin pointer
* Return Value : status -
*                    MD_OK or MD_ERROR or MD_NACK or user defined status
******************************************************************************/
MD_STATUS U_IICA1_Slave_Operation_Wait(void (*cb_user)(void))
{
    uint32_t value;

    /* Wait for a notification from the interrupt/callback */
    value = ulTaskNotifyTake_R_Helper_Ex( &g_iica1_slave_task, U_IICA1_Slave_Operation( cb_user ), portMAX_DELAY );

    return value & 0xFFFFU;
}

static void U_IICA1_Slave_Operation(void (*cb_user)(void))
{
    (*cb_user)();
    IICAMK1 = 0U; /* enable INTIICA1 interrupt */
}

/******************************************************************************
* Function Name: U_IICA1_Slave_Send_Start
* Description  : This function sends data as slave mode.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                cb_user -
*                    callback functin pointer
* Return Value : None
* Note         : Must be called from ISR context.
******************************************************************************/
void U_IICA1_Slave_Send_Start(const uint8_t *tx_buf, uint16_t tx_num, MD_STATUS (*cb_user)(MD_STATUS))
{
    g_iica1_slave_cb_user = cb_user;
    R_IICA1_Slave_Send( (uint8_t *)tx_buf, tx_num );
}

/******************************************************************************
* Function Name: U_IICA1_Slave_Receive_Start
* Description  : This function receives data as slave mode.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
*                cb_user -
*                    callback functin pointer
* Return Value : None
* Note         : Must be called from ISR context.
******************************************************************************/
void U_IICA1_Slave_Receive_Start(uint8_t *rx_buf, uint16_t rx_num, MD_STATUS (*cb_user)(MD_STATUS))
{
    g_iica1_slave_cb_user = cb_user;
    R_IICA1_Slave_Receive( (uint8_t *)rx_buf, rx_num );
}

/******************************************************************************
* Function Name: U_IICA1_Slave_Receive2_Start
* Description  : This function receives more data as slave mode.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
*                cb_user -
*                    callback functin pointer
* Return Value : None
* Note         : Must be called from ISR context.
******************************************************************************/
void U_IICA1_Slave_Receive2_Start(uint8_t *rx_buf, uint16_t rx_num, MD_STATUS (*cb_user)(MD_STATUS))
{
    uint8_t t_iica1_slave_status_flag;

    g_iica1_slave_cb_user = cb_user;
    t_iica1_slave_status_flag = g_iica1_slave_status_flag;
    R_IICA1_Slave_Receive( (uint8_t *)rx_buf, rx_num );
    g_iica1_slave_status_flag = t_iica1_slave_status_flag;
}

/******************************************************************************
* Function Name: U_IICA1_Slave_Send_Receive_Stop
* Description  : This function stops the IICA1 slave transmission/reception.
* Arguments    : None
* Return Value : None
* Note         : This is called from others internally.
******************************************************************************/
void U_IICA1_Slave_Send_Receive_Stop(void)
{
    IICAMK1 = 1U;  /* disable INTIIA1 interrupt */
}

/* End user code. Do not edit comment generated here */
