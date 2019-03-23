echo The starting directory is %CD%
cd /D "%~dp0"
echo The current directory is %CD%

@echo off

@:1

@: Environment
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin;%PATH%
@set LIB=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib;%LIB%
@set LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib;%LIB%
@set LIB=.\StlPort\;%LIB%
@set INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include;%INCLUDE%
@set INCLUDE=.\StlPort\;%INCLUDE%

@: Server
@del ".\\fonline_tnf.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__SERVER" /Fd".\\fonline_tnf.obj" /FD /c .\\fonline_tnf.cpp qmap_tools.cpp
link.exe /nologo /dll /incremental:no /machine:I386 .\\fonline_tnf.obj qmap_tools.obj /out:".\\fonline_tnf.dll"

@: Delete server stuff
@del ".\\fonline_tnf.obj"
@del ".\\fonline_tnf.exp"
@del ".\\fonline_tnf.lib
@del ".\\fonline_tnf.idb"

@del ".\\qmap_tools.obj"
@del ".\\qmap_tools.exp"
@del ".\\qmap_tools.lib"

@del ".\\q_sprites.obj"
@del ".\\q_sprites.exp"
@del ".\\q_sprites.lib"

@: Client
@del ".\\fonline_tnf_client.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__CLIENT" /Fd".\\fonline_tnf.obj" /FD /c .\\fonline_tnf.cpp qmap_tools.cpp q_sprites.cpp
link.exe /nologo /dll /incremental:no /machine:I386 .\\fonline_tnf.obj qmap_tools.obj q_sprites.obj /out:".\\fonline_tnf_client.dll"

@: Delete leftovers stuff
@del ".\\fonline_tnf.obj"
@del ".\\fonline_tnf.exp"
@del ".\\fonline_tnf.lib
@del ".\\fonline_tnf.idb"

@del ".\\fonline_tnf_client.obj"
@del ".\\fonline_tnf_client.exp"
@del ".\\fonline_tnf_client.lib"

@del ".\\qmap_tools.obj"
@del ".\\qmap_tools.exp"
@del ".\\qmap_tools.lib"

@del ".\\q_sprites.obj"
@del ".\\q_sprites.exp"
@del ".\\q_sprites.lib"
@del ".\\vc90.idb"

@pause

@goto 1