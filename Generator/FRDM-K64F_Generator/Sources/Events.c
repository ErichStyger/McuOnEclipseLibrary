/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

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
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

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
void McuRTOS_vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
  /* This will get called if a stack overflow is detected during the context
     switch.  Set configCHECK_FOR_STACK_OVERFLOWS to 2 to also check for stack
     problems within nested interrupts, but only do this for debug purposes as
     it will increase the context switch time. */
  (void)pxTask;
  (void)pcTaskName;
  taskDISABLE_INTERRUPTS();
  /* Write your code here ... */
  for(;;) {}
}

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
void McuRTOS_vApplicationMallocFailedHook(void)
{
  /* Called if a call to pvPortMalloc() fails because there is insufficient
     free memory available in the McuRTOS heap.  pvPortMalloc() is called
     internally by McuRTOS API functions that create tasks, queues, software
     timers, and semaphores.  The size of the McuRTOS heap is set by the
     configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
  taskDISABLE_INTERRUPTS();
  /* Write your code here ... */
  for(;;) {}
}

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
void McuRTOS_vApplicationTickHook(void)
{
  /* Called for every RTOS tick. */
  /* Write your code here ... */
}

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
void McuRTOS_vApplicationIdleHook(void)
{
  /* Called whenever the RTOS is idle (from the IDLE task).
     Here would be a good place to put the CPU into low power mode. */
  /* Write your code here ... */
}

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
void McuPercepio_OnTraceWrap(void)
{
#if 0 /* default implementation for gdb below ... */
  /* Write your code here ... */
  uint8_t buf[64];

  /* GDB: dump binary memory <file> <hexStartAddr> <hexEndAddr> */
  McuPercepio_vGetGDBDumpCommand(buf, sizeof(buf), "c:\\tmp\\trc.dump");
#endif
}

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
void McuEvents_AppHandleEvent(uint8_t event)
{
  (void)event; /* only to avoid compiler warning about unused variable */
  /* Write your code here ... */
}

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
void McuDS18B20_OnError(McuOneWire_Error error)
{
  /* Write your code here ... */
}

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
void McuDS18B20_OnTemperatureGet(uint8_t sensor_index, int32_t temperature)
{
  /* Write your code here ... */
}

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
void McuDS18B20_OnRomRead(uint8_t sensor_index, uint8_t *rom_code)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
