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

volatile bool g_task_CONIO_error = false;

#define CON_START_MESSAGE  "\r\nEnter characters\r\n"
#define CON_RECV_DEMO_SIZE 3

static volatile uint8_t recv_buff[CON_RECV_DEMO_SIZE + 1];
static volatile uint8_t recv_err_events;
static uint8_t send_buff[CON_RECV_DEMO_SIZE + 1];

#if defined(RENESAS_SIMULATOR_DEBUGGING)
/* Hardware or Renesas RL78 Simulator */
#define LED1_ERROR_BLINK_FREQUENCY_MS pdMS_TO_TICKS( !IsRenesasSimDebugMode() ? 250 : 25 )
#define CON_RECV_TIMEOUT_MS pdMS_TO_TICKS( !IsRenesasSimDebugMode() ? 60 * 1000UL : 6 * 1000UL )
//#define CON_RECV_TIMEOUT_MS (!IsRenesasSimDebugMode() ? pdMS_TO_TICKS( 60 * 1000UL ) : portMAX_DELAY ) /* for debug */
//#define CON_RECV_TIMEOUT_MS (!IsRenesasSimDebugMode() ? pdMS_TO_TICKS( 60 * 1000UL ) : 0 ) /* for debug */
#else
/* Hardware only */
#define LED1_ERROR_BLINK_FREQUENCY_MS pdMS_TO_TICKS( 250 )
#define CON_RECV_TIMEOUT_MS pdMS_TO_TICKS( 60 * 1000UL )
#endif

/* End user code. Do not edit comment generated here */

void task_CONIO(void * pvParameters)
{
/* Start user code for function. Do not edit comment generated here */

    MD_STATUS status;
    uint32_t cnt;

    INTERNAL_NOT_USED( pvParameters );

    U_UART3_Start();

    cnt = 0;
    for (;;)
    {
        if (0 == cnt)
        {
            status = U_UART3_Send_Wait( (uint8_t *)CON_START_MESSAGE, sizeof(CON_START_MESSAGE) - 1 );
        }
        cnt++;

        memset( (char *)recv_buff, 0, CON_RECV_DEMO_SIZE + 1 );

        status = U_UART3_Receive_Wait( recv_buff, CON_RECV_DEMO_SIZE, &recv_err_events, CON_RECV_TIMEOUT_MS );

        if (MD_OK == status)
        {
            taskENTER_CRITICAL();
            vPrintString( "Received Message: " );
            vPrintString( (char *)recv_buff );
            vPrintString( "\n" );
            taskEXIT_CRITICAL();
        }
        else
        {
            if (MD_RECV_TIMEOUT == status)
            {
                vPrintString( "Recv Timeout Error\n" );
            }
            else if (MD_RECV_ERROR == status)
            {
                if (SCI_EVT_FRAMING_ERR & recv_err_events)
                {
                    vPrintString( "Framing Error\n" );
                }
                if (SCI_EVT_PARITY_ERR & recv_err_events)
                {
                    vPrintString( "Parity Error\n" );
                }
                if (SCI_EVT_OVFL_ERR & recv_err_events)
                {
                    vPrintString( "Overrun Error\n" );
                }
            }

            g_task_CONIO_error = true;
            LED1 = LED_ON;
            while (true == g_task_CONIO_error)
            {
                vTaskDelay( LED1_ERROR_BLINK_FREQUENCY_MS );
                LED1 = ~LED1;
            }
            LED1 = LED_OFF;

            U_UART3_Receive_ClearError();

            cnt = 0;
            continue;
        }

        memcpy( send_buff, (void *)recv_buff, CON_RECV_DEMO_SIZE );

        status = U_UART3_Send_Start( (uint8_t *)send_buff, CON_RECV_DEMO_SIZE );

        nop();  /* for breakpoint, check timing chart on Simulator GUI */
        if (4 == cnt)
        {
            nop();  /* for breakpoint, check timing chart on Simulator GUI */
        }
    }

/* End user code. Do not edit comment generated here */
}
/* Start user code for other. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
