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

/*******************************************************************************
* File Name    : UART0.c
* Device(s)    : R5F100LE
* Tool-Chain   : CCRL
* Description  : リングバッファのサポート関数です
* Creation Date: 2016/08/11
*******************************************************************************/

/*******************************************************************************
Includes
*******************************************************************************/
#include "r_cg_macrodriver.h"
#include "UART3.h"
#include "r_cg_userdefine.h"

#define init_bf u_uart3_init_bf
#define chk_status u_uart3_chk_status
#define get_data u_uart3_get_data
#define get_blk u_uart3_get_blk
#define g_rx_buff g_uart3_rx_buff
#define g_rx_rdpt g_uart3_rx_rdpt
#define g_rx_dtno g_uart3_rx_dtno
#define g_rx_status g_uart3_rx_status
#define uartx_callback_receivedata u_uart3_callback_receivedata
#define RX_BUFF_SIZE UART3_RX_BUFF_SIZE
#define RX_DTPT_MASK (UART3_RX_BUFF_SIZE - 1)

/* Don't use '(uint8_t *)(maskptr) != (uint8_t *)NULL' because it causes compiler warnings in case of ICCRL78. */
#define SRMKx_SET(maskptr)   do{ if (0U != (uint8_t *)(maskptr) - (uint8_t *)NULL) { *(uint8_t *)(maskptr) = SRMK3; } SRMK3 = 1U; }while (0)
#define SRMKx_CLEAR(maskptr) do{ if (0U != (uint8_t *)(maskptr) - (uint8_t *)NULL) { *(uint8_t *)(maskptr) = SRMK3; } SRMK3 = 0U; }while (0)
/* Don't use 'SRMK3 = mask' because it generates non bit operation code. */
#define SRMKx_RESTORE(mask)  do{ if (0U == mask) { SRMK3 = 0U; }else{ SRMK3 = 1U; } }while (0)

#if 0    /* Non-OS */
#define ENTER_CRITICAL() do{ }while (0)
#define EXIT_CRITICAL()  do{ }while (0)
#elif 1    /* FreeRTOS */
#define ENTER_CRITICAL() taskENTER_CRITICAL()
#define EXIT_CRITICAL()  taskEXIT_CRITICAL()
#elif 0    /* SEGGER embOS */
/* Currently the specified RTOS is not supported. Please add the code here. */
#elif 0    /* Micrium MicroC/OS */
/* Currently the specified RTOS is not supported. Please add the code here. */
#elif 0    /* Renesas RI78V4 */
/* Currently the specified RTOS is not supported. Please add the code here. */
#endif

/*******************************************************************************
Pragma directive
*******************************************************************************/

/*******************************************************************************
Global variables and functions
*******************************************************************************/
volatile uint8_t g_rx_buff[RX_BUFF_SIZE];       /* 受信用リングバッファ     */
volatile uint8_t g_rx_rdpt;                     /* データの読み出しポインタ */
volatile uint8_t g_rx_dtno;                     /* 格納されているデータ数   */
volatile uint8_t g_rx_status;                   /* 受信ステータスフラグ     
        bit7 : オーバフロー・フラグ                                         */                                                                                                                          
/*******************************************************************************
* Function Name: init_bf
* Description  : リングバッファのポインタを初期化
* Arguments    : None
* Return Value : None
*******************************************************************************/
void init_bf(void)
{

    ENTER_CRITICAL();                           /* タスク切り替え禁止       */
    SRMKx_SET(NULL);                            /* INTSRxとの排他制御       */
    g_rx_rdpt = 0x00;                           /* 読み出しポインタ初期化   */
    g_rx_dtno = 0x00;                           /* データ数をクリア         */
    g_rx_status = 0x00;                         /* ステータスをクリア       */
    SRMKx_CLEAR(NULL);                          /* 排他制御完了             */
    EXIT_CRITICAL();                            /* タスク切り替え許可       */

}

/*******************************************************************************
* Function Name: chk_status
* Description  : リングバッファの状態確認
* Arguments    : None
* Return Value : 受信データ数+ステータス
*               bit7 : オーバフロー・フラグ
*               bit6,5 : 0,0
*               bit4-0 : データ数
*******************************************************************************/
uint8_t chk_status(void)
{
    uint8_t work;
    uint8_t mask;                               /* INTSRxのマスク状態の保存 */

    ENTER_CRITICAL();                           /* タスク切り替え禁止       */
    SRMKx_SET(&mask);                           /* INTSRxとの排他制御       */
    work = g_rx_status;                         /* ステータス読み出し       */
    g_rx_status = 0x00;                         /* ステータスをクリア       */
    work |= g_rx_dtno;                          /* データ数読み出し         */
    SRMKx_RESTORE(mask);                        /* 排他制御完了             */
    EXIT_CRITICAL();                            /* タスク切り替え許可       */
    return ( work );

}

/*******************************************************************************
* Function Name: get_data
* Description  : リングバッファから1データを読み出す
* Arguments    : None
* Return Value : データ
*******************************************************************************/
uint8_t get_data(void)
{
    uint8_t work;
    uint8_t mask;                               /* INTSRxのマスク状態の保存 */

    if ( 0 == g_rx_dtno )
    {
        work = 0x00;                            /* データがなければ，0x00   */
    }
    else
    {
        work = g_rx_buff[g_rx_rdpt];            /* データの読み出し         */

        ENTER_CRITICAL();                       /* タスク切り替え禁止       */
        SRMKx_SET(&mask);                       /* INTSRxとの排他制御       */
        g_rx_dtno--;
        g_rx_rdpt++;                            /* ポインタ更新             */
        g_rx_rdpt &= RX_DTPT_MASK;
        SRMKx_RESTORE(mask);                    /* 排他制御完了             */
        EXIT_CRITICAL();                        /* タスク切り替え許可       */
    }

    return ( work );

}

/*******************************************************************************
* Function Name: get_blk
* Description  : リングバッファから指定されたデータを読み出す（転送）
* Arguments    : 格納ポインタ，データ数
* Return Value : 残りデータ数
*******************************************************************************/
uint8_t get_blk(uint8_t * const buff, uint8_t number)
{

    uint8_t work;
    uint8_t mask;                               /* INTSRxのマスク状態の保存 */
    uint8_t dtno;                               /* リングバッファ内データ量 */
    uint8_t cnt;                                /* 転送用カウンタ           */
    uint8_t * gp_buff;                          /* 転送用ポインタ           */

    work = number;                              /* 要求データ数をセット     */
    dtno = g_rx_dtno;                           /* 変数がvolatileなので保持 */

    if ( ( 0 != number ) && ( 0 != dtno ) )
    {                               /* 転送データがある場合                 */

        gp_buff = buff;                         /* 転送先ポインタを設定     */

/* ---------------------------------------------------------------------------

    転送データ数を算出

----------------------------------------------------------------------------*/

        if ( number < dtno )
        {                           /* 要求数がデータ量以下の場合           */
            cnt = number;                       /* 転送データ数をセット     */
            work = 0;                           /* 残りデータ数は0にする    */
        }
        else
        {                           /* 要求数がデータ量以上の場合           */
            cnt = dtno;                    /* 転送数はバッファ内の全て */
            work = number - dtno;          /* 残りデータ数を算出       */
        }

/* ---------------------------------------------------------------------------

    データを転送

----------------------------------------------------------------------------*/

        for (  ; cnt > 0 ; cnt-- )
        {
            *gp_buff = g_rx_buff[g_rx_rdpt];    /* リングバッファから転送   */
            ENTER_CRITICAL();                   /* タスク切り替え禁止       */
            SRMKx_SET(&mask);                   /* INTSRxとの排他制御       */
            g_rx_rdpt++;                        /* 読み出しポインタを更新   */
            g_rx_rdpt &= RX_DTPT_MASK;
            g_rx_dtno--;                        /* データ数を－1            */
            SRMKx_RESTORE(mask);                /* 排他制御完了             */
            EXIT_CRITICAL();                    /* タスク切り替え許可       */
            gp_buff++;                          /* 転送ポインタを更新       */

        }
    

    }

    return( work );

}

/*******************************************************************************
* Function Name: uartx_callback_receivedata
* Description  : This function is a callback function when UARTx receives a data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
*******************************************************************************/
void uartx_callback_receivedata(uint16_t rx_data)
{
    /* 受信用リングバッファ制御 */

    uint8_t  setptr;

    if ( g_rx_dtno < RX_BUFF_SIZE )
    {                           /* バッファに空きあり                       */
        setptr = ((g_rx_rdpt + g_rx_dtno) & RX_DTPT_MASK);  /* 書き込みポインタ */
        g_rx_buff[setptr] = rx_data;    /* 受信データをバッファに格納       */
        g_rx_dtno++;
    }
    else
    {                           /* バッファがフル時の処理                   */
        g_rx_status = 0x80;             /* オーバフロー・フラグをセット     */
    }

}
