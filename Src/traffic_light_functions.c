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

#define PL1_OFFSET  19
#define PL2_OFFSET 11

#define TL_RED    0b001
#define TL_YELLOW 0b010
#define TL_GREEN  0b100

#define PL_RED 0b001
#define PL_GREEN 0b010
#define PL_BLUE 0b100

// Enum to represent traffic light colors
enum Colors {
	Red, Green, Yellow, Blue
};

// Struct to represent a traffic light
typedef struct {
	uint8_t offset;
	uint8_t red;
	uint8_t yellow;
	uint8_t green;
} TrafficLight;

// Struct to represent road directions
typedef struct {
	TrafficLight trafficLight1;
	TrafficLight trafficLight2;
	TrafficLight pedestrianLight;
} Road;

// Instances of traffic lights with predefined settings
const TrafficLight tl1 = { TL1_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl2 = { TL2_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl3 = { TL3_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl4 = { TL4_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight pl1 = { PL1_OFFSET, PL_RED, 0, PL_GREEN };
const TrafficLight pl2 = { PL2_OFFSET, PL_RED, 0, PL_GREEN };

// Instances of roads with predefined settings
const Road verticalRoad = { tl2, tl4, pl2 };
const Road horizontalRoad = { tl1, tl3, pl1 };

uint8_t pd1Pressed = 0;
uint8_t pd2Pressed = 0;

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

// Function to toggle a specific bit in the trafficLightBuffer
void traffic_buffer_toggle_bit(uint8_t offset) {
	trafficLightBuffer ^= 1 << offset;
}

// Function to transmit the trafficLightBuffer over SPI
void transmit_buffer() {
	//HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
	//NVIC_GetEnableIRQ()
	// Set the STCP pin low before data transmission
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 0);
	// Transmit the trafficLightBuffer over SPI
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &trafficLightBuffer, 3, 100);
	// Set the STCP pin high to latch the data
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 1);
	//HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

// Function to set the SR-buffer to represent color of a specific traffic light
void set_shiftregister_buffer(TrafficLight tl, uint8_t color) {
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

}

// Function to toggle the blue light of a specific traffic light
void toggle_blue_light(TrafficLight tl) {
	// Toggle the blue light bit at the offset
	trafficLightBuffer ^= (PL_BLUE << tl.offset);

	// Transmit the updated buffer to update the physical lights
	transmit_buffer();
}

// Function to set the color of traffic lights on a road and update the pedestrian light accordingly
void set_traffic_lights(Road road, uint8_t color) {
	// Set the color of each traffic light on the road
	set_shiftregister_buffer(road.trafficLight1, color);
	set_shiftregister_buffer(road.trafficLight2, color);

	// Update the pedestrian light based on the specified color
	if (color == Red) {
		set_shiftregister_buffer(road.pedestrianLight, Green);
	} else {
		set_shiftregister_buffer(road.pedestrianLight, Red);
	}

	// Transmit the updated buffer to update the physical lights
	transmit_buffer();
}

void _turn_off_lights() {
	trafficLightBuffer = 0;
	transmit_buffer();
}

void set_pedestrian_button_pressed(uint8_t button) {
	switch (button) {
	case 1:
		pd1Pressed = 1;
		break;
	case 2:
		pd2Pressed = 1;
		break;
	}
}

void reset_pedestrian_button_pressed(uint8_t button) {
	switch (button) {
	case 1:
		pd1Pressed = 0;
		break;
	case 2:
		pd2Pressed = 0;
		break;
	}
}

int pedestrian_button_pressed(uint8_t crossing) {
	switch (crossing) {
	case 1:
		return pd1Pressed;
		break;
	case 2:
		return pd2Pressed;
		break;
	}
	return -1;
}

// Function to check if a car is present in a specified lane
int is_car_present(uint8_t lane) {
	switch (lane) {
	case 1:
		return 1 ^ HAL_GPIO_ReadPin(TL1_Car_GPIO_Port, TL1_Car_Pin);
		break;
	case 2:
		return 1 ^ HAL_GPIO_ReadPin(TL2_Car_GPIO_Port, TL2_Car_Pin);
		break;
	case 3:
		return 1 ^ HAL_GPIO_ReadPin(TL3_Car_GPIO_Port, TL3_Car_Pin);
		break;
	case 4:
		return 1 ^ HAL_GPIO_ReadPin(TL4_Car_GPIO_Port, TL4_Car_Pin);
		break;
	}
	return -1;
}

int h_car_present() {
	if (is_car_present(1) || is_car_present(3))
		return 1;
	return 0;
}

int v_car_present() {
	if (is_car_present(2) || is_car_present(4))
		return 1;
	return 0;
}

void transistion_vertical_to_green(uint32_t yellowDelay) {
	set_traffic_lights(horizontalRoad, Yellow);
	HAL_Delay(yellowDelay);
	set_traffic_lights(horizontalRoad, Red);
	set_traffic_lights(verticalRoad, Yellow);
	HAL_Delay(yellowDelay);
	set_traffic_lights(verticalRoad, Green);
}

void transistion_horizontal_to_green(uint32_t yellowDelay) {
	set_traffic_lights(verticalRoad, Yellow);
	HAL_Delay(yellowDelay);
	set_traffic_lights(verticalRoad, Red);
	set_traffic_lights(horizontalRoad, Yellow);
	HAL_Delay(yellowDelay);
	set_traffic_lights(horizontalRoad, Green);
}
