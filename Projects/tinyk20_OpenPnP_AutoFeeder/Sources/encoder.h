/*
 * encoder.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */

#ifndef SOURCES_HARDWARE_ENCODER_H_
#define SOURCES_HARDWARE_ENCODER_H_

#include "platform.h"

#define ENC_INV		(0)		/* set depending on encoder connection 0/1 */
#define ENC_TICKS	(2)		/* Amount of 2 mm steps for component feed */

/*
 * Sets/Resets Trigger/Event state that step count is reached
 */
void ENC_SetTrigger(unsigned int tmp);

/*
* Returns Trigger/Event state that step count is reached
*/
unsigned int ENC_GetTrigger(void);

/*
 * resets step counter
 */
void ENC_ResetCnt(void);

/*
 * returns step size
 */
unsigned int ENC_GetStep(void);

/*
 * sets step size to value of parameter stepSize
 */
void ENC_SetStep(unsigned int stepSize);

/*
 * initializes counters and flags
 * starts ENC Task
 */
void ENC_Init(void);

#endif /* SOURCES_HARDWARE_ENCODER_H_ */
