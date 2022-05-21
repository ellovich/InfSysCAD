@echo off
cd scripts
@echo on

@echo | call Build_OCCT_win64-debug.bat
@echo | call GenProj_Vs2022.bat

@echo off
cd ..
pause
@echo on