/* ###################################################################
**     Filename    : main.c
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-11-25, 19:43, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
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
#include "Input1.h"
#include "Output1.h"
#include "McuGDisplaySSD1306.h"
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
#include "SCL1.h"
#include "McuSSD1306.h"
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
#include "McuFatFS.h"
#include "McuSDCard.h"
#include "SS1.h"
#include "McuSTM32HALI2C.h"
#include "McuLib.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
