//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "editbook.h"
#include "books.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmEditBook *frmEditBook;
//---------------------------------------------------------------------------
__fastcall TfrmEditBook::TfrmEditBook(TComponent* Owner)
   : TForm(Owner)
{
   cboMonth->ItemIndex = 0;   
}
__fastcall TfrmEditBook::TfrmEditBook(TComponent* Owner, BOOK_INFO book):
   TForm(Owner)
{
   cboMonth->ItemIndex = cboMonth->Items->IndexOf( Trim(book.month) );
   edtTitle->Text = book.title;
   edtYear->Text = book.year;
   edtAuthor->Text = book.author;
   edtGutNum->Text = book.gutNum;
}
//---------------------------------------------------------------------------
