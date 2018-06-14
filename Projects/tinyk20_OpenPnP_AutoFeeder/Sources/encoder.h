/*
 * encoder.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */

#ifndef SOURCES_HARDWARE_ENCODER_H_
#define SOURCES_HARDWARE_ENCODER_H_

/*
 * Sets/Resets Trigger/Event state that step count is reached
 */
void ENC_SetTrigger(unsigned int val);

/*
* Returns Trigger/Event state that step count is reached
*/
unsigned int ENC_GetTrigger(void);

/*
 * resets step counter
 */
void ENC_ResetCnt(void);

/*
 * initializes counters and flags
 * starts ENC Task
 */
void ENC_Init(void);

#endif /* SOURCES_HARDWARE_ENCODER_H_ */
