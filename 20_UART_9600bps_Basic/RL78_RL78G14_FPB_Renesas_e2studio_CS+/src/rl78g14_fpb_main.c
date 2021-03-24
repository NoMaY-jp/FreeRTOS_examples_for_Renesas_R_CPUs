/***********************************************************************
*
*  FILE        : tb_rx65n_main.c
*  DATE        : 2019-09-18
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "task_function.h"
#include "platform.h"

void main_task(void *pvParameters)
{

	INTERNAL_NOT_USED(pvParameters);

	/* Create all other application tasks here */

	while(1)
	{
		/* This NOP prevents infinite DTC pending caused by BR $$.
		 * (Moreover, such DTC pending causes infinite interrupt pending.
		 * Furthermore, such interrupt pending prevents task switching.
		 * So, in case of without this NOP, task switching never occur.)
		 */
		nop();
	}

	/* vTaskDelete(NULL); */

}
