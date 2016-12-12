readme.txt
----------
This folder contains all the library sources:
- config: application specific configuration files
- src: source files

Middleware:
- FreeRTOS: FreeRTOS realtime operating system
- TraceRecorder: Percipio trace library for FreeRTOS

Components:
- McuBitIO: General purpose pin library
- McuCriticalSection: criticl section handling
- McuEvents: event handling using a big array
- McuHardfault: ARM Cortex-M hardfault handler
- McuKinetisTools: utilities for NXP Kinetis ARM Cortex-M devices
- McuLibConfig: main configuration of the library, which SDK to be used
    Dependencies: none
- Utility: various string utilities
    Dependencies: McuLibConfig
- XFormat: tiny formatting and printing, similar to sprintf
    Dependencies: McuLibConfig
  

