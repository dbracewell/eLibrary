//---------------------------------------------------------------------------

#ifndef findH
#define findH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
typedef struct{
   int pos;
   int page;
}FIND_STRUCT;
//---------------------------------------------------------------------------
class TfrmFind : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TEdit *edtFindText;
   TLabel *Label1;
   TButton *cmdFind;
   TButton *cmdCancel;
   TGroupBox *GroupBox2;
   TListBox *lstFound;
   TRadioButton *rdoWholeBook;
   TRadioButton *rdoOnePage;
   TCheckBox *chkCase;
   TCheckBox *chkWords;
   TProgressBar *pbarFindStatus;
   TButton *cmdClear;
   TRichEdit *redtSearch;
   void __fastcall cmdFindClick(TObject *Sender);
   void __fastcall cmdCancelClick(TObject *Sender);
   void __fastcall lstFoundDblClick(TObject *Sender);
   void __fastcall cmdClearClick(TObject *Sender);
private:
   void __fastcall FindOnePage();
   void __fastcall FindInBook();
   void __fastcall unsupervisedFind();	// User declarations
public:		// User declarations
   __fastcall TfrmFind(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFind *frmFind;
//---------------------------------------------------------------------------
#endif
