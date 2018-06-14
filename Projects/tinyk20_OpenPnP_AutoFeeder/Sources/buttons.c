/*
 * buttons.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */
#include "buttons.h"
#include "Application.h"
#include "CS1.h"
#include "LED2.h"
#include "SW_FWD.h"
#include "SW_REV.h"
#if PL_TAPE_EN
  #include "SW_MSW.h" /* switch for tape */
#endif

#define LPRESS_CNT (200)		// change to react to task time delay

static volatile int REV_Counter;
static volatile uint8_t  REV_Flag, REV_Flag_LPress;
static volatile uint8_t  FWD_Flag;

/*
 * Iterate through button state
 * Switch case for each button type
 *
 * \todo improve using button-type-struct with state data
 *
 */
BUT_State BUT_GetState(BUT_Device button) {
	BUT_State but_state = BUT_IDLE;// = BUT_IDLE;
	uint8_t state = 0;

	switch (button){
		case BUT_FWD:
			state = SW_FWD_GetVal();
			if (FWD_INVERT) {
				if (state == 1){
					but_state = BUT_IDLE;
				}else{
					but_state = BUT_PRESS;
				}
			}else{
				if (state == 1){
					but_state = BUT_PRESS;
				}else{
					but_state = BUT_IDLE;
				}
			}
			if ((but_state == BUT_PRESS) && (FWD_Flag == 0)){
			  APP_ChangeWheelPos(1);
				FWD_Flag = 1;
			} else if ((but_state == BUT_IDLE) && (FWD_Flag == 1)){
				FWD_Flag = 0;
			}
		break;

		case BUT_REV:
			state = SW_REV_GetVal();
			if (REV_INVERT) {
				if (state == 1){
					but_state = BUT_IDLE;
				}else{
					but_state = BUT_PRESS;
				}
			}else{
				if (state == 1){
					but_state = BUT_PRESS;
				}else{
					but_state = BUT_IDLE;
				}
			}
			if (but_state == BUT_IDLE){
				REV_Flag = 0;
				REV_Flag_LPress = 0;
				REV_Counter = 0;
				//break;
			}else if ((but_state == BUT_PRESS) && (REV_Flag == 0)){
				REV_Flag = 1;
				REV_Counter = 0;
			}  else if (but_state == BUT_PRESS){
				REV_Counter++;
				if (REV_Counter >= LPRESS_CNT){
					//REV_Counter = 0;
					but_state = BUT_LPRESS;
					if(REV_Flag_LPress == 0){
					  APP_ChangeWheelPos(-1);
					}
					REV_Flag_LPress = 1;
				}
			}
		break;

#if PL_TAPE_EN
		case BUT_MSW:
			state = SW_MSW_GetVal();
			if (MSW_INVERT) {
				if (state == 1){
					but_state = BUT_IDLE;
				}else{
					but_state = BUT_PRESS;
				}
			}else{
				if (state == 1){
					but_state = BUT_PRESS;
				}else{
					but_state = BUT_IDLE;
				}
			}
		break;
#endif

		default:
		{
			state = 0;
			but_state = BUT_PRESS;
		}
	}
	return but_state;
}

/*
 * Counts Ticks when REV Button pressed
 *
 */
void BUT_Count(void) {
	CS1_CriticalVariable();

	CS1_EnterCritical();
	REV_Counter++;
	CS1_ExitCritical();
}

/*
 * iterates through all buttons to set their states
 */
void BUT_Process(void){
	(void)BUT_GetState(BUT_MSW);
	(void)BUT_GetState(BUT_REV);
	(void)BUT_GetState(BUT_FWD);
}

/*
 * Initializes button relevant flags and counters
 */
void BUT_Init(void){
	FWD_Flag = 0;
	REV_Flag = 0;
	REV_Counter = 0;
}
