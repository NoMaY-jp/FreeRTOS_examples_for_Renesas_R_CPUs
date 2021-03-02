/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "task_function.h"
/* Start user code for import. Do not edit comment generated here */
#include "freertos_start.h"
#include "platform.h"
#include "r_cg_serial.h"
#include "rl78g14fpbdef.h"
#include "demo_slave.h"

#if defined(RENESAS_SIMULATOR_DEBUGGING)
/* Hardware or Renesas RL78 Simulator */
#define LED0_BLINK_FREQUENCY_MS pdMS_TO_TICKS( !IsRenesasSimDebugMode() ? 1000 : 100 )
#else
/* Hardware only */
#define LED0_BLINK_FREQUENCY_MS pdMS_TO_TICKS( 1000 )
#endif
/* End user code. Do not edit comment generated here */

void task_LED0(void * pvParameters)
{
/* Start user code for function. Do not edit comment generated here */

    uint8_t last_LED0_stat;
    uint8_t send_buff[3];
    MD_STATUS status;

    INTERNAL_NOT_USED( pvParameters );

    if (IsRenesasSimDebugMode())
    {
        vTaskDelay( portMAX_DELAY - 1 );
    }

    send_buff[0] = DEMO_SLAVE_SET_LED_STAT;
    send_buff[1] = DEMO_SLAVE_LED0;
    send_buff[2] = LED_ON;
    U_IIC00_Master_Lock();
    status = U_IIC00_Master_Send_Wait( DEMO_SLAVE_I2C_ADDR7, send_buff, 3 );
    if (MD_OK != status)
    {
        configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
        for (;;)
        {
            nop();
        }
    }
    U_IIC00_Master_Unlock();
    last_LED0_stat = send_buff[2];

    for (;;)
    {
        vTaskDelay( LED0_BLINK_FREQUENCY_MS );

        send_buff[0] = DEMO_SLAVE_SET_LED_STAT;
        send_buff[1] = DEMO_SLAVE_LED0;
        send_buff[2] = (last_LED0_stat == LED_ON) ? LED_OFF : LED_ON;
        U_IIC00_Master_Lock();
        status = U_IIC00_Master_Send_Wait( DEMO_SLAVE_I2C_ADDR7, send_buff, 3 );
        if (MD_OK != status)
        {
            configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
            for (;;)
            {
                nop();
            }
        }
        U_IIC00_Master_Unlock();
        last_LED0_stat = send_buff[2];
    }

/* End user code. Do not edit comment generated here */
}
/* Start user code for other. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
