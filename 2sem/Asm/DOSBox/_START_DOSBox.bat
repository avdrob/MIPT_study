@echo off

pushd "%~dp0Config"

xcopy /e /i /y *.* "%LocalAppData%"                                >nul 2>nul
xcopy /e /i /y *.* "%UserProfile%\Local Settings\Application Data" >nul 2>nul

cd ..

subst S: .     >nul 2>nul
subst W: .\Doc >nul 2>nul

start ..\Far\Far @W:

start DOSBox.exe %*

popd
