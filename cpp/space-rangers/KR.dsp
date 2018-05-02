# Microsoft Developer Studio Project File - Name="KR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KR - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KR.mak" CFG="KR - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KR - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KR - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KR - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "NDEBUG"
# ADD RSC /l 0x422 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib ddraw.lib dinput.lib winmm.lib dplayx.lib dsound.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "KR - Win32 Debug"

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
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib ddraw.lib dinput.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "KR - Win32 Release"
# Name "KR - Win32 Debug"
# Begin Group "res"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\basa_m.bmp
# End Source File
# Begin Source File

SOURCE=.\basa_p.bmp
# End Source File
# Begin Source File

SOURCE=.\basa_r.bmp
# End Source File
# Begin Source File

SOURCE=.\basa_s.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\boss1.bmp
# End Source File
# Begin Source File

SOURCE=.\bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\chip_f.bmp
# End Source File
# Begin Source File

SOURCE=.\chip_m.bmp
# End Source File
# Begin Source File

SOURCE=.\cursor.bmp
# End Source File
# Begin Source File

SOURCE=.\earth.bmp
# End Source File
# Begin Source File

SOURCE=.\enemi_l.bmp
# End Source File
# Begin Source File

SOURCE=.\enemi_m.bmp
# End Source File
# Begin Source File

SOURCE=.\enemi_s.bmp
# End Source File
# Begin Source File

SOURCE=.\expl.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\katayri.bmp
# End Source File
# Begin Source File

SOURCE=.\klic.WAV
# End Source File
# Begin Source File

SOURCE=.\logo_all.bmp
# End Source File
# Begin Source File

SOURCE=.\MAINICON.ico
# End Source File
# Begin Source File

SOURCE=.\pirat_m.bmp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\ship_g.bmp
# End Source File
# Begin Source File

SOURCE=.\ship_h.bmp
# End Source File
# Begin Source File

SOURCE=.\ship_k.bmp
# End Source File
# Begin Source File

SOURCE=.\ship_p.bmp
# End Source File
# Begin Source File

SOURCE=.\star000.bmp
# End Source File
# Begin Source File

SOURCE=.\star001.bmp
# End Source File
# Begin Source File

SOURCE=.\star002.bmp
# End Source File
# Begin Source File

SOURCE=.\star003.bmp
# End Source File
# Begin Source File

SOURCE=.\title.bmp
# End Source File
# End Group
# Begin Group "other"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cwave.CPP
# End Source File
# Begin Source File

SOURCE=.\cwave.H
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\draw.cpp
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\input.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\music.cpp
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\sound.cpp
# End Source File
# End Target
# End Project
