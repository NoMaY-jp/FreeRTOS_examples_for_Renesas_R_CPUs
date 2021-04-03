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
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : freertos_object_init.c
 * Version      : 1.0
 * Description  :
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 07.12.2018 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "freertos_start.h"
#include "freertos_stack.h"
#include "task_function.h"
#include "demo_main.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
void Kernel_Object_init (void);
void Object_init_manual (void);
BaseType_t ret;
/***********************************************************************************************************************
 * Function Name: Kernel_Object_init
 * Description  : This function initializes FreeRTOS objects.
 * Arguments    : None.
 * Return Value : None.
 * Note         : Be aware that auto variables created on the stack in this
 *                function will be discarded after returning from this function.
 *                Therefore don't pass the address of auto variables to tasks.
 *                (Moreover, the stack used before starting scheduler will be
 *                re-used as interrupt dedicated stack after scheduler started.)
 **********************************************************************************************************************/
void Kernel_Object_init (void)
{
    /************** task creation ****************************/

#if (mainCREATE_NON_STANDARD_RTOS_DEMO == 1)

    xTaskCreateStatic_R_Helper( main_task, "MAIN_TASK", main_task_STACK_BUFF_DEPTH, NULL, 1, NULL );

    xTaskCreateStatic_R_Helper( task_LED0, "task_LED0", task_LED0_STACK_BUFF_DEPTH, NULL, 2, NULL );

    xTaskCreateStatic_R_Helper( task_LED1, "task_LED1", task_LED1_STACK_BUFF_DEPTH, NULL, 2, NULL );

    xTaskCreateStatic_R_Helper( task_CONIO, "task_CONIO", task_CONIO_STACK_BUFF_DEPTH, NULL, 3, NULL);

#endif /* mainCREATE_NON_STANDARD_RTOS_DEMO == 1 */

    /************** semaphore creation ***********************/

    /************** queue creation ***************************/

    /************** software time creation **************************/

    /************** event groups creation ********************/

    /************** stream buffer creation *************************/

    /************** message buffer creation *********************/

} /* End of function Kernel_Object_init()*/

/***********************************************************************************************************************
 * Function Name : Object_init_manual
 * Description   : This function re-initializes FreeRTOS objects and should be called at runtime.
 * Arguments     : None.
 * Return value  : None.
 * Note          : Be aware that auto variables created on the stack in this
 *                 function will be discarded after returning from this function.
 *                 Therefore don't pass the address of auto variables to tasks.
 *                 (Moreover, the stack used before starting scheduler will be
 *                 re-used as interrupt dedicated stack after scheduler started.)
 **********************************************************************************************************************/
void Object_init_manual (void)
{
    /************** task creation ****************************/
} /* End of function Object_init_manual()*/
