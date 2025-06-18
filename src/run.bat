@echo off
:loop
cls
echo Compiling...
g++ main.cpp -o main.exe
if errorlevel 1 (
    echo Compilation failed. Fix errors and press Enter to try again.
    pause >nul
    goto loop
)
echo Running...
main.exe
if %ERRORLEVEL%==100 goto loop
echo Program exited.
pause
