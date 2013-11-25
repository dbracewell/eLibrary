//---------------------------------------------------------------------------

#include <vcl.h>
#include "splash.h"
#include "eLib.h"
#include "books.h"
#include <registry.hpp>
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TsplashScreen *splashScreen;

//---------------------------------------------------------------------------
__fastcall TsplashScreen::TsplashScreen(TComponent* Owner)
   : TForm(Owner)
{
   appPath = IncludeTrailingBackslash(ExtractFilePath( Application->ExeName ));
}
//---------------------------------------------------------------------------


void __fastcall TsplashScreen::loadBookList()
{
   Application->ProcessMessages();

   if( FileExists( appPath + "books.dat" ) )
      ReadBookList(  appPath, mainForm->BookList,ProgressBar );

   mainForm->StatusBar->Panels->Items[1]->Text =
         IntToStr( mainForm->BookList->Items->Count ) + " Books Available";
}

void __fastcall TsplashScreen::loadMyLibrary()
{
   int bookcount = 0;

   if(FileExists( appPath + "lib.dat" ) )
      bookcount = readLibrary( mainForm->MyLibraryList, (appPath+"lib.dat"), ProgressBar );

   mainForm->StatusBar->Panels->Items[2]->Text =
         IntToStr( bookcount ) + " In Library";
}

void __fastcall TsplashScreen::loadSettings()
{
   TRegistry *reg = new TRegistry;
   bool r;

   ProgressBar->Position   = 0;
   ProgressBar->Max        = 26;
   reg->RootKey = HKEY_LOCAL_MACHINE;
   try
   {
      if ( reg->OpenKey("Software\\LN3GS Productions\\eLibrary", false) )
      {
         ProgressBar->Position++;
         if( reg->ValueExists("Maximized") )
         {
            r =  reg->ReadBool("Maximized");
            mainForm->WindowState = r? wsMaximized: wsNormal;
         }
         if( !r )
         {
            ProgressBar->Position++;
            if( reg->ValueExists("Width") )
               mainForm->Width =  reg->ReadInteger("Width");

            ProgressBar->Position++;
            if( reg->ValueExists("Height") )
               mainForm->Height = reg->ReadInteger("Height");

            ProgressBar->Position++;
            if( reg->ValueExists("Left") )
               mainForm->Left = reg->ReadInteger("Left");

            ProgressBar->Position++;
            
            if( reg->ValueExists("Top") )
               mainForm->Top = reg->ReadInteger("Top");
         }
         else
            ProgressBar->Position += 4;

         ProgressBar->Position++;            
         if( reg->ValueExists("GridLines") )
            if( !reg->ReadBool("GridLines") )
               mainForm->mnuViewGridLines->Click();

         ProgressBar->Position++;
         if( reg->ValueExists("Statusbar") )
           if( !reg->ReadBool("Statusbar") )
               mainForm->mnuViewStatusBar->Click();

         ProgressBar->Position++;
         if( reg->ValueExists("Toolbar") )
           if( !reg->ReadBool("Toolbar") )
               mainForm->mnuViewToolbar->Click();

         ProgressBar->Position++;
         mainForm->mySettings.server.server = "ibiblio.org";
         if( reg->ValueExists("Server") )
            mainForm->mySettings.server.server = reg->ReadString("Server");

         ProgressBar->Position++;
         if( reg->ValueExists("ClearNewLine") )
            mainForm->mySettings.addNewLine = reg->ReadBool("ClearNewLine");

         ProgressBar->Position++;
         if( reg->ValueExists("ServerDir") )
            mainForm->mySettings.server.directory = reg->ReadString("ServerDir" );

         ProgressBar->Position++;
         if( reg->ValueExists("Password") )
            mainForm->mySettings.server.password = reg->ReadString("Password");

         ProgressBar->Position++;
         if( reg->ValueExists("Proxy") )
            mainForm->mySettings.server.proxy = reg->ReadString("Proxy" );

         ProgressBar->Position++;
         if( reg->ValueExists("ListFontName" ) )
         {
            mainForm->mySettings.AppFont->Name = reg->ReadString("ListFontName" );
            mainForm->BookList->Font->Name =  mainForm->mySettings.AppFont->Name;
            mainForm->SearchList->Font->Name = mainForm->BookList->Font->Name;
            mainForm->MyLibraryList->Font->Name = mainForm->BookList->Font->Name;
         }

         ProgressBar->Position++;
         if( reg->ValueExists("ListFontSize") )
         {
            mainForm->mySettings.AppFont->Size = reg->ReadInteger("ListFontSize" );
            mainForm->BookList->Font->Size = mainForm->mySettings.AppFont->Size;
            mainForm->SearchList->Font->Size = mainForm->BookList->Font->Size;
            mainForm->MyLibraryList->Font->Size = mainForm->BookList->Font->Size;
         }

         ProgressBar->Position++;
         mainForm->mySettings.bookDir = appPath + "BookDir";
         if( reg->ValueExists("BookDir") )
            mainForm->mySettings.bookDir = reg->ReadString("BookDir");

         ProgressBar->Position++;
         if( reg->ValueExists("HTMLFont") )
            mainForm->mySettings.HTMLFont->Name = reg->ReadString("HTMLFont");

         ProgressBar->Position++;
         if( reg->ValueExists("HTMLFontSize") )
            mainForm->mySettings.HTMLFont->Size = reg->ReadInteger("HTMLFontSize");

         ProgressBar->Position++;
         if( reg->ValueExists("HTMLFontColor") )
            mainForm->mySettings.HTMLFont->Color = StringToColor(reg->ReadString("HTMLFontColor"));

         ProgressBar->Position++;
         if( reg->ValueExists("HTMLBackColor") )
            mainForm->mySettings.HTMLBack = StringToColor( reg->ReadString("HTMLBackColor") );

         //Get Reader Font information
         ProgressBar->Position++;
         if( reg->ValueExists("ReaderFont") )
            mainForm->mySettings.ReaderFont->Name = reg->ReadString("ReaderFont");
         ProgressBar->Position++;
         if( reg->ValueExists("ReaderFontSize") )
            mainForm->mySettings.ReaderFont->Size = reg->ReadInteger("ReaderFontSize");
         ProgressBar->Position++;
         if( reg->ValueExists("ReaderFontColor") )
            mainForm->mySettings.ReaderFont->Color = StringToColor(reg->ReadString("ReaderFontColor"));
         //Set Reader font
         mainForm->memBook->Font = mainForm->mySettings.ReaderFont;

         //Get and Set Reader Back color
         ProgressBar->Position++;
         if( reg->ValueExists("ReaderBackColor") )
            mainForm->mySettings.ReaderBack = StringToColor( reg->ReadString("ReaderBackColor") );
         mainForm->memBook->Color = mainForm->mySettings.ReaderBack;


         ProgressBar->Position++;
         if( reg->ValueExists("HTMLLpp") )
            mainForm->mySettings.HTMLLpp =  reg->ReadInteger("HTMLLpp");

         ProgressBar->Position++;
         if( reg->ValueExists("ReaderLpp") )
            mainForm->mySettings.ReaderLpp =  reg->ReadInteger("ReaderLpp");

         if( reg->ValueExists("StripHeader") )
            mainForm->mySettings.StripHeader = reg->ReadBool("StripHeader");   

      }// end of If was able to open key

   }
   __finally
   {   delete reg;   }
}

void __fastcall TsplashScreen::TimerTimer(TObject *Sender)
{
   Timer->Enabled = false;

   LoadAll();

}
//---------------------------------------------------------------------------


void __fastcall TsplashScreen::LoadAll()
{

   lblProg->Caption = "Loading Settings...";
   loadSettings();

   lblProg->Caption = "Loading My Library...";
   loadMyLibrary();

   lblProg->Caption = "Searching for books...";
   SearchForBooks();

   lblProg->Caption = "Loading Book List...";
   loadBookList();

   mainForm->StatusBar->Panels->Items[2]->Text =
         IntToStr( mainForm->MyLibraryList->Items->Count ) + " In Library";
   Close();
}


void __fastcall TsplashScreen::SearchForBooks()
{

   books->Directory = mainForm->mySettings.bookDir;
   ProgressBar->Position = 0;
   ProgressBar->Max = books->Items->Count;
   for( int i = 0; i < books->Items->Count; i++ )
   {
      ProgressBar->Position++;
      bool found = false;
      for( int j = 0; !found && j < mainForm->MyLibraryList->Items->Count; j++ )
         if( mainForm->MyLibraryList->Items->Item[j]->SubItems->Strings[VIEW_FILENAME] == books->Items->Strings[i] )
            found = true;



     if( !found )
     {
      TListItem *li = mainForm->MyLibraryList->Items->Add();
      BOOK_INFO b;
      b = getBookInfo(books->Items->Strings[i]);
      BookToListItem(li,b);
    }
  }  


}