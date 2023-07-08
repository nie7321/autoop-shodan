# Microsoft Developer Studio Generated NMAKE File, Based on shodan_plugin.dsp
!IF "$(CFG)" == ""
CFG=shodan_plugin - Win32 Debug
!MESSAGE No configuration specified. Defaulting to shodan_plugin - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "shodan_plugin - Win32 Release" && "$(CFG)" != "shodan_plugin - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shodan_plugin.mak" CFG="shodan_plugin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shodan_plugin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "shodan_plugin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "shodan_plugin - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\myplugin.aox"


CLEAN :
	-@erase "$(INTDIR)\plugin_main.obj"
	-@erase "$(INTDIR)\shodan_plugin.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\myplugin.aox"
	-@erase "$(OUTDIR)\myplugin.exp"
	-@erase "$(OUTDIR)\myplugin.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /Fp"$(INTDIR)\shodan_plugin.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\shodan_plugin.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\myplugin.pdb" /machine:I386 /out:"$(OUTDIR)\myplugin.aox" /implib:"$(OUTDIR)\myplugin.lib" 
LINK32_OBJS= \
	"$(INTDIR)\plugin_main.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\myplugin.aox" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "shodan_plugin - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\myplugin.aox"


CLEAN :
	-@erase "$(INTDIR)\plugin_main.obj"
	-@erase "$(INTDIR)\shodan_plugin.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\myplugin.aox"
	-@erase "$(OUTDIR)\myplugin.exp"
	-@erase "$(OUTDIR)\myplugin.ilk"
	-@erase "$(OUTDIR)\myplugin.lib"
	-@erase "$(OUTDIR)\myplugin.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /Fp"$(INTDIR)\shodan_plugin.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\shodan_plugin.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\myplugin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\myplugin.aox" /implib:"$(OUTDIR)\myplugin.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\plugin_main.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\myplugin.aox" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("shodan_plugin.dep")
!INCLUDE "shodan_plugin.dep"
!ELSE 
!MESSAGE Warning: cannot find "shodan_plugin.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "shodan_plugin - Win32 Release" || "$(CFG)" == "shodan_plugin - Win32 Debug"
SOURCE=.\plugin_main.cpp

"$(INTDIR)\plugin_main.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\shodan_plugin.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "shodan_plugin - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /Fp"$(INTDIR)\shodan_plugin.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\shodan_plugin.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "shodan_plugin - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /Fp"$(INTDIR)\shodan_plugin.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\shodan_plugin.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

