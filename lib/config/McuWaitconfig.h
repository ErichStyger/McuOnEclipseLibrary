#ifndef __McuWait_CONFIG_H
#define __McuWait_CONFIG_H

#include "McuLib.h" /* include library configuration */

#if !defined(McuWait_CONFIG_USE_CYCLE_COUNTER)
  #define McuWait_CONFIG_USE_CYCLE_COUNTER  (0 && (McuLib_CONFIG_CORTEX_M>=3))
    /*!< 1: Use hardware cycle counter (if present, only on Cortex-M3 or higher), 0: not using hardware cycle counter */
#endif

#if McuWait_CONFIG_USE_CYCLE_COUNTER
  #include "KIN1.h" /* include Cortex utility functions */
#endif

#endif /* __McuWait_CONFIG_H */
