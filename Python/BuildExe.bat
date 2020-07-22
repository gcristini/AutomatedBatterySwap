
set OUT_DIR=Build
set FILE_NAME=AutomatedBatterySwap
set ICON=ABS.ico

pyinstaller --onefile Main.py --name %FILE_NAME% --distpath %OUT_DIR% --icon %ICON%


copy DLT_Config.xml %OUT_DIR%
copy LT_Config.xml %OUT_DIR%
copy DBG_Config.xml %OUT_DIR%