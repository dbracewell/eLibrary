object frmOptions: TfrmOptions
  Left = 361
  Top = 177
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Options'
  ClientHeight = 330
  ClientWidth = 333
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 333
    Height = 294
    ActivePage = GeneralTab
    Align = alClient
    Style = tsFlatButtons
    TabOrder = 0
    object GeneralTab: TTabSheet
      Caption = '&General'
      object GroupBox1: TGroupBox
        Left = 8
        Top = 3
        Width = 313
        Height = 86
        Caption = 'Application Font'
        TabOrder = 0
        object Label1: TLabel
          Left = 8
          Top = 24
          Width = 55
          Height = 13
          Caption = 'Font Name:'
        end
        object Label2: TLabel
          Left = 16
          Top = 56
          Width = 47
          Height = 13
          Caption = 'Font Size:'
        end
        object cboAppFont: TComboBox
          Left = 72
          Top = 18
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = DataChange
        end
        object edtAppFontSize: TMaskEdit
          Left = 72
          Top = 48
          Width = 41
          Height = 21
          EditMask = '##;1;_'
          MaxLength = 2
          TabOrder = 1
          Text = '  '
          OnChange = DataChange
        end
      end
      object GroupBox8: TGroupBox
        Left = 8
        Top = 93
        Width = 313
        Height = 113
        Caption = 'Server'
        TabOrder = 1
        object Label11: TLabel
          Left = 6
          Top = 53
          Width = 63
          Height = 13
          Caption = 'Proxy Server:'
        end
        object Label12: TLabel
          Left = 16
          Top = 86
          Width = 53
          Height = 13
          Caption = 'Your Email:'
        end
        object Label13: TLabel
          Left = 35
          Top = 24
          Width = 34
          Height = 13
          Caption = 'Server:'
        end
        object ComboBox1: TComboBox
          Left = 75
          Top = 16
          Width = 206
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = DataChange
        end
        object edtProxy: TEdit
          Left = 75
          Top = 47
          Width = 206
          Height = 21
          Hint = 'Leave Blank If you don'#39't use a proxy server'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          OnChange = DataChange
        end
        object edtPassword: TEdit
          Left = 75
          Top = 79
          Width = 206
          Height = 21
          Hint = 'Used as password for server'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
          OnChange = DataChange
        end
      end
      object GroupBox9: TGroupBox
        Left = 8
        Top = 211
        Width = 313
        Height = 49
        Hint = 'Directory where your books are stored'
        Caption = 'Book Directory'
        TabOrder = 2
        object cmdBrowse: TSpeedButton
          Left = 279
          Top = 20
          Width = 23
          Height = 20
          Caption = '..'
          Flat = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          OnClick = cmdBrowseClick
        end
        object edtBookDir: TEdit
          Left = 11
          Top = 19
          Width = 262
          Height = 21
          Color = clBtnFace
          Enabled = False
          TabOrder = 0
        end
      end
    end
    object HTMLTab: TTabSheet
      Caption = '&HTML Settings'
      ImageIndex = 1
      object GroupBox2: TGroupBox
        Left = 8
        Top = 3
        Width = 313
        Height = 121
        Caption = 'Font'
        Color = clBtnFace
        ParentColor = False
        TabOrder = 0
        object Label3: TLabel
          Left = 8
          Top = 24
          Width = 55
          Height = 13
          Caption = 'Font Name:'
        end
        object Label4: TLabel
          Left = 16
          Top = 56
          Width = 47
          Height = 13
          Caption = 'Font Size:'
        end
        object Label5: TLabel
          Left = 12
          Top = 88
          Width = 51
          Height = 13
          Caption = 'Font Color:'
        end
        object cboHTMLFont: TComboBox
          Left = 72
          Top = 18
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = DataChange
        end
        object edtHTMLFontSize: TMaskEdit
          Left = 72
          Top = 48
          Width = 41
          Height = 21
          EditMask = '##;1;_'
          MaxLength = 2
          TabOrder = 1
          Text = '  '
          OnChange = DataChange
        end
        object pnlHTMLColor: TPanel
          Left = 72
          Top = 82
          Width = 145
          Height = 25
          Cursor = crHandPoint
          BevelOuter = bvNone
          BorderStyle = bsSingle
          TabOrder = 2
          OnClick = pnlHTMLColorClick
        end
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 131
        Width = 313
        Height = 57
        Caption = 'Background Color'
        TabOrder = 1
        object pnlHTMLBack: TPanel
          Left = 73
          Top = 22
          Width = 145
          Height = 25
          Cursor = crHandPoint
          BevelOuter = bvNone
          BorderStyle = bsSingle
          TabOrder = 0
          OnClick = pnlHTMLColorClick
        end
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 192
        Width = 313
        Height = 65
        TabOrder = 2
        object Label6: TLabel
          Left = 7
          Top = 31
          Width = 75
          Height = 13
          Caption = 'Lines Per Page:'
        end
        object edtHTMLLPP: TMaskEdit
          Left = 90
          Top = 23
          Width = 41
          Height = 21
          EditMask = '##;1;_'
          MaxLength = 2
          TabOrder = 0
          Text = '  '
          OnChange = DataChange
          OnExit = edtHTMLLPPExit
        end
      end
    end
    object ReaderTab: TTabSheet
      Caption = '&Reader Settings'
      ImageIndex = 2
      object GroupBox5: TGroupBox
        Left = 8
        Top = 192
        Width = 313
        Height = 65
        TabOrder = 0
        object Label7: TLabel
          Left = 7
          Top = 31
          Width = 75
          Height = 13
          Hint = 'Set to 0 to let the  program decide what is best for fullscreen'
          Caption = 'Lines Per Page:'
        end
        object edtReaderLPP: TMaskEdit
          Left = 90
          Top = 23
          Width = 41
          Height = 21
          Hint = 'Set to 0 to let the  program decide what is best for fullscreen'
          EditMask = '##;1;_'
          MaxLength = 2
          TabOrder = 0
          Text = '  '
          OnChange = DataChange
        end
        object chkClearNew: TCheckBox
          Left = 184
          Top = 14
          Width = 121
          Height = 17
          Hint = 'Makes Text Wordwrapabble'
          Caption = 'Clear Hard Returns'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
          OnClick = chkClearNewClick
        end
        object cbStripHeader: TCheckBox
          Left = 184
          Top = 38
          Width = 121
          Height = 17
          Hint = 
            'Take Gutenberg header out of the book. Uncheck if having trouble' +
            's opening a book'
          Caption = 'Strip Header'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
          OnClick = chkClearNewClick
        end
      end
      object GroupBox6: TGroupBox
        Left = 8
        Top = 131
        Width = 313
        Height = 57
        Caption = 'Background Color'
        TabOrder = 1
        object pnlReaderBack: TPanel
          Left = 73
          Top = 22
          Width = 145
          Height = 25
          Cursor = crHandPoint
          BevelOuter = bvNone
          BorderStyle = bsSingle
          TabOrder = 0
          OnClick = pnlHTMLColorClick
        end
      end
      object GroupBox7: TGroupBox
        Left = 8
        Top = 3
        Width = 313
        Height = 121
        Caption = 'Font'
        Color = clBtnFace
        ParentColor = False
        TabOrder = 2
        object Label8: TLabel
          Left = 8
          Top = 24
          Width = 55
          Height = 13
          Caption = 'Font Name:'
        end
        object Label9: TLabel
          Left = 16
          Top = 56
          Width = 47
          Height = 13
          Caption = 'Font Size:'
        end
        object Label10: TLabel
          Left = 12
          Top = 88
          Width = 51
          Height = 13
          Caption = 'Font Color:'
        end
        object cboReaderFont: TComboBox
          Left = 72
          Top = 18
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = DataChange
        end
        object edtReaderFontSize: TMaskEdit
          Left = 72
          Top = 48
          Width = 41
          Height = 21
          EditMask = '##;1;_'
          MaxLength = 2
          TabOrder = 1
          Text = '  '
          OnChange = DataChange
        end
        object pnlReaderColor: TPanel
          Left = 72
          Top = 82
          Width = 145
          Height = 25
          Cursor = crHandPoint
          BevelOuter = bvNone
          BorderStyle = bsSingle
          TabOrder = 2
          OnClick = pnlHTMLColorClick
        end
      end
    end
  end
  object pnlCommands: TPanel
    Left = 0
    Top = 294
    Width = 333
    Height = 36
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object Bevel1: TBevel
      Left = 0
      Top = 0
      Width = 333
      Height = 36
      Align = alClient
      Shape = bsTopLine
    end
    object cmdOk: TButton
      Left = 85
      Top = 8
      Width = 75
      Height = 25
      Caption = '&OK'
      ModalResult = 1
      TabOrder = 0
      OnClick = cmdOkClick
    end
    object cmdCancel: TButton
      Left = 169
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object cmdApply: TButton
      Left = 252
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Apply'
      Enabled = False
      TabOrder = 2
      OnClick = cmdApplyClick
    end
  end
  object ColorDialog: TColorDialog
    Ctl3D = True
    Left = 268
    Top = 83
  end
end
