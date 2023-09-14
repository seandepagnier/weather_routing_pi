@echo off
@echo Copies the current version Testplugin FE2 files to the present working directory.
@echo If there is no destination identified , it defaults to the present working directory.
dir

Rem source folder location
set "source=C:\Users\fcgle\source\testplugin_pi"

Rem target folder location
Rem set "target=D:\<Destination Target Folder Path>"

@echo Xcopy files and folders from %source% to the current directory
@echo Copy all files from one folder to another.
@echo Syntax: xcopy /s/z c:\source d:\target

xcopy /s %source%\ci\ .\ci\
rem xcopy /s %source%\ci\extras\
xcopy /s %source%\cmake\ .\cmake\
rem xcopy /s %source%\cmake\in-files\
xcopy /s %source%\.circleci\ .\.circleci\
xcopy /s %source%\mingw\ .\mingw\
xcopy /s %source%\appveyor.yml 
xcopy /s %source%\.travis.yml


Rem xcopy.exe is definitely your friend here. It's built into Windows, so its cost is nothing.
Rem  https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/xcopy
Rem  Just xcopy /s c:\source d:\target
Rem Copies files and directories, including subdirectories.
Rem  You'd probably want to tweak a few things; some of the options we also add include these:
Rem
Rem  <Source> 	Required. Specifies the location and names of the files you want to copy. 
Rem    This parameter must include either a drive or a path.
Rem  [<Destination>] 	Specifies the destination of the files you want to copy. 
Rem    This parameter can include a drive letter and colon, a directory name, a file name, or a combination of these.
Rem
Rem    /s/e - recursive copy, including copying empty directories.
Rem    /s - Copies directories and subdirectories, unless they are empty. If you omit /s, xcopy works within a single directory.
Rem    /e -	Copy all subdirectories, even if they are empty. Use /e with the /s and /t options.
Rem    /v - verify the copy against the original.
Rem    /h - copy system and hidden files.
Rem    /k - copy read-only attributes, otherwise, all files become read-write.
Rem    /x - if you care about permissions, if you worry about permissions /o or /x.
Rem    /y - don't prompt before overwriting existing files.
Rem    /z - if you think the copy might fail and you want to restart it, use this. 
Rem      It places a marker on each file as it copies, 
Rem      so you can rerun the xcopy command to pick up from where it left off.
Rem
Rem  If you think the xcopy might fail partway through 
Rem  (like when you are copying over a flaky network connection), 
Rem  or that you have to stop it and want to continue it later, 
Rem  you can use xcopy /s/z c:\source d:\target.