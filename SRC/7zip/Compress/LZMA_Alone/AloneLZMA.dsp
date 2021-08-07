# Microsoft Developer Studio Project File - Name="AloneLZMA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AloneLZMA - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AloneLZMA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AloneLZMA.mak" CFG="AloneLZMA - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AloneLZMA - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AloneLZMA - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AloneLZMA - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\\" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib shlwapi.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\..\NRLPack.exe" /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AloneLZMA - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_CONSOLE" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib shlwapi.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\..\NRLPack.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AloneLZMA - Win32 Release"
# Name "AloneLZMA - Win32 Debug"
# Begin Group "Compress"

# PROP Default_Filter ""
# Begin Group "LZMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\LZMA\LZMA.h
# End Source File
# Begin Source File

SOURCE=..\LZMA\LZMAEncoder.cpp
# End Source File
# Begin Source File

SOURCE=..\LZMA\LZMAEncoder.h
# End Source File
# End Group
# Begin Group "RangeCoder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\RangeCoder\RangeCoder.h
# End Source File
# Begin Source File

SOURCE=..\RangeCoder\RangeCoderBit.cpp
# End Source File
# Begin Source File

SOURCE=..\RangeCoder\RangeCoderBit.h
# End Source File
# Begin Source File

SOURCE=..\RangeCoder\RangeCoderBitTree.h
# End Source File
# Begin Source File

SOURCE=..\RangeCoder\RangeCoderOpt.h
# End Source File
# End Group
# Begin Group "LZ"

# PROP Default_Filter ""
# Begin Group "Pat"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\LZ\Patricia\Pat.h
# End Source File
# Begin Source File

SOURCE=..\LZ\Patricia\Pat4H.h
# End Source File
# Begin Source File

SOURCE=..\LZ\Patricia\PatMain.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\LZ\IMatchFinder.h
# End Source File
# Begin Source File

SOURCE=..\LZ\LZInWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\LZ\LZInWindow.h
# End Source File
# End Group
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Windows\Defs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Windows\FileIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Windows\FileIO.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Common\Alloc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\CommandLineParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\CommandLineParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Defs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\MyWindows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\NewHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\NewHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\String.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StringConvert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StringConvert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StringToInt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StringToInt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Vector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Vector.h
# End Source File
# End Group
# Begin Group "7zip Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Common\FileStreams.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\FileStreams.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\InBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\InBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\OutBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\OutBuffer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\depacker.h
# End Source File
# Begin Source File

SOURCE=.\ELF.cpp
# End Source File
# Begin Source File

SOURCE=.\ELF.h
# End Source File
# Begin Source File

SOURCE=..\..\ICoder.h
# End Source File
# Begin Source File

SOURCE=.\LzmaAlone.cpp
# End Source File
# End Target
# End Project
