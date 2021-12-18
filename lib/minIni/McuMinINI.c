/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuMinINI.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : minIni
**     Version     : Component 01.056, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2021-12-17, 13:47, # CodeGen: 760
**     Abstract    :
**         minIni is a programmer�s library to read and write ini files in embedded systems.
**     Settings    :
**          Component name                                 : McuMinINI
**          minIni Version                                 : 1.2B
**          SDK                                            : McuLib
**          Portable strnicmp()                            : yes
**          Use Real                                       : no
**          Read Only                                      : no
**          No Debug                                       : yes
**          Global Buffer                                  : no
**          FatFS                                          : Disabled
**          Source Folders                                 : 
**            Source Folder                                : minIni
**            Config Folder                                : minIni
**     Contents    :
**         ini_getbool    - int McuMinINI_ini_getbool(const mTCHAR *Section, const mTCHAR *Key, int...
**         ini_gets       - int McuMinINI_ini_gets(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR...
**         ini_puts       - int McuMinINI_ini_puts(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR...
**         ini_getl       - long McuMinINI_ini_getl(const mTCHAR *Section, const mTCHAR *Key, long...
**         ini_putl       - int McuMinINI_ini_putl(const mTCHAR *Section, const mTCHAR *Key, long Value,...
**         ini_getsection - int McuMinINI_ini_getsection(int idx, mTCHAR *Buffer, int BufferSize, const...
**         ini_getkey     - int McuMinINI_ini_getkey(const mTCHAR *Section, int idx, mTCHAR *Buffer, int...
**         ini_browse     - int McuMinINI_ini_browse(INI_CALLBACK Callback, const void *UserData, const...
**         ParseCommand   - uint8_t McuMinINI_ParseCommand(const unsigned char *cmd, bool *handled, const...
**         Deinit         - void McuMinINI_Deinit(void);
**         Init           - void McuMinINI_Init(void);
**
** (c) Copyright 2008-2012, CompuPhase;
** http      : www.compuphase.com
** Processor Expert port: Erich Styger, 2014-2021, http://www.mcuoneclipse.com
** License: See miniIni_LICENSE.txt and minIni_NOTICE.txt
** Adaptions for Processor Expert: (c) Copyright 2012-2020, Erich Styger
** ###################################################################*/
/*!
** @file McuMinINI.h
** @version 01.00
** @brief
**         minIni is a programmer�s library to read and write ini files in embedded systems.
*/         
/*!
**  @addtogroup McuMinINI_module McuMinINI module documentation
**  @{
*/         

/* MODULE McuMinINI. */

#include "McuMinINI.h"
#include "McuUtility.h" /* various utility functions */


#if McuMinINI_CONFIG_PARSE_COMMAND_ENABLED
static uint8_t PrintStatus(const McuShell_StdIOType *io) {
  uint8_t buf[16];

  McuShell_SendStatusStr((unsigned char*)"McuMinINI", (unsigned char*)"MinINI status\r\n", io->stdOut);
  McuUtility_Num32uToStr(buf, sizeof(buf), INI_BUFFERSIZE);
  McuUtility_strcat(buf, sizeof(buf), (unsigned char*)" bytes\r\n");
  McuShell_SendStatusStr((unsigned char*)"  buffer", buf, io->stdOut);

#if defined(INI_REAL)
  McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"supported\r\n");
#else
  McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"not supported\r\n");
#endif
  McuShell_SendStatusStr((unsigned char*)"  real", buf, io->stdOut);

#if defined(INI_READONLY)
  McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"read-only\r\n");
#else
  McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"read-write\r\n");
#endif
  McuShell_SendStatusStr((unsigned char*)"  mode", buf, io->stdOut);

  switch(McuMinINI_CONFIG_FS) {
    case McuMinINI_CONFIG_FS_TYPE_GENERIC:
      McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"Generic\r\n"); break;
    case McuMinINI_CONFIG_FS_TYPE_TINY_FS:
      McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"TinyFS\r\n"); break;
    case McuMinINI_CONFIG_FS_TYPE_FAT_FS:
      McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"FatFS\r\n"); break;
    case McuMinINI_CONFIG_FS_TYPE_FLASH_FS:
      McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"FLASH\r\n"); break;
    default:
      McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"ERROR\r\n"); break;
    }
  McuShell_SendStatusStr((unsigned char*)"  FS", buf, io->stdOut);
  return ERR_OK;
}
#endif
/*
** ===================================================================
**     Method      :  ini_getkey (component minIni)
**
**     Description :
**         Return the key inside a section
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Section         - The name of the section to browse
**                           through, or NULL to browse through the keys
**                           outside any section
**         idx             - The zero-based sequence number of the key
**                           to return
**       * Buffer          - A pointer to the buffer to copy into
**         BufferSize      - The maximum number of
**                           characters to copy
**       * Filename        - The name and full path of the .
**                           ini file to read from
**     Returns     :
**         ---             - The number of characters copied into the
**                           supplied buffer
** ===================================================================
*/
/**
int McuMinINI_ini_getkey(const mTCHAR *Section, int idx, mTCHAR *Buffer, int BufferSize, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_getbool (component minIni)
**
**     Description :
**         Return a boolean key.  A true boolean is found if one of the
**         following is matched:
**         - A string starting with 'y' or 'Y'
**         - A string starting with 't' or 'T'
**         - A string starting with '1'
**         A false boolean is found if one of the following is matched:
**         - A string starting with 'n' or 'N'
**         - A string starting with 'f' or 'F'
**         - A string starting with '0'
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Section         - The name of the section to search
**                           for
**       * Key             - The name of the entry to find the value of
**         DefValue        - Default value in the event of a
**                           failed read; it should be zero (0) or one
**                           (1).
**       * Filename        - The name and full path of the .
**                           ini file to read from
**     Returns     :
**         ---             - The true/false flag as interpreted at Key
** ===================================================================
*/
/**
int McuMinINI_ini_getbool(const mTCHAR *Section, const mTCHAR *Key, int DefValue, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_gets (component minIni)
**
**     Description :
**         Return the string for a given section and key.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Section         - The name of the section to search
**                           for
**       * Key             - Pointer to the name of the entry to find
**                           the value of
**       * DefValue        - Pointer to the default value in
**                           the event of a failed read
**       * Buffer          - A pointer to the buffer to copy into
**         BufferSize      - The maximum number of
**                           characters to copy
**       * Filename        - The name and full path of the .
**                           ini file to read from
**     Returns     :
**         ---             - The number of characters copied into the
**                           supplied buffer
** ===================================================================
*/
/**
int McuMinINI_ini_gets(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR *DefValue, nTCHAR *Buffer, int BufferSize, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_getl (component minIni)
**
**     Description :
**         Return the long integral value for a given section and key.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Section         - The name of the section to search
**                           for
**       * Key             - Pointer to the name of the entry to find
**                           the value of
**         DefValue        - Default value in the event of a
**                           failed read
**       * Filename        - The name and full path of the .
**                           ini file to read from
**     Returns     :
**         ---             - The value located at Key
** ===================================================================
*/
/**
long McuMinINI_ini_getl(const mTCHAR *Section, const mTCHAR *Key, long DefValue, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_getsection (component minIni)
**
**     Description :
**         Return the key inside a section
**     Parameters  :
**         NAME            - DESCRIPTION
**         idx             - The zero-based sequence number of the
**                           section to return
**       * Buffer          - A pointer to the buffer to copy into
**         BufferSize      - The maximum number of
**                           characters to copy
**       * Filename        - The name and full path of the .
**                           ini file to read from
**     Returns     :
**         ---             - The number of characters copied into the
**                           supplied buffer
** ===================================================================
*/
/**
int McuMinINI_ini_getsection(int idx, mTCHAR *Buffer, int BufferSize, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_putl (component minIni)
**
**     Description :
**         Writes a long integral value for a given section and key.
**         Method is only enabled if 'Read only' is set to  'no' in the
**         properties.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Section         - The name of the section to write
**                           the value in
**       * Key             - The name of the entry to write
**         Value           - The value to write
**       * Filename        - The name and full path of the .
**                           ini file to write to
**     Returns     :
**         ---             - 1 if successful, otherwise 0
** ===================================================================
*/
/**
int McuMinINI_ini_putl(const mTCHAR *Section, const mTCHAR *Key, long Value, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_puts (component minIni)
**
**     Description :
**         Writes a string value for a given section and key. Method is
**         only enabled if 'Read only' is set to  'no' in the
**         properties.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Section         - The name of the section to write
**                           the string in
**       * Key             - The name of the entry to write
**       * Value           - Pointer to the buffer the string, or
**                           NULL to erase the key
**       * Filename        - The name and full path of the .
**                           ini file to write to
**     Returns     :
**         ---             - 1 if successful, otherwise 0
** ===================================================================
*/
/**
int McuMinINI_ini_puts(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR *Value, const mTCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ini_browse (component minIni)
**
**     Description :
**         Function to browse the ini file. With this function the file
**         is opened only once.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Callback        - a pointer to a function that
**                           will be called for every setting in the INI
**                           file.
**         UserData        - arbitrary data, which the
**                           function passes on the the Callback function
**         Filename        - the name and full path of the .
**                           ini file to read from
**     Returns     :
**         ---             - 1 on success, 0 on failure (INI file not
**                           found)
** ===================================================================
*/
/**
int McuMinINI_ini_browse(INI_CALLBACK Callback, const void *UserData, const TCHAR *Filename)
{
  Implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  ParseCommand (component minIni)
**
**     Description :
**         Shell Command Line parser
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command line
**       * handled         - Pointer to variable which tells if
**                           the command has been handled or not
**       * io              - Pointer to I/O structure
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuMinINI_ParseCommand(const unsigned char *cmd, bool *handled, const McuShell_StdIOType *io)
{
  const unsigned char *p;
  unsigned char section[48], key[48], value[96], fileName[64];
  size_t lenRead;

  if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP)==0 || McuUtility_strcmp((char*)cmd, "McuMinINI help")==0) {
    McuShell_SendHelpStr((unsigned char*)"McuMinINI", (const unsigned char*)"Group of McuMinINI commands\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  read <f> <s> <k>", (const unsigned char*)"Read a key from a section in a file\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  write <f> <s> <k> <v>", (const unsigned char*)"Write a key with value to a section in a file\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  delkey <f> <s> <k>", (const unsigned char*)"Delete a key in a section of file\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  delsec <f> <s>", (const unsigned char*)"Delete a section in a file\r\n", io->stdOut);
    *handled = TRUE;
    return ERR_OK;
  } else if ((McuUtility_strcmp((char*)cmd, McuShell_CMD_STATUS)==0) || (McuUtility_strcmp((char*)cmd, "McuMinINI status")==0)) {
    *handled = TRUE;
    return PrintStatus(io);
  } else if (McuUtility_strncmp((char*)cmd, "McuMinINI read ", sizeof("McuMinINI read ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("McuMinINI read ")-1;
    if (McuUtility_ReadEscapedName(p, fileName, sizeof(fileName), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, section, sizeof(section), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, key, sizeof(key), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    if (*p != '\0') { /* still some arguments? */
      return ERR_FAILED;
    }
    if (ini_gets((const TCHAR *)section, (const TCHAR *)key, (const TCHAR *)"", (TCHAR *)value, sizeof(value), (const TCHAR *)fileName) == 0) {
      return ERR_FAILED;
    }
    McuShell_SendStr(key, io->stdOut);
    McuShell_SendStr((unsigned char*)"=", io->stdOut);
    McuShell_SendStr(value, io->stdOut);
    McuShell_SendStr((unsigned char*)"\r\n", io->stdOut);
    return ERR_OK;
  } else if (McuUtility_strncmp((char*)cmd, "McuMinINI write ", sizeof("McuMinINI write ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("McuMinINI write ")-1;
    if (McuUtility_ReadEscapedName(p, fileName, sizeof(fileName), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, section, sizeof(section), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, key, sizeof(key), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, value, sizeof(value), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    if (*p != '\0') { /* still some arguments? */
      return ERR_FAILED;
    }
    if (ini_puts((const TCHAR *)section, (const TCHAR *)key, (const TCHAR *)value, (const TCHAR *)fileName) == 0) {
      return ERR_FAILED;
    }
    return ERR_OK;
  } else if (McuUtility_strncmp((char*)cmd, "McuMinINI delkey ", sizeof("McuMinINI delkey ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("McuMinINI delkey ")-1;
    if (McuUtility_ReadEscapedName(p, fileName, sizeof(fileName), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, section, sizeof(section), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, key, sizeof(key), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    if (*p != '\0') { /* still some arguments? */
      return ERR_FAILED;
    }
    if (ini_puts((const TCHAR *)section, (const TCHAR *)key, NULL, (const TCHAR *)fileName) == 0) {
      return ERR_FAILED;
    }
    return ERR_OK;
  } else if (McuUtility_strncmp((char*)cmd, "McuMinINI delsec ", sizeof("McuMinINI delsec ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("McuMinINI delsec ")-1;
    if (McuUtility_ReadEscapedName(p, fileName, sizeof(fileName), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    McuUtility_SkipSpaces(&p);
    if (McuUtility_ReadEscapedName(p, section, sizeof(section), &lenRead, NULL, NULL)!=ERR_OK || lenRead==0) {
      return ERR_FAILED;
    }
    p += lenRead;
    if (*p != '\0') { /* still some arguments? */
      return ERR_FAILED;
    }
    if (ini_puts((const TCHAR *)section, NULL, NULL, (const TCHAR *)fileName) == 0) {
      return ERR_FAILED;
    }
    return ERR_OK;
  }
  return ERR_OK;
}
/*
** ===================================================================
**     Method      :  Deinit (component minIni)
**
**     Description :
**         Module de-initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuMinINI_Deinit(void)
{
  (void)ini_deinit(); /* call low level function */
}

/*
** ===================================================================
**     Method      :  Init (component minIni)
**
**     Description :
**         Module initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuMinINI_Init(void)
{
  (void)ini_init(); /* call low level function */
}

/* END McuMinINI. */

/*!
** @}
*/
