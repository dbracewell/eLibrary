//---------------------------------------------------------------------------

#include <vcl.h>
#include "FTP.H"
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{
   SERVER_INFO s;
   s.sAppName = ExtractFileName( Application->ExeName );
   s.server = "sailor.gutenberg.org";
   s.username = "anonymous";
   s.password = "kp3433@news.com";

   TStringList *fileList;
   fileList = new TStringList();

   fileList = ftpGetFileList(s,"*.txt" );
   ftpGetFile(s,"/pub/gutenberg/etext94/apoc10.txt","c:/t.txt" );
   ListBox1->Items->Assign( fileList );

   delete fileList;
}
//---------------------------------------------------------------------------
