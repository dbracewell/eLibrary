//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ftp.h"
#include "downFile.h"
#include "eLib.h"
#include "books.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDownload *frmDownload;
//---------------------------------------------------------------------------
__fastcall TfrmDownload::TfrmDownload(
      TComponent* Owner,
      AnsiString file,
      AnsiString year,
      AnsiString cap,
      SERVER_INFO *server)
   : TForm(Owner), FServer(server), FFileName( file ), FYear( year ),
     bookList(false)
{
   Caption = cap;
}
//---------------------------------------------------------------------------

__fastcall TfrmDownload::TfrmDownload(TComponent* Owner, AnsiString file, SERVER_INFO *server ):
   TForm(Owner), FServer(server),FFileName(file),bookList(true)
{
   FLocal = mainForm->mySettings.appPath + "books.txt";
   Caption = "Downloading Book List";
}

void __fastcall TfrmDownload::Timer1Timer(TObject *Sender)
{
   AnsiString directory;
   bool done = false;
   Timer1->Enabled = false;

   Application->ProcessMessages();
   Screen->Cursor = crHourGlass;

   if( bookList )
   {  //If Downloading a book list
      //Tell user we are downloading book list
      //by changing caption and starting animation
      lblFile->Caption = "Downloading Book List...";
      CopyAnimation->Active = true;
      Application->ProcessMessages();
      //Get File From Server **books.txt
      if( !ftpGetFile(*FServer,FFileName,FLocal) )
      {
         MessageBox( Application->Handle, "Could not download index file.\nPlease try a different server", "Error",MB_OK|MB_ICONERROR);
         Screen->Cursor = crDefault;
         Close();
      }
      else
      {
         //Create a book list file, load it, and update
         //program
         CopyAnimation->Active = false;
         lblFile->Caption = "Loading New Book List...";
         Application->ProcessMessages();
         mainForm->BookList->Items->BeginUpdate();
         WriteBookList( mainForm->mySettings.appPath );
         mainForm->BookList->Items->Clear();
         ReadBookList( mainForm->mySettings.appPath,mainForm->BookList );
         mainForm->BookList->Items->EndUpdate();
      }
   }
   else
   {  //Download A Book
      int i;

      //Find Filename minus the wildcards
      for( i = FFileName.Length(); !done && i > 0; i-- )
         if( FFileName[i] != '.' && FFileName[i] != 'x' )
            done = true;

      //If We were able to find it
      if( i > 0 )
      {
         AnsiString year;
         int iYear = StrToInt(FYear);

         //Books before 1990 are still stored in 1990 folder
         if( iYear <= 1990  )
            year = "90";
         else
            year = AnsiString(FYear[3]) + AnsiString(FYear[4]);

         //Initialize Directory to search and file to search for
         FServer->directory = FServer->directory+ "etext"+ year + "/";
         FFileName = FFileName.SubString(1,i+1) + "*.txt";

         //Find File
         lblFile->Caption = "Finding File...";
         Application->ProcessMessages();
         FFileName = ftpFindFile( *FServer, FFileName);

         if( FFileName == "" ) //If can't find it tell user
            Application->MessageBox("Error Finding Book","Ooops!",MB_OK);
         else
         {
            //Create local filename for book
            //and download book
            FLocal = mainForm->mySettings.bookDir + FFileName;
            lblFile->Caption = "Downloading " + FFileName;
            CopyAnimation->Active = true;
            Application->ProcessMessages();
            ftpGetFile(*FServer,FFileName,FLocal);

            //Inform user download is complete
            CopyAnimation->Active = false;
            lblFile->Caption = "Successfully download";

            TListItem *newitem;
            newitem = mainForm->MyLibraryList->Items->Add();

            //Copy Book Information to My Library
            if( mainForm->WorkArea->ActivePage == mainForm->BookListTab )
               newitem->Assign( mainForm->BookList->Selected );
            else
               newitem->Assign( mainForm->SearchList->Selected );

            newitem->SubItems->Strings[VIEW_FILENAME] = FFileName;
            mainForm->StatusBar->Panels->Items[2]->Text =
               IntToStr( mainForm->MyLibraryList->Items->Count ) + " In Library";
         }
      }
   }
   saveLibrary( mainForm->mySettings.appPath,mainForm->MyLibraryList->Items );
   Screen->Cursor = crDefault;
   Close();
}
//---------------------------------------------------------------------------


