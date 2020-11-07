/*
 *  @file  : timer.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 07 ott 2020
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

 #include "stm32f4xx_tim.h"

extern void TIM6_Config(uint16_t period, uint16_t prescaler);
extern void TIM7_Config(uint16_t period, uint16_t prescaler);
extern void TIM2_Config(void);

#endif /* INC_TIMER_H_ */
