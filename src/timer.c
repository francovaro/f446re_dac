/*
 *  @file  : timer.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 07 ott 2020
 */

/* Libraries */
#include "stm32f4xx_tim.h"
#include "timer.h"

/* Defines*/
/* Private data */
/* Private function */

/* Public function */
/**
 * @brief Set of TIM6
 * @param period
 * @param prescaler
 */
void TIM6_Config(uint16_t period, uint16_t prescaler)
{
  /* TIM6CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  if (period == 0)
  {
	  period = 1u;
  }

  if (prescaler == 0)
  {
	  prescaler = 1u;
  }

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = period - 1;						/*  Specifies the period value to be loaded into the active Auto-Reload Register at the next update event.*/
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;						/*Specifies the prescaler value used to divide the TIM clock.*/
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}

/**
 * @brief Set of TIM7
 * @param prescaler
 * @param period
 */
void TIM7_Config(uint16_t period, uint16_t prescaler)
{
  /* TIM7CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM7 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

  if (period == 0)
  {
	  period = 1u;
  }

  if (prescaler == 0)
  {
	  prescaler = 1u;
  }

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

  /* TIM7 TRGO selection */
  TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);

  /* TIM7 enable counter */
  TIM_Cmd(TIM7, ENABLE);
}

/**
 * @brief TIM2 set to trigger ADC => F upd = 9KHz
 */
void TIM2_Config(void)
{
	RCC_ClocksTypeDef 			RCC_ClocksStatus;

	RCC_GetClocksFreq(&RCC_ClocksStatus);
	uint16_t prescaler = ((RCC_ClocksStatus.PCLK1_Frequency*2)) / 1000000  - 1; //1 tick = 1us
	/* TIM2CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	/* TIM8 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period = 23;	/* 23 ms*/
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	/*
	 * sampling wit f = 43.47 kHz
	 */

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM8 TRGO selection */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* TIM8 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}
/* Private function */
/* IRQ handler */

