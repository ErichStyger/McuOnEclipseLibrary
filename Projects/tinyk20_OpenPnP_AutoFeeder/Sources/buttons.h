/*
 * buttons.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#ifndef SOURCES_BUTTONS_H_
#define SOURCES_BUTTONS_H_

#include "platform.h"
//#include "stdint.h"

/*
 * Invert High/Low Logic of buttons
 */
# define FWD_INVERT (1)
# define REV_INVERT (1)

#if PL_TAPE_EN
# define MSW_INVERT	(1)
#endif


typedef enum{
#if PL_TAPE_EN
	BUT_MSW,
#endif
	BUT_FWD,
	BUT_REV
}BUT_Device;


// DO NOT ADD BEFORE BUT_PRESS!!! - Direct state acquisition from IO
typedef enum{
	BUT_IDLE,		//off state	- > do not change
	BUT_PRESS,		// on state - > do not change
	BUT_LPRESS
}BUT_State;

/*
 * Return state of Button (BUT_State)
 */
BUT_State BUT_GetState(BUT_Device button);

/*
 * Tick Counter (called by RTOS Tick Hook)
 */
void BUT_Count(void);

/*
 * iterates through all buttons to set their states
 */
void BUT_Process(void);

/*
 * Initializes button relevant flags and counters
 */
void BUT_Init(void);



#endif /* SOURCES_BUTTONS_H_ */
