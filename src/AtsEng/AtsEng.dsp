# Microsoft Developer Studio Project File - Name="AtsEng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AtsEng - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AtsEng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AtsEng.mak" CFG="AtsEng - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AtsEng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AtsEng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AtsEng - Win32 Enu Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AtsEng - Win32 Enu Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AtsEng - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AtsEng - Win32 Release MinSize" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AtsEng - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /D "_AFXDLL" /D "_USE_CSTRING" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AtsEng - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /D "_AFXDLL" /D "ATS_HAS_DLL" /D "_USE_CSTRING" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AtsEng - Win32 Enu Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AtsEng___Win32_Enu_Release"
# PROP BASE Intermediate_Dir "AtsEng___Win32_Enu_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AtsEng___Win32_Enu_Release"
# PROP Intermediate_Dir "AtsEng___Win32_Enu_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AtsEng - Win32 Enu Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AtsEng___Win32_Enu_Static_Release"
# PROP BASE Intermediate_Dir "AtsEng___Win32_Enu_Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AtsEng___Win32_Enu_Static_Release"
# PROP Intermediate_Dir "AtsEng___Win32_Enu_Static_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AtsEng - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AtsEng___Win32_Static_Release"
# PROP BASE Intermediate_Dir "AtsEng___Win32_Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AtsEng___Win32_Static_Release"
# PROP Intermediate_Dir "AtsEng___Win32_Static_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /D "_USE_CSTRING" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "AFX_TARG_ENU" /d "AFX_TARG_CHS" /d "AFX_TARG_NEUD"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AtsEng - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AtsEng___Win32_Release_MinSize"
# PROP BASE Intermediate_Dir "AtsEng___Win32_Release_MinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AtsEng___Win32_Release_MinSize"
# PROP Intermediate_Dir "AtsEng___Win32_Release_MinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /D "_AFXDLL" /D "_USE_CSTRING" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ATS_EXPORTS" /D "_AFXDLL" /D "_USE_CSTRING" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "AtsEng - Win32 Release"
# Name "AtsEng - Win32 Debug"
# Name "AtsEng - Win32 Enu Release"
# Name "AtsEng - Win32 Enu Static Release"
# Name "AtsEng - Win32 Static Release"
# Name "AtsEng - Win32 Release MinSize"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AtsBaseFun.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsExpExt.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsExpMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsExpValue.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsObj.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsObject.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsParam.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsRegKey.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsSegIns.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsSegmentItem.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsSentence.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsSentIns.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsValue.cpp
# End Source File
# Begin Source File

SOURCE=.\AtsVarItem.cpp
# End Source File
# Begin Source File

SOURCE=.\BoolObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DllModule.cpp
# End Source File
# Begin Source File

SOURCE=.\DllModuleObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DoubleObj.cpp
# End Source File
# Begin Source File

SOURCE=.\IntObj.cpp
# End Source File
# Begin Source File

SOURCE=.\JfScriptEngineMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMemFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKeyObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptExpExt.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptLine.cpp
# End Source File
# Begin Source File

SOURCE=.\SentenceSite.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StringObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueSet.cpp
# End Source File
# Begin Source File

SOURCE=.\WndParam.cpp
# End Source File
# Begin Source File

SOURCE=.\WSModule.cpp
# End Source File
# Begin Source File

SOURCE=.\WSModuleObj.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AtsBaseFun.h
# End Source File
# Begin Source File

SOURCE=.\AtsCommDef.h
# End Source File
# Begin Source File

SOURCE=.\AtsDef.h
# End Source File
# Begin Source File

SOURCE=.\AtsEngine.h
# End Source File
# Begin Source File

SOURCE=.\AtsExpExt.h
# End Source File
# Begin Source File

SOURCE=.\AtsExpMgr.h
# End Source File
# Begin Source File

SOURCE=.\AtsExpValue.h
# End Source File
# Begin Source File

SOURCE=.\AtsFile.h
# End Source File
# Begin Source File

SOURCE=.\AtsObj.h
# End Source File
# Begin Source File

SOURCE=.\AtsObject.h
# End Source File
# Begin Source File

SOURCE=.\AtsObjPtr.h
# End Source File
# Begin Source File

SOURCE=.\AtsParam.h
# End Source File
# Begin Source File

SOURCE=.\AtsRegKey.h
# End Source File
# Begin Source File

SOURCE=.\AtsSegIns.h
# End Source File
# Begin Source File

SOURCE=.\AtsSegmentItem.h
# End Source File
# Begin Source File

SOURCE=.\AtsSentence.h
# End Source File
# Begin Source File

SOURCE=.\AtsSentIns.h
# End Source File
# Begin Source File

SOURCE=.\AtsString.h
# End Source File
# Begin Source File

SOURCE=.\AtsValue.h
# End Source File
# Begin Source File

SOURCE=.\AtsVarItem.h
# End Source File
# Begin Source File

SOURCE=.\BoolObj.h
# End Source File
# Begin Source File

SOURCE=.\DllModule.h
# End Source File
# Begin Source File

SOURCE=.\DllModuleObj.h
# End Source File
# Begin Source File

SOURCE=.\DoubleObj.h
# End Source File
# Begin Source File

SOURCE=.\IntObj.h
# End Source File
# Begin Source File

SOURCE=.\JfScriptEngineMgr.h
# End Source File
# Begin Source File

SOURCE=.\MyMemFile.h
# End Source File
# Begin Source File

SOURCE=.\RegKeyObj.h
# End Source File
# Begin Source File

SOURCE=.\ScriptExpExt.h
# End Source File
# Begin Source File

SOURCE=.\ScriptLine.h
# End Source File
# Begin Source File

SOURCE=.\SentenceSite.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringObj.h
# End Source File
# Begin Source File

SOURCE=.\ValueSet.h
# End Source File
# Begin Source File

SOURCE=.\WndParam.h
# End Source File
# Begin Source File

SOURCE=.\WSModule.h
# End Source File
# Begin Source File

SOURCE=.\WSModuleObj.h
# End Source File
# End Group
# End Target
# End Project
