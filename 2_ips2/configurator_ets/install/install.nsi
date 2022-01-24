;NSIS Modern User Interface
;Written by S.Panin
;version 3.3
;--------------------------------
!pragma warning error all

;Defines
  !define VERSION_MAJOR 2
  !define VERSION_MINOR 0
  !define VERSION_PATCH 16
  !define VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
  !define PROJECT "IPS"
  !define FULL_PROJECT "GIT-Comm ${PROJECT}"
  !define NAME "${PROJECT} Configurator"
  !define FILE_NAME "ips-configurator"
  !define FILE_INSTALL "${FILE_NAME}.exe"
  !define FILE_UNINSTALL "${FILE_NAME}-uninstall.exe"
  !define OUTPUTFILE "${FILE_NAME}-${VERSION}-win.exe"
  !define REG_ROOT "git-holding.ru\IPS\Configurator"
  !define FILES_PATH "..\\bin\\"
  !define UCRTBASE_PATH "c:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64\ucrt\"
  !define VC2019_PATH "c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Redist\MSVC\14.27.29016\x64\Microsoft.VC142.CRT\"

;--------------------------------
;Include Modern UI
  !include "MUI2.nsh"
;--------------------------------
;General
  Name "${NAME}"
  Caption "${NAME} ${VERSION}"
  OutFile "${OUTPUTFILE}"
  Unicode True

  InstallDir "$PROGRAMFILES64\${NAME}"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "${REG_ROOT}" ""

  RequestExecutionLevel admin
  SetCompressor /SOLID lzma														
  SetCompressorDictSize 32
;--------------------------------
;Interface Configuration
  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "header_ips.bmp"
  !define MUI_ABORTWARNING
  !define MUI_ICON "mui_ips.ico"
  !define MUI_UNICON "unmui_ips.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "wizard_ips.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "finish_ips.bmp"
;--------------------------------
;Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES 
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------
;Languages
  !insertmacro MUI_LANGUAGE "Russian" 
  !insertmacro MUI_LANGUAGE "English"

; The stuff to install
Section "Install"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR"
  
   File "${FILES_PATH}${FILE_INSTALL}"
   File /r "${FILES_PATH}*.dll"
   File "LICENSE.txt"

   SetOutPath "$INSTDIR\translations"
   File "${FILES_PATH}translations\*.qm"

   SetOutPath "$INSTDIR\sounds"
   File "${FILES_PATH}\sounds\*.*"

   SetOutPath "$INSTDIR\langs"
   File "${FILES_PATH}langs\*.qm"

   SetOutPath "$INSTDIR"
   File "${UCRTBASE_PATH}\*.dll"

   SetOutPath "$INSTDIR"
   File "${VC2019_PATH}\*.dll"
   
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\\${FILE_UNINSTALL}" 
   
  ; Store installation folder
  WriteRegStr HKCU "${REG_ROOT}" "" $INSTDIR
SectionEnd 


; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\${FULL_PROJECT}"
  CreateShortcut "$SMPROGRAMS\${FULL_PROJECT}\${NAME} ${VERSION} Uninstall.lnk" "$INSTDIR\${FILE_UNINSTALL}" "" "$INSTDIR\${FILE_UNINSTALL}" 0
  CreateShortcut "$SMPROGRAMS\${FULL_PROJECT}\${NAME} ${VERSION}.lnk" "$INSTDIR\${FILE_INSTALL}" "" "$INSTDIR\${FILE_INSTALL}" 0
  
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FILE_NAME}" "DisplayName" "${FILE_NAME}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FILE_NAME}" "UninstallString" "$INSTDIR\${FILE_UNINSTALL}"
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FILE_NAME}" "NoModify" 1
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FILE_NAME}" "NoRepair" 1


SectionEnd

!macro IsRunning 
    FindWindow $0 "" "IPS-configurator"
    StrCmp $0 0 notRunning
        MessageBox MB_OK|MB_ICONEXCLAMATION "IPS-configurator is running or open folder. Please close it first" /SD IDOK
        Abort
    notRunning:
!macroEnd

;--------------------------------
; Uninstaller
UninstallCaption "${NAME} ${VERSION} Uninstall"

Section "Uninstall"
  
  !insertmacro IsRunning

  ; Remove registry keys
  DeleteRegKey HKLM "${REG_ROOT}"

  ; Remove files and uninstaller
  Delete $INSTDIR\${FILE_UNINSTALL}

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${FULL_PROJECT}\${NAME} ${VERSION}.lnk"
  Delete "$SMPROGRAMS\${FULL_PROJECT}\${NAME} ${VERSION} Uninstall.lnk"
  RMDir "$SMPROGRAMS\${FULL_PROJECT}"

  RMDir /r "$INSTDIR"
  RMDir "$INSTDIR\..\."

DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${FILE_NAME}"

SectionEnd
