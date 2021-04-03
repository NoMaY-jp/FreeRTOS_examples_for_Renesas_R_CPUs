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
* Copyright (C) 2015-2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx130.c
* Description  : Data for r_gpio_rx driver specific to RX130.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.10.2015 1.00    First Release
*         : 01.02.2016 2.00    Changed the value of "g_gpio_dscr_support" for PORTJ
*         : 21.07.2017 2.30    Added support for RX130-512K
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX130)

/* Public interface header file for this package. */
#include "r_gpio_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* These arrays hold which pins have extra functionality. For example, not all pins have the option of enabling
 * open-drain N-channel output instead of the default CMOS output. Each entry in the array corresponds to a port.
 * Each bit in each entry corresponds to a pin on that port. If bit 3 of array entry [4] was set to 1 then that would
 * mean that PORT 4 PIN 3 supported the feature that array represented.
 *
 * These arrays are only used when GPIO_CFG_PARAM_CHECKING_ENABLE is set to 1 (checking enabled). If you know that
 * your code does not need to check the pins then you can set this macro to 0 and save a little execution time
 * and ROM space.
 *
 * Note: These arrays are defined for the largest package part. For smaller packages where some pins do not exist,
 *       pin checking is filtered by the enumerated port_pin list for that package as defined in r_gpio_rx130.h.
 */

#if (GPIO_CFG_PARAM_CHECKING_ENABLE == 1)
    #if defined(BSP_MCU_RX130_512KB)    //RX130-512K board support
const uint8_t g_gpio_cmos_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0xFC,     //PORT1   P12 to P17
    0xFF,     //PORT2   P20 to P27
    0xDF,     //PORT3   P30 to P34, P36, P37
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0xFF,     //PORTA   PA0 to PA7
    0xFF,     //PORTB   PB0 to PB7
    0xFF,     //PORTC   PC0 to PC7
    0x0F,     //PORTD   PD0 to PD3
    0x0F,     //PORTE   PE0 to PE3
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x08      //PORTJ   PJ3
};

const uint8_t g_gpio_open_drain_n_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0xFC,     //PORT1   P12 to P17
    0xCF,     //PORT2   P20 to P23, P26, P27
    0xDF,     //PORT3   P30 to P34, P36, P37
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0xFF,     //PORTA   PA0 to PA7
    0xFF,     //PORTB   PB0 to PB7
    0xFF,     //PORTC   PC0 to PC7
    0x07,     //PORTD   PD0 to PD2
    0x0F,     //PORTE   PE0 to PE3
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x08      //PORTJ   PJ3
};

const uint8_t g_gpio_open_drain_p_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0x00,     //PORT1   None
    0x00,     //PORT2   None
    0x00,     //PORT3   None
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x00,     //PORTA   None
    0x00,     //PORTB   None
    0x00,     //PORTC   None
    0x00,     //PORTD   None
    0x02,     //PORTE   PE1
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x00      //PORTJ   None
};

const uint8_t g_gpio_pull_up_support[GPIO_INFO_NUM_PORTS] =
{
    0xF8,     //PORT0   P03 to P07
    0xFC,     //PORT1   P12 to P17
    0xFF,     //PORT2   P20 to P27
    0xDF,     //PORT3   P30 to P34, P36, P37
    0xFF,     //PORT4   P40 to P47
    0x3F,     //PORT5   P50 to P55
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0xFF,     //PORTA   PA0 to PA7
    0xFF,     //PORTB   PB0 to PB7
    0xFF,     //PORTC   PC0 to PC7
    0xFF,     //PORTD   PD0 to PD7
    0xFF,     //PORTE   PE0 to PE7
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x0F,     //PORTH   PH0 to PH3
    0xCA      //PORTJ   PJ1, PJ3, PJ6, PJ7
};

const uint8_t g_gpio_dscr_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0xFC,     //PORT1   P12 to P17
    0xFF,     //PORT2   P20 to P27
    0x1F,     //PORT3   P30 to P34
    0x00,     //PORT4   None
    0x3F,     //PORT5   P50 to P55
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0xFF,     //PORTA   PA0 to PA7
    0xFF,     //PORTB   PB0 to PB7
    0xFF,     //PORTC   PC0 to PC7
    0xFF,     //PORTD   PD0 to PD7
    0xFF,     //PORTE   PE0 to PE7
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x0F,     //PORTH   PH0 to PH3
    0x0A,     //PORTJ   PJ1, PJ3
};

    #else    //RX130 board support
const uint8_t g_gpio_open_drain_n_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0xFC,     //PORT1   P12 to P17
    0xC0,     //PORT2   P26, P27
    0xD7,     //PORT3   P30 to P32, P34, P36, P37
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x7F,     //PORTA   PA0 to PA6
    0x0F,     //PORTB   PB0 to PB3
    0xFC,     //PORTC   PC2 to PC7
    0x07,     //PORTD   PD0 to PD2
    0x0F,     //PORTE   PE0 to PE3
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x00      //PORTJ   None
};

const uint8_t g_gpio_open_drain_p_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0x00,     //PORT1   None
    0x00,     //PORT2   None
    0x00,     //PORT3   None
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x00,     //PORTA   None
    0x00,     //PORTB   None
    0x00,     //PORTC   None
    0x00,     //PORTD   None
    0x02,     //PORTE   PE1
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x00      //PORTJ   None
};

const uint8_t g_gpio_pull_up_support[GPIO_INFO_NUM_PORTS] =
{
    0xF8,     //PORT0   P03 to P07
    0xFC,     //PORT1   P12 to P17
    0xC3,     //PORT2   P20, P21, P26, P27
    0xD7,     //PORT3   P30 to P32, P34, P36, P37
    0xFF,     //PORT4   P40 to P47
    0x30,     //PORT5   P54, P55
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x7F,     //PORTA   PA0 to PA6
    0xFF,     //PORTB   PB0 to PB7
    0xFC,     //PORTC   PC2 to PC7
    0x07,     //PORTD   PD0 to PD2
    0x3F,     //PORTE   PE0 to PE5
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x0F,     //PORTH   PH0 to PH3
    0xC2      //PORTJ   PJ1, PJ6, PJ7
};

const uint8_t g_gpio_dscr_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0xFC,     //PORT1   P12 to P17
    0xC3,     //PORT2   P20, P21, P26, P27
    0x17,     //PORT3   P30 to P32, P34
    0x00,     //PORT4   None
    0x30,     //PORT5   P54, P55
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x7F,     //PORTA   PA0 to PA6
    0xFF,     //PORTB   PB0 to PB7
    0xFC,     //PORTC   PC2 to PC7
    0x07,     //PORTD   PD0 to PD2
    0x3F,     //PORTE   PE0 to PE5
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x0F,     //PORTH   PH0 to PH3
    0x02,     //PORTJ   PJ1
};

    #endif

#endif

#endif

