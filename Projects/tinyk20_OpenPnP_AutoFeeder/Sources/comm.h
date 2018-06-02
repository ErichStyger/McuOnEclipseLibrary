/*
 * comm.h
 *
 *  Created on: 04.04.2018
 *      Author: Simon Huber
 */

#ifndef SOURCES_COMM_H_
#define SOURCES_COMM_H_

#include "platform.h"

#define ADD_NEWLINE (1)

/*
 * Send Error Messages
 * \todo Improve to implement queueing of messages
 */
void COMM_SendError(const unsigned char *cmd);

/*
 * Initialise Communication Buffer
 * Start COMM Task
 */
void COMM_Init(void);

#endif /* SOURCES_COMM_H_ */
