//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RemBookmarks.h"
#include "bookmark.h"
#include <vector.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRemBookmarks *frmRemBookmarks;
extern vector<BOOKMARK> bookmarks;
//---------------------------------------------------------------------------
__fastcall TfrmRemBookmarks::TfrmRemBookmarks(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemBookmarks::FormCreate(TObject *Sender)
{
   for( unsigned int i = 0; i < bookmarks.size(); i++ )
   {
      if( bookmarks[i].menu )
         lstBookmarks->Items->Add( bookmarks[i].menu->Caption );
      else
         lstBookmarks->Items->Add( "<<<Deleted on Program Termination>>>" );
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemBookmarks::cmdRemoveClick(TObject *Sender)
{
   int index = lstBookmarks->ItemIndex;
   if(   (lstBookmarks->Items->Strings[index] == "") ||
         (lstBookmarks->Items->Strings[index] == "<<<Deleted on Program Termination>>>") )
         return;

   if( bookmarks[index].menu )
   {
      delete bookmarks[index].menu;
      bookmarks[index].menu = NULL;
      lstBookmarks->Items->Strings[index] = "<<<Deleted on Program Termination>>>";
   }
}
//---------------------------------------------------------------------------
