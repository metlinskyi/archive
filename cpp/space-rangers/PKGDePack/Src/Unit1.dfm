object FrmMain: TFrmMain
  Left = 204
  Top = 110
  Width = 535
  Height = 440
  Caption = 
    #1056#1072#1089#1087#1072#1082#1086#1074#1097#1080#1082' '#1092#1072#1081#1083#1086#1074' PKG '#1076#1083#1103' '#1080#1075#1088#1099' "'#1050#1086#1089#1084#1080#1095#1077#1089#1082#1080#1077' '#1056#1077#1081#1085#1076#1078#1077#1088#1099'" v2.0 bet' +
    'a'
  Color = clBtnFace
  Constraints.MinHeight = 270
  Constraints.MinWidth = 535
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  DesignSize = (
    527
    413)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 5
    Top = 10
    Width = 57
    Height = 13
    Caption = #1060#1072#1081#1083' PKG:'
  end
  object Label2: TLabel
    Left = 5
    Top = 35
    Width = 78
    Height = 13
    Caption = #1056#1072#1089#1087#1072#1082#1086#1074#1072#1090#1100' '#1074':'
  end
  object edtFile: TEdit
    Left = 90
    Top = 5
    Width = 330
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    Text = 'c:\Program Files\'#1050#1086#1089#1084#1080#1095#1077#1089#1082#1080#1077' '#1056#1077#1081#1085#1076#1078#1077#1088#1099'\data\common.pkg'
  end
  object btnUnPack: TButton
    Left = 429
    Top = 15
    Width = 91
    Height = 25
    Anchors = [akTop, akRight]
    Caption = #1056#1072#1089#1087#1072#1082#1086#1074#1072#1090#1100
    TabOrder = 1
    OnClick = btnUnPackClick
  end
  object edtDest: TEdit
    Left = 90
    Top = 30
    Width = 330
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
    Text = 'C:\Temp\SRData'
  end
  object memDebug: TMemo
    Left = 5
    Top = 85
    Width = 515
    Height = 321
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 3
  end
  object prgFile: TProgressBar
    Left = 5
    Top = 60
    Width = 515
    Height = 16
    Anchors = [akLeft, akTop, akRight]
    Min = 0
    Max = 100
    TabOrder = 4
  end
end
