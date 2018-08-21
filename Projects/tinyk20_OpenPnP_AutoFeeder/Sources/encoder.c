/*
 * encoder.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */
#include "platform.h"
#include "encoder.h"

#if PL_ENCODER_EN
#include "ENC1.h"
#include "CS1.h"
#include "FRTOS1.h"
#include "CLS1.h"
#include "motor.h"
#include "comm.h"

#define ENC_QUEUE_LENGTH    (10)
#define ENC_QUEUE_ITEM_SIZE (sizeof(int8_t))  /* -1 (backward) or 1 (forward) will be added for the steps */
static xQueueHandle ENC_StepRequestQueue = NULL;

void ENC_RequestSteps(int8_t steps) {
  if (xQueueSendToBack(ENC_StepRequestQueue, &steps, portMAX_DELAY)!=pdPASS) {
    for(;;){} /* ups? */
  }
}

/*
 * calls status check of encoder
 * turns off sprocket motor accordingly
 */
static void ENC_task(void *param) {
  uint32_t err_cntr = 0;
	unsigned int enc_val = 0;
	int8_t steps;
  int32_t timeoutMs = 5000;

	(void)param;
	if (ENC1_GetVal()) { /* HIGH: sprocket wheel is not on a hole: move the sprocket with the motor to the hole */
	  COMM_SendStr("INFO: Initial sprocket move\r\n", FALSE);
	  MOT_Speed(MOT_SPROC, 100); /* turn on motor with 50% */
	  timeoutMs = 5000;
	  while(ENC1_GetVal()) {
	    vTaskDelay(pdMS_TO_TICKS(1)); /* wait until we reached to the hole */
      timeoutMs--;
      if (timeoutMs<0) {
        COMM_SendStr("ERROR: InitialSprocketMove\r\n", TRUE);
        break;
      }
	  }
	  MOT_Speed(MOT_SPROC, 0); /* turn off motor */
	  /* here the ENC1 should be LOW (light passes through the hole) */
	}
	for(;;) {
	  if (ENC1_GetVal()) { /* here the signal should be LOW (light goes through). If not, the wheel has moved? */
	    err_cntr++;
	  }
	  /* check if we get a semaphore to request a move */
	  if (xQueueReceive(ENC_StepRequestQueue, &steps, portMAX_DELAY)!=pdPASS) {
	    COMM_SendStr("blocked for semaphore, but got a timeout?\r\n", TRUE);
	  } else { /* received the semaphore, move sprocket motor */
	    while(steps!=0) {
	      timeoutMs = 5000;
        MOT_Speed(MOT_SPROC, steps>=0?100:-100); /* turn on motor  */
        while(!ENC1_GetVal()) { /* while signal is low and we are still over the hole */
          vTaskDelay(pdMS_TO_TICKS(1)); /* wait until the signal goes up and we passed the hole */
          timeoutMs--;
          if (timeoutMs<0) {
            COMM_SendStr("ERROR: StepForwardTapeFWTimeout\r\n", TRUE);
            break;
          }
        }
        /* now the signal is high (we are between holes: wait until the signal goes low again (we have reached the next hole) */
        timeoutMs = 5000;
        while(ENC1_GetVal()) {
          timeoutMs--;
          if (timeoutMs<0) {
            COMM_SendStr("ERROR: StepForwardTapeFWTimeout\r\n", TRUE);
            break;
          }
          vTaskDelay(pdMS_TO_TICKS(1)); /* wait until we reached to the hole */
        }
        /* now we reached the next hole */
        MOT_Speed(MOT_SPROC, 0); /* turn off motor */
        if (steps<0) {
          steps++;
        } else if (steps>0){
          steps--;
        }
	    }
	  }
		vTaskDelay(pdMS_TO_TICKS(1));
	} /* for */
}

/*
 * Initializes counters and flags
 * creates ENC Task
 */
void ENC_Init(void){
	ENC_StepRequestQueue = xQueueCreate(ENC_QUEUE_LENGTH, ENC_QUEUE_ITEM_SIZE);
  if (ENC_StepRequestQueue==NULL) {
    for(;;){} /* out of memory? */
  }
  vQueueAddToRegistry(ENC_StepRequestQueue, "EncStepReqQ");

  if (xTaskCreate(ENC_task, "ENC", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}
}

#endif
