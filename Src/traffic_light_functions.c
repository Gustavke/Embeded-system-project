/*
 * traffic_light_functions.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Gustav
 */
// Header files
#include <stdint.h>
#include "spi.h"
#include "usart.h"
#include "gpio.h"

// Constants for traffic light offsets and colors
#define TL1_OFFSET 16
#define TL2_OFFSET 8
#define TL3_OFFSET 0
#define TL4_OFFSET 3

#define TL_RED    0b001
#define TL_YELLOW 0b010
#define TL_GREEN  0b100

// Enum to represent traffic light colors
enum Colors {
	Red, Green, Yellow
};

// Struct to represent a traffic light
typedef struct {
	uint8_t offset;
	uint8_t red;
	uint8_t yellow;
	uint8_t green;
} TrafficLight;

// Instances of traffic lights with predefined settings
const TrafficLight tl1 = { TL1_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl2 = { TL2_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl3 = { TL3_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl4 = { TL4_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };

// Buffer to store the current state of traffic lights
uint32_t trafficLightBuffer;

// Function to set a specific bit in the trafficLightBuffer
void traffic_buffer_set_bit(uint8_t offset) {
	trafficLightBuffer |= 1 << offset;
}

// Function to reset a specific bit in the trafficLightBuffer
void traffic_buffer_reset_bit(uint8_t offset) {
	trafficLightBuffer &= ~(1 << offset);
}

// Function to transmit the trafficLightBuffer over SPI
void transmit_buffer() {
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 0);
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &trafficLightBuffer, 3, 100);
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 1);
}

// Function to set the color of a specific traffic light
void set_traffic_light(TrafficLight tl, uint8_t color) {
	// Clear bits at offset
	trafficLightBuffer &= ~(0x7 << tl.offset);

	// Set the color based on the provided enum
	switch (color) {
	case Red:
		trafficLightBuffer |= (tl.red << tl.offset);
		break;
	case Yellow:
		trafficLightBuffer |= (tl.yellow << tl.offset);
		break;
	case Green:
		trafficLightBuffer |= (tl.green << tl.offset);
		break;
	}

	// Transmit the updated buffer
	transmit_buffer();
}
