/*
 *  @file  : adc.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 07 ott 2020
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

__IO uint8_t gDMA_ADC_FT_event;
__IO uint8_t gDMA_ADC_HT_event;

extern void ADC_DMA_Init(void);
extern uint16_t ADC_return_val(uint8_t index);

#endif /* INC_ADC_H_ */
