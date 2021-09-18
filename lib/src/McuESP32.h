/*
 * Copyright (c) 2021, Erich Styger
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef McuESP32_H_
#define McuESP32_H_

#include "platform.h"
#include <stdint.h>
#include "McuESP32config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* used to deal with USB CDC flow control pins */
void McuESP32_UartState_Callback(uint8_t state);

#include "McuShell.h"
/*!
 * \brief Shell parser routine.
 * \param cmd Pointer to command line string.
 * \param handled Pointer to status if command has been handled. Set to TRUE if command was understood.
 * \param io Pointer to stdio handle
 * \return Error code, ERR_OK if everything was ok.
 */
uint8_t McuESP32_ParseCommand(const unsigned char *cmd, bool *handled, const McuShell_StdIOType *io);

/*!
 * \brief Set the default shell console standard I/O
 * \param stdio Standard I/O to be used
 * \return error code, ERR_OK if everything is fine
 */
uint8_t McuESP32_SetShellConsoleStdio(McuShell_ConstStdIOTypePtr stdio);

/*!
 * \brief Return the current shell console I/O
 * \return I/O handler
 */
McuShell_ConstStdIOTypePtr McuESP32_GetShellConsoleStdio(void);

/*!
 * \brief Returns the standard I/O of the UART connection to the ESP32
 * \return standard I/O handler
 */
McuShell_ConstStdIOTypePtr McuESP32_GetUartESPStdio(void);

void McuESP32_Deinit(void);
void McuESP32_Init(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* McuESP32_H_ */
