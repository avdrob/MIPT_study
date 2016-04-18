@echo off

call tasm %1
call tlink /t /3 %1

if     exist %1.com call td %1.com 
if not exist %1.com pause
