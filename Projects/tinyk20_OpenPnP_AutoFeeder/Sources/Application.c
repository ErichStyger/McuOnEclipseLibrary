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
#include "SW_PEELER.h"
#include "CLS1.h"
#include "encoder.h"
#include "motor.h"
#include "comm.h"
#include "SW_PEELER.h"

static void APP_FeederStepForward(void) {
  int32_t timeoutMs = 5000;

  COMM_SendStr("INFO: Step +1\r\n", FALSE);
  ENC_RequestSteps(1); /* request to move sprocket one step forward */
  vTaskDelay(pdMS_TO_TICKS(1000)); /* give time to move the sprocket */
  if (SW_PEELER_GetVal()!=0) { /* peeler switch not pressed ==> need to tighen the tape */
    MOT_Speed(MOT_TAPE, 100); /* tighten tape */
    do { /* pull the cover tape until switch is pressed */
      vTaskDelay(pdMS_TO_TICKS(500));
      timeoutMs -= 500;
      if (timeoutMs<0) {
        COMM_SendStr("ERROR: StepBackTapeTimeout\r\n", TRUE);
        COMM_SendError(" StepBackTapeTimeout");
        break;
      }
    } while (SW_PEELER_GetVal()!=0);
  }
  MOT_Speed(MOT_TAPE, 0); /* turn off motor */
}

static void APP_FeederStepBackward(void) {
  int32_t timeoutMs = 5000;

  COMM_SendStr("INFO: Step -1\r\n", FALSE);
  MOT_Speed(MOT_TAPE, -100); /* release the tape */
  vTaskDelay(pdMS_TO_TICKS(800)); /* give some time to release */
  if (SW_PEELER_GetVal()==0) { /* peeler switch still pressed ==> continue to release the tape */
    do {
      vTaskDelay(pdMS_TO_TICKS(100)); /* release the cover tape until the switch is released */
      timeoutMs -= 100;
      if (timeoutMs<0) {
        COMM_SendStr("ERROR: StepForwardTapeFWTimeout\r\n", TRUE);
        COMM_SendError(" StepForwardTapeFWTimeout");
        break;
      }
    } while (SW_PEELER_GetVal()==0);
  }
  /* peeler switch is released here */
  MOT_Speed(MOT_TAPE, 0); /* stop the tape motor */
  ENC_RequestSteps(-1); /* request to move sprocket one step backward */
  vTaskDelay(pdMS_TO_TICKS(1000)); /* give time to move the sprocket */
  /* tighten again the tape */
  timeoutMs = 5000;
  if (SW_PEELER_GetVal()!=0) { /* peeler switch not pressed ==> tighten the tape */
    MOT_Speed(MOT_TAPE, 100);
    do {
      vTaskDelay(pdMS_TO_TICKS(100)); /* pull the cover tape */
      timeoutMs -= 100;
      if (timeoutMs<0) {
        COMM_SendStr("ERROR: StepBackwardTapeBWTimeout\r\n", TRUE);
        COMM_SendError(" StepBackwardTapeBWTimeout");
        break;
      }
    } while (SW_PEELER_GetVal()!=0);
  }
  /* here the peeler switch is pressed */
  MOT_Speed(MOT_TAPE, 0); /* turn off motor */
}

/*
 * Application Task
 *
 * Contains Feeder FSMs, Button processing and error handler
 */
static void APP_task(void *param) {
  #define TASK_PERIOD_MS  (20)
  int cntr = 0;

	(void)param;
	for(;;) {
	  if (SW_FWD_GetVal()==0) { /* pressed */
	    vTaskDelay(pdMS_TO_TICKS(100)); /* debounce */
	    if (SW_FWD_GetVal()==0) { /* still pressed? */
	      APP_FeederStepForward();
	    }
	  }
	  if (SW_REV_GetVal()==0) { /* pressed */
      vTaskDelay(pdMS_TO_TICKS(100)); /* debounce */
      if (SW_REV_GetVal()==0) { /* still pressed? */
        APP_FeederStepBackward();
      }
	  }
		/* show heartbeat */
		if (cntr==0) {
      LED1_Off();
      cntr++;
		} else if (cntr==(1000/TASK_PERIOD_MS)) { /* every second a heartbeat */
      cntr = 0;
		  LED1_On(); /* on for 50 ms */
		} else {
	    cntr++;
		}
    vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_MS));
	} /* for */
}

 /*
  * Initializes all components belonging to app
  */
void APP_Init(void){
}

/*
 * creates APP Task
 */
void APP_Run(void) {
	if (xTaskCreate(APP_task, "App", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}
	vTaskStartScheduler();
	for(;;) {
	  /* should not get here */
	}
}
