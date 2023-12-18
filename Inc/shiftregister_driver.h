/*
 * shiftregister_driver.h
 *
 *  Created on: Dec 18, 2023
 *      Author: Gustav
 */

#ifndef INC_SHIFTREGISTER_DRIVER_H_
#define INC_SHIFTREGISTER_DRIVER_H_

// Constants for traffic light offsets and colors
#define TL1_OFFSET 16
#define TL2_OFFSET 8
#define TL3_OFFSET 0
#define TL4_OFFSET 3

#define PL1_OFFSET  19
#define PL2_OFFSET 11

#define TL_RED    0b001
#define TL_YELLOW 0b010
#define TL_GREEN  0b100

#define PL_RED 0b001
#define PL_GREEN 0b010
#define PL_BLUE 0b100

// Struct to represent a traffic light
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

// Enum to represent traffic light colors
enum Colors {
    Red, Green, Yellow, //Blue
};

// External declarations for global variables
extern const TrafficLight tl1;
extern const TrafficLight tl2;
extern const TrafficLight tl3;
extern const TrafficLight tl4;
extern const TrafficLight pl1;
extern const TrafficLight pl2;
extern const Road verticalRoad;
extern const Road horizontalRoad;
extern uint32_t trafficLightBuffer;

// Function prototypes
void traffic_buffer_set_bit(uint8_t offset);
void traffic_buffer_reset_bit(uint8_t offset);
void traffic_buffer_toggle_bit(uint8_t offset);
void transmit_buffer(void);
void set_shiftregister_buffer(TrafficLight tl, enum Colors color);
void toggle_blue_light(TrafficLight tl);
void _turn_off_lights();

#endif /* INC_SHIFTREGISTER_DRIVER_H_ */
