# Microsoft Developer Studio Project File - Name="shark_cheater" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

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
!MESSAGE "shark_cheater - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "shark_cheater - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"shark_cheater.exe"

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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

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
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Cheater.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Cheater.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Engine.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Cheat_Engine\Shark_Engine.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\CheaterApp\CheaterApp.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\CheaterApp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\CheatFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\CheatFile.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\Controls.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\EditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\EditDlg.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\OptionsDlg.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\SearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheaterApp\SearchDlg.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MAIN.RC
# End Source File
# Begin Source File

SOURCE=.\Rc.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\resource.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SharkApp.cpp
# End Source File
# Begin Source File

SOURCE=.\SharkApp.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\DLGAPP.ICO
# End Source File
# Begin Source File

SOURCE=.\DLGAPP_S.ICO
# End Source File
# End Group
# End Target
# End Project
