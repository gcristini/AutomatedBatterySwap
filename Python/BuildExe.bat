#Variables
set FILE_NAME=AutomatedBatterySwap
set BASE_DIR=Build\
set OUT_DIR=%BASE_DIR%%FILE_NAME%
set TEMP_DIR=%BASE_DIR%Tmp
set ICON=ABS.ico

#Create executable
pyinstaller --onefile Main.py --name %FILE_NAME% --distpath %OUT_DIR% --workpath %TEMP_DIR% --icon %ICON%

#Copy configfile into build directory
copy Config.xml %OUT_DIR%

#Delect .spec file
del %FILE_NAME%.spec