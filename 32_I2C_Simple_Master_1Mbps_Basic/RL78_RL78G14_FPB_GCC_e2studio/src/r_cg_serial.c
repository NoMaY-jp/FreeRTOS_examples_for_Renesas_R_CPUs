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

#include "freertos_start.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t   g_iic00_master_status_flag; /* iic00 start flag for send address check by master mode */
volatile uint8_t * gp_iic00_tx_address;        /* iic00 send data pointer by master mode */
volatile uint16_t  g_iic00_tx_count;           /* iic00 send data size by master mode */
volatile uint8_t * gp_iic00_rx_address;        /* iic00 receive data pointer by master mode */
volatile uint16_t  g_iic00_rx_count;           /* iic00 receive data size by master mode */
volatile uint16_t  g_iic00_rx_length;          /* iic00 receive data length by master mode */
volatile uint8_t   g_iica0_master_status_flag; /* iica0 master flag */
volatile uint8_t   g_iica0_slave_status_flag;  /* iica0 slave flag */
volatile uint8_t * gp_iica0_rx_address;        /* iica0 receive buffer address */
volatile uint16_t  g_iica0_rx_len;             /* iica0 receive data length */
volatile uint16_t  g_iica0_rx_cnt;             /* iica0 receive data count */
volatile uint8_t * gp_iica0_tx_address;        /* iica0 send buffer address */
volatile uint16_t  g_iica0_tx_cnt;             /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */

TaskHandle_t       g_iic00_master_task;        /* iic00 master task */
TaskHandle_t       g_iica0_slave_task;         /* iica0 slave task */
MD_STATUS       (* g_iica0_slave_cb_user)(MD_STATUS);  /* iica0 slave callback er function */

static QueueHandle_t g_iic00_master_mutex;     /* iic00 master mutex */

void U_IIC00_Master_Send_Receive_Stop(void);   /* for internal use */
void U_IICA0_Slave_Send_Receive_Stop(void);    /* for internal use */

static void U_IICA0_Slave_Operation(void (*cb_user)(void));

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

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0000_SAU_CK00_FCLK_0 | _0000_SAU_CK01_FCLK_0;
    R_IIC00_Create();
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Create
* Description  : This function initializes the IIC00 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Create(void)
{
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable IIC00 */
    IICMK00 = 1U;    /* disable INTIIC00 interrupt */
    IICIF00 = 0U;    /* clear INTIIC00 interrupt flag */
    /* Set INTIIC00 level2 priority */
    IICPR100 = 1U;
    IICPR000 = 0U;
    SIR00 = _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_EDGE_FALL | _0004_SAU_MODE_IIC | _0000_SAU_TRANSFER_END;
    SCR00 = _0000_SAU_TIMING_1 | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0000_SAU_MSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR00 = _1E00_IIC00_DIVISOR;
    SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1 | _0001_SAU_CH0_DATA_OUTPUT_1;
    /* Set SCL00 pin */
    P3 |= 0x01U;
    PM3 &= 0xFEU;
    /* Set SDA00 pin */
    P5 |= 0x01U;
    PM5 &= 0xFEU;
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Master_Send
* Description  : This function starts transferring data for IIC00 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iic00_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear IIC00 master status flag */
    adr &= 0xFEU;    /* send mode */
    g_iic00_master_status_flag = _01_SAU_IIC_SEND_FLAG;            /* set master status flag */
    SCR00 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR00 |= _8000_SAU_TRANSMISSION;
    /* Set paramater */
    g_iic00_tx_count = tx_num;
    gp_iic00_tx_address = tx_buf;
    /* Start condition */
    R_IIC00_StartCondition();
    IICIF00 = 0U;   /* clear INTIIC00 interrupt flag */
    IICMK00 = 0U;   /* enable INTIIC00 */
    SIO00 = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Master_Receive
* Description  : This function starts receiving data for IIC00 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iic00_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear master status flag */
    adr |= 0x01U;    /* receive mode */
    g_iic00_master_status_flag = _02_SAU_IIC_RECEIVE_FLAG;         /* set master status flag */
    SCR00 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR00 |= _8000_SAU_TRANSMISSION;
    /* Set parameter */
    g_iic00_rx_length = rx_num;
    g_iic00_rx_count = 0U;
    gp_iic00_rx_address = rx_buf;
    /* Start condition */
    R_IIC00_StartCondition();
    IICIF00 = 0U;   /* clear INTIIC00 interrupt flag */
    IICMK00 = 0U;   /* enable INTIIC00 */
    SIO00 = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Stop
* Description  : This function stops the IIC00 operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Stop(void)
{
    /* Stop transfer */
    IICMK00 = 1U;    /* disable INTIIC00 */
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable IIC00 */
    IICIF00 = 0U;    /* clear INTIIC00 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_IIC00_StartCondition
* Description  : This function starts IIC00 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_StartCondition(void)
{
    volatile uint8_t w_count;
    
    SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;    /* clear IIC00 SDA */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO0 &= ~_0100_SAU_CH0_CLOCK_OUTPUT_1;    /* clear IIC00 SCL */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;            /* enable IIC00 output */
    SS0 |= _0001_SAU_CH0_START_TRG_ON;              /* enable IIC00 */

    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_IIC00_StopCondition
* Description  : This function stops IIC00 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_StopCondition(void)
{
    volatile uint8_t w_count;
    
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;           /* disable IIC00 */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;       /* disable IIC00 output */
    SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;        /* clear IIC00 SDA */

    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }

    SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1; /* set IIC00 SCL */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;         /* set IIC00 SDA */

    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }
}

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
    /* Set INTIICA0 level 1 priority */
    IICAPR10 = 0U;
    IICAPR00 = 1U;
    /* Set SCLA0, SDAA0 pin */
    POM1 |= 0x30U;
    P1 &= 0xCFU;
    PM1 |= 0x30U;
    SMC0 = 1U;
    IICWL0 = _15_IICA0_IICWL_VALUE;
    IICWH0 = _14_IICA0_IICWH_VALUE;
    DFC0 = 1U; /* digital filter on */
    IICCTL01 |= _01_IICA_fCLK_HALF;
    SVA0 = _10_IICA0_SLAVEADDRESS;
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
* Function Name: R_IICA0_Slave_Send
* Description  : This function sends data as slave mode.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Slave_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iica0_tx_cnt = tx_num;
    gp_iica0_tx_address = tx_buf;
    g_iica0_slave_status_flag = 0U;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Slave_Receive
* Description  : This function receives data as slave mode.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Slave_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iica0_rx_len = rx_num;
    g_iica0_rx_cnt = 0U;
    gp_iica0_rx_address = rx_buf; 
    g_iica0_slave_status_flag  = 0U;
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name: U_IIC00_Master_init
* Description  : This function does additional initialization for IIC00 operation.
* Arguments    : None
* Return Value : None
******************************************************************************/
void U_IIC00_Master_init(void)
{
    xSemaphoreCreateMutexStatic_R_Helper(&g_iic00_master_mutex);
}

/******************************************************************************
* Function Name: U_IIC00_Master_Lock
* Description  : This function locks IIC00 module for exclusive operation.
* Arguments    : None
* Return Value : None
******************************************************************************/
void U_IIC00_Master_Lock(void)
{
    xSemaphoreTake(g_iic00_master_mutex, portMAX_DELAY);
}

/******************************************************************************
* Function Name: U_IIC00_Master_Unlock
* Description  : This function unlocks IIC00 module which was locked for exclusive operation.
* Arguments    : None
* Return Value : None
******************************************************************************/
void U_IIC00_Master_Unlock(void)
{
    xSemaphoreGive(g_iic00_master_mutex);
}

/******************************************************************************
* Function Name: U_IIC00_Master_Send_Wait
* Description  : This function starts transferring data for IIC00 in master mode.
* Arguments    : adr7 -
*                    slave device '7-bit' address (Not '8-bit' address)
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_OVERRUN or MD_NACK
******************************************************************************/
MD_STATUS U_IIC00_Master_Send_Wait(uint8_t adr7, const uint8_t *tx_buf, uint16_t tx_num)
{
    uint32_t value;

    /* Wait for a notification from the interrupt/callback */
    value = ulTaskNotifyTake_R_Helper_Ex( &g_iic00_master_task, R_IIC00_Master_Send( (adr7 << 1), (uint8_t *)tx_buf, tx_num ), portMAX_DELAY );

    R_IIC00_StopCondition();

    return value & 0xFFFFU;
}

/******************************************************************************
* Function Name: U_IIC00_Master_Receive_Wait
* Description  : This function starts receiving data for IIC00 in master mode.
* Arguments    : adr7 -
*                    slave device '7-bit' address (Not '8-bit' address)
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_OVERRUN or MD_NACK
******************************************************************************/
MD_STATUS U_IIC00_Master_Receive_Wait(uint8_t adr7, uint8_t *rx_buf, uint16_t rx_num)
{
    uint32_t value;

    /* Wait for a notification from the interrupt/callback */
    value = ulTaskNotifyTake_R_Helper_Ex( &g_iic00_master_task, R_IIC00_Master_Receive( (adr7 << 1), rx_buf, rx_num ), portMAX_DELAY );

    R_IIC00_StopCondition();

    return value & 0xFFFFU;
}

/******************************************************************************
* Function Name: U_IIC00_Master_Send_Receive_Stop
* Description  : This function stops the IIC00 master transmission/reception.
* Arguments    : None
* Return Value : None
* Note         : This is called from others internally.
******************************************************************************/
void U_IIC00_Master_Send_Receive_Stop(void)
{
    IICMK00 = 1U;   /* disable INTIIC00 interrupt */
}

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
* Function Name: U_IICA0_Slave_Operation_Wait
* Description  : This function starts IICA0 operation as slave mode.
* Arguments    : cb_user -
*                    callback functin pointer
* Return Value : status -
*                    MD_OK or MD_ERROR or MD_NACK or user defined status
******************************************************************************/
MD_STATUS U_IICA0_Slave_Operation_Wait(void (*cb_user)(void))
{
    uint32_t value;

    /* Wait for a notification from the interrupt/callback */
    value = ulTaskNotifyTake_R_Helper_Ex( &g_iica0_slave_task, U_IICA0_Slave_Operation( cb_user ), portMAX_DELAY );

    return value & 0xFFFFU;
}

static void U_IICA0_Slave_Operation(void (*cb_user)(void))
{
    (*cb_user)();
    IICAMK0 = 0U; /* enable INTIICA0 interrupt */
}

/******************************************************************************
* Function Name: U_IICA0_Slave_Send_Start
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
void U_IICA0_Slave_Send_Start(const uint8_t *tx_buf, uint16_t tx_num, MD_STATUS (*cb_user)(MD_STATUS))
{
    g_iica0_slave_cb_user = cb_user;
    R_IICA0_Slave_Send( (uint8_t *)tx_buf, tx_num );
}

/******************************************************************************
* Function Name: U_IICA0_Slave_Receive_Start
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
void U_IICA0_Slave_Receive_Start(uint8_t *rx_buf, uint16_t rx_num, MD_STATUS (*cb_user)(MD_STATUS))
{
    g_iica0_slave_cb_user = cb_user;
    R_IICA0_Slave_Receive( (uint8_t *)rx_buf, rx_num );
}

/******************************************************************************
* Function Name: U_IICA0_Slave_Receive2_Start
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
void U_IICA0_Slave_Receive2_Start(uint8_t *rx_buf, uint16_t rx_num, MD_STATUS (*cb_user)(MD_STATUS))
{
    uint8_t t_iica0_slave_status_flag;

    g_iica0_slave_cb_user = cb_user;
    t_iica0_slave_status_flag = g_iica0_slave_status_flag;
    R_IICA0_Slave_Receive( (uint8_t *)rx_buf, rx_num );
    g_iica0_slave_status_flag = t_iica0_slave_status_flag;
}

/******************************************************************************
* Function Name: U_IICA0_Slave_Send_Receive_Stop
* Description  : This function stops the IICA0 slave transmission/reception.
* Arguments    : None
* Return Value : None
* Note         : This is called from others internally.
******************************************************************************/
void U_IICA0_Slave_Send_Receive_Stop(void)
{
    IICAMK0 = 1U;  /* disable INTIIA0 interrupt */
}

/* End user code. Do not edit comment generated here */
