@echo off

set bc=%Root%Bin\BC

if exist %bc%\tcdef.dsk del %bc%\tcdef.dsk

if exist C:\tc*.swp del C:\tc*.swp
if exist    tc*.swp del    tc*.swp

%bc%\bc.exe %1 %2 %3 %4 %5 %6 %7 %8 %9 /r%Temp% /p
