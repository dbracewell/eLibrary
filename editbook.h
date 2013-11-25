//---------------------------------------------------------------------------

#ifndef editbookH
#define editbookH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include "books.h"
//---------------------------------------------------------------------------
class TfrmEditBook : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TComboBox *cboMonth;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *edtTitle;
   TEdit *edtAuthor;
   TLabel *Label3;
   TMaskEdit *edtYear;
   TLabel *Label4;
   TMaskEdit *edtGutNum;
   TLabel *Label5;
   TButton *cmdSave;
   TButton *cmdCancel;
private:	// User declarations
public:		// User declarations
   __fastcall TfrmEditBook(TComponent* Owner);
   __fastcall TfrmEditBook(TComponent* Owner, BOOK_INFO book);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditBook *frmEditBook;
//---------------------------------------------------------------------------
#endif
