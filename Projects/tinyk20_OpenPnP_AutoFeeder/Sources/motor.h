/*
 * motor_sproc.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */

#ifndef SOURCES_HARDWARE_MOTOR_H_
#define SOURCES_HARDWARE_MOTOR_H_

#include "platform.h"
#include <stdint.h>

#if PL_MOTOR_EN

#define MAX_SPEED (100)

typedef enum{
	// MOT_NULL,	// 0 is not driving
	MOT_FWD,
	MOT_REV
} MOT_Direction;

typedef enum{
	MOT_SPROC,
	MOT_TAPE
} MOT_Device;

/*
 * sets motor speed and direction
 * parameters: Motor device, speed (0-100), direction (FWD/REV)
 *
 * see typedefs for direction parameters
 */
uint8_t MOT_Speed(MOT_Device motor, uint8_t speedPercent, MOT_Direction dir);

/*
 * Initializes motor to 0 speed (Stop)
 * sets H-Bridge Mode
 */
void MOT_Init(void);

#endif

#endif /* SOURCES_HARDWARE_MOTOR_H_ */
