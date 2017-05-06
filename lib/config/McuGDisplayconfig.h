/**
 * \file
 * \brief Configuration header file for GDisplay
 *
 * This header file is used to configure settings of the GDisplay module.
 */

#ifndef __McuGDisplay_CONFIG_H
#define __McuGDisplay_CONFIG_H

#ifndef McuGDisplay_CONFIG_USE_WINDOW_CAPABILITY
  #define McuGDisplay_CONFIG_USE_WINDOW_CAPABILITY  (0)
    /*!< 1: Use display window capability; 0: No display window capability */
#endif

#if McuGDisplay_CONFIG_USE_WINDOW_CAPABILITY
  #define McuGDisplay_CONFIG_FCT_NAME_OPENWINDOW     McuSharpMemoryDisplay_OpenWindow
  #define McuGDisplay_CONFIG_FCT_NAME_WRITEPIXEL     McuSharpMemoryDisplay_WritePixel
  #define McuGDisplay_CONFIG_FCT_NAME_CLOSEWINDOW    McuSharpMemoryDisplay_CloseWindow
#else
  #define McuGDisplay_CONFIG_FCT_NAME_SETPIXEL       McuSharpMemoryDisplay_SetPixel
  #define McuGDisplay_CONFIG_FCT_NAME_CLRPIXEL       McuSharpMemoryDisplay_ClrPixel
#endif

#ifndef McuGDisplay_CONFIG_USE_DISPLAY_MEMORY_WRITE
  #define McuGDisplay_CONFIG_USE_DISPLAY_MEMORY_WRITE  (0)
    /*!< 1: Use display memory write; 0: Do not use display memory write */
#endif

#ifndef McuGDisplay_CONFIG_INVERTED_PIXEL_COLOR
  #define McuGDisplay_CONFIG_INVERTED_PIXEL_COLOR  (0)
    /*!< 1: Invert pixel color; 0: Do not invert pixel color */
#endif

#ifndef McuGDisplay_CONFIG_NOF_BITS_PER_PIXEL
  #define McuGDisplay_CONFIG_NOF_BITS_PER_PIXEL  (1)
    /*!< Number of bits per pixel */
#endif

#endif /* __McuGDisplay_CONFIG_H */
