@ECHO OFF
SET		prj_name=asm.vs.c.min
ECHO	= = = собираем проект %prj_name% = = =

IF		EXIST %prj_name%.asm.obj 		DEL %prj_name%.asm.obj 
CALL	ml /c %prj_name%.asm.asm	 >	%prj_name%.asm.err

IF  	NOT EXIST %prj_name%.asm.obj	GOTO err_asm

CALL	clock.make %1 -release %prj_name%.c %prj_name%.asm.obj 

IF		EXIST %prj_name%.asm.err		DEL %prj_name%.asm.err
IF		EXIST %prj_name%.asm.obj		DEL %prj_name%.asm.obj
IF		NOT	EXIST %prj_name%.exe		GOTO end

ECHO ::проект %prj_name% успешно сформирован
ECHO ::%prj_name%.exe для запуска
GOTO	end

:err_asm
ECHO -ERR: ошибка ассемблирования! (см. %prj_name%.asm.err)

:end
