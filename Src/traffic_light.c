/*
 * traffic_light.c
 *
 *  Created on: Dec 12, 2023
 *      Author: Gustav
 */

#include "traffic_light_functions.h"
#include "tim.h"
#include "stdint.h"
#include "inputs.h"
// Define states for state machine
typedef enum {
	VerticalGreen,
	VerticalWaiting,
	HorizontalGreen,
	HorizontalWaiting,
	Pedestrian1Waiting,
	Pedestrian2Waiting
} states;

static states State, NextState;

void traffic_light_3(void) {

	// Set durations for delays
	uint32_t toggleFreq = 500;
	uint32_t pedestrianDelay = 10000;
	uint32_t walkingDelay = 20000;
	uint32_t yellowDelay = 3000;
	uint32_t greenDelay = 20000;
	uint32_t redDelayMax = 20000;

	//Initialize traffic light
	TIM16->ARR = 10 * toggleFreq; //Set period of timer used for blinking
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt for blinking timer
	set_traffic_lights(verticalRoad, Green);
	set_traffic_lights(horizontalRoad, Red);

	uint32_t waitStart = HAL_GetTick();
	uint32_t pedestrianWalkStart = waitStart - walkingDelay;

	State = VerticalGreen;
	NextState = VerticalGreen;

	// Infinite loop to control the traffic light
	while (1) {
		State = NextState; // Update the current state

		// State machine logic
		switch (State) {
		case VerticalGreen:
			//Check if walking delay has elapsed
			if (HAL_GetTick() > pedestrianWalkStart + walkingDelay) {
				//Check if pedestrian button has been pressed
				if (pedestrian_button_pressed(2)) {
					HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); //Disable interrupt for PL2 button
					waitStart = HAL_GetTick();
					NextState = Pedestrian2Waiting;
				}
				//Check if vertical lanes are free from cars
				else if (!v_car_present()) {
					//Check if horizontal lanes have cars or greenDelay has elapsed
					if (h_car_present()
							|| HAL_GetTick() - waitStart > greenDelay) {
						transition_horizontal_to_green(yellowDelay);
						HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
						waitStart = HAL_GetTick();
						HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); //Disable interrupt for PL2 button
						HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable interrupt for PL1 button
						reset_pedestrian_button_pressed(2);
						NextState = HorizontalGreen;
					}
				}
				//Check if horizontal lanes have cars
				else if (h_car_present()) {
					waitStart = HAL_GetTick();
					NextState = HorizontalWaiting;
				}
			}

			break;

		case HorizontalGreen:
			//Check if walking delay has elapsed
			if (HAL_GetTick() > pedestrianWalkStart + walkingDelay) {
				//Check if pedestrian button has been pressed
				if (pedestrian_button_pressed(1)) {
					HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); //Disable interrupt for PL1 button
					waitStart = HAL_GetTick();
					NextState = Pedestrian1Waiting;
				}
				//Check if horizontal lanes are free from cars
				else if (!h_car_present()) {
					//Check if vertical lanes have cars or greenDelay has elapsed
					if (v_car_present()
							|| HAL_GetTick() - waitStart > greenDelay) {
						transition_vertical_to_green(yellowDelay);
						HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
						waitStart = HAL_GetTick();
						HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); //Disable interrupt for PL1 button
						HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); //Enable interrupt for PL2 button
						reset_pedestrian_button_pressed(1);
						NextState = VerticalGreen;
					}
				}
				//Check if horizontal lanes have cars
				else if (v_car_present()) {
					waitStart = HAL_GetTick();
					NextState = VerticalWaiting;
				}
			}

			break;

		case VerticalWaiting:
			//Check if horizontal lanes are free from cars or redDelayMax has elapsed
			if (!h_car_present()
					|| HAL_GetTick() - waitStart
							> redDelayMax - 2 * yellowDelay) {
				transition_vertical_to_green(yellowDelay);
				HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt blinking timer
				reset_pedestrian_button_pressed(1);
				waitStart = HAL_GetTick();
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); //Enable interrupt for PL2 button
				NextState = VerticalGreen;
			}
			//Check if pedestrian button has been pressed
			else if (pedestrian_button_pressed(1)) {
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); //Disable interrupt for PL1 button
				//Check if pedestrianDelay is less than the remaining redDelayMax
				if (pedestrianDelay
						< redDelayMax - (HAL_GetTick() - waitStart)) {
					waitStart = HAL_GetTick();
					NextState = Pedestrian1Waiting;
				}
			}
			break;
		case HorizontalWaiting: {
			//Check if vertical lanes are free from cars or redDelayMax has elapsed
			if (!v_car_present()
					|| HAL_GetTick() - waitStart
							> redDelayMax - 2 * yellowDelay) {
				transition_horizontal_to_green(yellowDelay);
				HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt blinking timer
				reset_pedestrian_button_pressed(2);
				waitStart = HAL_GetTick();
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable interrupt for PL1 button
				NextState = HorizontalGreen;
			}
			//Check if pedestrian button has been pressed
			else if (pedestrian_button_pressed(2)) {
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				//Check if pedestrianDelay is less than the remaining redDelayMax
				if (pedestrianDelay
						< redDelayMax - (HAL_GetTick() - waitStart)) {
					waitStart = HAL_GetTick();
					NextState = Pedestrian2Waiting;
				}

			}
		}
			break;
		case Pedestrian2Waiting:
			//Check if vertical lanes are free from cars or pedestrianDelay has elapsed
			if (!v_car_present()
					|| HAL_GetTick() - waitStart
							> pedestrianDelay - yellowDelay) {
				transition_horizontal_to_green(yellowDelay);
				HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt blinking timer
				reset_pedestrian_button_pressed(2);
				waitStart = HAL_GetTick();
				pedestrianWalkStart = HAL_GetTick();
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable interrupt for PL1 button
				NextState = HorizontalGreen;
			}
			//Check if cars are present in the horizontal lane and the redDelayMax is less than the remaining pedestrianDelay
			else if (h_car_present()
					&& redDelayMax
							< pedestrianDelay - (HAL_GetTick() - waitStart)) {
				waitStart = HAL_GetTick();
				pedestrianWalkStart = waitStart + redDelayMax;
				NextState = HorizontalWaiting;
			}

			break;
		case Pedestrian1Waiting:
			//Check if horizontal lanes are free from cars or pedestrianDelay has elapsed
			if (!h_car_present()
					|| HAL_GetTick() - waitStart
							> pedestrianDelay - yellowDelay) {
				transition_vertical_to_green(yellowDelay);
				HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn); // Disable interrupt blinking timer
				reset_pedestrian_button_pressed(1);
				waitStart = HAL_GetTick();
				pedestrianWalkStart = HAL_GetTick();
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); //Enable interrupt for PL1 button
				NextState = VerticalGreen;
			}
			//Check if cars are present in the vertical lane and the redDelayMax is less than the remaining pedestrianDelay
			else if (v_car_present()
					&& redDelayMax
							< pedestrianDelay - (HAL_GetTick() - waitStart)) {
				waitStart = HAL_GetTick();
				pedestrianWalkStart = waitStart + redDelayMax;
				NextState = VerticalWaiting;
			}

			break;

		}
	}
}
