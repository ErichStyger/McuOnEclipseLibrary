/*
 * platform.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#ifndef SOURCES_PLATFORM_H_
#define SOURCES_PLATFORM_H_

#define PL_BUTTON_EN		(1)		// Configuration uses Buttons (FW/RV)
#define PL_ENCODER_EN		(1)		// Configuration uses Optical Encoder
#define PL_MOTOR_EN			(1)		// Configuration uses Sprocket and/or Tape Peeler Motor
#define PL_TAPE_EN			(1)		// Configuration uses Tape Peeler
#define PL_COMM_EN			(1)		// Configuration uses UART Communication


void PL_Init();

#endif /* SOURCES_PLATFORM_H_ */
