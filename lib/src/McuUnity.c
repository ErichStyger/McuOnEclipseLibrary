/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "McuRTT.h"

void setUp(void) {
  /* run before each test*/
}

void tearDown(void) { 
  /* run after each test */
}

void McuUnity_putc(int c) {
  McuRTT_StdIOSendChar(c); /* using JRun with RTT */
}

void McuUnity_flush(void) {
  /* used to flush the communication channel */
}

void McuUnity_start(void) {
  /* used to open/start the communicatino channel */
}

void McuUnity_complete(void) {
  /* used to close/stop the communication channel */
}