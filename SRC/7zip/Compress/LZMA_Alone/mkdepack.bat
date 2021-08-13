@echo off
ee-as -o depacker.bin depacker.s
if errorlevel 1 goto error
ee-strip --output-target=binary depacker.bin
if errorlevel 1 goto error
bin2c -o depacker.h depacker.bin
:error
