/*
 * traffic_light_functions.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Gustav
 */

/**
 * @file traffic_light_functions.c
 * @brief Implementation of functions for controlling traffic lights.
 */

// Include necessary header files
#include <stdint.h>
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "shiftregister_driver.h"

/**
 * @brief Set the color of traffic lights on a road and update the pedestrian light accordingly.
 *
 * @param road The road structure containing information about traffic lights.
 * @param color The color to set for the traffic lights.
 */
void set_traffic_lights(Road road, enum Colors color) {
    // Check if the TIM1_UP_TIM16_IRQn interrupt is enabled and temporarily disable it
    uint8_t isInterruptEnabled =
            (NVIC->ISER[(uint32_t) ((int32_t) TIM1_UP_TIM16_IRQn) >> 5]
                    & (uint32_t) (1
                            << ((uint32_t) ((int32_t) TIM1_UP_TIM16_IRQn)
                                    & (uint32_t) 0x1F)));

    if (isInterruptEnabled)
        HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

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

    // Re-enable the TIM1_UP_TIM16_IRQn interrupt if it was initially enabled
    if (isInterruptEnabled)
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

/**
 * @brief Transition the vertical road to green with a specified yellow delay.
 *
 * @param yellowDelay The duration of the yellow light before the transition.
 */
void transition_vertical_to_green(uint32_t yellowDelay) {
    // Set horizontal road lights to yellow, wait, and then set them to red
    set_traffic_lights(horizontalRoad, Yellow);
    HAL_Delay(yellowDelay);
    set_traffic_lights(horizontalRoad, Red);

    // Temporarily disable the TIM1_UP_TIM16_IRQn interrupt to stop blinking
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

    // Set vertical road lights to yellow, wait, and then set them to green
    set_traffic_lights(verticalRoad, Yellow);
    HAL_Delay(yellowDelay);
    set_traffic_lights(verticalRoad, Green);
}

/**
 * @brief Transition the horizontal road to green with a specified yellow delay.
 *
 * @param yellowDelay The duration of the yellow light before the transition.
 */
void transition_horizontal_to_green(uint32_t yellowDelay) {
    // Set vertical road lights to yellow, wait, and then set them to red
    set_traffic_lights(verticalRoad, Yellow);
    HAL_Delay(yellowDelay);
    set_traffic_lights(verticalRoad, Red);

    // Temporarily disable the TIM1_UP_TIM16_IRQn interrupt to stop blinking
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

    // Set horizontal road lights to yellow, wait, and then set them to green
    set_traffic_lights(horizontalRoad, Yellow);
    HAL_Delay(yellowDelay);
    set_traffic_lights(horizontalRoad, Green);
}
