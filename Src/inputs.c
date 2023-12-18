/*
 * inputs.c
 *
 *  Created on: Dec 18, 2023
 *      Author: Gustav
 */

/**
 * @file inputs.c
 * @brief Implementation of functions for handling pedestrian buttons and car presence detection.
 */

#include "stdint.h"
#include "gpio.h"

// Global variables to track the state of pedestrian buttons
uint8_t pd1Pressed = 0; /**< Flag indicating the state of pedestrian button 1. */
uint8_t pd2Pressed = 0; /**< Flag indicating the state of pedestrian button 2. */

/**
 * @brief Set the specified pedestrian button as pressed.
 *
 * @param button The pedestrian button number to set as pressed (1 or 2).
 */
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

/**
 * @brief Reset the specified pedestrian button as not pressed.
 *
 * @param button The pedestrian button number to reset (1 or 2).
 */
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

/**
 * @brief Check if the specified pedestrian button is pressed.
 *
 * @param crossing The pedestrian crossing number to check (1 or 2).
 * @return 1 if the pedestrian button is pressed, 0 if not, -1 for an invalid crossing number.
 */
int pedestrian_button_pressed(uint8_t crossing) {
    switch (crossing) {
    case 1:
        return pd1Pressed;
        break;
    case 2:
        return pd2Pressed;
        break;
    }
    return -1; // Invalid crossing number
}

/**
 * @brief Check if a car is present in the specified lane.
 *
 * @param lane The lane number to check (1 to 4).
 * @return 1 if a car is present, 0 if not, -1 for an invalid lane number.
 */
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
    return -1; // Invalid lane number
}

/**
 * @brief Check if a car is present in the horizontal lanes.
 *
 * @return 1 if a car is present in horizontal lanes, 0 if not.
 */
int h_car_present() {
    if (is_car_present(1) || is_car_present(3))
        return 1; // Car present in horizontal lanes
    return 0; // No car in horizontal lanes
}

/**
 * @brief Check if a car is present in the vertical lanes.
 *
 * @return 1 if a car is present in vertical lanes, 0 if not.
 */
int v_car_present() {
    if (is_car_present(2) || is_car_present(4))
        return 1; // Car present in vertical lanes
    return 0; // No car in vertical lanes
}
