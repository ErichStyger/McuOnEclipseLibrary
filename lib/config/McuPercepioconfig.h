/**
 * \file
 * \brief Configuration header file for Percepio Trace for FreeRTOS
 *
 * This header file is used to configure settings of the Percepio trace.
 */

#ifndef __McuPercepio_CONFIG_H
#define __McuPercepio_CONFIG_H

#ifndef McuPercepio_CONFIG_START_TRACE_IN_STARTUP_MODE
  #define McuPercepio_CONFIG_START_TRACE_IN_STARTUP_MODE    TRC_INIT
    /*!< Percepio trace startup mode used for calling vTraceEnable() from Startup():
      Snapshot Mode:
        TRC_INIT: Initializes trace module. Application needs to start tracing later with vTraceEnable(TRC_START);
        TRC_START: Initializes and starts tracing
      Streaming Mode:
        TRC_INIT: Initializes trace module. Application needs to start tracing later with vTraceEnable(TRC_START);
        TRC_START_AWAIT_HOST: Initializes trace module and blocks (waits for data to be retrieved */
#endif

#endif /* __McuPercepio_CONFIG_H */
