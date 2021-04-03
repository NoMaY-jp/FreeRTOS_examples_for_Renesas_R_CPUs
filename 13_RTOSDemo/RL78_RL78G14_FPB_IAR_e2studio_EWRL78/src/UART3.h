/***********************************************************************************************************************
* File Name    : UART0.h
* Device(s)    : R5F100LE
* Tool-Chain   : CCRL
* Description  : This file includes UART0 definition.
* Creation Date: 2016/08/10
***********************************************************************************************************************/

void u_uart3_init_bf(void);             /* リングバッファの初期化           */
uint8_t u_uart3_chk_status(void);       /* リングバッファの状態確認         */
uint8_t u_uart3_get_data(void);         /* リングバッファから1データ読み出し*/
uint8_t u_uart3_get_blk(uint8_t * const buff, uint8_t number);
                                        /* リングバッファからの転送         */
void u_uart3_callback_receivedata(uint16_t rx_data);
                                        /* データ受信毎のコールバック関数   */

#define UART3_RX_BUFF_SIZE 16U                               /* 受信用リングバッファのサイズ(0x40以下の2のべき乗)*/
#define UART3_RX_BUFF_SIZE_BITS ((UART3_RX_BUFF_SIZE<<1)-1)  /* chk_status(void)の戻り値の受信データ数の切り出し*/
extern volatile uint8_t g_uart3_rx_buff[UART3_RX_BUFF_SIZE]; /* 受信用リングバッファ     */
extern volatile uint8_t g_uart3_rx_rdpt;                     /* データの読み出しポインタ */
extern volatile uint8_t g_uart3_rx_dtno;                     /* 格納されているデータ数   */
extern volatile uint8_t g_uart3_rx_status;                   /* 受信ステータスフラグ     */


