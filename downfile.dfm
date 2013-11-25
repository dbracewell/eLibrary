object frmDownload: TfrmDownload
  Left = 313
  Top = 467
  BorderStyle = bsDialog
  Caption = 'frmDownload'
  ClientHeight = 81
  ClientWidth = 272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lblFile: TLabel
    Left = 0
    Top = 64
    Width = 26
    Height = 13
    Caption = 'lblFile'
  end
  object CopyAnimation: TAnimate
    Left = 0
    Top = 0
    Width = 272
    Height = 60
    Active = False
    CommonAVI = aviCopyFile
    StopFrame = 26
  end
  object Timer1: TTimer
    Interval = 1
    OnTimer = Timer1Timer
    Left = 112
    Top = 16
  end
end
