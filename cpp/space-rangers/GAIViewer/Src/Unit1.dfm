object frmMain: TfrmMain
  Left = 209
  Top = 111
  Width = 651
  Height = 400
  Caption = 'GAI & GI Viewer '#1076#1083#1103' '#1080#1075#1088#1099' "'#1050#1086#1089#1084#1080#1095#1077#1089#1082#1080#1077' '#1088#1077#1081#1085#1076#1078#1077#1088#1099'"  v2.0 alpha'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 555
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  DesignSize = (
    643
    373)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 105
    Top = 5
    Width = 535
    Height = 71
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    DesignSize = (
      535
      71)
    object lblFrame: TLabel
      Left = 10
      Top = 50
      Width = 35
      Height = 13
      Caption = 'Frame: '
    end
    object lblFile: TLabel
      Left = 10
      Top = 20
      Width = 57
      Height = 13
      Caption = 'GAI, GI File:'
    end
    object prgFrame: TProgressBar
      Left = 85
      Top = 45
      Width = 440
      Height = 16
      Anchors = [akLeft, akTop, akRight]
      Min = 0
      Max = 100
      TabOrder = 0
    end
    object edtFile: TEdit
      Left = 85
      Top = 15
      Width = 414
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
    end
    object btnBrowse: TButton
      Left = 503
      Top = 15
      Width = 21
      Height = 21
      Anchors = [akTop, akRight]
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = btnBrowseClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 5
    Top = 5
    Width = 91
    Height = 141
    TabOrder = 1
    object btnLoad: TButton
      Left = 10
      Top = 15
      Width = 75
      Height = 25
      Caption = 'Load'
      TabOrder = 0
      OnClick = btnLoadClick
    end
    object btnPlay: TButton
      Left = 10
      Top = 45
      Width = 75
      Height = 25
      Caption = 'Play'
      TabOrder = 1
      OnClick = btnPlayClick
    end
    object btnStop: TButton
      Left = 10
      Top = 75
      Width = 75
      Height = 25
      Caption = 'Stop'
      TabOrder = 2
      OnClick = btnStopClick
    end
    object btnExtract: TButton
      Left = 10
      Top = 105
      Width = 75
      Height = 25
      Caption = 'Extract'
      TabOrder = 3
      OnClick = btnExtractClick
    end
  end
  object grpBack: TGroupBox
    Left = 5
    Top = 155
    Width = 91
    Height = 141
    TabOrder = 2
    object rdbBlack: TRadioButton
      Left = 10
      Top = 15
      Width = 56
      Height = 17
      Caption = 'Black'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object rdbBlue: TRadioButton
      Left = 10
      Top = 40
      Width = 56
      Height = 17
      Caption = 'Blue'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object rdbGreen: TRadioButton
      Left = 10
      Top = 65
      Width = 56
      Height = 17
      Caption = 'Green'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object rdbRed: TRadioButton
      Left = 10
      Top = 90
      Width = 56
      Height = 17
      Caption = 'Red'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object rdbWhite: TRadioButton
      Left = 10
      Top = 115
      Width = 56
      Height = 17
      Caption = 'White'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
  end
  object Panel1: TPanel
    Left = 105
    Top = 80
    Width = 535
    Height = 270
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 3
    object Splitter1: TSplitter
      Left = 264
      Top = 1
      Width = 7
      Height = 268
      Cursor = crHSplit
    end
    object grpBlock: TGroupBox
      Left = 1
      Top = 1
      Width = 263
      Height = 268
      Align = alLeft
      TabOrder = 0
      object imgPreview: TImage
        Left = 5
        Top = 10
        Width = 105
        Height = 105
        AutoSize = True
      end
    end
    object grpBlock2: TGroupBox
      Left = 271
      Top = 1
      Width = 263
      Height = 268
      Align = alClient
      TabOrder = 1
      object imgPreview2: TImage
        Left = 5
        Top = 10
        Width = 105
        Height = 105
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 354
    Width = 643
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object chkDebug: TCheckBox
    Left = 5
    Top = 305
    Width = 91
    Height = 17
    Caption = 'Extract GI'
    TabOrder = 5
  end
  object dlgSave: TSaveDialog
    Filter = 'All Files (*.*)|*.*'
    Left = 220
    Top = 45
  end
  object dlgOpen: TOpenDialog
    Filter = 
      'All Files (*.*)|*.*|Support (*.gai, *.gi)|*.gai; *.gi|GI File (*' +
      '.gi)|*.gi|GAI File (*.gai)|*.gai'
    FilterIndex = 2
    Left = 265
    Top = 45
  end
end
