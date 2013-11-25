object frmEditBook: TfrmEditBook
  Left = 340
  Top = 318
  BorderStyle = bsDialog
  Caption = 'Edit Book'
  ClientHeight = 247
  ClientWidth = 281
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 185
    Height = 233
    Caption = 'Book Information'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 30
      Height = 13
      Caption = 'Month'
    end
    object Label2: TLabel
      Left = 16
      Top = 104
      Width = 20
      Height = 13
      Caption = 'Title'
    end
    object Label3: TLabel
      Left = 16
      Top = 144
      Width = 31
      Height = 13
      Caption = 'Author'
    end
    object Label4: TLabel
      Left = 16
      Top = 64
      Width = 22
      Height = 13
      Caption = 'Year'
    end
    object Label5: TLabel
      Left = 16
      Top = 184
      Width = 104
      Height = 13
      Caption = 'Gutenberg Filenumber'
    end
    object cboMonth: TComboBox
      Left = 16
      Top = 40
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      Items.Strings = (
        'Jan'
        'Feb'
        'Mar'
        'Apr'
        'May'
        'Jun'
        'Jul'
        'Aug'
        'Sep'
        'Oct'
        'Nov'
        'Dec')
    end
    object edtTitle: TEdit
      Left = 16
      Top = 120
      Width = 145
      Height = 21
      TabOrder = 1
    end
    object edtAuthor: TEdit
      Left = 16
      Top = 160
      Width = 145
      Height = 21
      TabOrder = 2
    end
    object edtYear: TMaskEdit
      Left = 16
      Top = 80
      Width = 145
      Height = 21
      EditMask = '####;1;_'
      MaxLength = 4
      TabOrder = 3
      Text = '    '
    end
    object edtGutNum: TMaskEdit
      Left = 16
      Top = 200
      Width = 145
      Height = 21
      EditMask = '#######;1;_'
      MaxLength = 7
      TabOrder = 4
      Text = '       '
    end
  end
  object cmdSave: TButton
    Left = 200
    Top = 16
    Width = 75
    Height = 25
    Caption = '&Save'
    ModalResult = 1
    TabOrder = 1
  end
  object cmdCancel: TButton
    Left = 200
    Top = 48
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
