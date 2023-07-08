# Microsoft Developer Studio Project File - Name="mp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mp - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mp.mak" CFG="mp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mp - Win32 ReleaseGP2" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SDKSrc/Public/dlls", NVGBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Releasemp"
# PROP Intermediate_Dir ".\Releasemp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Zi /O2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /Fp".\Releasemp/mod.pch" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\Releasemp/mod.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\mp.def" /out:"c:\games\sierra\half-life\tfc\dlls\shodan.dll"
# SUBTRACT LINK32 /profile /incremental:yes /nodefaultlib

!ELSEIF  "$(CFG)" == "mp - Win32 ReleaseGP2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mp___Win32_ReleaseGP2"
# PROP BASE Intermediate_Dir "mp___Win32_ReleaseGP2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ReleaseGP2"
# PROP Intermediate_Dir ".\ReleaseGP2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /Zi /O2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /Fp".\Releasemp/mod.pch" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Zi /O2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /D "GP2_DLL" /Fp".\Releasemp/mod.pch" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\Releasemp/mod.bsc"
# ADD BSC32 /nologo /o".\Releasemp/mod.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\mp.def" /out:"c:\games\sierra\half-life\tfc\dlls\shodan.dll"
# SUBTRACT BASE LINK32 /profile /incremental:yes /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib /nologo /subsystem:windows /dll /map /machine:I386 /def:".\shodan_gp2.def" /out:"c:\games\sierra\half-life\tfc\dlls\shodan_gp2.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "mp - Win32 Release"
# Name "mp - Win32 ReleaseGP2"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\3rdperson.cpp
# End Source File
# Begin Source File

SOURCE=.\ablaster.cpp
# End Source File
# Begin Source File

SOURCE=.\admincommands.cpp
# End Source File
# Begin Source File

SOURCE=.\airstrike.cpp
# End Source File
# Begin Source File

SOURCE=.\aogarg.cpp
# End Source File
# Begin Source File

SOURCE=.\aohgrunt.cpp
# End Source File
# Begin Source File

SOURCE=.\aomonsters.cpp
# End Source File
# Begin Source File

SOURCE=.\basiccommands.cpp
# End Source File
# Begin Source File

SOURCE=.\biggun.cpp
# End Source File
# Begin Source File

SOURCE=.\buying.cpp
# End Source File
# Begin Source File

SOURCE=.\c4.cpp
# End Source File
# Begin Source File

SOURCE=.\cheatcommands.cpp
# End Source File
# Begin Source File

SOURCE=.\CIniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\commands.cpp
# End Source File
# Begin Source File

SOURCE=.\credits.cpp
# End Source File
# Begin Source File

SOURCE=.\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\dualbar.cpp
# End Source File
# Begin Source File

SOURCE=.\energyblast.cpp
# End Source File
# Begin Source File

SOURCE=.\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\flashbang.cpp
# End Source File
# Begin Source File

SOURCE=.\funcommands.cpp
# End Source File
# Begin Source File

SOURCE=.\generalstuff.cpp
# End Source File
# Begin Source File

SOURCE=.\grapple.cpp
# End Source File
# Begin Source File

SOURCE=.\gundrop.cpp
# End Source File
# Begin Source File

SOURCE=.\h_export.cpp
# End Source File
# Begin Source File

SOURCE=.\helpsystem.cpp
# End Source File
# Begin Source File

SOURCE=.\holoduke.cpp
# End Source File
# Begin Source File

SOURCE=.\hwradar.cpp
# End Source File
# Begin Source File

SOURCE=.\icons.cpp
# End Source File
# Begin Source File

SOURCE=.\inisettings.cpp
# End Source File
# Begin Source File

SOURCE=.\kickvote.cpp
# End Source File
# Begin Source File

SOURCE=.\laserfence.cpp
# End Source File
# Begin Source File

SOURCE=.\leveleditor.cpp
# End Source File
# Begin Source File

SOURCE=.\lights.cpp
# End Source File
# Begin Source File

SOURCE=.\linkfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\mapvoting.cpp
# End Source File
# Begin Source File

SOURCE=.\mediflag.cpp
# End Source File
# Begin Source File

SOURCE=.\messageflag.cpp
# End Source File
# Begin Source File

SOURCE=.\modelprecache.cpp
# End Source File
# Begin Source File

SOURCE=.\nightgame.cpp
# End Source File
# Begin Source File

SOURCE=.\playercommands.cpp
# End Source File
# Begin Source File

SOURCE=.\playerstuff.cpp
# End Source File
# Begin Source File

SOURCE=.\plugin_api.cpp
# End Source File
# Begin Source File

SOURCE=.\plugins.cpp
# End Source File
# Begin Source File

SOURCE=.\proxmine.cpp
# End Source File
# Begin Source File

SOURCE=.\ranks.cpp
# End Source File
# Begin Source File

SOURCE=.\responsebot.cpp
# End Source File
# Begin Source File

SOURCE=.\revive.cpp
# End Source File
# Begin Source File

SOURCE=.\servercommands.cpp
# End Source File
# Begin Source File

SOURCE=.\skins.cpp
# End Source File
# Begin Source File

SOURCE=.\snarks.cpp
# End Source File
# Begin Source File

SOURCE=.\stealth.cpp
# End Source File
# Begin Source File

SOURCE=.\superflame.cpp
# End Source File
# Begin Source File

SOURCE=.\tazar.cpp
# End Source File
# Begin Source File

SOURCE=.\teleporters.cpp
# End Source File
# Begin Source File

SOURCE=.\testentity.cpp
# End Source File
# Begin Source File

SOURCE=.\trainstuff.cpp
# End Source File
# Begin Source File

SOURCE=.\tripmine.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\vampirism.cpp
# End Source File
# Begin Source File

SOURCE=.\versionres.rc
# End Source File
# Begin Source File

SOURCE=.\world.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\3rdperson.h
# End Source File
# Begin Source File

SOURCE=.\admincommands.h
# End Source File
# Begin Source File

SOURCE=.\aogarg.h
# End Source File
# Begin Source File

SOURCE=.\aohgrunt.h
# End Source File
# Begin Source File

SOURCE=.\aomonsters.h
# End Source File
# Begin Source File

SOURCE=.\autoop.h
# End Source File
# Begin Source File

SOURCE=.\buying.h
# End Source File
# Begin Source File

SOURCE=.\cdll_dll.h
# End Source File
# Begin Source File

SOURCE=.\CIniFile.h
# End Source File
# Begin Source File

SOURCE=.\commands.h
# End Source File
# Begin Source File

SOURCE=.\credits.h
# End Source File
# Begin Source File

SOURCE=.\customweapons.h
# End Source File
# Begin Source File

SOURCE=.\decals.h
# End Source File
# Begin Source File

SOURCE=.\enginecallback.h
# End Source File
# Begin Source File

SOURCE=.\extdll.h
# End Source File
# Begin Source File

SOURCE=.\leveleditor.h
# End Source File
# Begin Source File

SOURCE=.\plugin_api.h
# End Source File
# Begin Source File

SOURCE=.\plugins.h
# End Source File
# Begin Source File

SOURCE=.\shake.h
# PROP Exclude_From_Scan -1

!IF  "$(CFG)" == "mp - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "mp - Win32 ReleaseGP2"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\skins.h
# End Source File
# Begin Source File

SOURCE=.\testentity.h
# End Source File
# Begin Source File

SOURCE=.\trains.h
# End Source File
# Begin Source File

SOURCE=.\userstrings.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=.\voting.h
# End Source File
# End Group
# End Target
# End Project
