/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/* Libraries */
#include "stm32f4xx.h"
#include "inc\lib_uart.h"
#include "string.h"
#include "adc.h"
#include "timer.h"
#include "dac.h"
			

int main(void)
{
	ADC_DMA_Init();	/* set ADC3 channel 4 */

	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	UART_lib_sendData("Hello!\n", strlen("Hello!\n"));

	while(1)
	{

	}
}
