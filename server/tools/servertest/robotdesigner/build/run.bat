@echo off

title robot designer

set ide.home=.

set CLASSPATH=%CLASSPATH%
set CLASSPATH=%CLASSPATH%;%ide.home%\conf
for %%i in (".\lib\*.jar") do call ".\cpappend.bat" %%i

start javaw -Dide.home=%ide.home% com.cms.designer.Main

:end
