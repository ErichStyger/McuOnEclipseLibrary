REM Batch files to move the HD44780 files from the src folder into the HD44780 folder
SET REPO=..\..\lib

REM DEL "%REPO%\config\*.*""

ECHO *** Copy HDD44780 files ***
MOVE "%REPO%\src\McuHD44780.*"  "%REPO%\HD44780\"
MOVE "%REPO%\src\DB01.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB11.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB21.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB31.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB41.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB51.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB61.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\DB71.*"        "%REPO%\HD44780\"
MOVE "%REPO%\src\EN1.*"         "%REPO%\HD44780\"
MOVE "%REPO%\src\EN2.*"         "%REPO%\HD44780\"
MOVE "%REPO%\src\RS1.*"         "%REPO%\HD44780\"
MOVE "%REPO%\src\RW1.*"         "%REPO%\HD44780\"
