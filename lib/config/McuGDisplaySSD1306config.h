/**
 * \file
 * \brief Configuration header file for GDisplay
 *
 * This header file is used to configure settings of the GDisplay module.
 */

#ifndef __McuGDisplaySSD1306_CONFIG_H
#define __McuGDisplaySSD1306_CONFIG_H

#ifndef McuGDisplaySSD1306_CONFIG_DISPLAY_MODULE_NAME
  #define McuGDisplaySSD1306_CONFIG_DISPLAY_MODULE_NAME McuSSD1306
    /*!< Symbol name used for module */
#endif

#ifndef McuGDisplaySSD1306_CONFIG_DISPLAY_HEADER_FILE
  #define McuGDisplaySSD1306_CONFIG_DISPLAY_HEADER_FILE "McuSSD1306.h"
    /*!< Header file to be included as interface to the display */
#endif
#include McuGDisplaySSD1306_CONFIG_DISPLAY_HEADER_FILE  /* include LCD interface */

#ifndef McuGDisplaySSD1306_CONFIG_USE_WINDOW_CAPABILITY
  #define McuGDisplaySSD1306_CONFIG_USE_WINDOW_CAPABILITY  (0)
    /*!< 1: Use display window capability; 0: No display window capability */
#endif

#if McuGDisplaySSD1306_CONFIG_USE_WINDOW_CAPABILITY
  #define McuGDisplaySSD1306_CONFIG_FCT_NAME_OPENWINDOW     McuSSD1306_OpenWindow
  #define McuGDisplaySSD1306_CONFIG_FCT_NAME_WRITEPIXEL     McuSSD1306_WritePixel
  #define McuGDisplaySSD1306_CONFIG_FCT_NAME_CLOSEWINDOW    McuSSD1306_CloseWindow
#else
  #define McuGDisplaySSD1306_CONFIG_FCT_NAME_SETPIXEL       McuSSD1306_SetPixel
  #define McuGDisplaySSD1306_CONFIG_FCT_NAME_CLRPIXEL       McuSSD1306_ClrPixel
#endif

#ifndef McuGDisplaySSD1306_CONFIG_USE_DISPLAY_MEMORY_WRITE
  #define McuGDisplaySSD1306_CONFIG_USE_DISPLAY_MEMORY_WRITE  (0)
    /*!< 1: Use display memory write; 0: Do not use display memory write */
#endif

#ifndef McuGDisplaySSD1306_CONFIG_INVERTED_PIXEL_COLOR
  #define McuGDisplaySSD1306_CONFIG_INVERTED_PIXEL_COLOR  (0)
    /*!< 1: Invert pixel color; 0: Do not invert pixel color */
#endif

#ifndef McuGDisplaySSD1306_CONFIG_NOF_BITS_PER_PIXEL
  #define McuGDisplaySSD1306_CONFIG_NOF_BITS_PER_PIXEL  (1)
    /*!< Number of bits per pixel */
#endif

#endif /* __McuGDisplaySSD1306_CONFIG_H */
