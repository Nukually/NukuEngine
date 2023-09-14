@echo off
rmdir /s /q build
cmake -S . -B .\build
pause