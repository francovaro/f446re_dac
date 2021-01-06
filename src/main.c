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
#include "adc.h"
#include "timer.h"
#include "dac.h"

#include <stdio.h>
#include <string.h>
			
#define SAMPLE_NUMBER	200

int main(void)
{
	uint8_t i;
	uint32_t sum;
	uint16_t sample[SAMPLE_NUMBER];
	uint16_t counter = 0;
	char 	 string[10] = {0};

	ADC_DMA_Init();	/* set ADC3 channel 4 */
	TIM2_Config();
	ADC_SoftwareStartConv(ADC1);

	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	UART_lib_sendData("Hello!\n", strlen("Hello!\n"));

	while(1)
	{
		if (gDMA_ADC_HT_event == SET)
		{
			gDMA_ADC_HT_event = RESET;
			sample[counter++] = ADC_return_val(0);
		}

		if (gDMA_ADC_FT_event == SET)
		{
			gDMA_ADC_FT_event = RESET;
			sample[counter++] = ADC_return_val(1);
		}

		if (counter == SAMPLE_NUMBER)
		{
			sum = 0;

			for (i=0; i < counter; i++)
			{
				sum += sample[i];
			}

			counter = 0;
			if (sum != 0)
			{
				sum/=SAMPLE_NUMBER;
				sum = (sum*100)/4096;
				snprintf(string, sizeof(string), "%ld\n", sum);
				UART_lib_sendData(string, strlen(string));
			}


		}
	}
}
