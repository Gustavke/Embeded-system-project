/*
 * traffic_light.h
 *
 *  Created on: Dec 12, 2023
 *      Author: Gustav
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

typedef enum {
	PedestrianRed, PedestrianWaiting, PedestrianGreen
} states;

static states State, NextState;
void traffic_light();

#endif /* INC_TRAFFIC_LIGHT_H_ */
