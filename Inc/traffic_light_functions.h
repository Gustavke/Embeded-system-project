/*
 * traffic_light_functions.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Gustav
 */

#ifndef INC_TRAFFIC_LIGHT_FUNCTIONS_H_
#define INC_TRAFFIC_LIGHT_FUNCTIONS_H_

#include <stdint.h>
#include "shiftregister_driver.h"

// Function prototypes
void set_traffic_lights(Road road, enum Colors color);
void transition_vertical_to_green(uint32_t yellowDelay);
void transition_horizontal_to_green(uint32_t yellowDelay);

#endif /* INC_TRAFFIC_LIGHT_FUNCTIONS_H_ */
