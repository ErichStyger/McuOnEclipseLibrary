/*
 * Copyright (c) 2021, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MININI_MCUFLASH_H_
#define MININI_MCUFLASH_H_


#include "McuFlashconfig.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Decides if memory is accessible. On some architectures it needs to be prepared first.
 * \param addr Memory area to check
 * \param nofBytes Number of bytes to check
 * \return true if memory can be accessed, false otherwise
 */
bool McuFlash_IsAccessible(const void *addr, size_t nofBytes);

/*!
 * \brief Decides if memory is erased or not.
 * \param addr Memory area to check
 * \param nofBytes Number of bytes to check
 * \return true if memory is erased, false otherwise
 */
bool McuFlash_IsErased(const void *addr, size_t nofBytes);

/*!
 * \brief Erases a memory area
 * \param addr Memory area to erase
 * \param nofBytes Number of bytes to erase
 * \return Error code, ERR_OK if everything is fine
 */
uint8_t McuFlash_Erase(void *addr, size_t nofBytes);


/*!
 * \brief Program the flash memory with data
 * \param addr Address where to store the data
 * \param data Pointer to the data
 * \param dataSize Number of data bytes
 * \return Error code, ERR_OK if everything is fine
 */
uint8_t McuFlash_ProgramFlash(void *addr, const void *data, size_t dataSize);

#include "McuShell.h"
uint8_t McuFlash_ParseCommand(const unsigned char *cmd, bool *handled, const McuShell_StdIOType *io);

/*!
 * \brief Module de-initialization
 */
void McuFlash_Deinit(void);

/*!
 * \brief Module initialization
 */
void McuFlash_Init(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* MININI_MCUFLASH_H_ */
