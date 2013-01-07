; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "LingosHook"
!define PRODUCT_VERSION "1.6.700"
!define PRODUCT_PUBLISHER "Jie"
!define PRODUCT_WEB_SITE "http://lingoshook.googlecode.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"
!include "x64.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON ".\res\lh_logo32.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\LingosHookApp.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\Output\LingosHook-Setup.exe"
InstallDir "$PROGRAMFILES\Jie\LingosHook"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function InstallX64
  Push $R0
  ClearErrors
  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{8220EEFE-38CD-377E-8595-13398D740ACE}" "Version"
  IfErrors 0 VSRedistInstalled
  MessageBox MB_ICONQUESTION|MB_YESNO "LingosHook need MS VC++ 2008 Redistributable, do you want to install it ?" IDNO VSRedistInstalled
  File "..\Setup-Files\vcredist_x64.exe"
  ExecWait "$INSTDIR\vcredist_x64.exe"
VSRedistInstalled:
   Exch $R0
FunctionEnd

Function InstallX86
  Push $R0
  ClearErrors
  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{9A25302D-30C0-39D9-BD6F-21E6EC160475}" "Version"
  IfErrors 0 VSRedistInstalled
  MessageBox MB_ICONQUESTION|MB_YESNO "LingosHook need MS VC++ 2008 Redistributable, do you want to install it ?" IDNO VSRedistInstalled
  File "..\Setup-Files\vcredist_x86.exe"
  ExecWait "$INSTDIR\vcredist_x86.exe"
VSRedistInstalled:
   Exch $R0
FunctionEnd

Function CheckVCRedist
  ${If} ${RunningX64}
    Call InstallX64
  ${Else}
    Call InstallX86
  ${EndIf}
FunctionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR\lang\zh_cn"
  File ".\lang\zh_cn\lang.mo"
  File ".\lang\zh_cn\lang.po"
  SetOutPath "$INSTDIR\plugins"
  File "..\Output\plugins\P_WordExport.dll"
  File "..\Output\plugins\P_SetHomepage.dll"
  File "..\Output\plugins\P_DataCleanup.dll"
  SetOutPath "$INSTDIR\res"
  File ".\res\check.ico"
  File ".\res\checked.ico"
  File ".\res\lh_hook.ico"
  File ".\res\lh_logo16.ico"
  File ".\res\lh_logo32.ico"
  File ".\res\lh_logo_main.ico"
  File ".\res\lh_normal.ico"
  File ".\res\unchecked.ico"
  SetOutPath "$INSTDIR"
  File "..\Output\Release\tidy.dll"
  File "..\Setup-Files\sqlite3.dll"
  File "..\Output\Release\LingosHookApp.exe"
  File "..\Output\Release\LingosHook.dll"
  File "..\Output\Release\LHSpeak.dll"
  
  Call CheckVCRedist
  
  CreateDirectory "$SMPROGRAMS\LingosHook"
  CreateShortCut "$SMPROGRAMS\LingosHook\LingosHook.lnk" "$INSTDIR\LingosHookApp.exe"

SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\LingosHook\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\LingosHookApp.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\LingosHookApp.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\res\unchecked.ico"
  Delete "$INSTDIR\res\lh_normal.ico"
  Delete "$INSTDIR\res\lh_logo_main.ico"
  Delete "$INSTDIR\res\lh_logo32.ico"
  Delete "$INSTDIR\res\lh_logo16.ico"
  Delete "$INSTDIR\res\lh_hook.ico"
  Delete "$INSTDIR\res\checked.ico"
  Delete "$INSTDIR\res\check.ico"
  Delete "$INSTDIR\plugins\P_WordExport.dll"
  Delete "$INSTDIR\plugins\P_SetHomepage.dll"
  Delete "$INSTDIR\plugins\P_DataCleanup.dll"
  Delete "$INSTDIR\lang\zh_cn\lang.po"
  Delete "$INSTDIR\lang\zh_cn\lang.mo"
  Delete "$INSTDIR\vcredist_x86.exe"
  Delete "$INSTDIR\vcredist_x64.exe"
  Delete "$INSTDIR\LHSpeak.dll"
  Delete "$INSTDIR\LingosHook.dll"
  Delete "$INSTDIR\LingosHookApp.exe"
  Delete "$INSTDIR\sqlite3.dll"
  Delete "$INSTDIR\tidy.dll"

  Delete "$SMPROGRAMS\LingosHook\Uninstall.lnk"
  Delete "$SMPROGRAMS\LingosHook\Website.lnk"
  Delete "$SMPROGRAMS\LingosHook\LingosHook.lnk"

  RMDir "$INSTDIR\lang\zh_cn"
  RMDir "$INSTDIR\lang"
  RMDir "$INSTDIR\res"
  RMDir "$INSTDIR\plugins"
  RMDir "$SMPROGRAMS\LingosHook"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd