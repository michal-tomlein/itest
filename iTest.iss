[Setup]
AppName=iTest
AppVerName=iTest 1.4.2
AppVersion=1.4.2
AppId=iTest-1.0
AppPublisher=Michal Tomlein
AppPublisherURL=http://itest.sourceforge.net
AppSupportURL=http://itest.sourceforge.net
AppUpdatesURL=http://itest.sourceforge.net
DefaultDirName={pf}\iTest
DefaultGroupName=iTest
LicenseFile=LICENCE
OutputDir=.
OutputBaseFilename=iTest-1.4.2-PREVIEW7-win32
SetupIconFile=iTestServer\images\itest.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations=yes

[Registry]
Root: HKCR; Subkey: ".itdb"; ValueType: string; ValueName: ""; ValueData: "iTest.itdb"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "iTest.itdb"; ValueType: string; ValueName: ""; ValueData: "iTest Database"; Flags: uninsdeletekey
Root: HKCR; Subkey: "iTest.itdb\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\itdb.ico"
Root: HKCR; Subkey: "iTest.itdb\Shell\Open\Command"; ValueType: string; ValueName: ""; ValueData: """{app}\iTestServer.exe"" ""%1"""
Root: HKCR; Subkey: ".itos"; ValueType: string; ValueName: ""; ValueData: "iTest.itos"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "iTest.itos"; ValueType: string; ValueName: ""; ValueData: "iTest Off-line Test Session"; Flags: uninsdeletekey
Root: HKCR; Subkey: "iTest.itos\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\itos.ico"
Root: HKCR; Subkey: "iTest.itos\Shell\Open\Command"; ValueType: string; ValueName: ""; ValueData: """{app}\iTestClient.exe"" ""%1"""
Root: HKCR; Subkey: ".itcl"; ValueType: string; ValueName: ""; ValueData: "iTest.itcl"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "iTest.itcl"; ValueType: string; ValueName: ""; ValueData: "iTest Off-line Client Log"; Flags: uninsdeletekey
Root: HKCR; Subkey: "iTest.itcl\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\itcl.ico"

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "slovak"; MessagesFile: "compiler:Languages\Slovak.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "iTestServer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "iTestClient.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "itdb.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "itos.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "itcl.ico"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\iTestServer"; Filename: "{app}\iTestServer.exe"
Name: "{group}\iTestClient"; Filename: "{app}\iTestClient.exe"
Name: "{group}\{cm:ProgramOnTheWeb,iTest}"; Filename: "http://itest.sourceforge.net"
Name: "{group}\{cm:UninstallProgram,iTest}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\iTestServer"; Filename: "{app}\iTestServer.exe"; Tasks: desktopicon
Name: "{commondesktop}\iTestClient"; Filename: "{app}\iTestClient.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\iTestServer"; Filename: "{app}\iTestServer.exe"; Tasks: quicklaunchicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\iTestClient"; Filename: "{app}\iTestClient.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\iTestServer.exe"; Description: "{cm:LaunchProgram,iTest}"; Flags: nowait postinstall skipifsilent
