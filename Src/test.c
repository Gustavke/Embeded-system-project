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
	set_traffic_light(tl1,Green);
	set_traffic_light(tl1,Yellow);
	set_traffic_light(tl1,Red);

	set_traffic_light(tl2,Green);
	set_traffic_light(tl2,Yellow);
	set_traffic_light(tl2,Red);

	set_traffic_light(tl3,Green);
	set_traffic_light(tl3,Yellow);
	set_traffic_light(tl3,Red);

	set_traffic_light(tl4,Green);
	set_traffic_light(tl4,Yellow);
	set_traffic_light(tl4,Red);

}


void test_program(void) {
	test_traffic_light();

}


