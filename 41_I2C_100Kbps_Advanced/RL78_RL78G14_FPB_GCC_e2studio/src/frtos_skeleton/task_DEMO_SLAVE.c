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

#include "platform.h"
#include "rl78g14fpbdef.h"
#include "r_cg_serial.h"
#include "demo_slave.h"

/* End user code. Do not edit comment generated here */

void task_DMSLV(void * pvParameters)
{
/* Start user code for function. Do not edit comment generated here */

    uint8_t recv_buff[3];
    uint8_t send_buff[1];
    MD_STATUS status;

    INTERNAL_NOT_USED( pvParameters );

    for (;;)
    {
        status = U_IICA1_Slave_Receive_Wait( &recv_buff[0], 1 );
        if (MD_OK != status)
        {
            configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
            for (;;)
            {
                nop();
            }
        }
        switch (recv_buff[0])
        {
        case DEMO_SLAVE_GET_SW_STAT:
            status = U_IICA1_Slave_Receive2_Wait( &recv_buff[1], 1 );
            if (MD_OK != status)
            {
                configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
                for (;;)
                {
                    nop();
                }
            }
            switch (recv_buff[1])
            {
            case DEMO_SLAVE_SW1:
                send_buff[0] = SW1;
                break;
            default:
                configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
                send_buff[0] = SW1_RELEASE;
                break;
            }
            U_IICA1_Slave_Send_Wait( &send_buff[0], 1 );
            break;
        case DEMO_SLAVE_SET_LED_STAT:
            status = U_IICA1_Slave_Receive2_Wait( &recv_buff[1], 2 );
            if (MD_OK != status)
            {
                configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
            }
            switch (recv_buff[1])
            {
            case DEMO_SLAVE_LED0:
                LED0 = recv_buff[2];
                break;
            case DEMO_SLAVE_LED1:
                LED1 = recv_buff[2];
                break;
            default:
                configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
                break;
            }
            break;
        default:
            configASSERT( ( volatile void * ) NULL );   /* Force an assert. */
            break;
        }

        nop();  /* for breakpoint, check timing chart on Simulator GUI */
    }

/* End user code. Do not edit comment generated here */
}
/* Start user code for other. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
