//---------------------------------------------------------------------------

#include <vcl.h>
#include "FTP.H"
#pragma hdrstop

#include "options.h"
#include "ShellDialogs.h"
#include <shellapi.hpp>
#include "settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmOptions *frmOptions;
bool ChangeDir = false;
//---------------------------------------------------------------------------

__fastcall TfrmOptions::TfrmOptions(TComponent* Owner, Settings &old):TForm(Owner)
{
  for( int i =0; i < Screen->Fonts->Count; i++ )
   {
      cboAppFont->Items->Add( Screen->Fonts->Strings[i] );
      cboHTMLFont->Items->Add( Screen->Fonts->Strings[i] );
      cboReaderFont->Items->Add( Screen->Fonts->Strings[i] );
   }

   FSettings.HTMLFont = new TFont;
   FSettings.HTMLFont->Assign( this->Font );
   FSettings.AppFont = new TFont;
   FSettings.AppFont->Assign( this->Font );
   FSettings.ReaderFont = new TFont;
   FSettings.ReaderFont->Assign( this->Font );

   CopySettings( FSettings, old );

   LoadServers();
   ComboBox1->ItemIndex = 0;



   UpdateData(false);
   cmdApply->Enabled = false;
   ChangeDir = false;

}

__fastcall TfrmOptions::TfrmOptions(TComponent* Owner)
   : TForm(Owner)
{}
//---------------------------------------------------------------------------


void __fastcall TfrmOptions::UpdateData(bool ToData)
{
   if( ToData )
   {
      if(edtAppFontSize->Text.Trim() == "") edtAppFontSize->Text = "8";
      if(edtHTMLFontSize->Text.Trim() == "") edtHTMLFontSize->Text = "8";
      if(edtReaderFontSize->Text.Trim() == "") edtReaderFontSize->Text = "8";
      FSettings.AppFont->Size = StrToInt(edtAppFontSize->Text.Trim());
      FSettings.AppFont->Name = cboAppFont->Items->Strings[ cboAppFont->ItemIndex ];
      FSettings.bookDir = edtBookDir->Text;
      
      FSettings.HTMLFont->Size = StrToInt(edtHTMLFontSize->Text.Trim());
      FSettings.HTMLFont->Name = cboHTMLFont->Items->Strings[ cboHTMLFont->ItemIndex ];
      FSettings.HTMLFont->Color = pnlHTMLColor->Color;
      FSettings.HTMLBack = pnlHTMLBack->Color;
      FSettings.HTMLLpp = StrToInt(edtHTMLLPP->Text.Trim());
      
      FSettings.ReaderFont->Size = StrToInt(edtReaderFontSize->Text.Trim());
      FSettings.ReaderFont->Name = cboReaderFont->Items->Strings[ cboReaderFont->ItemIndex ];
      FSettings.ReaderFont->Color = pnlReaderColor->Color;
      FSettings.ReaderBack = pnlReaderBack->Color;
      FSettings.ReaderLpp = StrToInt(edtReaderLPP->Text.Trim());

      FSettings.server.proxy = edtProxy->Text;
      FSettings.server.password = edtPassword->Text;
      FSettings.addNewLine = !chkClearNew->Checked;
      FSettings.StripHeader = cbStripHeader->Checked;
      if( ChangeDir )MoveFiles();
   }
   else
   {

      edtAppFontSize->Text = IntToStr(FSettings.AppFont->Size);
      cboAppFont->ItemIndex = cboAppFont->Items->IndexOf( FSettings.AppFont->Name );
      edtBookDir->Text = FSettings.bookDir;
      edtHTMLFontSize->Text = IntToStr(FSettings.HTMLFont->Size);
      cboHTMLFont->ItemIndex = cboHTMLFont->Items->IndexOf( FSettings.HTMLFont->Name );
      pnlHTMLColor->Color = FSettings.HTMLFont->Color ;
      pnlHTMLBack->Color = FSettings.HTMLBack;
      edtHTMLLPP->Text = IntToStr(FSettings.HTMLLpp);

      edtReaderFontSize->Text = IntToStr(FSettings.ReaderFont->Size);
      cboReaderFont->ItemIndex = cboReaderFont->Items->IndexOf( FSettings.ReaderFont->Name );
      pnlReaderColor->Color = FSettings.ReaderFont->Color ;
      pnlReaderBack->Color = FSettings.ReaderBack;
      edtReaderLPP->Text = IntToStr(FSettings.ReaderLpp);

      edtProxy->Text = FSettings.server.proxy;
      edtPassword->Text = FSettings.server.password;
      chkClearNew->Checked = !FSettings.addNewLine;
      ComboBox1->ItemIndex = FindServer( FSettings.server.server );
      cbStripHeader->Checked = FSettings.StripHeader;
   }
}
void __fastcall TfrmOptions::FormDestroy(TObject *Sender)
{
   delete FSettings.HTMLFont;
   delete FSettings.AppFont;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOptions::cmdOkClick(TObject *Sender)
{
   if( cmdApply->Enabled)
      cmdApplyClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TfrmOptions::pnlHTMLColorClick(TObject *Sender)
{
   if( ColorDialog->Execute() )
   {
      dynamic_cast<TPanel *>(Sender)->Color = ColorDialog->Color;
      cmdApply->Enabled = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmOptions::DataChange(TObject *Sender)
{
    cmdApply->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOptions::cmdApplyClick(TObject *Sender)
{
   //copyServer( FSettings.server, servers[ ComboBox1->ItemIndex ] );
   FSettings.server = servers[ComboBox1->ItemIndex];
   UpdateData( true );
   CopySettings( mainForm->mySettings, FSettings );
   cmdApply->Enabled = false;

   mainForm->BookList->Font->Name =  mainForm->mySettings.AppFont->Name;
   mainForm->SearchList->Font->Name = mainForm->BookList->Font->Name;
   mainForm->MyLibraryList->Font->Name = mainForm->BookList->Font->Name;

   mainForm->BookList->Font->Size = mainForm->mySettings.AppFont->Size;
   mainForm->SearchList->Font->Size = mainForm->BookList->Font->Size;
   mainForm->MyLibraryList->Font->Size = mainForm->BookList->Font->Size;

   mainForm->memBook->Font->Assign( mainForm->mySettings.ReaderFont );
   mainForm->memBook->Color = mainForm->mySettings.ReaderBack;

}
//---------------------------------------------------------------------------

void __fastcall TfrmOptions::chkClearNewClick(TObject *Sender)
{
   if( Cursor == crHelp )
      ShowHint;
   else
      DataChange(Sender);
}
//---------------------------------------------------------------------------


int __fastcall TfrmOptions::FindServer(AnsiString ServerName)
{
   for( unsigned int i = 0; i < servers.size(); i++ )
      if( ServerName == servers[i].server )
         return i;

   return -1;
}

void __fastcall TfrmOptions::LoadServers()
{
   ifstream serverFile( (mainForm->mySettings.appPath + "servers.txt").c_str() );
   char buff[500];
   int numOfServers;

   serverFile.getline(buff,sizeof(buff));
   numOfServers = StrToInt( Trim(buff) );
   servers.resize( numOfServers );

   for( unsigned int i = 0; i < servers.size(); i++ )
        servers[i] = FSettings.server;
//      copyServer( servers[i], FSettings.server );


   for( unsigned int i = 0; i < servers.size(); i++)
   {
      serverFile.getline(buff,sizeof(buff));
      ComboBox1->Items->Add(buff);
      serverFile.getline(buff,sizeof(buff));
      servers[i].server = buff;
      serverFile.getline(buff,sizeof(buff));
      servers[i].directory = buff;
   }
   serverFile.close();
}
void __fastcall TfrmOptions::cmdBrowseClick(TObject *Sender)
{
   AnsiString newFolder;
   newFolder = BrowseForFolder( "Choose New Book Directory",this );
   if( (newFolder != "") && (newFolder != edtBookDir->Text) )
   {
      edtBookDir->Text = newFolder ;
      ChangeDir = true;
      DataChange(Sender);
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmOptions::MoveFiles()
{
   TSHFileOpStruct moveOp;
   AnsiString fromDir;
   char from[500], to[500];
   ChangeDir = false;
   Screen->Cursor = crHourGlass;
   fromDir = mainForm->mySettings.bookDir;

   ZeroMemory( &moveOp, sizeof(moveOp) );
   moveOp.hwnd = Application->Handle;
   moveOp.wFunc = FO_MOVE;
   moveOp.fFlags = FOF_SIMPLEPROGRESS |FOF_NOCONFIRMATION |	FOF_FILESONLY | FOF_RENAMEONCOLLISION;
   moveOp.lpszProgressTitle = "Moving books to new Directory";
   strcpy( from, (fromDir+"*.txt").c_str() );
   from[ strlen(from)+1 ] = '\0';
   moveOp.pFrom = from;

   strcpy( to, FSettings.bookDir.c_str() );
   to[ strlen(to)+1 ] = '\0';
   moveOp.pTo = to;

   SHFileOperation( &moveOp );
   Screen->Cursor = crDefault;
}

void __fastcall TfrmOptions::edtHTMLLPPExit(TObject *Sender)
{
   TMaskEdit *thisEdit = ( (TMaskEdit*)Sender );

   if( StrToInt( thisEdit->Text.Trim() ) <= 29 )
   {
         ShowMessage("Needs to be greater than 29");
         thisEdit->SetFocus();
   }
}
//---------------------------------------------------------------------------


