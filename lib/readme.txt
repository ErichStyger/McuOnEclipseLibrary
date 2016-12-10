readme.txt
----------
This folder contains all the library sources:
- config: application specific configuration files
- src: source files

Components:
- McuLibConfig: main configuration of the library, which SDK to be used
  Dependencies: none
  
- Utility: various string utilities
  Dependencies: McuLibConfig
  
- XFormat: tiny formatting and printing, similar to sprintf
  Dependencies: McuLibConfig
