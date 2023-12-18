/*
 * inputs.h
 *
 *  Created on: Dec 18, 2023
 *      Author: Gustav
 */

#ifndef INC_INPUTS_H_
#define INC_INPUTS_H_

// Global variables to track the state of pedestrian buttons
extern uint8_t pd1Pressed;
extern uint8_t pd2Pressed;

// Function prototypes
void set_pedestrian_button_pressed(uint8_t button);
void reset_pedestrian_button_pressed(uint8_t button);
int pedestrian_button_pressed(uint8_t crossing);
int is_car_present(uint8_t lane);
int h_car_present();
int v_car_present();

#endif /* INC_INPUTS_H_ */
