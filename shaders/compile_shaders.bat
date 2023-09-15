@echo off
setlocal enabledelayedexpansion

echo Compiling shaders...

for %%f in (*.vert) do (
    echo Compiling vertex shader: %%f...
    glslc.exe %%f -o %%~nf_vert.spv
)

for %%f in (*.frag) do (
    echo Compiling fragment shader: %%f...
    glslc.exe %%f -o %%~nf_frag.spv
)

echo Compilation finished.
