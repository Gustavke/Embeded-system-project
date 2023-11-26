/*
 * traffic_light_functions.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Gustav
 */
#include <stdint.h>
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#define TL1_RED_OFFSET 16
#define TL1_YELLOW_OFFSET 17
#define TL1_GREEN_OFFSET 18
#define PL1_RED_OFFSET  19
#define PL1_GREEN_OFFSET 20
#define PL1_BLUE_OFFSET 21

#define TL2_RED_OFFSET 8
#define TL2_YELLOW_OFFSET 9
#define TL2_GREEN_OFFSET 10
#define PL2_RED_OFFSET 11
#define PL2_GREEN_OFFSET 12
#define PL2_BLUE_OFFSET 13

#define TL3_RED_OFFSET 0
#define TL3_YELLOW_OFFSET 1
#define TL3_GREEN_OFFSET 2
#define TL4_RED_OFFSET 3
#define TL4_YELLOW_OFFSET 4
#define TL4_GREEN_OFFSET 5


#define TL1_OFFSET 16
#define PL1_OFFSET  19
#define TL2_OFFSET 8
#define PL2_OFFSET 11
#define TL3_OFFSET 0
#define TL4_OFFSET 3



#define TL_RED 0b001
#define TL_YELLOW 0b010
#define TL_GREEN 0b100

#define PL_RED 0b001
#define PL_GREEN 0b010
#define PL_BLUE 0b100



uint32_t trafficLightBuffer;

void set_traffic_light(uint8_t offset, uint8_t color){
	//Clear bits at offset
	trafficLightBuffer &= ~(0x7 << offset);
	//Set new color
	trafficLightBuffer |= ~(color << offset);

	transmit_buffer();
}


void traffic_buffer_set_bit(uint8_t offset) {
	trafficLightBuffer |= 1 << offset;
}

void traffic_buffer_reset_bit(uint8_t offset) {
	trafficLightBuffer &= ~(1 << offset);
}

void transmit_buffer() {
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 0);
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &trafficLightBuffer, 3, 100);
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 1);
}
