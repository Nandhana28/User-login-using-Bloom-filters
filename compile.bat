@echo off
echo Compiling User Authentication System...
cl /EHsc /std:c++11 BloomFilter.cpp UserAuth.cpp main.cpp /Fe:auth_system.exe
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Run auth_system.exe to start the program
) else (
    echo Compilation failed!
)
pause
