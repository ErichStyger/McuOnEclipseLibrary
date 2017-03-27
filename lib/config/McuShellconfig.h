#ifndef __McuShell_CONFIG_H
#define __McuShell_CONFIG_H

#ifndef McuShell_CONFIG_BLOCKING_SEND_ENABLED
  #define McuShell_CONFIG_BLOCKING_SEND_ENABLED  (1)
    /*!< 1: Sending is blocking (with an optional timeout); 0: Do not block on sending */
#endif

#ifndef McuShell_CONFIG_BLOCKING_SEND_TIMEOUT_MS
  #define McuShell_CONFIG_BLOCKING_SEND_TIMEOUT_MS  (20)
    /*!< Total blocking time (timeout) in milliseconds, uses 0 for blocking without a timeout */
#endif

#ifndef McuShell_CONFIG_BLOCKING_SEND_TIMEOUT_WAIT_MS
  #define McuShell_CONFIG_BLOCKING_SEND_TIMEOUT_WAIT_MS  (5)
    /*!< waiting time during blocking, use 0 (zero) for polling */
#endif

#ifndef McuShell_CONFIG_BLOCKING_SEND_RTOS_WAIT
  #define McuShell_CONFIG_BLOCKING_SEND_RTOS_WAIT  (1)
    /*!< 1: Use WaitmsOS() instead of Waitms(); 0: Use Waitms() instead of WaitOSms() */
#endif

#ifndef McuShell_CONFIG_USE_MUTEX
  #define McuShell_CONFIG_USE_MUTEX  (0)
    /*!< 1: use RTOS mutex; 0: do not use RTOS mutex */
#endif

#endif /* __McuShell_CONFIG_H */
