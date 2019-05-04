@echo off

@: Environment
@set PATH=D:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE;%PATH%
@set PATH=D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin;%PATH%
@set LIB=D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib;%LIB%
@set LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib;%LIB%
@set LIB=.\StlPort\;%LIB%
@set INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include;%INCLUDE%
@set INCLUDE=D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include;%INCLUDE%
@set INCLUDE=.\StlPort\;%INCLUDE%

@: Server
@del ".\\fonline_tnf.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__SERVER" /Fd".\\fonline_tnf.obj" /FD /c .\\fonline_tnf.cpp qmap_tools.cpp
link.exe /nologo /dll /incremental:no /machine:I386 .\\fonline_tnf.obj qmap_tools.obj /out:".\\fonline_tnf.dll"

@: Client
@del ".\\fonline_tnf_client.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__CLIENT" /Fd".\\fonline_tnf.obj" /FD /c .\\fonline_tnf.cpp qmap_tools.cpp q_sprites.cpp
link.exe /nologo /dll /incremental:no /machine:I386 .\\fonline_tnf.obj qmap_tools.obj q_sprites.obj /out:".\\fonline_tnf_client.dll"

@: Delete stuff
@del ".\\fonline_tnf.obj"
@del ".\\fonline_tnf.exp"
@del ".\\fonline_tnf.lib
@del ".\\fonline_tnf.idb"

@del ".\\fonline_tnf_client.exp"
@del ".\\fonline_tnf_client.lib"

@del ".\\qmap_tools.obj"
@del ".\\q_sprites.obj"

@pause