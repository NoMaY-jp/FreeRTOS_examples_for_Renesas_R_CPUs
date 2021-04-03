/***********************************************************************************************************************
* File Name    : UART0.h
* Device(s)    : R5F100LE
* Tool-Chain   : CCRL
* Description  : This file includes UART0 definition.
* Creation Date: 2016/08/10
***********************************************************************************************************************/

void u_uart3_init_bf(void);             /* �����O�o�b�t�@�̏�����           */
uint8_t u_uart3_chk_status(void);       /* �����O�o�b�t�@�̏�Ԋm�F         */
uint8_t u_uart3_get_data(void);         /* �����O�o�b�t�@����1�f�[�^�ǂݏo��*/
uint8_t u_uart3_get_blk(uint8_t * const buff, uint8_t number);
                                        /* �����O�o�b�t�@����̓]��         */
void u_uart3_callback_receivedata(uint16_t rx_data);
                                        /* �f�[�^��M���̃R�[���o�b�N�֐�   */

#define UART3_RX_BUFF_SIZE 16U                               /* ��M�p�����O�o�b�t�@�̃T�C�Y(0x40�ȉ���2�ׂ̂���)*/
#define UART3_RX_BUFF_SIZE_BITS ((UART3_RX_BUFF_SIZE<<1)-1)  /* chk_status(void)�̖߂�l�̎�M�f�[�^���̐؂�o��*/
extern volatile uint8_t g_uart3_rx_buff[UART3_RX_BUFF_SIZE]; /* ��M�p�����O�o�b�t�@     */
extern volatile uint8_t g_uart3_rx_rdpt;                     /* �f�[�^�̓ǂݏo���|�C���^ */
extern volatile uint8_t g_uart3_rx_dtno;                     /* �i�[����Ă���f�[�^��   */
extern volatile uint8_t g_uart3_rx_status;                   /* ��M�X�e�[�^�X�t���O     */


