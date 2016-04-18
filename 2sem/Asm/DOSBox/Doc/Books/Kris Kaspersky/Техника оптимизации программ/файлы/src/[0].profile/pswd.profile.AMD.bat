@ECHO OFF
cl.exe /c /Zi pswd.c
IF NOT EXIST pswd.obj GOTO err_compil

link.exe  /DEBUG pswd.obj
IF NOT EXIST pswd.exe GOTO err_link
GOTO end

:err_compil
ECHO -ERR: Ошибка компиляции!
goto end


:err_link
ECHO -ERR: Ошибка линковки!
goto end

:end