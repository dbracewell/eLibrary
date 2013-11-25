////////////////////////////////////////////////////////////////////////////////
// bookmark.cpp
// David Bracewell
// 5/2001
////////////////////////////////////////////////////////////////////////////////
#include "bookmark.h"
vector<BOOKMARK> bookmarks;
unsigned int numOfBookmarks = 0;

////////////////////////////////////////////////////////////////////////////////
// NAME: readBookmarks
// ARGUMENTS:  AnsiString FileName, name of ini file to write
//             TMenuItem *parent, Parent to add menu items to
// RETURNS: NONE
// PURPOSE: read bookmark file and display in menu
////////////////////////////////////////////////////////////////////////////////
void readBookmarks(AnsiString FileName, TMenuItem *parent)
{
   TIniFile *bmarkFile = new TIniFile( FileName );
   int lpp, page;
   BOOK_INFO book;
   unsigned int TotalNumber;

   //Read number of bookmarks
   TotalNumber = bmarkFile->ReadInteger( "NUMBER","NUMBER",0);

   for( unsigned int i = 0; i < TotalNumber; i++ )
   {
      book.title     =  bmarkFile->ReadString(  IntToStr(i+1), "Caption", "" );
      book.filename  =  bmarkFile->ReadString(  IntToStr(i+1), "Filename", "");
      lpp            =  bmarkFile->ReadInteger( IntToStr(i+1), "LPP", 0 );
      page           =  bmarkFile->ReadInteger( IntToStr(i+1), "Page", 0 );
      addBookmark( book, lpp, page, parent );
   }

   delete bmarkFile;
}

////////////////////////////////////////////////////////////////////////////////
// NAME: writeBookmarks
// ARGUMENTS:  AnsiString FileName, name of ini file to write
// RETURNS: NONE
// PURPOSE: Save bookmarks to ini file
////////////////////////////////////////////////////////////////////////////////
void writeBookmarks(AnsiString FileName)
{
   if( FileExists(FileName) )
      DeleteFile( FileName );
   TIniFile *bmarkFile = new TIniFile( FileName );
   unsigned int TotalNumber = 0;
   for( unsigned int i = 0; i < bookmarks.size(); i++ )
   {
      //if the menu exists
      if( bookmarks[i].menu )
      {
         TotalNumber++;
         bmarkFile->WriteString( IntToStr(TotalNumber), "Caption", bookmarks[i].menu->Caption );
         bmarkFile->WriteString( IntToStr(TotalNumber), "Filename", bookmarks[i].filename );
         bmarkFile->WriteInteger( IntToStr(TotalNumber), "LPP", bookmarks[i].lpp );
         bmarkFile->WriteInteger( IntToStr(TotalNumber), "Page", bookmarks[i].pos );
      }
   }
   bmarkFile->WriteInteger("NUMBER","NUMBER",TotalNumber);
   delete bmarkFile;
}

////////////////////////////////////////////////////////////////////////////////
// NAME: addBookmark
// ARGUMENTS:  BOOK_INFO &book, book to bookmark
//             int lpp, the lines per page as displayed
//             unsigned int page, page being displayed
//             TMenuItem *parent, menu to add bookmark item to
// RETURNS: NONE
// PURPOSE: Create bookmark and display in menu
////////////////////////////////////////////////////////////////////////////////
void addBookmark( BOOK_INFO &book, int lpp, int page, TMenuItem *parent )
{
   BOOKMARK newBookmark;
   TMenuItem *newMenu = new TMenuItem(parent);

   //Create Menu Item and add to menu
   if( book.title.Pos( ", Page " ) > 0 )
      newMenu->Caption = book.title;
   else
      newMenu->Caption = book.title + ", Page " +IntToStr(page+1);

   numOfBookmarks++; //Increment Bookmark Counter
   newMenu->Tag = numOfBookmarks;
   parent->Add(newMenu);
   
   //Setup bookmark info
   newBookmark.menu = newMenu;
   newBookmark.lpp  = lpp;
   newBookmark.pos  = page;
   newBookmark.filename = book.filename;

   //add bookmark to list
   bookmarks.push_back(newBookmark);
}
 