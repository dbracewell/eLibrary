//---------------------------------------------------------------------------

#ifndef optionsH
#define optionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "Settings.h"
#include "ftp.h"
#include <Dialogs.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TfrmOptions : public TForm
{
__published:	// IDE-managed Components
   TPageControl *PageControl1;
   TPanel *pnlCommands;
   TTabSheet *GeneralTab;
   TButton *cmdOk;
   TButton *cmdCancel;
   TGroupBox *GroupBox1;
   TComboBox *cboAppFont;
   TLabel *Label1;
   TMaskEdit *edtAppFontSize;
   TLabel *Label2;
   TTabSheet *HTMLTab;
   TGroupBox *GroupBox2;
   TLabel *Label3;
   TLabel *Label4;
   TComboBox *cboHTMLFont;
   TMaskEdit *edtHTMLFontSize;
   TLabel *Label5;
   TGroupBox *GroupBox3;
   TPanel *pnlHTMLColor;
   TColorDialog *ColorDialog;
   TPanel *pnlHTMLBack;
   TGroupBox *GroupBox4;
   TLabel *Label6;
   TMaskEdit *edtHTMLLPP;
   TTabSheet *ReaderTab;
   TGroupBox *GroupBox5;
   TLabel *Label7;
   TMaskEdit *edtReaderLPP;
   TGroupBox *GroupBox6;
   TPanel *pnlReaderBack;
   TGroupBox *GroupBox7;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TComboBox *cboReaderFont;
   TMaskEdit *edtReaderFontSize;
   TPanel *pnlReaderColor;
   TBevel *Bevel1;
   TGroupBox *GroupBox8;
   TComboBox *ComboBox1;
   TEdit *edtProxy;
   TEdit *edtPassword;
   TLabel *Label11;
   TLabel *Label12;
   TLabel *Label13;
   TButton *cmdApply;
   TCheckBox *chkClearNew;
   TGroupBox *GroupBox9;
   TEdit *edtBookDir;
   TSpeedButton *cmdBrowse;
    TCheckBox *cbStripHeader;
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall cmdOkClick(TObject *Sender);
   void __fastcall pnlHTMLColorClick(TObject *Sender);
   void __fastcall DataChange(TObject *Sender);
   void __fastcall cmdApplyClick(TObject *Sender);
   void __fastcall chkClearNewClick(TObject *Sender);
   void __fastcall cmdBrowseClick(TObject *Sender);
   void __fastcall edtHTMLLPPExit(TObject *Sender);
private:	// User declarations
   int __fastcall FindServer(AnsiString ServerName);
   void __fastcall LoadServers();
   void __fastcall MoveFiles();
public:		// User declarations
   Settings FSettings;
   __fastcall TfrmOptions(TComponent* Owner);
   __fastcall TfrmOptions(TComponent* Owner, Settings &old);
   void __fastcall UpdateData(bool ToData);
   vector<SERVER_INFO> servers;



};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOptions *frmOptions;
//---------------------------------------------------------------------------
#endif
