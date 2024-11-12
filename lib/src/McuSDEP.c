/*
 * Copyright (c) 2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 *   Input   ->  dataRx_Buffer[]  ->  sdepTask()
 *                                      /  \
 *   Output  <- McuSDEP_SendMessage()<-/    \-> forward_Buffer[]  (e.g. to shell)
 */

#include "McuSDEP.h"
#if McuSDEP_CONFIG_IS_ENABLED
#include "McuLib.h"
#include "McuRTOS.h"
#include "McuLog.h"
#include "McuArmTools.h"
#include "McuRB.h"
#include "McuUtility.h"
#include "usb_host_cdc.h"
#if McuLib_CONFIG_CPU_IS_RPxxxx
  #include "pico/util/queue.h"
#endif

/* ------------------------------------------------------------ */
/* functions to read and write the SDEP communication channel */
/* Send a byte over the SDEP channel */

#if McuLib_CONFIG_CPU_IS_RPxxxx
  static queue_t dataRx_buffer; /* using multicore/interrupt aware queue, as data is comming from core 1 */
#else
  static McuRB_Handle_t dataRx_buffer;
#endif

/* callbacks to deal with the SDEP communication channel */
static struct McuSDEP_callbacks_s {
  void (*send_char)(unsigned char ch);      /* callback to send a character/byte to the SDEP channel */
  void (*flush)(void);                      /* callback to flush the SDEP output buffer */
  void (*forward_char)(unsigned char ch);   /* forwarding characters if not SDEP message */
} McuSDEP_callbacks;

void McuSDEP_SetFlushCallback( void (*flush_cb)(void)) {
  McuSDEP_callbacks.flush = flush_cb;
}

void McuSDEP_SetSendCharCallback( void (*send_char_cb)(unsigned char)) {
  McuSDEP_callbacks.send_char = send_char_cb;
}

void McuSDEP_SetForwardCharCallback( void (*forward_char_cb)(unsigned char)) {
  McuSDEP_callbacks.forward_char = forward_char_cb;
}

void McuSDEP_QueueSDEP_Rx(unsigned char ch) {
#if McuLib_CONFIG_CPU_IS_RPxxxx
  queue_add_blocking(&dataRx_buffer, &ch);
#else
  McuRB_Put(dataRx_buffer, &ch);
#endif
}

static bool McuSDEP_RxDataAvailable(void) {
#if McuLib_CONFIG_CPU_IS_RPxxxx
  return !queue_is_empty(&dataRx_buffer);
#else
  return McuRB_NofElements(dataRx_buffer)!=0;
#endif
}

static int McuSDEP_ReadByte(void) {
  unsigned char c;

#if McuLib_CONFIG_CPU_IS_RPxxxx
  if (queue_is_empty(&dataRx_buffer)) {
    return EOF;
  }
  queue_remove_blocking(&dataRx_buffer, &c);
  return c;
#else
  if (McuRB_NofElements(dataRx_buffer)==0) {
    return EOF;
  }
  if (McuRB_Get(dataRx_buffer, &ch)==ERR_OK) {
    return ch;
  } else {
    return EOF;
  }
#endif
}

static uint8_t McuSDEP_SendByte(uint8_t c) {
  McuSDEP_callbacks.send_char(c);
}

static void McuSDEP_Flush(void) {
  if (McuSDEP_callbacks.flush!=NULL) {
    McuSDEP_callbacks.flush();
  }
}

void McuSDEP_SendBlock(unsigned char *data, size_t count) {
  for(int i=0; i<count; i++) {
    McuSDEP_SendByte(data[i]);
  }
}

#if McuSDEP_CONFIG_USE_FREERTOS
  static QueueHandle_t shellToSDEPQueue; /* data from shell to be sent with ESDEP */
#else
  static McuRB_Handle_t shellToSDEPBuf;
#endif

void McuSDEP_ShellToSDEP_PutChar(unsigned char ch) {
#if McuSDEP_CONFIG_USE_FREERTOS
  if (xQueueSend(shellToSDEPQueue, &ch, portMAX_DELAY)!= pdPASS) {
    McuLog_fatal("failed sending");
    for(;;){}
  }
#else
  McuRB_Put(shellToSDEPBuf, &ch);
#endif
}

unsigned char McuSDEP_ShellToSDEP_GetChar(void) {
  unsigned char ch;
#if McuSDEP_CONFIG_USE_FREERTOS
  if (xQueueReceive(shellToSDEPQueue, &ch, portMAX_DELAY)!= pdPASS) {
    McuLog_fatal("failed sending");
    for(;;){}
  }
#else
  McuRB_Get(shellToSDEPBuf, &ch);
#endif
  return ch;
}

size_t McuSDEP_ShellToSDEP_NofElements(void) {
#if McuSDEP_CONFIG_USE_FREERTOS
  return uxQueueMessagesWaiting(shellToSDEPQueue);
#else
  return McuRB_NofElements(shellToSDEPBuf);
#endif
}

static uint8_t crc8_bytecalc(unsigned char byte, uint8_t* seed) {
  #define CRC8_POLYNOM (0x07)
  uint8_t i;
  uint8_t flag;
  uint8_t polynom = CRC8_POLYNOM;

  for (i = 0; i < 8; i++) {
    if (*seed & 0x80) {
      flag = 1;
    } else {
      flag = 0;
    }
    *seed <<= 1;
    if (byte & 0x80) {
      *seed |= 1;
    }
    byte <<= 1;
    if (flag) {
      *seed ^= polynom;
    }
  }
  return *seed;
}

static uint8_t crc8_messagecalc(unsigned char *msg, uint8_t len, uint8_t* seed) {
  for (int i = 0; i < len; i++) {
    crc8_bytecalc(msg[i], seed);
  }
  uint8_t crc = crc8_bytecalc(0, seed);
  return crc;
}

static uint8_t McuSDEP_Crc8(McuSDEPmessage_t *message) {
  uint8_t crc, seed = 0;

  crc = crc8_bytecalc(message->type, &seed);
  crc = crc8_bytecalc((uint8_t) message->cmdId, &seed);
  crc = crc8_bytecalc((uint8_t) (message->cmdId >> 8), &seed);
  crc = crc8_bytecalc(message->payloadSize, &seed);
  crc = crc8_messagecalc((unsigned char*) message->payload, message->payloadSize & ~McuSDEP_PAYLOADBYTE_MORE_DATA_BIT, &seed);
  return crc;
}

static void DecodeMessage(McuSDEPmessage_t *msg, unsigned char *buf, size_t bufSize) {
  McuUtility_strcpy(buf, bufSize, "type:");
  switch(msg->type) {
    case McuSDEP_MSG_TYPE_COMMAND:
      McuUtility_strcat(buf, bufSize, "CMD");
      break;
    case McuSDEP_MSG_TYPE_RESPONSE:
      McuUtility_strcat(buf, bufSize, "RSP");
      break;
    case McuSDEP_MSG_TYPE_ALERT:
      McuUtility_strcat(buf, bufSize, "ALE");
      break;
    case McuSDEP_MSG_TYPE_ERROR:
      McuUtility_strcat(buf, bufSize, "ERR");
      break;
    default: break;
  }
  McuUtility_strcat(buf, bufSize, " cmd:");
  McuUtility_strcatNum16Hex(buf, bufSize, msg->cmdId);
  McuUtility_strcat(buf, bufSize, " size:");
  McuUtility_strcatNum8Hex(buf, bufSize, msg->payloadSize);
  if (msg->payloadSize>0) {
    McuUtility_strcat(buf, bufSize, " data:");
    for(int i=0; i<msg->payloadSize; i++) {
      McuUtility_strcatNum8Hex(buf, bufSize, msg->payload[i]);
      if (i<msg->payloadSize-1) { /* not for the last byte */
        McuUtility_chcat(buf, bufSize, ' ');
      }
    }
  }
  McuUtility_strcat(buf, bufSize, " crc:");
  McuUtility_strcatNum8Hex(buf, bufSize, msg->crc);
}

uint8_t McuSDEP_SendMessage(McuSDEPmessage_t *msg) {
  msg->crc = McuSDEP_Crc8(msg);
  #if McuSDEP_CONFIG_USE_FRAMING
  McuSDEP_SendByte(McuSDEP_CONFIG_MSG_FRAMING_START);
  #endif
  McuSDEP_SendByte(msg->type);
  McuSDEP_SendByte((uint8_t) msg->cmdId); /* send little endian first */
  McuSDEP_SendByte((uint8_t) (msg->cmdId >> 8));
  McuSDEP_SendByte(msg->payloadSize);
  McuSDEP_SendBlock(msg->payload, msg->payloadSize & ~McuSDEP_PAYLOADBYTE_MORE_DATA_BIT);
  McuSDEP_SendByte(msg->crc);
  #if McuSDEP_CONFIG_USE_FRAMING
  McuSDEP_SendByte(McuSDEP_CONFIG_MSG_FRAMING_END);
  #endif
  McuSDEP_Flush();
  unsigned char buf[96];
  DecodeMessage(msg, buf, sizeof(buf));
  McuLog_info("Tx: %s", buf);
  return ERR_OK;
}

uint8_t McuSDEP_SendResponseForCmd(uint16_t cmdId) {
  McuSDEPmessage_t msg;

  msg.type = McuSDEP_MSG_TYPE_COMMAND;
  msg.cmdId = cmdId;
  msg.payload = NULL;
  msg.payloadSize = 0;
  msg.crc = McuSDEP_Crc8(&msg);
  McuSDEP_ID_HandleIncommingMessage(&msg);
  return ERR_OK;
}

uint8_t McuSDEP_SendCommand(uint16_t cmdId) {
  McuSDEPmessage_t msg;

  msg.type = McuSDEP_MSG_TYPE_COMMAND;
  msg.cmdId = cmdId;
  msg.payload = NULL;
  msg.payloadSize = 0;
  msg.crc = McuSDEP_Crc8(&msg);
  McuSDEP_SendMessage(&msg);
  return ERR_OK;
}

uint8_t McuSDEP_SendError(uint16_t cmdErrorId) {
  McuSDEPmessage_t msg;

  msg.type = McuSDEP_MSG_TYPE_ERROR;
  msg.cmdId = cmdErrorId;
  msg.payload = NULL;
  msg.payloadSize = 0;
  msg.crc = McuSDEP_Crc8(&msg);
  McuSDEP_SendMessage(&msg);
  return ERR_OK;
}

static inline bool isValidSDEPType(unsigned char ch) {
  return    ch == McuSDEP_MSG_TYPE_COMMAND 
         || ch == McuSDEP_MSG_TYPE_RESPONSE 
         || ch == McuSDEP_MSG_TYPE_ERROR
         || ch == McuSDEP_MSG_TYPE_ALERT;
}

static uint8_t readByte(unsigned char *buf, size_t bufSize, size_t *currBufIdx, int (*read)(void)) {
  int ch;

  if (*currBufIdx>=bufSize) {
    return ERR_OVERFLOW;
  }
  ch = read();
  if (ch==EOF) { /* no data available */
    return ERR_NOTAVAIL;
  }
  buf[(*currBufIdx)++] = ch;
  return ERR_OK;
}

uint8_t McuSDEP_ParseSDEPMessage(unsigned char *buf, size_t bufSize, size_t *currBufIdx, McuSDEPmessage_t *msg, int (*read)(void)) {
  uint8_t res;

  /* 0: message type: 8 bits */
  if (*currBufIdx==0) { /* at the start */
    res = readByte(buf, bufSize, currBufIdx, read);
    if (res!=ERR_OK) {
      return res;
    }
    if (!isValidSDEPType(buf[(*currBufIdx)-1])) {
      return ERR_FAILED; /* not valid message type */
    }
    msg->type = buf[(*currBufIdx)-1];
    /* initialize other fields */
    msg->cmdId = 0;
    msg->payloadSize = 0;
    msg->payload = 0;
    msg->crc = 0;
    memset(buf+1, 0, bufSize-1); /* initialize buffer, except the first byte */
  }
  /* 1: message command ID: low byte*/
  if (*currBufIdx==1) { /* first byte of cmd */
    res = readByte(buf, bufSize, currBufIdx, read);
    if (res!=ERR_OK) {
      return res;
    }
    msg->cmdId = buf[(*currBufIdx)-1]; /* low byte */
  }
  /* 2: message command ID: high byte*/
  if (*currBufIdx==2) { /* second byte of cmd */
    res = readByte(buf, bufSize, currBufIdx, read);
    if (res!=ERR_OK) {
      return res;
    }
    msg->cmdId |= buf[(*currBufIdx)-1]<<8; /* high byte */
    if (!McuSDEP_ID_isValidSDEPCommand(msg->cmdId)) {
      return ERR_FAILED; /* not valid message type */
    }
  }
  /* 3: message payload size: 8 bits */
  if (*currBufIdx==3) {
    res = readByte(buf, bufSize, currBufIdx, read);
    if (res!=ERR_OK) {
      return res;
    }
    msg->payloadSize = buf[(*currBufIdx)-1];
  }
  /* 4: payload, if any, followed by CRC */
  if (*currBufIdx>=4 && msg->payloadSize!=0 && *currBufIdx<3+msg->payloadSize+1) { /* with payload: read data first */
    do { /* read payload */
      res = readByte(buf, bufSize, currBufIdx, read);
      if (res!=ERR_OK) {
        return res;
      }
    } while(*currBufIdx<3+msg->payloadSize+1);
    /* finished reading payload: point to beginning of data: */
    msg->payload = &buf[*currBufIdx - msg->payloadSize];
  }
  /* last item: CRC */
  res = readByte(buf, bufSize, currBufIdx, read);
  if (res!=ERR_OK) {
    return res;
  }
  msg->crc = buf[(*currBufIdx)-1];
  if (McuSDEP_Crc8(msg) != msg->crc) {
    return ERR_CRC; /* crc does not match */
  }
  return ERR_OK; /* valid message */
}

static uint8_t PrintHelp(McuShell_ConstStdIOType *io) {
  McuShell_SendHelpStr((unsigned char*)"McuSDEP", (const unsigned char*)"Group of McuSDEP commands\r\n", io->stdOut);
  McuShell_SendHelpStr((unsigned char*)"  help|status", (const unsigned char*)"Print help or status information\r\n", io->stdOut);
  McuShell_SendHelpStr((unsigned char*)"  send cmd <id>", (const unsigned char*)"Send command\r\n", io->stdOut);
  McuShell_SendHelpStr((unsigned char*)"  resp cmd <id>", (const unsigned char*)"Send response for command\r\n", io->stdOut);
  McuShell_SendHelpStr((unsigned char*)"  text <txt>", (const unsigned char*)"Send text\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t PrintStatus(McuShell_ConstStdIOType *io) {
  McuShell_SendStatusStr((const unsigned char*)"McuSDEP", (const unsigned char*)"McuSDEP module status\r\n", io->stdOut);
  return ERR_OK;
}

uint8_t McuSDEP_ParseCommand(const uint8_t *cmd, bool *handled, McuShell_ConstStdIOType *io) {
  const unsigned char *p;
  uint32_t val;
  
  if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP)==0 || McuUtility_strcmp((char*)cmd, "McuSDEP help")==0) {
    *handled = TRUE;
    return PrintHelp(io);
  } else if ((McuUtility_strcmp((char*)cmd, McuShell_CMD_STATUS)==0) || (McuUtility_strcmp((char*)cmd, "McuSDEP status")==0)) {
    *handled = TRUE;
    return PrintStatus(io);
  } else if (McuUtility_strncmp((char*)cmd, "McuSDEP send cmd ", sizeof("McuSDEP send cmd ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("McuSDEP send cmd ")-1;
    if (McuUtility_xatoi(&p, &val)!=ERR_OK) {
      return ERR_FAILED;
    }
    return McuSDEP_SendCommand(val);
  } else if (McuUtility_strncmp((char*)cmd, "McuSDEP resp cmd ", sizeof("McuSDEP resp cmd ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("McuSDEP resp cmd ")-1;
    if (McuUtility_xatoi(&p, &val)!=ERR_OK) {
      return ERR_FAILED;
    }
    return McuSDEP_SendResponseForCmd(val);
  } else if (McuUtility_strncmp((char*)cmd, "McuSDEP text ", sizeof("McuSDEP text ")-1)==0) {
    *handled = TRUE;
    const unsigned char *p = cmd + sizeof("McuSDEP text ")-1;
    while(*p!='\0') {
      McuSDEP_SendByte(*p);
      p++;
    }
    McuSDEP_SendByte('\n');
    McuSDEP_Flush();
    return ERR_OK;
  }
  return ERR_OK;
}

static void sdepTask(void *pv) {
  uint8_t ch, res;
  uint8_t buf[McuSDEP_MESSAGE_MAX_NOF_BYTES + 1];
  uint8_t debugBuf[64];
  size_t bufIdx = 0;
  McuSDEPmessage_t msg;
  bool checkingSDEP = false;
  int timeoutMs;

  for(;;) {
    if (McuSDEP_RxDataAvailable()) {
      timeoutMs = 0;
      res = McuSDEP_ParseSDEPMessage(buf, sizeof(buf), &bufIdx, &msg, McuSDEP_ReadByte);
      if (res==ERR_OK) { /* parsed a valid SDEP message */
        bufIdx = 0; /* start for new iteration */
        DecodeMessage(&msg, debugBuf, sizeof(debugBuf));
        McuLog_info("Rx: %s", debugBuf);
        McuSDEP_ID_HandleIncommingMessage(&msg);
      } else if (res==ERR_NOTAVAIL) { /* need to read more data */
        vTaskDelay(pdMS_TO_TICKS(5));
      } else if (res==ERR_FAILED) { /* not SDEP, forward to shell */
        if (McuSDEP_callbacks.forward_char!=NULL) {
          for(int i=0; i<bufIdx; i++) {
            McuSDEP_callbacks.forward_char(buf[i]); /* forward character */
          }
        }
        bufIdx = 0; /* start for new iteration */
      } else if (res==ERR_OVERFLOW) { /* buffer overflow */
        McuSDEP_SendError(McuSDEP_CMD_TYPE_ERROR_OVERFLOW);
        bufIdx = 0; /* start for new iteration */
      } else if (res==ERR_CRC) { /* CRC error*/
        McuSDEP_SendError(McuSDEP_CMD_TYPE_ERROR_INVALIDCRC);
        bufIdx = 0; /* start for new iteration */
      }
    } else {
      vTaskDelay(pdMS_TO_TICKS(20));
      timeoutMs += 20;
      if (bufIdx>0 && timeoutMs>500) { /* we have data in the buffer and expect it an SDEP, but takes too long */
        /* abort and forward it */
        if (McuSDEP_callbacks.forward_char!=NULL) {
          for(int i=0; i<bufIdx; i++) {
            McuSDEP_callbacks.forward_char(buf[i]); /* forward character */
          }
        }
        bufIdx = 0; /* start for new iteration */
      }
    } /* if */
  }
}

void McuSDEP_Deinit(void) {
  /* \todo */
}

void McuSDEP_Init(void) {
#if McuSDEP_CONFIG_USE_FREERTOS
  BaseType_t res = xTaskCreate(sdepTask, "sdepTask", 1*1024/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, NULL);
  if (res!=pdPASS) {
    McuLog_fatal("creating sdepTask task failed!");
    for(;;) {}
  }
#endif
#if McuLib_CONFIG_CPU_IS_RPxxxx
  queue_init(&dataRx_buffer, sizeof(uint8_t), McuSDEP_CONFIG_RX_BUFFER_SIZE);
#else
  McuRB_Config_t config;

  McuRB_GetDefaultconfig(&config);
  config.elementSize = sizeof(uint8_t);
  config.nofElements = McuSDEP_CONFIG_RX_BUFFER_SIZE;
  dataRx_buffer = McuRB_InitRB(&config);
  if (dataRx_buffer==NULL) {
    McuLog_fatal("creating dataRX_buffer failed!");
    for(;;) {/* error */}
  }
#endif
#if McuSDEP_CONFIG_USE_FREERTOS
  shellToSDEPQueue = xQueueCreate(McuSDEP_CONFIG_SHELL_TO_SDEP_QUEUE_LENGTH, sizeof(uint8_t));
  if (shellToSDEPQueue==NULL) {
    for(;;){} /* out of memory? */
  }
  vQueueAddToRegistry(shellToSDEPQueue, "shellToSDEP");
#else
  McuRB_Config_t config;

  McuRB_GetDefaultconfig(&config);
  config.elementSize = sizeof(uint8_t);
  config.nofElements = McuSDEP_CONFIG_SHELL_TO_SDEP_QUEUE_LENGTH;
  shellToSDEPBuf = McuRB_InitRB(&config);
  if (shellToSDEPBuf==NULL) {
    for(;;) {/* error */}
  }
#endif
}

#endif /* McuSDEP_CONFIG_IS_ENABLED */