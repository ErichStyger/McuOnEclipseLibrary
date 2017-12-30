/**
 * \file
 * \brief Configuration header file for GenericI2C
 *
 * This header file is used to configure settings of the Generic I2C module.
 */

#ifndef __McuGenericI2C_CONFIG_H
#define __McuGenericI2C_CONFIG_H

#if !defined(McuGenericI2C_CONFIG_USE_ON_REQUEST_BUS_EVENT)
  #define McuGenericI2C_CONFIG_USE_ON_REQUEST_BUS_EVENT    (1)
    /*!< 1: generate user events for requesting bus; 0: no user events */
  #define McuGenericI2C_CONFIG_ON_REQUEST_BUS_EVENT   McuGenericI2C_OnRequestBus
  void McuGenericI2C_CONFIG_ON_REQUEST_BUS_EVENT(void); /* prototype */
#endif

#if !defined(McuGenericI2C_CONFIG_USE_ON_RELEASE_BUS_EVENT)
  #define McuGenericI2C_CONFIG_USE_ON_RELEASE_BUS_EVENT    (1)
    /*!< 1: generate user events for releasing bus; 0: no user events */
  #define McuGenericI2C_CONFIG_ON_RELEASE_BUS_EVENT   McuGenericI2C_OnReleaseBus
  void McuGenericI2C_CONFIG_ON_RELEASE_BUS_EVENT(void); /* prototype */
#endif

#if !defined(McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT)
  #define McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT    (1)
    /*!< 1: generate user events for errors; 0: no error events */
  #define McuGenericI2C_CONFIG_ON_ERROR_EVENT   McuGenericI2C_OnError
  void McuGenericI2C_CONFIG_ON_ERROR_EVENT(void); /* prototype */
#endif

#if !defined(McuGenericI2C_CONFIG_USE_MUTEX)
  #define McuGenericI2C_CONFIG_USE_MUTEX             (1)
    /*!< 1: Use a mutex to protect access to the bus; 0: no mutex used */
#endif


#endif /* __McuGenericI2C_CONFIG_H */
