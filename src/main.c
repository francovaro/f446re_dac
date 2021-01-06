/**
 *  @file  : main
 *
 *  @brief
 *
 *  @author: Francesco Varani
 *  @date  : 06 Gennaio 2021
 */

/* Libraries */
#include "stm32f4xx.h"
#include "inc\lib_uart.h"
#include "adc.h"
#include "timer.h"
#include "dac.h"

#include <stdio.h>
#include <string.h>

/* Const and private data*/
#define SAMPLE_NUMBER	200

/* Private function declaration */
static void Handle_ADC_event(void);

/* Public function */
int main(void)
{
	/* ADC set up*/
	ADC_DMA_Init();	/* set ADC0 channel 1 */
	TIM2_Config();

	/* DAC set up */
	DAC_initPin();
	DAC_initialization(e_dac_triangle, e_dac_channel_1);
	DAC_initialization(e_dac_triangle, e_dac_channel_2);

	ADC_SoftwareStartConv(ADC1);

	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	UART_lib_sendData("Hello!\n", strlen("Hello!\n"));

	while(1)
	{
		Handle_ADC_event();
	}
}

/* Private functions */
/**
 *
 */
static void Handle_ADC_event(void)
{
	uint8_t 		i;
	uint32_t 		sum;
	static uint16_t sample[SAMPLE_NUMBER];
	static uint16_t counter = 0;
	char 	 		string[10] = {0};

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
