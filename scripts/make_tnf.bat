@echo off

@:1

@: Environment
@set PATH=C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE;%PATH%
@set PATH=C:\Program Files\Microsoft Visual Studio 9.0\VC\BIN;%PATH%
@set LIB=C:\Program Files\Microsoft Visual Studio 9.0\VC\LIB;%LIB%
@set LIB=C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib;%LIB%
@set LIB=.\StlPort;%LIB%
@set INCLUDE=C:\Program Files\Microsoft Visual Studio 9.0\VC\include;%INCLUDE%
@set INCLUDE=C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include;%INCLUDE%
@set INCLUDE=.\StlPort;%INCLUDE%

@: Server
@del ".\\fonline_tnf.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__SERVER" /Fo".\\" /Fd".\\fonline_tnf.obj" /FD /c ".\\fonline_tnf.cpp"
link.exe /nologo /dll /incremental:no /machine:I386 ".\\fonline_tnf.obj" /out:".\\fonline_tnf.dll"

@: Client
@del ".\\fonline_tnf_client.dll"
cl.exe /nologo /MT /W3 /O2 /Gd /D "__CLIENT" /Fo".\\fonline_tnf_client.obj" /Fd".\\" /FD /c ".\\fonline_tnf.cpp"
link.exe /nologo /dll /incremental:no /machine:I386 ".\\fonline_tnf_client.obj" /out:".\\fonline_tnf_client.dll"

@: Delete unnecessary stuff
@del ".\\fonline_tnf.obj"
@del ".\\fonline_tnf.exp"
@del ".\\fonline_tnf.lib"
@del ".\\fonline_tnf.idb"
@del ".\\fonline_tnf_client.obj"
@del ".\\fonline_tnf_client.exp"
@del ".\\fonline_tnf_client.lib"
@del ".\\vc90.idb"

@pause

@goto 1