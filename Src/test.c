/*
 * test.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Gustav
 */
#include "stdint.h"
#include  "stdbool.h"
#include  "traffic_light_functions.h"
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


void test_program(void) {
	test_shiftregister_buffer();

}


