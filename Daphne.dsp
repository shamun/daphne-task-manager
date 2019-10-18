# Microsoft Developer Studio Project File - Name="Daphne" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Daphne - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Daphne.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Daphne.mak" CFG="Daphne - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Daphne - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Daphne - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Daphne - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zd /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 psapi.lib pdh.lib shlwapi.lib libintl.lib rasapi32.lib /nologo /subsystem:windows /map /machine:I386

!ELSEIF  "$(CFG)" == "Daphne - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 psapi.lib pdh.lib shlwapi.lib libintl.lib rasapi32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"uuid.lib" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Lenguaje...
PostBuild_Cmds=update_po.cmd
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Daphne - Win32 Release"
# Name "Daphne - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AuthUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BackDoorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Daphne.cpp
# End Source File
# Begin Source File

SOURCE=.\Daphne.rc
# End Source File
# Begin Source File

SOURCE=.\DaphneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Inspector.cpp
# End Source File
# Begin Source File

SOURCE=.\KillAllDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Killer.cpp
# End Source File
# Begin Source File

SOURCE=.\KMIEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KMIEditPListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MBWebAgreeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewTrapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PPAdv.cpp
# End Source File
# Begin Source File

SOURCE=.\PPHighlight.cpp
# End Source File
# Begin Source File

SOURCE=.\PPKill.cpp
# End Source File
# Begin Source File

SOURCE=.\PPLook.cpp
# End Source File
# Begin Source File

SOURCE=.\PPStartup.cpp
# End Source File
# Begin Source File

SOURCE=.\PPTraps.cpp
# End Source File
# Begin Source File

SOURCE=.\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessList.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcList.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertiesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RunProcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SCManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SendMsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Trap.cpp
# End Source File
# Begin Source File

SOURCE=.\W32Values.cpp
# End Source File
# Begin Source File

SOURCE=.\WndsPopUp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AuthUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\BackDoorDlg.h
# End Source File
# Begin Source File

SOURCE=.\CrashDlg.h
# End Source File
# Begin Source File

SOURCE=.\Daphne.h
# End Source File
# Begin Source File

SOURCE=.\DaphneDlg.h
# End Source File
# Begin Source File

SOURCE=.\exception.h
# End Source File
# Begin Source File

SOURCE=.\Inspector.h
# End Source File
# Begin Source File

SOURCE=.\KillAllDlg.h
# End Source File
# Begin Source File

SOURCE=.\Killer.h
# End Source File
# Begin Source File

SOURCE=.\KMIEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\KMIEditPListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MBWebAgreeDlg.h
# End Source File
# Begin Source File

SOURCE=.\memdc.h
# End Source File
# Begin Source File

SOURCE=.\NewTrapDlg.h
# End Source File
# Begin Source File

SOURCE=.\PPAdv.h
# End Source File
# Begin Source File

SOURCE=.\PPHighlight.h
# End Source File
# Begin Source File

SOURCE=.\PPKill.h
# End Source File
# Begin Source File

SOURCE=.\PPLook.h
# End Source File
# Begin Source File

SOURCE=.\PPStartup.h
# End Source File
# Begin Source File

SOURCE=.\PPTraps.h
# End Source File
# Begin Source File

SOURCE=.\Process.h
# End Source File
# Begin Source File

SOURCE=.\ProcessList.h
# End Source File
# Begin Source File

SOURCE=.\ProcList.h
# End Source File
# Begin Source File

SOURCE=.\PropertiesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RunProcDlg.h
# End Source File
# Begin Source File

SOURCE=.\SCManager.h
# End Source File
# Begin Source File

SOURCE=.\SendMsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Trap.h
# End Source File
# Begin Source File

SOURCE=.\W32Values.h
# End Source File
# Begin Source File

SOURCE=.\WndsPopUp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrowcop.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00007.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Daphne.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Daphne.ico
# End Source File
# Begin Source File

SOURCE=.\res\Daphne.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\null.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE="..\..\..\Program Files\Microsoft Platform SDK\Lib\AdvAPI32.Lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\Program Files\Microsoft Visual Studio\VC98\Lib\UUID.LIB"
# End Source File
# End Target
# End Project
