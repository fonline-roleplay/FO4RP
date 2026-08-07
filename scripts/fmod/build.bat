@echo off
setlocal

set "OUTPUT_DIR=bin"
set "OUTPUT_DLL_NAME=fofmod_client64.dll"
set "FMOD_RUNTIME_OUTPUT_NAME=fmod64.dll"
set "FMOD_IMPORT_LIB=lib\fmod64\fmod_vc.lib"
set "FMOD_RUNTIME=lib\fmod64\fmod.dll"
set "CLIENT_IMPORT_LIB=..\..\..\..\fo2 sdk\Client\ClientOGL64.lib"
set "OBJ_DIR=obj\x64"
set "TEMP_OUTPUT_DIR=%OBJ_DIR%\output"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Error: vswhere.exe was not found. Install Visual Studio with C++ build tools.
    exit /b 1
)

for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_PATH=%%I"
if not defined VS_PATH (
    echo Error: no Visual Studio installation with x64 C++ build tools was found.
    exit /b 1
)

if not exist "%FMOD_IMPORT_LIB%" (
    echo Error: FMOD import library "%FMOD_IMPORT_LIB%" was not found.
    exit /b 1
)
if not exist "%FMOD_RUNTIME%" (
    echo Error: FMOD runtime "%FMOD_RUNTIME%" was not found.
    exit /b 1
)
if not exist "%CLIENT_IMPORT_LIB%" (
    echo Error: client import library "%CLIENT_IMPORT_LIB%" was not found.
    exit /b 1
)

call "%VS_PATH%\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64
if errorlevel 1 exit /b %errorlevel%

if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
if errorlevel 1 exit /b %errorlevel%
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"
if errorlevel 1 exit /b %errorlevel%
if not exist "%TEMP_OUTPUT_DIR%" mkdir "%TEMP_OUTPUT_DIR%"
if errorlevel 1 exit /b %errorlevel%
del /q "%OBJ_DIR%\*.obj" 2>nul

set "COMMON_OPTIONS=/nologo /MT /W3 /O2 /std:c++17 /D__CLIENT /DSKIP_PRAGMAS /D_HAS_STD_BYTE=0 /Iinc /Iinc\tinydir /Iinc\fmod /Iinc\miniz /Iinc\cwalk"

echo Compiling fofmod sources...
for %%F in (
    src\cwalk\cwalk.c
    src\fofmodid.cpp
    src\fofmodlock.cpp
    src\fofmodthread.cpp
    src\perfcounter.cpp
    src\zipfile.cpp
    src\archive.cpp
    src\util.cpp
    src\fofmod.cpp
    src\fofmoddsp.cpp
    src\fofmoddspAS.cpp
    src\fofmodchannelcontrol.cpp
    src\fofmodchannelgroup.cpp
    inc\miniz\miniz.c
    src\cachedata.cpp
    src\fofmodlistener.cpp
    src\fofmodsystem.cpp
    src\fofmodsound.cpp
    src\fofmodsoundAS.cpp
    src\fofmodchannel.cpp
    src\fofmodchannelAS.cpp
    src\refcount.cpp
    ..\AngelScript\scriptarray_api.cpp
) do (
    cl.exe %COMMON_OPTIONS% /c /Fo"%OBJ_DIR%\%%~nF.obj" "%%F"
    if errorlevel 1 goto :failed
)

echo Linking %OUTPUT_DLL_NAME%...
link.exe /nologo /dll /incremental:no /machine:X64 "%OBJ_DIR%\*.obj" "%FMOD_IMPORT_LIB%" "%CLIENT_IMPORT_LIB%" /implib:"%OBJ_DIR%\fofmod_client.lib" /out:"%TEMP_OUTPUT_DIR%\%OUTPUT_DLL_NAME%"
if errorlevel 1 goto :failed

move /y "%TEMP_OUTPUT_DIR%\%OUTPUT_DLL_NAME%" "%OUTPUT_DIR%\%OUTPUT_DLL_NAME%" >nul
if errorlevel 1 goto :failed
copy /y "%FMOD_RUNTIME%" "%OUTPUT_DIR%\%FMOD_RUNTIME_OUTPUT_NAME%" >nul
if errorlevel 1 goto :failed

echo Build completed successfully.
exit /b 0

:failed
set "BUILD_ERROR=%errorlevel%"
del /q "%TEMP_OUTPUT_DIR%\%OUTPUT_DLL_NAME%" 2>nul
echo Build failed with error %BUILD_ERROR%.
exit /b %BUILD_ERROR%
