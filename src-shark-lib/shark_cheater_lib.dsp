# Microsoft Developer Studio Project File - Name="shark_cheater" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=shark_cheater - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "shark_cheater.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shark_cheater.mak" CFG="shark_cheater - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shark_cheater - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "shark_cheater - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "shark_cheater - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"shark_cheater.lib"

!ELSEIF  "$(CFG)" == "shark_cheater - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"shark_cheater.lib"

!ENDIF 

# Begin Target

# Name "shark_cheater - Win32 Release"
# Name "shark_cheater - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "CheaterApp"

# PROP Default_Filter ""
# Begin Group "Cheat_Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Base.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Cheater.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Cheater.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Engine.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Engine.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CheaterApp\CheaterApp.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\CheaterApp.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\CheatFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\CheatFile.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Controls.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\EditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\EditDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\SearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\SearchDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DLGAPP.ICO
# End Source File
# Begin Source File

SOURCE=.\DLGAPP_S.ICO
# End Source File
# Begin Source File

SOURCE=.\MAIN.RC
# End Source File
# Begin Source File

SOURCE=.\MAIN.res
# End Source File
# Begin Source File

SOURCE=.\Rc.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SharkApp.cpp
# End Source File
# Begin Source File

SOURCE=.\SharkApp.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
