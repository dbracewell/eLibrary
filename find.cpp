//---------------------------------------------------------------------------

#include <vcl.h>
#include <vector.h>
#pragma hdrstop

#include "find.h"
#include "eLib.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAX_FIND_HEIGHT 396
#define MIN_FIND_HEIGHT 144
extern vector<AnsiString> openBook;
vector<FIND_STRUCT> positionInPage;
int cPage;

TfrmFind *frmFind;
//---------------------------------------------------------------------------
__fastcall TfrmFind::TfrmFind(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmFind::cmdFindClick(TObject *Sender)
{
   if( edtFindText->Text.Trim() == "" )
      return;

   Screen->Cursor = crHourGlass;
   if( rdoWholeBook->Checked )
   {
      pbarFindStatus->Position = 0;
      pbarFindStatus->Max = openBook.size();
      Height = MAX_FIND_HEIGHT;
      lstFound->Tag = edtFindText->Text.Length();
      lstFound->Items->Clear();
      positionInPage.clear();
      lstFound->Items->BeginUpdate();
      FindInBook();
      lstFound->Items->EndUpdate();
      pbarFindStatus->Position = 0;
   }
   else
   {
      Height = MIN_FIND_HEIGHT;
      FindOnePage();
   }
   Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TfrmFind::cmdCancelClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

/*******************************************************************************
*  FindOnPage
*  Search only current page in book for text.
*******************************************************************************/
void __fastcall TfrmFind::FindOnePage()
{
   int startPos = 0;
   TSearchTypes searchOptions;

   //Determine Search Options
   if( chkCase->Checked )
      searchOptions << stMatchCase;
   if( chkWords->Checked )
      searchOptions << stWholeWord;

   //If there is something selected then Start from the end of that
   if( mainForm->memBook->SelLength )
      startPos = mainForm->memBook->SelStart + mainForm->memBook->SelLength;

   //Find text
   int newpos = mainForm->memBook->FindText(edtFindText->Text,
                        startPos,
                        mainForm->memBook->Text.Length(),
                        searchOptions);
   //If found show it
   if (newpos != -1)
   {
       mainForm->memBook->SetFocus();
       mainForm->memBook->SelStart = newpos;
       mainForm->memBook->SelLength = edtFindText->Text.Length();
   }
   else
   {
      ShowMessage("End of document reached.");
      mainForm->memBook->SelStart = 0;
   }
}

void __fastcall TfrmFind::FindInBook()
{
   for( unsigned int i = 0; i < openBook.size(); i++ )
   {
      cPage = i;
      redtSearch->Lines->Text = openBook[i];
      pbarFindStatus->Position++;
      unsupervisedFind();
   }
}

void __fastcall TfrmFind::unsupervisedFind()
{
   TSearchTypes searchOptions;
   int TotalLength;
   int start, newpos = 0;
   FIND_STRUCT foundAt;
   TotalLength = (redtSearch->Lines->Text.Length() - edtFindText->Text.Length() );

   //Determine Search Options
   if( chkCase->Checked )
      searchOptions << stMatchCase;
   if( chkWords->Checked )
      searchOptions << stWholeWord;

   for( start = 0; (newpos != -1) && (start < TotalLength);)
   {
      //Find text
      newpos = redtSearch->FindText(edtFindText->Text,
                        start,
                        redtSearch->Text.Length(),
                        searchOptions);
      //If found show it
      if (newpos != -1)
      {
         foundAt.page = cPage;
         foundAt.pos  = newpos;
         int line = redtSearch->Perform( EM_EXLINEFROMCHAR,0,(LPARAM) newpos );
         lstFound->Items->Add("Page " +IntToStr(cPage+1) + ": Line " + IntToStr(line+1)+
                              ": \"" + redtSearch->Lines->Strings[line] +"\"" );
         positionInPage.push_back( foundAt );
      }

      start = newpos + edtFindText->Text.Length();
   }
}
void __fastcall TfrmFind::lstFoundDblClick(TObject *Sender)
{
   if( lstFound->Items->Count > 0 )
   {
      mainForm->GoToPage( positionInPage[ lstFound->ItemIndex ].page );
      mainForm->memBook->SetFocus();
      mainForm->memBook->SelStart = positionInPage[ lstFound->ItemIndex ].pos;
      mainForm->memBook->SelLength = lstFound->Tag;
   }
         
}
//---------------------------------------------------------------------------

void __fastcall TfrmFind::cmdClearClick(TObject *Sender)
{
   lstFound->Items->BeginUpdate();
   lstFound->Items->Clear();
   lstFound->Items->EndUpdate();
   positionInPage.clear();
}
//---------------------------------------------------------------------------

