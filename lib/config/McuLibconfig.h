#ifndef __McuLibCONFIG_H
#define __McuLibCONFIG_H

/* Identifiers used to identify the SDK */
#define McuLib_CONFIG_SDK_GENERIC             0 /* using a generic API/SDK */
#define McuLib_CONFIG_SDK_PROCESSOR_EXPERT    1 /* using Processor Expert SDK */
#define McuLib_CONFIG_SDK_KINETIS_1_3         2 /* using NXP Kinetis SDK V1.3 */
#define McuLib_CONFIG_SDK_KINETIS_2_0         3 /* using NXP Kinetis SDK V2.0 */
#define McuLib_CONFIG_SDK_MCUXPRESSO_2_0      4 /* same as Kinetis SDK v2.0 */

/* specify the SDK and API used */
#ifndef McuLib_CONFIG_SDK_VERSION_USED
  #define McuLib_CONFIG_SDK_VERSION_USED  McuLib_CONFIG_SDK_PROCESSOR_EXPERT
#endif

#endif /* __McuLibCONFIG_H */

