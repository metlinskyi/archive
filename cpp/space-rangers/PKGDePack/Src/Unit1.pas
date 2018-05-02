unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls;

type
  TFrmMain = class(TForm)
    edtFile: TEdit;
    btnUnPack: TButton;
    Label1: TLabel;
    Label2: TLabel;
    edtDest: TEdit;
    memDebug: TMemo;
    prgFile: TProgressBar;
    procedure btnUnPackClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FrmMain: TFrmMain;

implementation

{$R *.dfm}

function OKGF_ZLib_UnCompress2(bufUnPack: PChar; maxSize: Cardinal; bufPack: PChar; sizePack:Cardinal):Cardinal;
    cdecl; external 'okgf.dll' name 'OKGF_ZLib_UnCompress2';

var
    pFile: file of byte;
    tFile: file of byte;

const
    lnRcName = 126;
    lnRcBase = $C;
    lnRcHead = $9E;
    maxBase = 1000;
    maxHead = 100000;

    TYP_UNPACK = 1;
    TYP_PACK = 2;
    TYP_GROUP = 3;

type
    TBaseRecord = record
        UnKnow1: cardinal;
        countRec: cardinal;
        sizeRec: cardinal;
        //--- System ---
        startHead: Integer;
    end;

    THeadRecord = record
        UnKnow1: cardinal;
        TrueSize: cardinal;
        FileName: string[lnRcName];
        TypeData: cardinal;
        UnKnow2: cardinal;
        ZeroCheck: cardinal;
        UnKnow3: cardinal;
        offsetFile: cardinal;
        nullTerm: cardinal;
        //--- System ---
        sizePack: cardinal;
    end;

var
    curBase: Integer;
    masBase: array [1..maxBase] of TBaseRecord;
    curHead: Integer;
    masHead: array [1..maxHead] of THeadRecord;

//-------------------------------------

procedure Debug(s:string);
begin
    with FrmMain.memDebug.Lines do
    begin
        if Count>=1000 then Delete(0);
        Add(s)
    end;
end;

procedure ReadOneBase();
begin
    curBase:=curBase + 1;
    with masBase[curBase] do
    begin
        BlockRead(pFile, UnKnow1, 4);
        BlockRead(pFile, countRec, 4);
        BlockRead(pFile, sizeRec, 4);
        startHead:=curHead;
    end;
end;

function ReadHeadStr(ln:Integer):string;
var
    s:string;
    buf: array [1..lnRcName] of char;
    i:Integer;
begin
    s:='';
    BlockRead(pFile, buf, ln);
    i:=1;
    while (i<=lnRcName) and (buf[i]<>#0) do
    begin
        s:=s + buf[i];
        i:=i + 1;
    end;
    Result:=s;
end;

procedure ReadOneHead();
begin
    curHead:=curHead + 1;
    with masHead[curHead] do
    begin
        BlockRead(pFile, UnKnow1, 4);
        BlockRead(pFile, TrueSize, 4);
        FileName:=ReadHeadStr(lnRcName);
        BlockRead(pFile, TypeData, 4);
        BlockRead(pFile, UnKnow2, 4);
        BlockRead(pFile, ZeroCheck, 4);
        BlockRead(pFile, UnKnow3, 4);
        BlockRead(pFile, offsetFile, 4);
        BlockRead(pFile, nullTerm, 4);
        sizePack:=0;
    end;
end;

procedure ProcessUnPack(fPos, size:Cardinal; path:string);
var
    buf:Pointer;
    rd:Integer;
begin
    GetMem(buf, size);
    try
        Seek(pFile, fPos+4);
        BlockRead(pFile, buf^, size, rd);
        AssignFile(tFile, path);
        ReWrite(tFile);
        BlockWrite(tFile, buf^, size);
        Closefile(tFile);

        Debug('FILE: ' + path);
    finally
        FreeMem(buf);
    end;
end;

procedure ProcessPack(fPos, uSize:Cardinal; path:string);
var
    rd:Integer;
    bufPack, bufUnPack: PChar;
    packSize, doneSize, unzSize: Cardinal;
begin
{
//  Выгрузка без распаковки ZL02
//  ---------------------------------
    Seek(pFile, fPos+4);
    BlockRead(pFile, pSize, 4, rd);
    GetMem(bufUnPack, uSize);
    try
        Seek(pFile, fPos+8);
        BlockRead(pFile, bufUnPack^, uSize, rd);
        AssignFile(tFile, path);
        ReWrite(tFile);
        BlockWrite(tFile, bufUnPack^, uSize, rd);
        CloseFile(tFile);
    finally
        FreeMem(bufUnPack);
    end;
}
    GetMem(bufUnPack, uSize);
    try
        FrmMain.prgFile.Max:=uSize;
        FrmMain.prgFile.Position:=0;
        //-------------------------
        doneSize:=0;
        Seek(pFile, fPos+4);
        while (doneSize<uSize) do
        begin
            BlockRead(pFile, packSize, 4, rd);
            GetMem(bufPack, packSize);
            try
                BlockRead(pFile, bufPack^, packSize, rd);
                //Внешняя функция из OKGF.DLL
                unzSize:=OKGF_ZLib_UnCompress2(bufUnPack + doneSize, 65536, bufPack, packSize);
                doneSize:=doneSize + unzSize;
            finally
                FreeMem(bufPack);
            end;
            FrmMain.prgFile.Position:=doneSize;
        end;
        FrmMain.prgFile.Position:=0;
        //---------------------------------
        AssignFile(tFile, path);
        ReWrite(tFile);
        BlockWrite(tFile, bufUnPack^, uSize, rd);
        CloseFile(tFile);
        //---------------------------------
        Debug('ZL02: ' + path);
    finally
        FreeMem(bufUnPack);
    end;
end;

procedure ReadHeaders(fPos:Cardinal; path:string);
var
    i: Integer;
    cnt, psz, typ, ofs, siz: Cardinal;
    nam:string;
begin
    Debug('GRP: ' + path);
    CreateDir(path);
    Seek(pFile, fPos);
    ReadOneBase();
    psz:=curHead + 1;
    masBase[curBase].startHead:=psz;
    cnt:=masBase[curBase].countRec;
    for i:=1 to cnt do ReadOneHead();
    //----------------------------------
    for i:=psz to psz+cnt-1 do
    begin
        typ:=masHead[i].TypeData;
        ofs:=masHead[i].offsetFile;
        siz:=masHead[i].TrueSize;
        nam:=path + '\' + masHead[i].FileName;
        if typ=TYP_GROUP then
            ReadHeaders(ofs, nam)
        else if typ=TYP_UNPACK then
            ProcessUnPack(ofs, siz, nam)
        else if typ=TYP_PACK then
            ProcessPack(ofs, siz, nam);
        //---------------------------------
        Application.ProcessMessages;
    end;
end;

procedure StartRead(path, dest:string);
var
    xPos: Cardinal;
begin
    curHead:=0; curBase:=0;
    AssignFile(pFile, path);
    Reset(pFile);
    BlockRead(pFile, xPos, 4);
    ReadHeaders(xPos, dest);
    CloseFile(pFile);
end;

procedure TFrmMain.btnUnPackClick(Sender: TObject);
begin
    memDebug.Clear;
    btnUnPack.Enabled:=false;
    StartRead(edtFile.Text, edtDest.Text);
    btnUnPack.Enabled:=true;
    ShowMessage('UnPack Done');
end;

end.
