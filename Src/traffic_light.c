/*
 * traffic_light.c
 *
 *  Created on: Dec 12, 2023
 *      Author: Gustav
 */

#include "traffic_light_functions.h"
#include "tim.h"
#include "stdint.h"
// Define states for state machine
typedef enum {
	PedestrianRed, PedestrianWaiting, PedestrianGreen
} states;

static states State, NextState;
void traffic_light(void) {

	// Set durations for delays
	uint32_t pedestrianDelay = 10000;
	uint32_t walkingDelay = 20000;
	uint32_t yellowDelay = 3000;
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt for blinking timer
	set_traffic_lights(verticalRoad, Green);
	State = PedestrianRed; // Initialize the state machine
	NextState = PedestrianRed;

	// Infinite loop to control the traffic light
	while (1) {
		State = NextState; // Update the current state

		// State machine logic
		switch (State) {
		case PedestrianRed:
			// Check if the pedestrian button is pressed
			if (pedestrian_button_pressed(2)) {
				HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn); // Enable interrupt blinking timer
				NextState = PedestrianWaiting;
			}
			break;

		case PedestrianWaiting: {
			// Delay before activating yellow light
			HAL_Delay(pedestrianDelay - yellowDelay);
			set_traffic_lights(verticalRoad, Yellow);

			// Delay for yellow light duration
			HAL_Delay(yellowDelay);

			HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt for blinking timer
			set_traffic_lights(verticalRoad, Red); // Set red light for vertical road
			NextState = PedestrianGreen; // Transition to the next state
		}
			break;

		case PedestrianGreen: {
			// Delay for pedestrian walk duration
			HAL_Delay(walkingDelay);

			// Set yellow light
			set_traffic_lights(verticalRoad, Yellow);
			HAL_Delay(yellowDelay);

			set_traffic_lights(verticalRoad, Green); // Set green light for vertical road
			NextState = PedestrianRed; // Transition to the next state
		}
			break;
		}
	}
}
