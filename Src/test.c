/*
 * test.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Gustav
 */
#include "stdint.h"
#include  "stdbool.h"
#include  "traffic_light_functions.h"
#include "tim.h"
#include "stm32l4xx_it.h"
// Function to test the shift register buffer by setting different colors and transmitting

void test_shiftregister_buffer() {
	// Set and transmit traffic light 1 (tl1) colors
	set_shiftregister_buffer(tl1, Green);
	transmit_buffer();
	set_shiftregister_buffer(tl1, Yellow);
	transmit_buffer();
	set_shiftregister_buffer(tl1, Red);
	transmit_buffer();

	// Set and transmit traffic light 2 (tl2) colors
	set_shiftregister_buffer(tl2, Green);
	transmit_buffer();
	set_shiftregister_buffer(tl2, Yellow);
	transmit_buffer();
	set_shiftregister_buffer(tl2, Red);
	transmit_buffer();

	// Set and transmit traffic light 3 (tl3) colors
	set_shiftregister_buffer(tl3, Green);
	transmit_buffer();
	set_shiftregister_buffer(tl3, Yellow);
	transmit_buffer();
	set_shiftregister_buffer(tl3, Red);
	transmit_buffer();

	// Set and transmit traffic light 4 (tl4) colors
	set_shiftregister_buffer(tl4, Green);
	transmit_buffer();
	set_shiftregister_buffer(tl4, Yellow);
	transmit_buffer();
	set_shiftregister_buffer(tl4, Red);
	transmit_buffer();

	// Set and transmit pedestrian light 1 (pl1) colors
	set_shiftregister_buffer(pl1, Green);
	transmit_buffer();
	set_shiftregister_buffer(pl1, Red);
	transmit_buffer();

	// Set and transmit pedestrian light 2 (pl2) colors
	set_shiftregister_buffer(pl2, Green);
	transmit_buffer();
	set_shiftregister_buffer(pl2, Red);
	transmit_buffer();
}

void test_blue_light_toggle() {
	//Turn off all lights before test start
	_turn_off_lights();
	//Toggle pedestrian light 1
	toggle_blue_light(pl1);
	toggle_blue_light(pl1);
	//Toggle pedestrian light 2
	toggle_blue_light(pl2);
	toggle_blue_light(pl2);
}

void test_traffic_lights() {

	//Turn off all lights before test start
	_turn_off_lights();

	//Set each color for traffic lights on vertical road
	set_traffic_lights(verticalRoad, Red);
	set_traffic_lights(verticalRoad, Yellow);
	set_traffic_lights(verticalRoad, Green);

	//Set each color for traffic lights on horizontal road
	set_traffic_lights(horizontalRoad, Red);
	set_traffic_lights(horizontalRoad, Yellow);
	set_traffic_lights(horizontalRoad, Green);

}

void test_car_switches() {
	_turn_off_lights();
	while (1) {
		if (is_car_present(1))
			set_shiftregister_buffer(tl1, Green);
		else
			set_shiftregister_buffer(tl1, Red);

		if (is_car_present(2))
			set_shiftregister_buffer(tl2, Green);
		else
			set_shiftregister_buffer(tl2, Red);

		if (is_car_present(3))
			set_shiftregister_buffer(tl3, Green);
		else
			set_shiftregister_buffer(tl3, Red);

		if (is_car_present(4))
			set_shiftregister_buffer(tl4, Green);
		else
			set_shiftregister_buffer(tl4, Red);

		transmit_buffer();
	}
}

void test_pedestrian_buttons() {
	_turn_off_lights();
	while (1) {
		if (pedestrian_button_pressed(1)) {
			set_shiftregister_buffer(tl1, Green);
			transmit_buffer();
			while (pedestrian_button_pressed(1))
				;
		} else {
			set_shiftregister_buffer(tl1, Red);
			transmit_buffer();
		}

		if (pedestrian_button_pressed(2)) {
			set_shiftregister_buffer(tl2, Green);
			transmit_buffer();
			while (pedestrian_button_pressed(2))
				;
		} else {
			set_shiftregister_buffer(tl2, Red);
			transmit_buffer();
		}
	}
}

void test_blinking_light() {
	while (1) {
		TIM16->ARR = 10000;
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
		HAL_Delay(5000);
		TIM16->ARR = 5000;
		HAL_Delay(5000);
		HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
		HAL_Delay(5000);
	}
}
void test_if_interrupt_enabled() {
	_turn_off_lights();
	while (1) {
		_turn_off_lights();

		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
		if (NVIC->ISER[(uint32_t) ((int32_t) TIM1_UP_TIM16_IRQn) >> 5]
				& (uint32_t) (1
						<< ((uint32_t) ((int32_t) TIM1_UP_TIM16_IRQn)
								& (uint32_t) 0x1F))) {
			set_traffic_lights(verticalRoad, Green);
		}
		HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
		if (NVIC->ISER[(uint32_t) ((int32_t) TIM1_UP_TIM16_IRQn) >> 5]
				& (uint32_t) (1
						<< ((uint32_t) ((int32_t) TIM1_UP_TIM16_IRQn)
								& (uint32_t) 0x1F))) {
			set_traffic_lights(verticalRoad, Red);
		}

	}
}
void test_program(void) {
	test_if_interrupt_enabled();
	test_blinking_light();
	//test_pedestrian_buttons();
	test_car_switches();
	test_blue_light_toggle();
	test_shiftregister_buffer();
	test_traffic_lights();
}

