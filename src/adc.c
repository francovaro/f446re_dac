/*
 *  @file  : adc.c
 *	
 *  @brief	
 *
 *  @author: francesco varani
 *  @date  : 07 ott 2020
 */

/* Libraries */
#include "stm32f4xx_adc.h"
#include "adc.h"

/* Defines*/
#define			ADC_BUFFER_SIZE	2

/* Private data */
__IO uint16_t 	_adc_buffer[ADC_BUFFER_SIZE];
__IO uint16_t	_adc_read;

/* Private function */
static void DMA_ADC_Config(void);
static void DMA_NVIC_Configuration(void);

/* Public function */
/**
 * @brief Init ADC peripheral with DMA and triggered by TIM
 * @param void
 * @return void
 */
void ADC_DMA_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* configure DMA for ADC */
	DMA_ADC_Config();
	// PF6 ADC3 channel 4

	/* Initialize structures */
	GPIO_StructInit(&GPIO_InitStructure);
	ADC_StructInit(&ADC_InitStructure);
	ADC_CommonStructInit(&ADC_CommonInitStructure);

	/* Config PIN */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_0 );
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF , &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	// ADC3 is using APB2 => 90 MHz

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent ;					//0 ;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					//  ADCCLK => APB2/4 -> 22.5 mhz ?
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		/* for multi ADC !*/
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC Init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;

	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	// TIM8 will trigger

	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; // sure ?

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1 , &ADC_InitStructure);

	/*
	 * ADC convesrion time: 15 cycles
	 * = 15*(1/45Mhz) = 3.33E-7 => 0.33 mS
	 */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
	ADC_DMACmd(ADC3, ENABLE); /* Enable ADC1 DMA */

	ADC_Cmd(ADC3, ENABLE);
}
/**
 * @brief return ADC read value
 * @param index index inside the buffer to return
 * @return indexed buffer value
 */
uint16_t ADC_return_val(uint8_t index)
{
	if (index >= ADC_BUFFER_SIZE)
	{
		return 0;
	}

	return _adc_buffer[index];
}

/* Private function */
/**
 * @brief Init DMA for ADC
 * @param void
 * @return void
 */
static void DMA_ADC_Config(void)
{
	DMA_InitTypeDef       DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* ADC3: DMA2 stream 0 channel 2*/

	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&_adc_buffer[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;	// not sure
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC | DMA_IT_HT,  ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	gDMA_ADC_FT_event = RESET;
	gDMA_ADC_HT_event = RESET;

	//while(DMA_GetCmdStatus(DMA2_Stream0)!=ENABLE);		//check if DMA is ready

	DMA_NVIC_Configuration();
}

/**
 * @brief Init DMA for ADC
 * @param void
 * @return void
 */
static void DMA_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/* IRQ hanlder */
/**
 * @brief IRQ Handler for DMA
 */
void DMA2_Stream0_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0))
	{
		gDMA_ADC_HT_event = SET;
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
	}

	/* Test on DMA Stream Transfer Complete interrupt */
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
	{
		gDMA_ADC_FT_event = SET;
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	}

}

