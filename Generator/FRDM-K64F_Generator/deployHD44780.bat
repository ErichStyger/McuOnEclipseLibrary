REM Batch files to deploy generated code to GitHub repository folder
REM Repo is in C:\Users\Erich Styger\Data\GitRepos\McuOnEclipseLibrary\lib
SET REPO=..\..\lib

REM DEL "%REPO%\config\*.*"

ECHO *** Copy HDD44780 files ***
REM DEL "%REPO%\HD4470\*.*"
COPY .\Generated_Code\McuHD44780.*  "%REPO%\HD44780\"
COPY .\Generated_Code\DB01.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB11.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB21.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB31.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB41.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB51.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB61.*        "%REPO%\HD44780\"
COPY .\Generated_Code\DB71.*        "%REPO%\HD44780\"
COPY .\Generated_Code\EN1.*         "%REPO%\HD44780\"
COPY .\Generated_Code\EN2.*         "%REPO%\HD44780\"
COPY .\Generated_Code\RS1.*         "%REPO%\HD44780\"
COPY .\Generated_Code\RW1.*         "%REPO%\HD44780\"
