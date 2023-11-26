/*
 * traffic_light_functions.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Gustav
 */

#ifndef INC_TRAFFIC_LIGHT_FUNCTIONS_H_
#define INC_TRAFFIC_LIGHT_FUNCTIONS_H_

#include <stdint.h>

#define TL1_OFFSET 16
#define TL2_OFFSET 8
#define TL3_OFFSET 0
#define TL4_OFFSET 3

#define TL_RED 0b001
#define TL_YELLOW 0b010
#define TL_GREEN 0b100

enum Colors {
	Red, Green, Yellow
};

typedef struct {
    uint8_t offset;
    uint8_t red;
    uint8_t yellow;
    uint8_t green;
} TrafficLight;

extern const TrafficLight tl1;
extern const TrafficLight tl2;
extern const TrafficLight tl3;
extern const TrafficLight tl4;

extern uint32_t trafficLightBuffer;

void traffic_buffer_set_bit(uint8_t offset);
void traffic_buffer_reset_bit(uint8_t offset);
void transmit_buffer(void);
void set_traffic_light(TrafficLight tl, uint8_t color);

#endif /* INC_TRAFFIC_LIGHT_FUNCTIONS_H_ */
