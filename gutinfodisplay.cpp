//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gutInfoDisplay.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmGutInfo *frmGutInfo;
//---------------------------------------------------------------------------
__fastcall TfrmGutInfo::TfrmGutInfo(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmGutInfo::FormResize(TObject *Sender)
{
   int newLeft;
   newLeft = (pnlControls->Width /2) - cmdOk->Width;
   cmdOk->Left = newLeft;
}
//---------------------------------------------------------------------------
