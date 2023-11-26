/*
 * test.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Gustav
 */
#include "stdint.h"
#include  "stdbool.h"
#include  "traffic_light_functions.h"
void test_traffic_light(){
	for(int i = 0; i < 18; i++){
	traffic_light_set(i);
	transmit_buffer();
	}
}


void test_program(void) {
	test_traffic_light();

}


