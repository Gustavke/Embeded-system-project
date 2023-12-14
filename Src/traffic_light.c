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
	PedestrianRed,
	PedestrianWaiting,
	PedestrianGreen,
	VerticalGreen,
	VerticalWaiting,
	HorizontalGreen,
	HorizontalWaiting
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
				HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);// Enable interrupt blinking timer
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
			reset_pedestrian_button_pressed(2);
			NextState = PedestrianRed; // Transition to the next state
		}
			break;
		}
	}
}

void traffic_light_2(void) {

	// Set durations for delays
	uint32_t pedestrianDelay = 10000;
	uint32_t walkingDelay = 20000;
	uint32_t yellowDelay = 3000;
	uint32_t greenDelay = 20000;
	uint32_t redDelayMax = 20000;
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt for blinking timer
	set_traffic_lights(verticalRoad, Green);
	set_traffic_lights(horizontalRoad, Red);
	State = VerticalGreen; // Initialize the state machine
	NextState = VerticalGreen;
	uint32_t lastChange = HAL_GetTick();

	// Infinite loop to control the traffic light
	while (1) {
		State = NextState; // Update the current state

		// State machine logic
		switch (State) {
		case VerticalGreen: {

			if (!v_car_present()) {
				if (h_car_present()
						|| HAL_GetTick() - lastChange > greenDelay) {
					set_traffic_lights(verticalRoad, Yellow);
					HAL_Delay(yellowDelay);
					set_traffic_lights(verticalRoad, Red);
					set_traffic_lights(horizontalRoad, Yellow);
					HAL_Delay(yellowDelay);
					set_traffic_lights(horizontalRoad, Green);
					lastChange = HAL_GetTick();
					NextState = HorizontalGreen;
				}
			} else if (h_car_present()) {
				lastChange = HAL_GetTick();
				NextState = HorizontalWaiting;
			}
		}
			break;

		case HorizontalGreen: {

			if (!h_car_present()) {
				if (v_car_present()
						|| HAL_GetTick() - lastChange > greenDelay) {
					set_traffic_lights(horizontalRoad, Yellow);
					HAL_Delay(yellowDelay);
					set_traffic_lights(horizontalRoad, Red);
					set_traffic_lights(verticalRoad, Yellow);
					HAL_Delay(yellowDelay);
					set_traffic_lights(verticalRoad, Green);
					lastChange = HAL_GetTick();
					NextState = VerticalGreen;
				}
			} else if (v_car_present()) {
				lastChange = HAL_GetTick();
				NextState = VerticalWaiting;
			}
		}
			break;
		case VerticalWaiting: {
			if (!h_car_present() || HAL_GetTick() - lastChange > redDelayMax) {
				set_traffic_lights(horizontalRoad, Yellow);
				HAL_Delay(yellowDelay);
				set_traffic_lights(horizontalRoad, Red);
				set_traffic_lights(verticalRoad, Yellow);
				HAL_Delay(yellowDelay);
				set_traffic_lights(verticalRoad, Green);
				lastChange = HAL_GetTick();
				NextState = VerticalGreen;
			}
		}
			break;
		case HorizontalWaiting: {
			if (!v_car_present() || HAL_GetTick() - lastChange > redDelayMax) {
				set_traffic_lights(verticalRoad, Yellow);
				HAL_Delay(yellowDelay);
				set_traffic_lights(verticalRoad, Red);
				set_traffic_lights(horizontalRoad, Yellow);
				HAL_Delay(yellowDelay);
				set_traffic_lights(horizontalRoad, Green);
				lastChange = HAL_GetTick();
				NextState = HorizontalGreen;
			}

		}
		}

	}
}
