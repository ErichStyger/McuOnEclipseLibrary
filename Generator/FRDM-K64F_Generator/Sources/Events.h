/* ###################################################################
**     Filename    : Events.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-11-25, 19:43, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "McuUtility.h"
#include "McuXFormat.h"
#include "McuRTOS.h"
#include "McuWait.h"
#include "McuCriticalSection.h"
#include "McuRingbuffer.h"
#include "McuHardFault.h"
#include "McuArmTools.h"
#include "McuRTT.h"
#include "McuShell.h"
#include "McuSystemView.h"
#include "McuTimeout.h"
#include "McuPercepio.h"
#include "McuUSB.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "McuLED.h"
#include "LEDpin1.h"
#include "McuQuadCounter.h"
#include "C11.h"
#include "BitIoLdd2.h"
#include "C21.h"
#include "BitIoLdd3.h"
#include "McuTrigger.h"
#include "McuEvents.h"
#include "McuTimeDate.h"
#include "RTC1.h"
#include "McuGenericSWSPI.h"
#include "Clock1.h"
#include "BitIoLdd4.h"
#include "Output1.h"
#include "BitIoLdd5.h"
#include "McuGDisplay.h"
#include "McuSharpMemoryDisplay.h"
#include "SCEpin1.h"
#include "BitIoLdd6.h"
#include "McuGFont.h"
#include "McuFontDisplay.h"
#include "McuPidInt.h"
#include "McuPidFloat.h"
#include "McuGenericI2C.h"
#include "McuGenericSWI2C.h"
#include "SDA1.h"
#include "BitIoLdd7.h"
#include "SCL1.h"
#include "BitIoLdd8.h"
#include "McuI2CSpy.h"
#include "McuFontHelv08Normal.h"
#include "McuFontHelv10Normal.h"
#include "McuFontHelv12Normal.h"
#include "McuFontHelv14Normal.h"
#include "McuFontHelv18Normal.h"
#include "McuFontHelv24Normal.h"
#include "McuFontCour08Normal.h"
#include "McuFontCour10Normal.h"
#include "McuFontCour12Normal.h"
#include "McuFontCour14Normal.h"
#include "McuFontCour18Normal.h"
#include "McuFontCour24Normal.h"
#include "McuFontHelv08Bold.h"
#include "McuFontHelv10Bold.h"
#include "McuFontHelv12Bold.h"
#include "McuFontHelv14Bold.h"
#include "McuFontHelv18Bold.h"
#include "McuFontHelv24Bold.h"
#include "McuFontCour08Bold.h"
#include "McuFontCour10Bold.h"
#include "McuFontCour12Bold.h"
#include "McuFontCour14Bold.h"
#include "McuFontCour18Bold.h"
#include "McuFontCour24Bold.h"
#include "McuFXOS8700.h"
#include "McuOneWire.h"
#include "DQ1.h"
#include "InputRB1.h"
#include "DbgRd1.h"
#include "McuDS18B20.h"
#include "McuLib.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MK64FN1M0LL12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void);


void McuRTOS_vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
/*
** ===================================================================
**     Event       :  McuRTOS_vApplicationStackOverflowHook (module Events)
**
**     Component   :  McuRTOS [McuRTOS]
**     Description :
**         if enabled, this hook will be called in case of a stack
**         overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pxTask          - Task handle
**       * pcTaskName      - Pointer to task name
**     Returns     : Nothing
** ===================================================================
*/

void McuRTOS_vApplicationMallocFailedHook(void);
/*
** ===================================================================
**     Event       :  McuRTOS_vApplicationMallocFailedHook (module Events)
**
**     Component   :  McuRTOS [McuRTOS]
**     Description :
**         If enabled, the McuRTOS will call this hook in case memory
**         allocation failed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuRTOS_vApplicationTickHook(void);
/*
** ===================================================================
**     Event       :  McuRTOS_vApplicationTickHook (module Events)
**
**     Component   :  McuRTOS [FreeRTOS]
**     Description :
**         If enabled, this hook will be called by the RTOS for every
**         tick increment.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuRTOS_vApplicationIdleHook(void);
/*
** ===================================================================
**     Event       :  McuRTOS_vApplicationIdleHook (module Events)
**
**     Component   :  McuRTOS [FreeRTOS]
**     Description :
**         If enabled, this hook will be called when the RTOS is idle.
**         This might be a good place to go into low power mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuPercepio_OnTraceWrap(void);
/*
** ===================================================================
**     Event       :  McuPercepio_OnTraceWrap (module Events)
**
**     Component   :  McuPercepio [PercepioTrace]
**     Description :
**         Called for trace ring buffer wrap around. This gives the
**         application a chance to dump the trace buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuEvents_AppHandleEvent(uint8_t event);
/*
** ===================================================================
**     Event       :  McuEvents_AppHandleEvent (module Events)
**
**     Component   :  McuEvents [SimpleEvents]
**     Description :
**         
**     Parameters  :
**         NAME            - DESCRIPTION
**         event           - Event (event number) to be processed.
**     Returns     : Nothing
** ===================================================================
*/

void McuDS18B20_OnError(McuOneWire_Error error);
/*
** ===================================================================
**     Event       :  McuDS18B20_OnError (module Events)
**
**     Component   :  McuDS18B20 [DS18B20]
**     Description :
**         Event called in error case
**     Parameters  :
**         NAME            - DESCRIPTION
**       * error           - 
**     Returns     : Nothing
** ===================================================================
*/
void McuDS18B20_OnTemperatureGet(uint8_t sensor_index, int32_t temperature);
/*
** ===================================================================
**     Event       :  McuDS18B20_OnTemperatureGet (module Events)
**
**     Component   :  McuDS18B20 [DS18B20]
**     Description :
**         Event called for getting temperature
**     Parameters  :
**         NAME            - DESCRIPTION
**       * sensor_index    - 
**       * temperature     - 
**     Returns     : Nothing
** ===================================================================
*/
void McuDS18B20_OnRomRead(uint8_t sensor_index, uint8_t *rom_code);
/*
** ===================================================================
**     Event       :  McuDS18B20_OnRomRead (module Events)
**
**     Component   :  McuDS18B20 [DS18B20]
**     Description :
**         Event called for reading ROM value
**     Parameters  :
**         NAME            - DESCRIPTION
**       * sensor_index    - 
**       * rom_code        - 
**     Returns     : Nothing
** ===================================================================
*/
/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
