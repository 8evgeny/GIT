; Скрипт создан при помощи мастера создания скриптов.
; СМ. ДОКУМЕНТАЦИЮ ДЛЯ ИЗУЧЕНИЯ ДЕТАЛЕЙ ОТНОСИТЕЛЬНО СОЗДАНИЯ ФАЙЛОВ СКРИПТА INNO SETUP!

[Setup]
AppName=Configurator
AppVerName=Configurator_v1
AppPublisher=GIT
AppPublisherURL=https://git-holding.ru
AppSupportURL=https://git-holding.ru
AppUpdatesURL=https://git-holding.ru
DefaultDirName={pf}\Configurator
DefaultGroupName=Configurator
AllowNoIcons=yes
OutputDir=C:\Users\Professional\Desktop\2_install
OutputBaseFilename=Configurator_setup
SetupIconFile=C:\Users\Professional\Desktop\configurator.ico
Password=42
Compression=lzma
SolidCompression=yes

[Languages]
Name: "russian"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\Professional\Desktop\2_configurator\configurator.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Professional\Desktop\2_configurator\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; ОТМЕТЬТЕ: Не используйте "Флажки: Проигнорировать версию" на любых общедоступных системных файлах

[Icons]
Name: "{group}\Configurator"; Filename: "{app}\configurator.exe"
Name: "{group}\{cm:ProgramOnTheWeb,Configurator}"; Filename: "https://git-holding.ru"
Name: "{group}\{cm:UninstallProgram,Configurator}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Configurator"; Filename: "{app}\configurator.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Configurator"; Filename: "{app}\configurator.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\configurator.exe"; Description: "{cm:LaunchProgram,Configurator}"; Flags: nowait postinstall skipifsilent

