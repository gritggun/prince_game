# Microsoft Developer Studio Project File - Name="PrinceGame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PrinceGame - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PrinceGame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrinceGame.mak" CFG="PrinceGame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrinceGame - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PrinceGame - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PrinceGame - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "PrinceGame - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PrinceGame - Win32 Release"
# Name "PrinceGame - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PrinceGame.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinceGame.rc
# End Source File
# Begin Source File

SOURCE=.\PrinceGameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\PrinceGame.h
# End Source File
# Begin Source File

SOURCE=.\PrinceGameDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bgSkin01.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bgSkinMask.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Blue_Level1.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Blue_Level2.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Blue_Level3.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Blue_Level4.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Cyan_Level1.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Cyan_Level2.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Cyan_Level3.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Cyan_Level4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FIRE.bmp
# End Source File
# Begin Source File

SOURCE=.\items\FOUR.bmp
# End Source File
# Begin Source File

SOURCE=.\items\FOUR_MASK.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Green_Level1.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Green_Level2.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Green_Level3.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Green_Level4.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Level1_Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Level2_Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Level3_Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Level4_Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\items\ONE.bmp
# End Source File
# Begin Source File

SOURCE=.\items\ONE_MASK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OVER.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OVER_MASK.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Pink_Level1.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Pink_Level2.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Pink_Level3.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Pink_Level4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PrinceGame.ico
# End Source File
# Begin Source File

SOURCE=.\res\PrinceGame.rc2
# End Source File
# Begin Source File

SOURCE=.\marble\Red_Level1.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Red_Level2.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Red_Level3.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Red_Level4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\START.bmp
# End Source File
# Begin Source File

SOURCE=.\res\START_MASK.bmp
# End Source File
# Begin Source File

SOURCE=.\items\THREE.bmp
# End Source File
# Begin Source File

SOURCE=.\items\THREE_MARK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TIMEOVER.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TIMEOVER_MASK.bmp
# End Source File
# Begin Source File

SOURCE=.\items\TWO.bmp
# End Source File
# Begin Source File

SOURCE=.\items\TWO_MARK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wave1.bin
# End Source File
# Begin Source File

SOURCE=.\marble\Yellow_Level1.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Yellow_Level2.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Yellow_Level3.bmp
# End Source File
# Begin Source File

SOURCE=.\marble\Yellow_Level4.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\wave\bgsound01.wav
# End Source File
# Begin Source File

SOURCE=.\wave\cancel.wav
# End Source File
# Begin Source File

SOURCE=.\wave\item.wav
# End Source File
# Begin Source File

SOURCE=.\wave\move.wav
# End Source File
# Begin Source File

SOURCE=.\wave\over.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\wave\start.wav
# End Source File
# Begin Source File

SOURCE=.\wave\wait.WAV
# End Source File
# End Target
# End Project
