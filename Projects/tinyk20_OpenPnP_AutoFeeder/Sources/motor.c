/*
 * motor_sproc.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon Huber
 */

#include "motor.h"
/*	Motor Drivers
 *
 * 	Using DRV8835 H-Bridge IC for motor control.
 *	MODE = 1 (is set high in Init
 *	xEnable (0 = Brake, 1 = FWD/REV)
 *	xPhase 	(X = Brake, 0 = FWD, 1 = REV)
 *
 *	//Special care for when motor is connected backwards.
 *	Use Invert in header file
 */

#if PL_MOTOR_EN
#include "DIRA.h"
#include "DIRB.h"
#include "PWMA.h"
#include "PWMB.h"
#include "HMODE.h"
#include "KIN1.h"

static bool MOT_SPROC_INV = TRUE;
static bool MOT_TAPE_INV  = FALSE;

/*
 * sets direction of sprocket motor
 * checks for inverted mount
 */
static void MOT_Dir_Sproc(MOT_Direction dir){
  bool tmp = TRUE;

	if (dir == MOT_DIR_FWD){
		tmp = TRUE;
	} else if (dir == MOT_DIR_REV) {
		tmp = FALSE;
	}
	if (MOT_SPROC_INV) {
		tmp = !tmp;
	}
	DIRA_PutVal(tmp);
}

/*
 * sets direction of tape peeler motor
 * checks for inverted mount
 */
static void MOT_Dir_Tape(MOT_Direction dir){
  bool tmp = TRUE;

	if (dir == MOT_DIR_FWD) {
		tmp = TRUE;
	} else if (dir == MOT_DIR_REV) {
		tmp = FALSE;
	}
	if(MOT_TAPE_INV){
		tmp = !tmp;
	}
	DIRB_PutVal(tmp);
}

/*
 * sets motor speed and direction
 */
uint8_t MOT_Speed(MOT_Device motor, int8_t speedPercent) {
	uint32_t pwmVal;
	uint8_t percentU;

  if (speedPercent > MAX_SPEED) {
    speedPercent = MAX_SPEED;
  } else if (speedPercent < -MAX_SPEED) {
    speedPercent = -MAX_SPEED;
  }
	if (speedPercent<0) {
	  percentU = -speedPercent;
	} else {
	  percentU = speedPercent;
	}
	pwmVal = ((percentU*0xFFFF)/100);
	if(motor == MOT_SPROC) {
		MOT_Dir_Sproc(speedPercent<0?MOT_DIR_REV:MOT_DIR_FWD);
		PWMA_SetRatio16(pwmVal);	// sets H-Bridge speed
		return 1;
	} else if (motor == MOT_TAPE) {
		MOT_Dir_Tape(speedPercent<0?MOT_DIR_REV:MOT_DIR_FWD);
		PWMB_SetRatio16(pwmVal);	// sets H-Bridge speed
		return 1;
	} else { /* error case */
		return 0;
	}
}

static const KIN1_UID FeederMcuIDs[] = {
  /* 0  */ {{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x08,0x00,0x1C,0x00,0x0D,0x24,0x40,0x4E,0x45}},
  /* 1  */ {{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x27,0x00,0x44,0x00,0x0D,0x24,0x40,0x4E,0x45}},
  /* 2  */ {{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x18,0x00,0x0A,0x00,0x0D,0x24,0x40,0x4E,0x45}},
  /* 3  */ {{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x4D,0x00,0x3B,0x00,0x0D,0x24,0x40,0x4E,0x45}},
};

/*
 * initializes motor to 0 speed (Stop)
 * sets H-Bridge Mode
 */
void MOT_Init(void) {
  KIN1_UID id;
  uint8_t res;

  res = KIN1_UIDGet(&id);
  if (res!=ERR_OK) {
    for(;;); /* error */
  }
  if (KIN1_UIDSame(&id, &FeederMcuIDs[0])) { /* Feeder 0 */
    MOT_SPROC_INV = FALSE;
    MOT_TAPE_INV  = TRUE;
  } else if (KIN1_UIDSame(&id, &FeederMcuIDs[1])) { /* Feeder 1 */
    MOT_SPROC_INV = TRUE;
    MOT_TAPE_INV  = FALSE;
  } else if (KIN1_UIDSame(&id, &FeederMcuIDs[2])) { /* Feeder 2 */
    MOT_SPROC_INV = FALSE;
    MOT_TAPE_INV  = FALSE;
  } else if (KIN1_UIDSame(&id, &FeederMcuIDs[3])) { /* Feeder 3 */
    MOT_SPROC_INV = FALSE;
    MOT_TAPE_INV  = TRUE;
  } else { /* default */
    MOT_SPROC_INV = TRUE;
    MOT_TAPE_INV  = FALSE;
  }
	HMODE_SetVal();
	MOT_Speed(MOT_SPROC, 0);
	MOT_Speed(MOT_TAPE, 0);
}

#endif
