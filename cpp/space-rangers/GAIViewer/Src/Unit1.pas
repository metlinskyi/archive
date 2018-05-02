unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, ComCtrls, ImgList;

type
  TfrmMain = class(TForm)
    dlgSave: TSaveDialog;
    GroupBox1: TGroupBox;
    prgFrame: TProgressBar;
    lblFrame: TLabel;
    GroupBox3: TGroupBox;
    btnLoad: TButton;
    btnPlay: TButton;
    btnStop: TButton;
    btnExtract: TButton;
    edtFile: TEdit;
    lblFile: TLabel;
    btnBrowse: TButton;
    dlgOpen: TOpenDialog;
    grpBack: TGroupBox;
    rdbBlack: TRadioButton;
    rdbBlue: TRadioButton;
    rdbGreen: TRadioButton;
    rdbRed: TRadioButton;
    rdbWhite: TRadioButton;
    Panel1: TPanel;
    grpBlock: TGroupBox;
    imgPreview: TImage;
    grpBlock2: TGroupBox;
    imgPreview2: TImage;
    Splitter1: TSplitter;
    StatusBar1: TStatusBar;
    chkDebug: TCheckBox;
    procedure btnLoadClick(Sender: TObject);
    procedure btnPlayClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnStopClick(Sender: TObject);
    procedure btnExtractClick(Sender: TObject);
    procedure btnBrowseClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

// Алгоритм расшифровки был взят отсюда, и реализован в UnPackGIData
{
function OKGR_TransBuf_DrawClip_WORD(
    bufDraw: PChar;
    width  : Cardinal;
    startX : Cardinal;
    startY : Cardinal;
    bufGI  : PChar;
    rect   : PRect
    ):Cardinal;
    cdecl; external 'okgf.dll' name 'OKGR_TransBuf_DrawClip_WORD';
}

var
    dFile: file of byte;
    pFile: file of byte;
    
const
    maxBlock = 10;
    maxGI = 1000;

type
    TXRecord = record
        offset: Cardinal;
        size: Cardinal;
        startX, startY: Cardinal;
        finishX, finishY: Cardinal;
    end;
    TGIRecord = record
        countBlock: Integer;
        masBlock: array [0..maxBlock-1] of TXRecord; 
        posX, posY, sizeX, sizeY: Cardinal;
        giType, count, width, height: Cardinal;
        wTime: Cardinal;
        Image, Image2, Image3: TBitMap;
    end;

var
    maxG, maxI: Cardinal;
    
    Stop:boolean;
    countGI, unknowGI, unLoad: Integer;
    masGI: array [0..maxGI-1] of TGIRecord;

procedure DebugUnLoad(bufGI: PChar; fSize: Cardinal);
var
    wr: Cardinal;
begin
    AssignFile(dFile, frmMain.edtFile.Text + '_' + IntToHex(unLoad,4) + '.gi');
    ReWrite(dFile);
    BlockWrite(dFile, bufGI^, fSize, wr);
    CloseFile(dFile);
    unLoad:=unLoad+1;
end;

procedure ClearRect(sizeX, sizeY:Cardinal; var rect: TRect);
begin
    rect.Top:=0; rect.Left:=0;
    rect.Right:=sizeX; rect.Bottom:=sizeY;
    rect.TopLeft.X:=0; rect.TopLeft.Y:=0;
    rect.BottomRight.X:=sizeX; rect.BottomRight.Y:=sizeY;
end;

function GetColorMask():word;
begin
    Result:=0;
    with frmMain do
    begin
        if rdbBlack.Checked then Result:=$0000;
        if rdbBlue.Checked then Result:=$001F;
        if rdbGreen.Checked then Result:=$07E0;
        if rdbRed.Checked then Result:=$F800;
        if rdbWhite.Checked then Result:=$FFFF;
    end;
end;

procedure FillBufDraw(buf:PChar; sizeG:Cardinal; color:word);
var
    i:Cardinal;
begin
    i:=0;
    while (i<sizeG) do
    begin
        buf[i+0]:=Chr(Lo(color));
        buf[i+1]:=Chr(Hi(color));
        i:=i+2;
    end;
end;

procedure FillBitMap(buf: PChar; sizeX, sizeY:Cardinal; var img:TBitmap);
var
    P: PByteArray;
    x, y, i: Cardinal;
begin
    i:=0;
    for y:=0 to sizeY-1 do
    begin
        P:=img.ScanLine[y];
        for x:=0 to sizeX-1 do
        begin
            P[x*2+0]:=Byte(buf[i+0]);
            P[x*2+1]:=Byte(buf[i+1]);
            i:=i+2;
        end;
    end;
end;

function GetLongBuf(buf:PChar; offs: Cardinal):Cardinal;
var
    x:Cardinal;
begin
    x:=0;
    x:=x + Byte(buf[offs+0]);
    x:=x + Byte(buf[offs+1])*256;
    x:=x + Byte(buf[offs+2])*256*256;
    x:=x + Byte(buf[offs+3])*256*256*256;
    Result:=x
end;

procedure UnPackGIData(offset: Cardinal; bufGI, bufDraw: PChar; var lnx, lny: Cardinal);
var
    c: byte;
    kGI, kDraw, j, i, cnt: Cardinal;
begin
    cnt:=GetLongBuf(bufGI, offset+0);
    lnx:=GetLongBuf(bufGI, offset+4);
    lny:=GetLongBuf(bufGI, offset+8);
    kGI:=offset+$10; kDraw:=0; i:=0;
    while (i<cnt) do
    begin
        //----------------------------------
        if kGI>=maxI then
        begin
{
            ShowMessage('maxI = ' + IntToHex(maxI,8) +
                        ', kGI = ' + IntToHex(kGI,8) +
                        ', i = ' + IntToHex(i,8) +
                        ', cnt = ' + IntToHex(cnt,8)
                        );
}
        end;
        //----------------------------------
        c:=Byte(bufGI[kGI]); kGI:=kGI+1;
        if c=0 then
        begin
            i:=i+1;
        end
        else if c=$80 then
        begin
            kDraw:=kDraw + lnx*2;

            if kDraw>=maxG then ShowMessage('maxG = ' + IntToHex(maxG,8) + ', kDraw = ' + IntToHex(kDraw,8));

            i:=i+1;
        end else if (c and $80)<>0 then
        begin
            c:=c and $7F;
            for j:=1 to c do
            begin
                bufDraw[kDraw]:=bufGI[kGI];
                bufDraw[kDraw+1]:=bufGI[kGI+1];
                kDraw:=kDraw+2; kGI:=kGI+2; i:=i+2;

                if kDraw>=maxG then ShowMessage('maxG = ' + IntToHex(maxG,8) + ', kDraw = ' + IntToHex(kDraw,8));
            end;
        end else
        begin
            kDraw:=kDraw+c*2;

            if kDraw>=maxG then ShowMessage('maxG = ' + IntToHex(maxG,8) + ', kDraw = ' + IntToHex(kDraw,8));

            i:=i+1;
        end;
    end;

//    ShowMessage('kDraw = ' + IntToHex(kDraw,8));
end;

procedure TransHeadBlock(bufGI:PChar);
var
    ofs, cnt, i:Cardinal;
begin
    cnt:=0; ofs:=0;
    for i:=1 to 3 do
    begin
        case i of
            1: ofs:=$80;
            2: ofs:=$60;
            3: ofs:=$40;
        end;
        if GetLongBuf(bufGI, ofs)<>0 then
        begin
            with masGI[countGI].masBlock[cnt] do
            begin
                offset:=GetLongBuf(bufGI, ofs);
                size:=GetLongBuf(bufGI, ofs+4);
                startX:=GetLongBuf(bufGI, ofs+8);
                startY:=GetLongBuf(bufGI, ofs+$C);
                finishX:=GetLongBuf(bufGI, ofs+$10);
                finishY:=GetLongBuf(bufGI, ofs+$14);
                cnt:=cnt+1;
            end;
        end;
    end;
    masGI[countGI].countBlock:=cnt;
end;

procedure ProcessNormalGI(bufGI:PChar; sizeG:Cardinal);
const
    offset = $60;
begin
    with masGI[countGI] do
    begin
        width:=GetLongBuf(bufGI, $50);
        height:=GetLongBuf(bufGI, $54);
        if Image=nil then Image:=TBitMap.Create;
        Image.PixelFormat:=pf16bit;
        Image.Height:=height;
        Image.Width:=width;
        FillBitMap(bufGI+offset, width, height, Image);
    end;
    countGI:=countGI+1;
end;

procedure ProcessShortGI(bufGI:PChar; sizeG:Cardinal);
const
    offset = $70;
var
    bufDraw: PChar;
begin
    with masGI[countGI] do
    begin
        GetMem(bufDraw, sizeG);
        try
            FillBufDraw(bufDraw, sizeG, GetColorMask());
            UnPackGIData(offset-$10, bufGI, bufDraw, width, height);
            if Image=nil then Image:=TBitMap.Create;
            Image.PixelFormat:=pf16bit;
            Image.Height:=height;
            Image.Width:=width;
            FillBitMap(bufDraw, width, height, Image);
        finally
            FreeMem(bufDraw)
        end;
    end;
    countGI:=countGI+1;
end;

procedure ProcessLongGI(bufGI:PChar; sizeG:Cardinal);
var
    bufDraw: PChar;
    offset: Cardinal;
begin
   GetMem(bufDraw, sizeG);
   try
        with masGI[countGI] do
        begin
            TransHeadBlock(bufGI);
            //----------------------------------------
            if countBlock>=3 then
            begin
                offset:=masBlock[2].offset;
                FillBufDraw(bufDraw, sizeG, GetColorMask());
                UnPackGIData(offset, bufGI, bufDraw, width, height);
                //-----------------------------------------
                if Image=nil then Image:=TBitMap.Create;
                Image.PixelFormat:=pf16bit;
                Image.Height:=height;
                Image.Width:=width;
                FillBitMap(bufDraw, width, height, Image);
            end;
            //----------------------------------------
            if countBlock>=2 then
            begin
                offset:=masBlock[1].offset;
                FillBufDraw(bufDraw, sizeG, GetColorMask());
                UnPackGIData(offset, bufGI, bufDraw, width, height);
                //-----------------------------------------
                if Image2=nil then Image2:=TBitMap.Create;
                Image2.PixelFormat:=pf16bit;
                Image2.Height:=height;
                Image2.Width:=width;
                FillBitMap(bufDraw, width, height, Image2);
            end;
            //----------------------------------------
            countGI:=countGI+1;
        end;
    finally
        FreeMem(bufDraw)
    end;
end;

procedure ProcessOneGIData(fPos, fSize: Cardinal);
var
    rd, sizeG: Cardinal;
    bufGI: PChar;
begin
    Seek(pFile, fPos);
    GetMem(bufGI, fSize*100);  //запас :) без него глючит 
    try
        BlockRead(pFile, bufGI^, fSize, rd);
        //---------------------------------------
        if frmMain.chkDebug.Checked then DebugUnLoad(bufGI, fSize);    //разлагает GAI на отдельные GI-шки и кидает их на винт
        //---------------------------------------
        with masGI[countGI] do
        begin
            posX:=GetLongBuf(bufGI, 4);
            posY:=GetLongBuf(bufGI, 8);
            sizeX:=GetLongBuf(bufGI, $10);
            sizeY:=GetLongBuf(bufGI, $14);
            giType:=GetLongBuf(bufGI, $28);
            sizeG:=fSize*256; //!!! с запасом :) т.к. неизвестно размер распакованой GI

            maxG:=sizeG; maxI:=fSize; //Debug

            case giType of
                0: ProcessNormalGI(bufGI, sizeG);
                1: ProcessShortGI(bufGI, sizeG);
                2: ProcessLongGI(bufGI, sizeG);
            else
                //ShowMessage('Unknow giType = ' + IntToStr(giType));
                unknowGI:=unknowGI+1;
            end;
        end;
    finally
        FreeMem(bufGI);
    end;
end;

procedure DrawGI(dat:TGIRecord);
begin
    frmMain.imgPreview.Picture:=nil;
    frmMain.imgPreview2.Picture:=nil;
    if dat.Image<>nil then frmMain.imgPreview.Picture.Bitmap:=dat.Image;
    if dat.Image2<>nil then frmMain.imgPreview2.Picture.Bitmap:=dat.Image2;
    Application.ProcessMessages;
end;

procedure ClearImage();
var
    i,j:Integer;
begin
    unLoad:=0;
    countGI:=0;
    for i:=0 to maxGI-1 do
    begin
        with masGi[i] do
        begin
            countBlock:=0;
            for j:=0 to maxBlock-1 do
            begin
                with masBlock[j] do
                begin
                    offset:=0; size:=0;
                    startX:=0; startY:=0;
                    finishX:=0; finishY:=0;
                end;
            end;
            posX:=0; posY:=0; sizeX:=0; sizeY:=0;
            giType:=$FF; count:=0;
            width:=0; height:=0;
            if Image<>nil then Image.Destroy;
            Image:=nil;
            if Image2<>nil then Image2.Destroy;
            Image2:=nil;
            if Image3<>nil then Image3.Destroy;
            Image3:=nil;
        end;
    end;
end;

procedure StartPlay();
var
    i, dx:Integer;
begin
    if countGI>0 then
    begin
        if countGI=1 then
            DrawGI(masGI[0])
        else begin
            frmMain.prgFrame.Max:=countGI;
            i:=0; Stop:=false; dx:=1;
            while not Stop do
            begin
                DrawGI(masGI[i]);
                frmMain.prgFrame.Position:=i;
                frmMain.lblFrame.Caption:='Frame: ' + IntToStr(i) + '/' + IntToStr(countGI-1);
                Sleep(masGI[i].wTime);
                i:=i+dx;
                if i>=countGI-1 then dx:=-1;
                if i<=0 then dx:=1;
            end;
        end;
    end;
end;

procedure ReadGIFile();
var
    fPos, fSize: Cardinal;
begin
    countGI:=0; unknowGI:=0;
    fPos:=0; fSize:=FileSize(pFile);
    ProcessOneGIData(fPos, fSize);
    if countGI>0 then DrawGI(masGI[0]);
end;

function ReadWaitTime(ofsTime, numGI:Cardinal ):Cardinal;
var
    slTime, rd: Cardinal;
begin
    Result:=0;
    if ofsTime<>0 then
    begin
        Seek(pFile, ofsTime+$16+numGI*8);
        BlockRead(pFile, slTime, 4, rd);
        Result:=slTime;
    end;
end;

procedure ReadGAIFile();
var
    head: array [0..3] of char;
    cntGi, giPos, giSize, ofsTime, rd, i: Cardinal;
begin
    countGI:=0; unknowGI:=0;
    Seek(pFile, $18); BlockRead(pFile, cntGi, 4, rd);
    Seek(pFile, $20); BlockRead(pFile, ofsTime, 4, rd);
    for i:=0 to cntGi-1 do
    begin
        Seek(pFile, $30+i*8); BlockRead(pFile, giPos, 4, rd);
        Seek(pFile, $34+i*8); BlockRead(pFile, giSize, 4, rd);
        //------------------------------------------------------
        Seek(pFile, giPos); BlockRead(pFile, head, 4, rd);
        if (head[0]='g') and (head[1]='i') then
        begin
            ProcessOneGIData(giPos, giSize);
            masGI[i].wTime:=ReadWaitTime(ofsTime, i);
        end
        else unknowGI:=unknowGI+1;
    end;
    if countGI>0 then DrawGI(masGI[0]);
end;

procedure ReadSupFile(FileName:string);
var
    rd: Cardinal;
    head: array [0..3] of char;
begin
    ClearImage();
    AssignFile(pFile, FileName); Reset(pFile);
    BlockRead(pFile, head, 4, rd);
    if (head[0]='g') and (head[1]='i') then
        ReadGIFile()
    else if (head[0]='g') and (head[1]='a') and (head[2]='i') then
        ReadGAIFile()
    else
        ShowMessage('Этот файл не является GI и GAI файлом');
    CloseFile(pFile);
end;

procedure ExtractGI(path:string);
var
    i:Integer;
begin
    for i:=0 to countGI-1 do
    begin
        if masGI[i].Image<>nil then masGI[i].Image.SaveToFile(path + '_1_' + IntToHex(i,4) + '.bmp');
        if masGI[i].Image2<>nil then masGI[i].Image2.SaveToFile(path + '_2_' + IntToHex(i,4) + '.bmp');
    end;
end;

procedure InitData();
begin
    Stop:=true;
end;

procedure TfrmMain.btnLoadClick(Sender: TObject);
var
    s:string;
begin
    btnExtract.Enabled:=false;
    btnPlay.Enabled:=false;
    btnLoad.Enabled:=false;
    ReadSupFile(edtFile.Text);
    if unknowGI>0 then s:=#13#10 + 'Среди них не поддерживаемых: ' + IntToStr(unknowGI);
    if countGI>0 then
    begin
        frmMain.lblFrame.Caption:='Frame: ' + IntToStr(0) + '/' + IntToStr(countGI-1);
        frmMain.prgFrame.Max:=countGI;
        frmMain.prgFrame.Position:=0;;
    end;
    btnPlay.Enabled:=true;
    btnLoad.Enabled:=true;
    btnExtract.Enabled:=true;
    ShowMessage('Загружено фреймов: ' + IntToStr(countGI) + s);
end;

procedure TfrmMain.btnPlayClick(Sender: TObject);
begin
    btnExtract.Enabled:=false;
    btnPlay.Enabled:=false;
    btnLoad.Enabled:=false;
    StartPlay();
    btnPlay.Enabled:=true;
    btnLoad.Enabled:=true;
    btnExtract.Enabled:=true;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
begin
    InitData();
end;

procedure TfrmMain.btnStopClick(Sender: TObject);
begin
    Stop:=true;
end;

procedure TfrmMain.btnExtractClick(Sender: TObject);
begin
    if dlgSave.Execute then
    begin
        ExtractGI(dlgSave.FileName);
        ShowMessage('Извлечение картинок завершено');
    end;
end;

procedure TfrmMain.btnBrowseClick(Sender: TObject);
begin
    if dlgOpen.Execute then edtFile.Text:=dlgOpen.FileName;
end;

end.

