/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "inc\lib_uart.h"
#include "string.h"
			

int main(void)
{
	UART_lib_config(e_UART_2, DISABLE);
	UART_lib_sendData("Hello!\n", strlen("Hello!\n"));

	while(1)
	{

	}
}
