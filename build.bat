@echo off
cd shaders
call compile_shaders.bat
cd ..
rmdir /s /q build
cmake -S . -B .\build
pause