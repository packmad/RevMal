@echo off
setlocal EnableExtensions EnableDelayedExpansion

if "%~1"=="" (
  echo Usage: build.bat file.asm
  exit /b 1
)

set "FILE=%~1"
set "NAME=%~n1"

rem --- Detect architecture by available assembler
set "ARCH="
where /q ml64
if not errorlevel 1 (
  set "ARCH=64"
) else (
  where /q ml
  if not errorlevel 1 set "ARCH=32"
)

if not defined ARCH (
  echo ERROR: Could not find ml64 or ml in PATH.
  echo Use a Native Tools Command Prompt.
  exit /b 2
)

if "%ARCH%"=="64" (
  set "OBJ=!NAME!64.obj"
  set "OUT=!NAME!64.exe"

  echo [x64] Assembling "!FILE!"...
  ml64 /c /Fo "!OBJ!" "!FILE!"
  if errorlevel 1 exit /b 1

  echo [x64] Linking "!OUT!"...
  link /nologo /entry:main /subsystem:console "!OBJ!" kernel32.lib /out:"!OUT!"
  if errorlevel 1 exit /b 1
) else (
  set "OBJ=!NAME!32.obj"
  set "OUT=!NAME!32.exe"

  echo [x86] Assembling "!FILE!"...
  ml /c /coff /Fo "!OBJ!" "!FILE!"
  if errorlevel 1 exit /b 1

  echo [x86] Linking "!OUT!"...
  link /nologo /entry:main /subsystem:console "!OBJ!" kernel32.lib /out:"!OUT!"
  if errorlevel 1 exit /b 1
)

del "!OBJ!" >nul 2>&1
echo Done: "!OUT!"
endlocal