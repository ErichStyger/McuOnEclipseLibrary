/*
 * encoder.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */

#ifndef SOURCES_HARDWARE_ENCODER_H_
#define SOURCES_HARDWARE_ENCODER_H_

#include "platform.h"
#include <stdint.h>

#define ENC_STEP_SIZE_MM   (2) /* encoder on sprocket is for 2 mm steps */

/* request ENC_STEP_SIZE_MM mm forward or backward steps */
void ENC_RequestSteps(int8_t nofSteps);

/*
 * initializes counters and flags
 * starts ENC Task
 */
void ENC_Init(void);

#endif /* SOURCES_HARDWARE_ENCODER_H_ */
