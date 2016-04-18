@ECHO OFF
IF #%1#==#MAKE_FOR# GOTO make_it

REM MAKE ALL
ECHO = = = СБОРКА ПРИМЕРА, ДЕМОНСТРУЮЩЕГО ОПРЕДЕЛЕНИЕ РАЗМЕРА КОДОВОГО КЭША = = =
ECHO Утилита к книге \"Техника  оптимизации  програм\"  /* название рабочее */
ECHO @ECHO OFF 																		> code.cache.size.run.bat 
ECHO ECHO = = = демонстрация определения размера кэша  = = = 						>> code.cache.size.run.bat 
ECHO ECHO Утилита к книге \"Техника  оптимизации  програм\"  /* название рабочее */	>> code.cache.size.run.bat 
ECHO ECHO N NOPs                        ...CLOCK...                                 >> code.cache.size.run.bat 
ECHO ECHO ------------------------------------------------------------------------- >> code.cache.size.run.bat 

FOR %%A IN (2,4,8,16,32,64,128,256,512,1024,2048) DO CALL  %0 MAKE_FOR %%A
ECHO DEL %%0 																		>> code.cache.size.run.bat 

GOTO end

:make_it
ECHO /%0/%1/%2 *
SHIFT
ECHO	CODE_SIZE EQU %1 					> code.cache.size.mod
ECHO	#define CODE_SIZE %1 				> code.cache.size.h
TYPE	code.cache.size.xm 					>> code.cache.size.mod
CALL	clock.make.bat   code.cache.size.c	> NUL
DEL		code.cache.size.mod
DEL		code.cache.size.h
IF 		NOT EXIST code.cache.size.exe 		GOTO err
IF 		EXIST code.cache.size.%1.exe 		DEL code.cache.size.%1.exe
REN  	code.cache.size.exe code.cache.size.%1.exe
ECHO 	code.cache.size.%1.exe 				>> code.cache.size.run.bat 
ECHO 	DEL code.cache.size.%1.exe 			>> code.cache.size.run.bat 
GOTO 	end

:err
ECHO -ERR ошибка компиляции! подробнее см. code.cache.size.err 
TYPE code.cache.size.err
EXIT
:end

