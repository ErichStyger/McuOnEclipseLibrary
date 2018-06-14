/*
 * Application.c
 *
 *  Created on: 17.02.2018
 *      Author: Erich Styger
 */

#include "platform.h"
#include "Application.h"
#include "FRTOS1.h"
#include "LED1.h"
#include "LED2.h"
#include "WAIT1.h"
#include "ENC1.h"
#include "SW_FWD.h"
#include "SW_REV.h"
#include "DIRA.h"
#include "DIRB.h"
#include "PWMA.h"
#include "PWMB.h"
#include "CLS1.h"
#include "buttons.h"
#include "encoder.h"
#include "motor.h"
#include "comm.h"

#define APP_TIMEOUT (1)
#if APP_TIMEOUT
#include "TU2.h"

static LDD_TDeviceData *timerHandle;
static TU2_TValueType timerVal;
#endif

typedef enum{
  FSM_INIT,
  FSM_IDLE,
  FSM_FWD,
  FSM_REV_PEEL,
  FSM_REV_SPROC,
  FSM_RUN,
  FSM_STOP,
  FSM_ERROR
} FSM_State;

typedef enum{
  PEEL_OFF,
  PEEL_FWD,
  PEEL_REV,
  PEEL_FWD_DONE,
  PEEL_REV_DONE
}PEEL_State;

static FSM_State taskState;
static PEEL_State peelState;
static int APP_Counter; /* desired position of the wheel */
static uint8_t error_res;

void APP_ChangeWheelPos(int delta) {
  CS1_CriticalVariable();

  CS1_EnterCritical();
  APP_Counter += delta;
  CS1_ExitCritical();
}

/*
 * Error Handling for feeder
 *
 * \todo implementing other errors and codes
 */
static void APP_ErrorHandler(void) {
#if APP_TIMEOUT
	timerVal = TU2_GetCounterValue(timerHandle);
	if(timerVal >= 0xA000){
		error_res = ERR_OVERFLOW; //timeout
	}
#endif
	switch(error_res){
	case ERR_FAILED:
		//to be implemented
		break;

	case ERR_VALUE:
		//to be implemented
		// ERR_VALUE could be used in Communication of Step Size
		break;

	case ERR_OVERFLOW:
		COMM_SendError(" OVERFLOW/TIMEOUT");
		taskState = FSM_ERROR;
		break;

	case ERR_OK:
		//to be implemented
		//everything is ok
		break;

	} /* switch */
}

/*
 * iterates through peeler states
 * functionality split from RunFeeder
 *
 * peelState may be changed in RunFeeder
 */
static void RunPeeler(void){
	switch (peelState){
		case PEEL_OFF:
			MOT_Speed(MOT_TAPE,0,MOT_FWD);
			break;

		case PEEL_FWD:
			if(BUT_GetState(BUT_MSW)==0){
				MOT_Speed(MOT_TAPE,100,MOT_FWD);
			}
			break;

		case PEEL_REV:
			MOT_Speed(MOT_TAPE,100,MOT_REV);
			break;

		case PEEL_FWD_DONE:
			if(BUT_GetState(BUT_MSW)==1){	// should change for rev
				peelState = PEEL_OFF;
			}
			break;

		case PEEL_REV_DONE:
			if(BUT_GetState(BUT_MSW)==0){	// should change for rev
				peelState = PEEL_OFF;
			}
			break;

		default:
			peelState = PEEL_OFF;
			break;
	} /* switch */
}

/*
 * Main FSM for Feeder
 *
 * reacts to changes in APP_Counter, for running forward/backwards.
 */
static void RunFeeder(void){
	switch (taskState){
		case FSM_INIT:
			ENC_ResetCnt();
			peelState = PEEL_OFF;;
			MOT_Speed(MOT_SPROC,0,MOT_FWD);
			MOT_Speed(MOT_TAPE,0,MOT_FWD);
			taskState = FSM_IDLE;
			for(int i = 0; i < 4; i++){
				LED1_Neg();
				WAIT1_WaitOSms(100);
			}
			break;

		case FSM_IDLE:
  #if APP_TIMEOUT
      TU2_ResetCounter(timerHandle);
  #endif
      ENC_ResetCnt();
      if(APP_Counter > 0){
        taskState = FSM_FWD;
        APP_Counter--;
        break;
      } else if (APP_Counter < 0) {
        taskState = FSM_REV_PEEL;
        APP_Counter++;
        break;
      } //else 0 -> Stay in IDLE
      if(BUT_GetState(BUT_REV)==BUT_PRESS){
        taskState = FSM_STOP;
        peelState = PEEL_OFF;
        break;
      }
		  break;

		case FSM_FWD:
			// turn sprocket motor forward and count steps
			MOT_Speed(MOT_SPROC,100,MOT_FWD);
			taskState = FSM_RUN;
			peelState = PEEL_FWD;
		  break;

		case FSM_REV_PEEL:
			// First turn peeler back
			peelState = PEEL_REV;
			taskState = FSM_REV_SPROC;
  		break;

		case FSM_REV_SPROC:
			// let Peeler Run for a certain time (time calculated)
			WAIT1_WaitOSms(700);
			// then change case to reverse sprocket
			MOT_Speed(MOT_SPROC,100,MOT_REV);
			taskState = FSM_RUN;
			peelState = PEEL_REV_DONE;
			break;

		case FSM_RUN:
			if(ENC_GetTrigger() == 1){
				ENC_SetTrigger(0);
				taskState  = FSM_IDLE;
				peelState = PEEL_FWD_DONE;
			}
			// use button REV to stop
			if(BUT_GetState(BUT_REV)==BUT_PRESS){
				taskState = FSM_STOP;
				peelState = PEEL_OFF;
				break;
			}
			break;

		case FSM_STOP:
			MOT_Speed(MOT_SPROC, 0, MOT_FWD);
			taskState = FSM_IDLE;
			break;

		case FSM_ERROR:
			MOT_Speed(MOT_SPROC, 0, MOT_FWD);
			MOT_Speed(MOT_TAPE, 0, MOT_FWD);
			for(;;) { /* Blink Status LED */
				WAIT1_WaitOSms(50);
				LED1_Neg();
			}
			break;

		default:
			// should not get here.
			taskState = FSM_ERROR;
	}
}

/*
 * Application Task
 *
 * Contains Feeder FSMs, Button processing and error handler
 */
 static void APP_task(void *param) {
  int cntr = 0;
	(void)param;
	for(;;) {
		RunFeeder();
		RunPeeler();
		BUT_Process();
		APP_ErrorHandler();
		if (cntr==0) {
      LED1_Off();
      cntr++;
		} else if (cntr==20) { /* every second a heartbeat */
      cntr = 0;
		  LED1_On(); /* on for 50 ms */
		} else {
	    cntr++;
		}
    vTaskDelay(pdMS_TO_TICKS(50));
	} /* for */
}

 /*
  * Initializes all components belonging to app
  */
void APP_Init(void){
	error_res = ERR_OK;		// set ErrorFlag to OK
	APP_Counter = 0;		// set start to 0 (no inc/dec of tape needed)
	taskState = FSM_INIT;
	timerHandle = TU2_Init(NULL);
	BUT_Init();
}

/*
 * creates APP Task
 */
void APP_Run(void) {

	if (xTaskCreate(APP_task, "FSM", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}

	vTaskStartScheduler();
	for(;;) {
	}
}
