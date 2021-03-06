#Variables
set FILE_NAME=AutomatedBatterySwap
set SOURCES_DIR=Sources\
set BASE_DIR=Build\
set OUT_DIR=%BASE_DIR%%FILE_NAME%\
set TEMP_DIR=%BASE_DIR%Tmp
set ICON=ABS.ico

#Create executable
pyinstaller  %SOURCES_DIR%\Main.py --name %FILE_NAME% --distpath %OUT_DIR% --workpath %TEMP_DIR% --icon %ICON%

#Copy configfile into build directory
copy %SOURCES_DIR%\Config.xml %OUT_DIR%%FILE_NAME%

#Delect .spec file
del %FILE_NAME%.spec