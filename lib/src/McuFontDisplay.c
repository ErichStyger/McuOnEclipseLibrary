/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuFontDisplay.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : FontDisplay
**     Version     : Component 01.200, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-08-10, 19:29, # CodeGen: 671
**     Abstract    :
**          Driver to write fonts using GFont component
**     Settings    :
**          Bean name                                      : McuFontDisplay
**          System                                         : 
**            InhGDisplay                                  : McuGDisplaySSD1306
**     Contents    :
**         GetFontHeight    - void McuFontDisplay_GetFontHeight(McuFontDisplay_Font *font,...
**         GetStringHeight  - McuFontDisplay_PixelDim McuFontDisplay_GetStringHeight(uint8_t *str,...
**         GetCharWidth     - void McuFontDisplay_GetCharWidth(uint8_t ch, McuFontDisplay_PixelDim...
**         GetStringWidth   - McuFontDisplay_PixelDim McuFontDisplay_GetStringWidth(uint8_t *str,...
**         WriteString      - void McuFontDisplay_WriteString(uint8_t *str, McuFontDisplay_PixelColor...
**         WriteStringWidth - void McuFontDisplay_WriteStringWidth(uint8_t *str, McuFontDisplay_PixelColor...
**         WriteChar        - void McuFontDisplay_WriteChar(uint8_t ch, McuFontDisplay_PixelColor color,...
**         Deinit           - void McuFontDisplay_Deinit(void);
**         Init             - void McuFontDisplay_Init(void);
**
** * Copyright (c) 2011-2020, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuFontDisplay.h
** @version 01.00
** @brief
**          Driver to write fonts using GFont component
*/         
/*!
**  @addtogroup McuFontDisplay_module McuFontDisplay module documentation
**  @{
*/         

/* MODULE McuFontDisplay. */

#include "McuFontDisplay.h"
#include <stddef.h> /* for NULL */

/*
** ===================================================================
**     Method      :  WriteChar (component FontDisplay)
**
**     Description :
**         Function to write a character at the start point. Set the
**         start point with the function setCursor.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - the character to print
**         color           - Foreground color to be used (for the
**                           character pixels)
**       * xCursor         - Pointer to x position of character
**                           (upper left corner). On return this will
**                           contain the next x position.
**       * yCursor         - Pointer to y position of character
**                           (upper left corner). On return this will
**                           contain the next y position.
**       * font            - Pointer to font information
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_WriteChar(uint8_t ch, McuFontDisplay_PixelColor color, McuFontDisplay_PixelDim *xCursor, McuFontDisplay_PixelDim *yCursor, McuFontDisplay_Font *font)
{
  PGFONT_CharInfo charStruct;          /* font information */
  uint8_t *data;                       /* actual character of string text[] */
  uint8_t w;                           /* counter variable row bits of character */
  uint8_t h;                           /* counter variable column bits of character */
  McuFontDisplay_PixelDim currY;
  McuFontDisplay_PixelDim currX;
  signed char b;                       /* bit position in byte stream */

  if (ch=='\t') {                      /* tabulator */
   ch = ' ';                           /* use a space instead */
  }
  charStruct = font->GetFontChar((uint8_t)ch);
  if (ch=='\n') {                      /* move to a new line */
   *yCursor += font->boundingBoxHeight; /* set next cursor position */
   return;
  }
  if (ch=='\r') {                      /* move to beginning of line */
   return;                             /* do nothing. Only the caller may know what the beginning of line is */
  }
  data = (uint8_t*)charStruct->CharBMP; /* get the pointer */
  if (data != NULL) {                  /* printable character. Only if we have a character info. This is not the case e.g. for \n */
    currY =  (McuFontDisplay_PixelDim)(*yCursor
           + font->boundingBoxHeight   /* height of box. This includes the space between lines plus the space under the base line */
           - font->lineSpaceBoxHeight  /* space between two lines */
           - font->underlineBoxHeight  /* space below the base line */
           - charStruct->offsetY
           - charStruct->height);
    currX = (McuFontDisplay_PixelDim)(*xCursor + charStruct->offsetX);
    h = 0;
    for(;;) {                          /* breaks, process line by line */
      w = 0;                           /* width position */
      b = 7;                           /* bit position, MSB first */
      for(;;) {                        /* breaks, prints one pixel line */
        if ((((*data)&(1<<b))>>b)==1) { /* note that we do not change the background pixels */
          McuGDisplaySSD1306_PutPixel((McuFontDisplay_PixelDim)(currX+w), currY, color);
        }
        w++;                           /* width counter */
        b--;                           /* next pixel */
        if((b==-1)||(w==charStruct->width)) { /* last bit of byte reached or last bit of the width reached */
          if(w==charStruct->width) {   /* last bit of the row from the current character */
            break;                     /* out of while loop */
          } else {
            b = 7;
            data++;                    /* next byte */
          }
        }
      } /* for */
      data++;                          /* next byte */
      h++;                             /* height counter increment */
      currY++;                         /* one row up */
      if(h==(charStruct->height)){     /* finished character */
        break;
      }                                /* next row of character */
    } /* for */
    *xCursor += charStruct->dwidth;    /* set next cursor position */
  } /* if printable character */
}

/*
** ===================================================================
**     Method      :  WriteString (component FontDisplay)
**
**     Description :
**         Function to write a string or character at the startpoint.
**         Set the startpoint with the function setCursor.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * str             - Pointer to the character string
**         color           - Foreground color to be used (for the
**                           character pixels)
**       * xCursor         - Pointer to x position of first
**                           character (upper left corner). On return
**                           this will contain the next y position.
**       * yCursor         - Pointer to y position of character
**                           (upper left corner). On return this will
**                           contain the next y position.
**       * font            - Pointer to font information
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_WriteString(uint8_t *str, McuFontDisplay_PixelColor color, McuFontDisplay_PixelDim *xCursor, McuFontDisplay_PixelDim *yCursor, McuFontDisplay_Font *font)
{
  McuFontDisplay_PixelDim x = *xCursor;

  while(*str!='\0') {
    if (*str=='\r') {
      *xCursor = x;
    } else if (*str=='\n') {
      *xCursor = x;
      *yCursor += font->boundingBoxHeight;
    } else {
      McuFontDisplay_WriteChar(*str, color, xCursor, yCursor, font);
    }
    str++;
  }
}

/*
** ===================================================================
**     Method      :  WriteStringWidth (component FontDisplay)
**
**     Description :
**         Function to write a string or character at the startpoint.
**         Set the startpoint with the function setCursor. If the
**         string reaches the given width, the text gets wrapped to a
**         new line.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * str             - Pointer to the character string
**         color           - Foreground color to be used (for the
**                           character pixels)
**       * xCursor         - Pointer to x position of first
**                           character (upper left corner). On return
**                           this will contain the next y position.
**       * yCursor         - Pointer to y position of character
**                           (upper left corner). On return this will
**                           contain the next y position.
**       * font            - Pointer to font information
**         width           - Maximum of witdh of the text box. If
**                           text reaches the end of the box, the text
**                           gets wrapped to a new line.
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_WriteStringWidth(uint8_t *str, McuFontDisplay_PixelColor color, McuFontDisplay_PixelDim *xCursor, McuFontDisplay_PixelDim *yCursor, McuFontDisplay_Font *font, McuFontDisplay_PixelDim width)
{
  McuFontDisplay_PixelDim x = *xCursor;

  while(*str!='\0') {
    if (*str=='\r') {
      *xCursor = x;
    } else if (*str=='\n') {
      *xCursor = x;
      *yCursor += font->boundingBoxHeight;
    } else {
      if ((McuFontDisplay_PixelDim)((*xCursor - x) +  ((PGFONT_CharInfo)(font->GetFontChar((uint8_t)*str)))->dwidth) >= width)
      {  /* check if we would reach the end of the box: wrap to a new line */
        *xCursor = x;
        *yCursor += font->boundingBoxHeight;
      }
      McuFontDisplay_WriteChar(*str, color, xCursor, yCursor, font);
    }
    str++;
  }
}

/*
** ===================================================================
**     Method      :  GetCharWidth (component FontDisplay)
**
**     Description :
**         returns the width (in pixels) of a font character.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - character for which to calculate the width
**       * charWidth       - Pointer to width of the
**                           character in pixels (this is only for the
**                           character, without the space after the
**                           character)
**       * totalWidth      - Pointer to the total width
**                           of the character, including to the space
**                           after the character.
**       * font            - Pointer to font information
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_GetCharWidth(uint8_t ch, McuFontDisplay_PixelDim *charWidth, McuFontDisplay_PixelDim *totalWidth, McuFontDisplay_Font *font)
{
  PGFONT_CharInfo charStruct;

  charStruct = font->GetFontChar((uint8_t)ch);
  if (charStruct != NULL) {
    *charWidth = (McuFontDisplay_PixelDim)(charStruct->width+charStruct->offsetX);
    *totalWidth = (McuFontDisplay_PixelDim)charStruct->dwidth;
  } else {
    *charWidth = 0;
    *totalWidth = 0;
  }
}

/*
** ===================================================================
**     Method      :  GetStringWidth (component FontDisplay)
**
**     Description :
**         returns the width (in pixels) of a font string. For multiple
**         line strings (separated by \
**         ) or for strings with \r it returns the longest string
**         length.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * str             - Pointer to the string.
**       * font            - Pointer to font information
**       * lastCharSpace   - Pointer to the width
**                           of the last character space, useful if you
**                           combine multiple strings. The pointer may
**                           be NULL.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
McuFontDisplay_PixelDim McuFontDisplay_GetStringWidth(uint8_t *str, McuFontDisplay_Font *font, McuFontDisplay_PixelDim *lastCharSpace)
{
  unsigned char *p;                    /* pointer to the actual character */
  McuFontDisplay_PixelDim currWidth;   /* current width of string */
  McuFontDisplay_PixelDim maxWidth;    /* maximum width of string */
  McuFontDisplay_PixelDim maxWidthSpace; /* last space of maximum string */
  McuFontDisplay_PixelDim charSpace;   /* last character space */
  McuFontDisplay_PixelDim charWidth, totalWidth; /* character and total width of character */

  currWidth = maxWidth = 0;
  charWidth = totalWidth = 0;
  charSpace = maxWidthSpace = 0;
  p = str;
  for (;;) {                           /* breaks */
    if (*p=='\n' || *p=='\r' || *p=='\0') { /* multi-line string */
      currWidth -= charSpace;          /* subtract last space */
      if (currWidth > maxWidth) {      /* remember maximum line */
        maxWidth = currWidth;
        maxWidthSpace = charSpace;
      }
      currWidth = charSpace = 0;
      if (*p == '\0') {
        break;
      }
    } else {
      McuFontDisplay_GetCharWidth(*p, &charWidth, &totalWidth, font);
      currWidth += totalWidth;
      charSpace = (McuFontDisplay_PixelDim)(totalWidth-charWidth); /* calculate space of last/current character */
    }
    p++;                               /* next character */
  } /* for */
  if (lastCharSpace != NULL) {
    *lastCharSpace = maxWidthSpace;
  }
  return maxWidth;                     /* return width */
}

/*
** ===================================================================
**     Method      :  GetFontHeight (component FontDisplay)
**
**     Description :
**         returns the height (in pixels) of a font.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * font            - Pointer to font information
**       * charHeight      - Pointer to height of the
**                           character in pixels.
**       * totalHeight     - Pointer to the total
**                           height of the character, including to the
**                           space below the character.
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_GetFontHeight(McuFontDisplay_Font *font, McuFontDisplay_PixelDim *charHeight, McuFontDisplay_PixelDim *totalHeight)
{
  *charHeight = (McuFontDisplay_PixelDim)(font->boundingBoxHeight-font->lineSpaceBoxHeight);
  *totalHeight = font->boundingBoxHeight;
}

/*
** ===================================================================
**     Method      :  GetStringHeight (component FontDisplay)
**
**     Description :
**         returns the height (in pixels) of a font string. For
**         multiple line strings (separated by newline) or for strings
**         with \r it returns the longest list.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * str             - Pointer to the string.
**       * font            - Pointer to font information
**       * lastLineSpace   - Pointer to the height
**                           of the last line space, useful if you
**                           combine multiple strings. The pointer may
**                           be NULL.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
McuFontDisplay_PixelDim McuFontDisplay_GetStringHeight(uint8_t *str, McuFontDisplay_Font *font, McuFontDisplay_PixelDim *lastLineSpace)
{
  uint16_t nofLines;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  if (*str == '\0') {
    if (lastLineSpace != NULL) {
      *lastLineSpace = 0;
    }
    return 0;
  }
  nofLines = 1;
  while (*str != '\0') {               /* breaks */
    if (*str=='\n') {                  /* multi-line string */
      nofLines++;
    }
    str++;                             /* next character */
  }
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);
  if (lastLineSpace != NULL) {
    *lastLineSpace = (McuFontDisplay_PixelDim)(totalHeight-charHeight);
  }
  return (McuFontDisplay_PixelDim)((nofLines-1)*totalHeight + charHeight); /* return height */
}

/*
** ===================================================================
**     Method      :  Deinit (component FontDisplay)
**
**     Description :
**         Driver de-initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_Deinit(void)
{
  /* nothing needed */
}

/*
** ===================================================================
**     Method      :  Init (component FontDisplay)
**
**     Description :
**         Driver initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuFontDisplay_Init(void)
{
  /* nothing needed */
}

/* END McuFontDisplay. */

/*!
** @}
*/
