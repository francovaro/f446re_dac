/*
 *  @file  : dac.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 07 ott 2020
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include <stdint.h>

typedef enum
{
	e_dac_escalator 	= 0,
	e_dac_sine 			= 1u,
	e_dac_noise 		= 2u,
	e_dac_triangle	 	= 3u,
	e_dac_buffer	 	= 4u,
	e_dac_max
} t_dac_function;

typedef enum
{
	e_dac_channel_1 	= 0,
	e_dac_channel_2		= 1u,
	e_dac_channel_max
} t_dac_channel;

__IO uint8_t gDMA_DAC1_FT_event;
__IO uint8_t gDMA_DAC1_HT_event;

__IO uint8_t gDMA_DAC2_FT_event;
__IO uint8_t gDMA_DAC2_HT_event;

extern void DAC_initPin(void);
extern uint8_t DAC_initialization(t_dac_function function, t_dac_channel channel);
extern void DMA_Feed_Buffer(uint16_t newSample, t_dac_channel channel);

#endif /* INC_DAC_H_ */
