/*
 * traffic_light_functions.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Gustav
 */

#ifndef INC_TRAFFIC_LIGHT_FUNCTIONS_H_
#define INC_TRAFFIC_LIGHT_FUNCTIONS_H_

#include <stdint.h>

#define TL1_RED_OFFSET    12
#define TL1_YELLOW_OFFSET 13
#define TL1_GREEN_OFFSET  14
#define PL1_RED_OFFSET    15
#define PL1_GREEN_OFFSET  16
#define PL1_BLUE_OFFSET   17

#define TL2_RED_OFFSET    6
#define TL2_YELLOW_OFFSET 7
#define TL2_GREEN_OFFSET  8
#define PL2_RED_OFFSET    9
#define PL2_GREEN_OFFSET  10
#define PL2_BLUE_OFFSET   11

#define TL3_RED_OFFSET    0
#define TL3_YELLOW_OFFSET 1
#define TL3_GREEN_OFFSET  2
#define TL4_RED_OFFSET    3
#define TL4_YELLOW_OFFSET 4
#define TL4_GREEN_OFFSET  5

extern uint32_t trafficLightBuffer;

void traffic_light_set(uint8_t offset);

void transmit_buffer();

#endif /* INC_TRAFFIC_LIGHT_FUNCTIONS_H_ */
