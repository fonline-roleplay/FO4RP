@echo off
setlocal

rem Output DLL names. Keep these in sync with the names referenced by the scripts.
set "OUTPUT_DLL_NAME=fonline_tnf64.dll"
set "CLIENT_OUTPUT_DLL_NAME=fonline_tnf_client64.dll"
set "BUILD_CLIENT=1"

rem Locate Visual Studio and initialize its native x64 build environment.
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

call "%VS_PATH%\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64
if errorlevel 1 exit /b %errorlevel%

echo Building server library %OUTPUT_DLL_NAME%...
cl.exe /nologo /MT /W3 /O2 /Gd /D__SERVER /c /Fo"fonline_tnf_server.obj" fonline_tnf.cpp
if errorlevel 1 goto :failed
cl.exe /nologo /MT /W3 /O2 /Gd /D__SERVER /c /Fo"qmap_tools_server.obj" qmap_tools.cpp
if errorlevel 1 goto :failed
cl.exe /nologo /MT /W3 /O2 /Gd /D__SERVER /c /Fo"scriptarray_api_server.obj" AngelScript\scriptarray_api.cpp
if errorlevel 1 goto :failed
link.exe /nologo /dll /incremental:no /machine:X64 fonline_tnf_server.obj qmap_tools_server.obj scriptarray_api_server.obj /implib:"fonline_tnf_server.lib" /out:"%OUTPUT_DLL_NAME%.new"
if errorlevel 1 goto :failed
move /y "%OUTPUT_DLL_NAME%.new" "%OUTPUT_DLL_NAME%" >nul
if errorlevel 1 goto :failed

if /i not "%BUILD_CLIENT%"=="1" goto :success

echo Building client library %CLIENT_OUTPUT_DLL_NAME%...
cl.exe /nologo /MT /W3 /O2 /Gd /D__CLIENT /c /Fo"fonline_tnf_client.obj" fonline_tnf.cpp
if errorlevel 1 goto :failed
cl.exe /nologo /MT /W3 /O2 /Gd /D__CLIENT /c /Fo"qmap_tools_client.obj" qmap_tools.cpp
if errorlevel 1 goto :failed
cl.exe /nologo /MT /W3 /O2 /Gd /D__CLIENT /c /Fo"q_sprites_client.obj" q_sprites.cpp
if errorlevel 1 goto :failed
cl.exe /nologo /MT /W3 /O2 /Gd /D__CLIENT /c /Fo"scriptarray_api_client.obj" AngelScript\scriptarray_api.cpp
if errorlevel 1 goto :failed
link.exe /nologo /dll /incremental:no /machine:X64 fonline_tnf_client.obj qmap_tools_client.obj q_sprites_client.obj scriptarray_api_client.obj /implib:"fonline_tnf_client.lib" /out:"%CLIENT_OUTPUT_DLL_NAME%.new"
if errorlevel 1 goto :failed
move /y "%CLIENT_OUTPUT_DLL_NAME%.new" "%CLIENT_OUTPUT_DLL_NAME%" >nul
if errorlevel 1 goto :failed

:success
call :cleanup
echo Build completed successfully.
exit /b 0

:failed
set "BUILD_ERROR=%errorlevel%"
call :cleanup
echo Build failed with error %BUILD_ERROR%.
exit /b %BUILD_ERROR%

:cleanup
del /q fonline_tnf_server.obj qmap_tools_server.obj scriptarray_api_server.obj fonline_tnf_client.obj qmap_tools_client.obj q_sprites_client.obj scriptarray_api_client.obj 2>nul
del /q fonline_tnf_server.exp fonline_tnf_server.lib fonline_tnf_client.exp fonline_tnf_client.lib 2>nul
del /q "%OUTPUT_DLL_NAME%.new" "%CLIENT_OUTPUT_DLL_NAME%.new" 2>nul
exit /b 0
