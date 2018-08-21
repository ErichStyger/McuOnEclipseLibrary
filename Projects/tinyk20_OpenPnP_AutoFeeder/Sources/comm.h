/*
 * comm.h
 *
 *  Created on: 04.04.2018
 *      Author: Simon Huber
 */

#ifndef SOURCES_COMM_H_
#define SOURCES_COMM_H_

#include "platform.h"
#include "CLS1.h"

#define ADD_NEWLINE (1)

void COMM_SendStr(const char *str, bool isError);
/*
 * Send Error Messages
 */
void COMM_SendError(const unsigned char *cmd);

uint8_t COMM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
/*
 * Initialise Communication Buffer
 * Start COMM Task
 */
void COMM_Init(void);

#endif /* SOURCES_COMM_H_ */
