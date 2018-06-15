/*
 * comm.c
 *
 *  Created on: 04.04.2018
 *      Author: Simon Huber
 */

#include "comm.h"

/*
 * CMD AND ERROR DECLARATION
 *
 * Each Message String begins with either CMD or ERR
 * This defines if the address is to be evaluated as command or
 * incremented and passed on for error evaluation in OpenPnP
 *
 * The address follows the CMD/ERR header. Then followed by the command/message
 * which is evaluated in a second step.
 *
 * CMD 0 FWD xx | CMD 0 REV xx
 * ERR 0 Unknown command | ERR 0 Incorrect Step Format | ERR 0 Software Error |
 */

#if PL_COMM_EN
#include "CLS1.h"
#include "UTIL1.h"
#include "CS1.h"
#include "FRTOS1.h"
#include "Application.h"
#include "RTT1.h"
#include "KIN1.h"

#if PL_ENCODER_EN
  #include "encoder.h"
#endif

#define MAX_BUFF (64)
static unsigned char buffer[MAX_BUFF];

/*
 * Send Error-Style message to Master
 */
void COMM_SendError(const unsigned char *cmd){
	unsigned char buf[MAX_BUFF];

	UTIL1_strcpy(buf, sizeof(buf), "CMD -1 ERR ");
	UTIL1_strcat(buf, sizeof(buf), cmd);
	CLS1_SendStr(buf, CLS1_GetStdio()->stdOut);
}

/*
 * Send Status-Style message to Master
 */
static void COMM_SendStatusOk(const unsigned char *cmd) {
	unsigned char buf[MAX_BUFF];

	UTIL1_strcpy(buf, sizeof(buf), "CMD -1 ");
	UTIL1_strcat(buf, sizeof(buf), cmd);
  UTIL1_strcat(buf, sizeof(buf), " ok");
	CLS1_SendStr(buf, CLS1_GetStdio()->stdOut);
}

static uint8_t PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr((unsigned char*)"CMD", (unsigned char*)"Group of CMD commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  0 FWD <n> mm", (unsigned char*)"Move feeder <n> mm forward (multiple of 2)\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  0 REV <n> mm", (unsigned char*)"Move feeder <n> mm backward (multiple of 2)\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  0 CNT",        (unsigned char*)"Count number of feeders\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t PrintStatus(const CLS1_StdIOType *io) {
  CLS1_SendStatusStr((unsigned char*)"COMM", (unsigned char*)"\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  tbd", (uint8_t*)"nothing", io->stdOut);
  CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
  return ERR_OK;
}


/*
 * Parse through received messages
 * Messages with address 0 processed
 * otherwise decremented and sent on
 *
 * changed from CLS1_ParseCommand()
 *
 * \todo improve to send back errors for handling
 */
uint8_t COMM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  int32_t value;	/* signed since address decrementing falls below 0 */
  int32_t mm;
  uint8_t res;
  const unsigned char *p;
  uint8_t replyBuf[32];

  if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP) == 0
    || UTIL1_strcmp((char*)cmd, "CMD help") == 0) {
    *handled = TRUE;
    return PrintHelp(io);
  } else if (   (UTIL1_strcmp((char*)cmd, CLS1_CMD_STATUS)==0)
             || (UTIL1_strcmp((char*)cmd, "CMD status") == 0)
            )
  {
    *handled = TRUE;
    return PrintStatus(io);
  } else if (UTIL1_strncmp((char*)cmd, "CMD ", sizeof("CMD ")-1)==0) {  /* Check if it is command, e.g. "CMD 0 FWD 4 mm" */
    p = cmd+sizeof("CMD ")-1;
		res = UTIL1_xatoi(&p, &value); /* read address */
		if (res==ERR_OK && value==0) { /* if address is 0 execute command */
			if (UTIL1_strncmp((char*)p, " FWD ", sizeof(" FWD ")-1)==0) {
				p += sizeof(" FWD ")-1;
				res = UTIL1_xatoi(&p, &mm);
				if (res==ERR_OK && (mm >= ENC_STEP_SIZE_MM) && (mm%ENC_STEP_SIZE_MM == 0)) {	/* number of steps (in mm), must be 2, 4, 6, etc */
				  if (UTIL1_strcmp(p, " mm")!=0) {
	          COMM_SendError("missing 'mm' for command!");
	          return ERR_FAILED;
				  }
				  /* here we have received a correct command */
				  ENC_RequestSteps(mm/ENC_STEP_SIZE_MM);
				  UTIL1_strcpy(replyBuf, sizeof(replyBuf), "FWD ");
				  UTIL1_strcatNum32s(replyBuf, sizeof(replyBuf), mm);
          UTIL1_strcat(replyBuf, sizeof(replyBuf), " mm");
					COMM_SendStatusOk(replyBuf);
	        *handled = TRUE;
					return ERR_OK;
				} else { /* wrong step size */
					COMM_SendError("invalid step size");
					return ERR_VALUE;
				}
			} else if (UTIL1_strncmp((char*)p, " REV ", sizeof(" REV ")-1)==0) {
				p += sizeof(" REV ")-1;
        res = UTIL1_xatoi(&p, &mm);
        if (res==ERR_OK && (mm >= ENC_STEP_SIZE_MM) && (mm%ENC_STEP_SIZE_MM == 0)) {  /* number of steps (in mm), must be 2, 4, 6, etc */
          if (UTIL1_strcmp(p, " mm")!=0) {
            COMM_SendError("missing 'mm' for command!");
            return ERR_FAILED;
          }
          /* here we have received a correct command */
          ENC_RequestSteps(mm/ENC_STEP_SIZE_MM);
          UTIL1_strcpy(replyBuf, sizeof(replyBuf), "REV ");
          UTIL1_strcatNum32s(replyBuf, sizeof(replyBuf), mm);
          UTIL1_strcat(replyBuf, sizeof(replyBuf), " mm");
          COMM_SendStatusOk(replyBuf);
          *handled = TRUE;
          return ERR_OK;
        } else { /* wrong step size */
          COMM_SendError("invalid step size");
          return ERR_VALUE;
        }
      } else if (UTIL1_strcmp((char*)p, " CNT")==0) {
        COMM_SendStatusOk("CNT");
        *handled = TRUE;
			} else {
				COMM_SendError("unknown command");
				return ERR_VALUE;
			}
			return ERR_OK;
		  // else decrement address and send command on
		} else {
      unsigned char newCMD[32];

			UTIL1_strcpy(newCMD, sizeof(newCMD), "CMD ");
			UTIL1_strcatNum32s(newCMD, sizeof(newCMD), value-1); /* address minus 1 */
			UTIL1_strcat(newCMD, sizeof(newCMD), p); /* append rest of command string */
			CLS1_SendStr((unsigned char*)newCMD, io->stdOut);
			*handled = TRUE;
			return ERR_OK;
		}
	} else { /* generate unknown command error */
		COMM_SendError("unknown message");
		*handled = TRUE;
		return ERR_FAILED;
	}
}

/*
 * Iterate through received characters
 * changed from CLS1_ReadAndParse()
 */
static uint8_t COMM_ReadAndParse(uint8_t *cmdBuf, size_t cmdBufSize, CLS1_ConstStdIOType *io) {
  /* IMPORTANT NOTE: this function *appends* to the buffer, so the buffer needs to be initialized first! */
  uint8_t res = ERR_OK;
  size_t len;
  bool handled;


  if (io==NULL) { /* no I/O handler? */
    return ERR_FAILED;
  }
  len = UTIL1_strlen((const char*)cmdBuf);
  if (CLS1_ReadLine(cmdBuf, cmdBuf+len, cmdBufSize-len, io)) {
    len = UTIL1_strlen((const char*)cmdBuf); /* length of buffer string */
    if (len==0) { /* error case */
      return ERR_FAILED;
    } else if (len==1 && (cmdBuf[0]=='\n' || cmdBuf[0]=='\r')) { /* eat preceding newline characters */
      cmdBuf[0] = '\0';
    }
    if (len>=cmdBufSize-1) {           /* buffer overflow? Parse what we have, will be likely return an error */
      //(void)CLS1_ParseWithCommandTable(cmdBuf, io, parseCallback);
      (void)COMM_ParseCommand(cmdBuf, &handled, io);
#if ADD_NEWLINE
      CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
#endif
      cmdBuf[0] = '\0'; /* start again */
      res = ERR_OVERFLOW;
    } else if (cmdBuf[len-1]=='\n' || cmdBuf[len-1]=='\r') { /* line end: parse command */
      cmdBuf[len-1] = '\0';            /* remove line end character for parser */
      res = COMM_ParseCommand(cmdBuf, &handled, io);
#if ADD_NEWLINE
      CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
#endif
      cmdBuf[0] = '\0';                /* start again */
    } else {
      /* continue to append to buffer */
    }
  }
  return res;
}

/* ******************************************************************
 * UART Standard I/O over UART2 which is connected to the SWD connector pins 7 and 8
 * ******************************************************************/
 #include "AS2.h"

 static bool UART_KeyPressed(void) {
   return AS2_GetCharsInRxBuf()!=0;
 }

 static void UART_SendChar(uint8_t ch) {
   CLS1_SendCharFct(ch, AS2_SendChar);
 }

 static void UART_ReceiveChar(uint8_t *p) {
   if (AS2_RecvChar(p)!=ERR_OK) {
     *p = '\0';
   }
 }

 static CLS1_ConstStdIOType UART_stdio = {
   .stdIn = UART_ReceiveChar,
   .stdOut = UART_SendChar,
   .stdErr = UART_SendChar,
   .keyPressed = UART_KeyPressed,
 };

static uint8_t UART_DefaultShellBuffer[CLS1_DEFAULT_SHELL_BUFFER_SIZE]; /* default buffer which can be used by the application */

typedef struct {
  CLS1_ConstStdIOType *stdio;
  unsigned char *buf;
  size_t bufSize;
} SHELL_IODesc;

static const SHELL_IODesc ios[] =
{
  {&RTT1_stdio, RTT1_DefaultShellBuffer, sizeof(RTT1_DefaultShellBuffer)},
  {&UART_stdio, UART_DefaultShellBuffer, sizeof(UART_DefaultShellBuffer)},
};

static const CLS1_ParseCommandCallback CmdParserTable[] =
{
  CLS1_ParseCommand, /* Processor Expert Shell component, is first in list */
#if KIN1_PARSE_COMMAND_ENABLED
  KIN1_ParseCommand,
#endif
  COMM_ParseCommand,
  NULL /* sentinel */
};

void COMM_SendStr(const char *str, bool isError) {
  int i;

  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    CLS1_SendStr(str, isError? ios[i].stdio->stdErr : ios[i].stdio->stdOut);
  }
}

/*
 * Communication-task
 *
 */
static void COMM_task(void *param) {
  int i;
  (void)param;
  /* initialize buffers */
  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    ios[i].buf[0] = '\0';
  }
  COMM_SendStr("Starting COMM task\r\n", FALSE);
  for(;;) {
    /* process all I/Os */
    for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
      (void)CLS1_ReadAndParseWithCommandTable(ios[i].buf, ios[i].bufSize, ios[i].stdio, CmdParserTable);
    }
    //error_res = COMM_ReadAndParse(buffer, sizeof(buffer), CLS1_GetStdio());
    (void)COMM_ReadAndParse(buffer, sizeof(buffer), CLS1_GetStdio());
    vTaskDelay(pdMS_TO_TICKS(10));
  } /* for */
}

/*
 * Initialize Buffer and start task
 */
void COMM_Init(void){
	buffer[0] = '\0';
	if (xTaskCreate(COMM_task, "Comm", 700/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}
}

#endif
