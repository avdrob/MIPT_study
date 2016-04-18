@ECHO OFF
IF "%1"=="__class"    goto END
IF "%1"=="__typeinfo" goto USAGE
IF "%1"=="/?" goto HELP
IF "%1"=="-5.0" GOTO OLD

bcc32.exe -I"C:\Program Files\BCC55\Include" -L"C:\Program Files\BCC55\LIB" %1 %2 %3 %4 %5 %6 %7
IF ERRORLEVEL==1 goto ERR
GOTO end

:OLD
SETLOCAL
SET PATH=I:\BC5\bin\

bcc32.exe -II:\BC5\INCLUDE  -LI:\BC5\LIB %1 %2 %3 %4 %5 %6 %7
ENDLOCAL
GOTO  end

:V55


GOTO END

:USAGE
ECHO Borland C++ 5.0         [bcc32.exe -5.0 $args]   
ECHO Borland C++ 5.5         [bcc32.exe $args]
GOTO END

:HELP
bcc32.exe 
GOTO END

:END
set errorlevel=666
:ERR

