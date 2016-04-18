@Echo Off
@Cls
@Echo.

If Not "%1" == "" Set Root=%1:\

Set Tmp=%Root%\Temp
Set Temp=%Tmp%
Set TmpDir=%Tmp%

Path=Z:\;%Root%Bin\Sys;%Root%Bin\Sys\DOS;%Root%Bin;Bin;\Bin;W:\Bin
Set Prompt=$P$G

Set VC=%Root%Bin\Sys\VC
Set NC=%VC%
Set BGI=%Root%Bin\BGI
Set Exe=%Root%Temp
Set DOS4G=Quiet

Keyb ru > nul
%Root%Bin\Sys\Drv\Cyr\UniKbd /ib1 /k:crs

Mount S: %Root%.
Mount W: %Root%Doc
Echo.

C:     >nul 2>nul
%Root% >nul 2>nul
W:     >nul 2>nul

%VC%\vc

exit