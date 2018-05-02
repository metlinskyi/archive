program Project1;

uses
  Forms,
  Unit1 in 'Unit1.pas' {FrmMain};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'PKGUnPacker';
  Application.CreateForm(TFrmMain, FrmMain);
  Application.Run;
end.
