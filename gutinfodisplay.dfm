object frmGutInfo: TfrmGutInfo
  Left = 192
  Top = 103
  BorderIcons = [biMaximize]
  BorderStyle = bsSingle
  Caption = 'Book Information'
  ClientHeight = 453
  ClientWidth = 688
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object pnlControls: TPanel
    Left = 0
    Top = 416
    Width = 688
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object cmdOk: TButton
      Left = 304
      Top = 5
      Width = 75
      Height = 25
      Caption = '&OK'
      ModalResult = 1
      TabOrder = 0
    end
  end
  object memInfo: TMemo
    Left = 0
    Top = 0
    Width = 688
    Height = 416
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 1
  end
end
