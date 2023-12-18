/*
 * shiftregister_driver.c
 *
 *  Created on: Dec 18, 2023
 *      Author: Gustav
 */

/**
 * @file shiftregister_driver.c
 * @brief Implementation of functions to control shift registers for traffic lights.
 */

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

// Struct to represent a traffic light
typedef struct {
	uint8_t offset; /**< Offset in the shift register for the traffic light. */
	uint8_t red;    /**< Bit pattern for the red light. */
	uint8_t yellow; /**< Bit pattern for the yellow light. */
	uint8_t green;  /**< Bit pattern for the green light. */
} TrafficLight;

// Instances of traffic lights with predefined settings
const TrafficLight tl1 = { TL1_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl2 = { TL2_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl3 = { TL3_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight tl4 = { TL4_OFFSET, TL_RED, TL_YELLOW, TL_GREEN };
const TrafficLight pl1 = { PL1_OFFSET, PL_RED, 0, PL_GREEN };
const TrafficLight pl2 = { PL2_OFFSET, PL_RED, 0, PL_GREEN };

// Struct to represent road directions
typedef struct {
	TrafficLight trafficLight1;    /**< First traffic light on the road. */
	TrafficLight trafficLight2;    /**< Second traffic light on the road. */
	TrafficLight pedestrianLight;  /**< Pedestrian traffic light on the road. */
} Road;

// Instances of roads with predefined settings
const Road verticalRoad = { tl2, tl4, pl2 };
const Road horizontalRoad = { tl1, tl3, pl1 };

// Enum to represent traffic light colors
enum Colors {
	Red,    /**< Red color for traffic lights. */
	Green,  /**< Green color for traffic lights. */
	Yellow, /**< Yellow color for traffic lights. */
	//Blue  /**< Blue color (commented out as not currently used). */
};

// Buffer to store the current state of traffic lights
uint32_t trafficLightBuffer;

/**
 * @brief Transmit the trafficLightBuffer over SPI.
 */
void transmit_buffer() {
	// Set the STCP pin low before data transmission
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 0);
	// Transmit the trafficLightBuffer over SPI
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &trafficLightBuffer, 3, 100);
	// Set the STCP pin high to latch the data
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, 1);
}

/**
 * @brief Set the SR-buffer to represent the color of a specific traffic light.
 *
 * @param tl The TrafficLight structure representing the traffic light.
 * @param color The color to set for the traffic light.
 */
void set_shiftregister_buffer(TrafficLight tl, enum Colors color) {
	// Clear bits at the offset
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

/**
 * @brief Toggle the blue light of a specific traffic light.
 *
 * @param tl The TrafficLight structure representing the traffic light.
 */
void toggle_blue_light(TrafficLight tl) {
	// Toggle the blue light bit at the offset
	trafficLightBuffer ^= (PL_BLUE << tl.offset);

	// Transmit the updated buffer to update the physical lights
	transmit_buffer();
}

/**
 * @brief Turn off all traffic lights.
 */
void _turn_off_lights() {
	trafficLightBuffer = 0;
	transmit_buffer();
}
