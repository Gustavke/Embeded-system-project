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

// Struct to represent road directions
typedef struct {
	TrafficLight trafficLight1;
	TrafficLight trafficLight2;
	TrafficLight pedestrianLight;
} Road;

extern const TrafficLight tl1;
extern const TrafficLight tl2;
extern const TrafficLight tl3;
extern const TrafficLight tl4;
extern const TrafficLight pl1;
extern const TrafficLight pl2;

extern const Road verticalRoad;
extern const Road horizontalRoad;

extern uint8_t pd1Pressed;
extern uint8_t pd2Pressed;

extern uint32_t trafficLightBuffer;

void traffic_buffer_set_bit(uint8_t offset);
void traffic_buffer_reset_bit(uint8_t offset);
void traffic_buffer_toggle_bit(uint8_t offset);
void transmit_buffer(void);
void toggle_blue_light(TrafficLight tl);
void set_traffic_light(TrafficLight tl, uint8_t color);
void set_traffic_lights(Road road, uint8_t color);
void _turn_off_lights();
void set_pedestrian_button_pressed(uint8_t button);
void reset_pedestrian_button_pressed(uint8_t button);
int pedestrian_button_pressed(uint8_t crossing);
int is_car_present(uint8_t lane);
int h_car_present();
int v_car_present();
void transistion_horizontal_to_green(uint32_t yellowDelay);
void transistion_vertical_to_green(uint32_t yellowDelay);

#endif /* INC_TRAFFIC_LIGHT_FUNCTIONS_H_ */
