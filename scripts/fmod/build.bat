@echo off

@:1

@: Environment
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN;%PATH%
@set LIB=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\LIB;%LIB%
@set LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v6.0A\Lib;%LIB%
@set LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib;%LIB%
@set LIB=..\StlPort;%LIB%
@set LIB=.\lib;%LIB%
@set LIB=.\lib\fmod;%LIB%
@set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include;%INCLUDE%
@set INCLUDE=..\StlPort;%INCLUDE%
@set INCLUDE=.\inc;%INCLUDE%
@set INCLUDE=.\inc\tinydir;%INCLUDE%
@set INCLUDE=.\inc\fmod;%INCLUDE%
@set INCLUDE=.\inc\miniz;%INCLUDE%
@set INCLUDE=.\inc\cwalk;%INCLUDE%
@set INCLUDE=..\;%INCLUDE%

@: Client
if not exist ".\bin\" mkdir .\bin\
@del ".\bin\fofmod_client.dll"

SET COMPILE_OPTIONS = /nologo /MT /W3 /O2 /Gd /D "__CLIENT" /D "FOFMOD_DEBUG"
SET LINK_OPTIONS = /nologo /dll /DEBUG /incremental:no /machine:I386

cl.exe /nologo /MT /W3 /O2 /Gd /D "__CLIENT" /D "FOFMOD_DEBUG" /Fd"fofmod_client.obj" /FD /c .\src\cwalk\cwalk.c .\src\perfcounter.cpp .\src\zipfile.cpp .\src\archive.cpp .\src\util.cpp .\src\fofmod.cpp .\inc\miniz\miniz.c .\src\cachedata.cpp .\src\fofmodlistener.cpp .\src\fofmodsystem.cpp  .\src\fofmodsound.cpp .\src\fofmodchannel.cpp  .\src\refcount.cpp
link.exe /nologo /dll /DEBUG /incremental:no /machine:I386 cwalk.obj perfcounter.obj archive.obj zipfile.obj util.obj cachedata.obj fofmod.obj miniz.obj fofmodlistener.obj fofmodsystem.obj fofmodsound.obj fofmodchannel.obj  refcount.obj fmod_vc.lib /out:".\bin\fofmod_client.dll"

@: Delete leftovers stuff
@del ".\\cwalk.obj"
@del ".\\perfcounter.obj"
@del ".\\archive.obj"
@del ".\\zipfile.obj"
@del ".\\util.obj"
@del ".\\miniz.obj"
@del ".\\cachedata.obj"
@del ".\\fofmod.obj"
@del ".\\fofmodlistener.obj"
@del ".\\fofmodsystem.obj"
@del ".\\refcount.obj"	
@del ".\\fofmodsound.obj"
@del ".\\fofmodchannel.obj"
@del ".\\fofmod_client.obj"
@del ".\\fofmod_client.exp"
@del ".\\fofmod_client.lib"
@del ".\\fofmod_client.idb"

@del ".\bin\fofmod_client.obj"
@del ".\bin\fofmod_client.exp"
@del ".\bin\fofmod_client.lib"
@del ".\bin\fofmod_client.idb"
