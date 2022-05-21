@echo off
cd ..
@echo on
call products\premake\bin\premake5.exe vs2022
@echo off
pause
cd scripts
@echo on