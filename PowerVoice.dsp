# Microsoft Developer Studio Project File - Name="PowerVoice" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PowerVoice - Win32 Debug Dongjing English
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PowerVoice.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PowerVoice.mak" CFG="PowerVoice - Win32 Debug Dongjing English"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PowerVoice - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "PowerVoice - Win32 Debug Dongjing" (based on "Win32 (x86) Application")
!MESSAGE "PowerVoice - Win32 Debug Eglish" (based on "Win32 (x86) Application")
!MESSAGE "PowerVoice - Win32 Debug Dongjing English" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PowerVoice - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib libdtimt.lib LIBDXXMT.lib sctools.lib libsrlmt.lib libgc.lib libfaxmt.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc" /out:"PowerVoice.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PowerVoice - Win32 Debug Dongjing"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PowerVoice___Win32_Debug_Dongjing"
# PROP BASE Intermediate_Dir "PowerVoice___Win32_Debug_Dongjing"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PowerVoice___Win32_Debug_Dongjing"
# PROP Intermediate_Dir "PowerVoice___Win32_Debug_Dongjing"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "DONGJIN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib libdtimt.lib LIBDXXMT.lib sctools.lib libsrlmt.lib libgc.lib libfaxmt.lib /nologo /subsystem:windows /debug /machine:I386 /out:"PowerVoice.exe" /pdbtype:sept
# ADD LINK32 version.lib DNDti.LIB DNDxxx.LIB DNSCTOOLS.LIB DNSRL.LIB Dngc.lib DNFax.LIB DNISDN.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"PowerVoice.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PowerVoice - Win32 Debug Eglish"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PowerVoice___Win32_Debug_Eglish"
# PROP BASE Intermediate_Dir "PowerVoice___Win32_Debug_Eglish"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PowerVoice___Win32_Debug_Eglish"
# PROP Intermediate_Dir "PowerVoice___Win32_Debug_Eglish"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib libdtimt.lib LIBDXXMT.lib sctools.lib libsrlmt.lib libgc.lib libfaxmt.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc" /out:"PowerVoice.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib libdtimt.lib LIBDXXMT.lib sctools.lib libsrlmt.lib libgc.lib libfaxmt.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc" /out:"PowerVoice.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PowerVoice - Win32 Debug Dongjing English"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PowerVoice___Win32_Debug_Dongjing_English"
# PROP BASE Intermediate_Dir "PowerVoice___Win32_Debug_Dongjing_English"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PowerVoice___Win32_Debug_Dongjing_English"
# PROP Intermediate_Dir "PowerVoice___Win32_Debug_Dongjing_English"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "DONGJIN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "DONGJIN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0x400 /D "DTISC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib DNDti.LIB DNDxxx.LIB DNSCTOOLS.LIB DNSRL.LIB Dngc.lib DNFax.LIB DNISDN.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"PowerVoice.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib DNDti.LIB DNDxxx.LIB DNSCTOOLS.LIB DNSRL.LIB Dngc.lib DNFax.LIB DNISDN.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"PowerVoice.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PowerVoice - Win32 Debug"
# Name "PowerVoice - Win32 Debug Dongjing"
# Name "PowerVoice - Win32 Debug Eglish"
# Name "PowerVoice - Win32 Debug Dongjing English"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_CommonFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\_DataQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\_Global.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowsePath.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\channelcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\DataRecorderConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTreeControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoLoadCallFlow.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoLoadResource.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGetPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSeleCallFlowTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSELERESOURCE.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgViewLicense.cpp
# End Source File
# Begin Source File

SOURCE=.\FaxManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FaxResource.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IVRChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\IVRClient_Admin.cpp
# End Source File
# Begin Source File

SOURCE=.\IVRClient_MSI.cpp
# End Source File
# Begin Source File

SOURCE=.\IVRGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManulCallCom.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MessageQ.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDBReader.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MySQLLog.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MySysInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MyThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MyThreadList.cpp
# End Source File
# Begin Source File

SOURCE=.\PageExternal.cpp
# End Source File
# Begin Source File

SOURCE=.\PageGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\PagePBX.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\PCSClientSock.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\PCSCommunicationEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\PCSExtClientSock.cpp
# End Source File
# Begin Source File

SOURCE=.\PowerVoice.cpp
# End Source File
# Begin Source File

SOURCE=.\PowerVoice.rc
# End Source File
# Begin Source File

SOURCE=.\PowerVoiceDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PowerVoiceView.cpp
# End Source File
# Begin Source File

SOURCE=.\ProductLicense.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonClass\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadProcessGCEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadSysMain.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonClass\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TTSPackage\TTSInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\XYTrace.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_CommonFunc.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\_DataQueue.h
# End Source File
# Begin Source File

SOURCE=.\_Global.h
# End Source File
# Begin Source File

SOURCE=.\_NodeFunc.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\_Protocol_IVRAdmin.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\_Protocol_IVRExt.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\_Protocol_IVRS.h
# End Source File
# Begin Source File

SOURCE=.\BrowsePath.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\channelcontrol.h
# End Source File
# Begin Source File

SOURCE=.\CheckGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\ClientThread.h
# End Source File
# Begin Source File

SOURCE=.\ConfigBox.h
# End Source File
# Begin Source File

SOURCE=.\ConfigSheet.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\DataRecorderConnect.h
# End Source File
# Begin Source File

SOURCE=.\DirTreeControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoLoadCallFlow.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoLoadResource.h
# End Source File
# Begin Source File

SOURCE=.\DlgGetPassword.h
# End Source File
# Begin Source File

SOURCE=.\DlgSeleCallFlowTable.h
# End Source File
# Begin Source File

SOURCE=.\DLGSELERESOURCE.h
# End Source File
# Begin Source File

SOURCE=.\DlgViewLicense.h
# End Source File
# Begin Source File

SOURCE=.\FaxManager.h
# End Source File
# Begin Source File

SOURCE=.\FaxResource.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IVRChannel.h
# End Source File
# Begin Source File

SOURCE=.\IVRClient_Admin.h
# End Source File
# Begin Source File

SOURCE=.\IVRClient_MSI.h
# End Source File
# Begin Source File

SOURCE=.\IVRGroup.h
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManulCallCom.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MessageQ.h
# End Source File
# Begin Source File

SOURCE=.\MyDBReader.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MySQLLog.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MySysInfo.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\MyThread.h
# End Source File
# Begin Source File

SOURCE=.\MyThreadList.h
# End Source File
# Begin Source File

SOURCE=.\PageExternal.h
# End Source File
# Begin Source File

SOURCE=.\PageGroup.h
# End Source File
# Begin Source File

SOURCE=.\PagePBX.h
# End Source File
# Begin Source File

SOURCE=.\PageSystem.h
# End Source File
# Begin Source File

SOURCE=.\PCSClientSock.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\PCSCommunicationEvent.h
# End Source File
# Begin Source File

SOURCE=.\PCSExtClientSock.h
# End Source File
# Begin Source File

SOURCE=.\PowerVoice.h
# End Source File
# Begin Source File

SOURCE=.\PowerVoiceDoc.h
# End Source File
# Begin Source File

SOURCE=.\PowerVoiceView.h
# End Source File
# Begin Source File

SOURCE=.\ProductLicense.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\ThreadProcessGCEvents.h
# End Source File
# Begin Source File

SOURCE=.\ThreadSysMain.h
# End Source File
# Begin Source File

SOURCE=..\CommonClass\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\TTSPackage\TTSInterface.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\XYTrace.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\075.ico
# End Source File
# Begin Source File

SOURCE=.\res\096.ico
# End Source File
# Begin Source File

SOURCE=.\res\Agent.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\busy_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\CARTOON2.ICO
# End Source File
# Begin Source File

SOURCE=".\res\DM Penfold's Pet.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Document.ico
# End Source File
# Begin Source File

SOURCE=.\res\error.ICO
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\PCSG_SRC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PowerVoice.ico
# End Source File
# Begin Source File

SOURCE=.\res\PowerVoice.rc2
# End Source File
# Begin Source File

SOURCE=.\res\PowerVoiceDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ready.ICO
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\start.ICO
# End Source File
# Begin Source File

SOURCE=.\res\stop.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Twetie.ico
# End Source File
# End Group
# Begin Group "LIB"

# PROP Default_Filter "LIB"
# Begin Source File

SOURCE=.\TTSPackage\InterPhonic\Lib\iFlyTTS.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\PowerVoice.reg
# End Source File
# Begin Source File

SOURCE=..\PVC_Version.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section PowerVoice : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section PowerVoice : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section PowerVoice : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:102
# 	2:21:SplashScreenInsertKey:4.0
# End Section
