
set FILE_NAME=AutomatedBatterySwap
set BASE_DIR=Build\
set OUT_DIR=%BASE_DIR%%FILE_NAME%
set TEMP_DIR=%BASE_DIR%Tmp

set ICON=ABS.ico

pyinstaller --onefile Main.py --name %FILE_NAME% --distpath %OUT_DIR% --workpath %TEMP_DIR% --icon %ICON%


copy DLT_Config.xml %OUT_DIR%
copy LT_Config.xml %OUT_DIR%
copy DBG_Config.xml %OUT_DIR%

del %FILE_NAME%.spec