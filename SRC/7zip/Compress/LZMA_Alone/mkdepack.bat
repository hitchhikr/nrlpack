@echo off
ee-as -o depacker.bin depacker.s
ee-strip --output-target=binary depacker.bin
bin2c -o depacker.h depacker.bin