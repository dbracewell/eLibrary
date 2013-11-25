////////////////////////////////////////////////////////////////////////////////
// about.cpp
// David Bracewell
// 3/2001
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "about.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{}
//---------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// Label4Click
// open browser to ln3gs production's homepage
////////////////////////////////////////////////////////////////////////////////
void __fastcall TAboutBox::Label4Click(TObject *Sender)
{
   ShellExecute(  Application->Handle,
                  "open",
                  "http://www.ln3gs.com",
                  "",
                  ExtractFilePath(ParamStr(0)).c_str(),
                  SW_MAXIMIZE);
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// Label5Click
// open browser to project gutenberg's homepage
////////////////////////////////////////////////////////////////////////////////
void __fastcall TAboutBox::Label5Click(TObject *Sender)
{
   ShellExecute(  Application->Handle,
                  "open",
                  "http://www.promo.net/pg/",
                  "",
                  ExtractFilePath(ParamStr(0)).c_str(),
                  SW_MAXIMIZE);
}
//---------------------------------------------------------------------------



