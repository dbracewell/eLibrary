object frmRemBookmarks: TfrmRemBookmarks
  Left = 307
  Top = 258
  BorderStyle = bsDialog
  Caption = 'Remove Bookmarks'
  ClientHeight = 300
  ClientWidth = 356
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lstBookmarks: TListBox
    Left = 8
    Top = 8
    Width = 257
    Height = 289
    ItemHeight = 13
    TabOrder = 0
  end
  object cmdOk: TButton
    Left = 272
    Top = 8
    Width = 75
    Height = 25
    Caption = '&Close'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object cmdRemove: TButton
    Left = 272
    Top = 40
    Width = 75
    Height = 25
    Caption = '&Remove'
    TabOrder = 2
    OnClick = cmdRemoveClick
  end
end
