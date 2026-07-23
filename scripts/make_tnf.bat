@echo off

:1

@: Environment
@echo Setting up EnvVars...

@set VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0

@set PATH=%VS_PATH%\Common7\IDE;%PATH%
@set PATH=%VS_PATH%\VC\BIN;%PATH%
@set LIB=%VS_PATH%\VC\LIB;%LIB%
@set LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v6.0A\Lib;%LIB%
@set LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib;%LIB%
@set LIB=.\StlPort;%LIB%
@set INCLUDE=%VS_PATH%\VC\include;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include;%INCLUDE%
@set INCLUDE=.\StlPort;%INCLUDE%

@: Server
@echo Building Server lib...
@del ".\\fonline_tnf.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__SERVER" /Fd".\\fonline_tnf.obj" /FD /c .\\fonline_tnf.cpp qmap_tools.cpp
link.exe /nologo /dll /incremental:no /machine:I386 .\\fonline_tnf.obj qmap_tools.obj /out:".\\fonline_tnf.dll"

@: Delete server stuff
@del ".\\fonline_tnf.obj"
@del ".\\fonline_tnf.exp"
@del ".\\fonline_tnf.lib
@del ".\\fonline_tnf.idb"

@del ".\\qmap_tools.obj"

@: Client
@echo Building Client lib...

cl.exe /nologo /MT /W3 /O2 /Gd /D "__CLIENT" /Fd".\\fonline_tnf.obj" /FD /c .\\fonline_tnf.cpp qmap_tools.cpp q_sprites.cpp
link.exe /nologo /dll /incremental:no /machine:I386 .\\fonline_tnf.obj qmap_tools.obj q_sprites.obj /out:".\\fonline_tnf_client.dll"

@: Delete leftovers stuff
@del ".\\fonline_tnf.obj"

@del ".\\fonline_tnf.idb"


@del ".\\fonline_tnf_client.exp"
@del ".\\fonline_tnf_client.lib"

@del ".\\qmap_tools.obj"

@del ".\\q_sprites.obj"

@pause

@goto 1