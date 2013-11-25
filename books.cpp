////////////////////////////////////////////////////////////////////////////////
// bookmark.cpp
// David Bracewell
// 5/2001
////////////////////////////////////////////////////////////////////////////////
#include "books.h"
#include "gutInfoDisplay.h"
#include <fstream.h>
#include <vector.h>

////////////////////////////////////////////////////////////////////////////////
// NAME: GetBookHeading
// ARGUMENTS:  ifstream &book, input file stream of opened book
// RETURNS: NONE
// PURPOSE: get and display gutenberg header information
////////////////////////////////////////////////////////////////////////////////
void GetBookHeading( ifstream &book )
{
   char buff[255];
   AnsiString heading="";
   do
   {
      book.getline( buff, sizeof(buff) );
      heading += AnsiString(buff) +"\n";
   }while( (AnsiString(buff).UpperCase().Pos("*END") == 0) && (AnsiString(buff).UpperCase().Pos("*** START")==0) && (!book.eof()) );

   //Display Heading
   TfrmGutInfo * info = new TfrmGutInfo( Application );
   info->memInfo->Lines->Text = heading;
   info->ShowModal();
   delete info;
   Application->ProcessMessages();
}

////////////////////////////////////////////////////////////////////////////////
// NAME: bookToPages
// ARGUMENTS:  AnsiString FName, filename of the book to open
//             vector<AnsiString> &book, stores pages of book
//             int lpp, the lines per page
//             TProgressBar *pBar, progress bar to show progress of opening
//             bool AddNewLine, true keep original formating, false make book
//                               wordwrappable
// RETURNS: NONE
// PURPOSE: Open book and read in pages based on the number of lines per page
////////////////////////////////////////////////////////////////////////////////
void bookToPages( AnsiString FName,
                  vector<AnsiString> &book,
                  int lpp,
                  TProgressBar *pBar,
                  bool AddNewLine,
                  bool StripHeader )
{
   int lineCount;
   AnsiString tempPage;
   if( lpp <= 0 ) lpp = 30;
   //Get total Number of lines
   TStringList *Testing = new TStringList();
   Testing->LoadFromFile( FName);
   pBar->Position = 0;
   pBar->Max = Testing->Count+1;
   delete Testing;

   //Clear Book
   book.clear();

   ifstream inFile( FName.c_str() );
   char buff[255];
   if( inFile.fail() ) return;

   //Get and Display Heading
  if( StripHeader )
      GetBookHeading( inFile );

   while( !inFile.eof() )
   {
      tempPage="";
      for( lineCount = 0; (!inFile.eof()) && (lineCount < lpp); )
      {
         pBar->Position++;
         inFile.getline(buff,sizeof(buff));
         if( (lineCount != 0) || (Trim(buff) != "") )
         {
            tempPage += Trim(AnsiString(buff));

            if(  AddNewLine )
               tempPage += "\n";
            else
            {
               if( Trim(buff) == "" )
                  tempPage += "\n";
               else
                  tempPage += " ";
            }
            lineCount++;
         }
      }
      book.push_back( tempPage );
   }
   inFile.close();
}
////////////////////////////////////////////////////////////////////////////////
// NAME: bookToHTML
// ARGUMENTS:  AnsiString FName, filename of HMTL File to save to
//             AnsiString book,  filename of book to open
//             AnsiString bookName, Book title
//             TFont * font,  font information for HTML File
//             TColor back,   background color of HTML File
//             int lpp, lines per page
//             TProgressBar *pBar, show progress of exporting
// RETURNS: NONE
// PURPOSE: Open read and export book to HTML File
////////////////////////////////////////////////////////////////////////////////
void bookToHTML(  AnsiString     FName,
                  AnsiString     book,
                  AnsiString     bookName,
                  TFont       *  font,
                  TColor         back,
                  int            lpp,
                  TProgressBar   *pBar )
{
   ofstream htmlFile(FName.c_str());   //HTML File
   ifstream bFile;                     //Book File
   int pageCount = 0;                  //Number of pages
   char buff[255];                     //Input buffer
   AnsiString FontC;                   //Font Color
   AnsiString BackC;                   //Background Color

   //--- Make sure the lines per page is valid ---
   if( lpp <= 0 ) lpp = 30;

   //--- Get total Number of lines ---
   TStringList *Testing = new TStringList();
   Testing->LoadFromFile( book );
   pBar->Position = 0;
   pBar->Max = Testing->Count+1;
   delete Testing;

   //--- Open book and check for errors ---
   bFile.open( book.c_str() );
   if( htmlFile.fail() || bFile.fail() )
      return;

   //--- Get and Display Book Heading ---
   GetBookHeading( bFile );

   //--- Convert Font Color and Background Color to a hex string ---
   FontC  = "#" + IntToHex( GetRValue( font->Color ), 2 )+
                  IntToHex( GetGValue( font->Color ), 2 ) +
                  IntToHex( GetBValue( font->Color ), 2 );
   BackC  = "#" + IntToHex( GetRValue( back ), 2 )+
                  IntToHex( GetGValue( back ), 2 ) +
                  IntToHex( GetBValue( back ), 2 );


   //--- Write the HTML Header and CSS Info ---
   htmlFile << "<HTML>\n"
            << "<HEAD>\n"
            << "<TITLE>" << bookName.c_str()<< "</TITLE>\n"
            << "<style type=\"text/css\"><!--td { font-family: \""
            << font->Name.c_str() << "\"; font-size: "
            << IntToStr(font->Size).c_str() << "pt; color: "
            << FontC.c_str() << "}--></style>\n"
            << "</HEAD>\n<BODY BGCOLOR=\""
            << BackC.c_str() << "\">\n<TABLE border=0 cellspacing=50>\n";

   ////////////////////////////////
   // Go Through the the book file
   // and create html for book
   ///////////////////////////////
   while( !bFile.eof() )
   {
      int lCount = 0;
      pageCount++;

      //Write Table row information
      htmlFile << "<TR><TD WIDTH=\"50%\" ALIGN=\"LEFT\" VALIGN=\"Top\">\n";

      //Get and Create Left side (odd numbered) page
      while( !bFile.eof() && (lCount < lpp) )
      {
         bFile.getline(buff,sizeof(buff));
         if( lCount > 0 || Trim(buff)!="")
         {
            htmlFile << buff << "<BR>\n" ;
            lCount++;
         }
         pBar->Position++;
      }

      //Write Table information
      htmlFile << "<CENTER> " << IntToStr(pageCount).c_str()
               << "</CENTER>\n</TD>\n<TD WIDTH=\"50%\" ALIGN=\"LEFT\" VALIGN=\"Top\">\n" ;

      pageCount++;
      //Get and Create right size (even numbered) page
      lCount = 0;
      while( !bFile.eof() && (lCount < lpp) )
      {
         bFile.getline(buff,sizeof(buff));
         if( lCount > 0 || Trim(buff)!="")
         {
            htmlFile << buff << "<BR>\n";
            lCount++;
         }
         pBar->Position++;
      }

      //Write end of row information
      htmlFile << "<CENTER> " << IntToStr(pageCount).c_str()
               << "</CENTER>\n</TD>\n</TR>\n";
   }

   //Write HTML Footer and close files
   htmlFile << "</TABLE>\n</BODY>\n</HTML>\n";
   bFile.close();
   htmlFile.close();
}


////////////////////////////////////////////////////////////////////////////////
// NAME: getBookInfo
// ARGS: AnsiString FName - name of ebook
// RETURNS: BOOK_INFO - Information about the ebook
// PURPOSE: Read in a ebook and gather information about it
////////////////////////////////////////////////////////////////////////////////
BOOK_INFO getBookInfo( AnsiString FName )
{
   TStringList *eBook = new TStringList(); //Holds Book in which to insert and auto fill
   AnsiString  gutJunk1 = "The Project Gutenberg Etext of";
   AnsiString  gutJunk2 = "The Project Gutenberg Etext";
   AnsiString  gutJunk3 = "Project Gutenberg Etext";
   AnsiString  gutJunk4 = "Project Gutenberg's Original";
   AnsiString  gutJunk5 = "Project Gutenberg's";
   AnsiString  gutJunk6 = "The Project Gutenberg EBook of";
   BOOK_INFO   rBook;
   int pos;
   AnsiString tStr;

   eBook->LoadFromFile(FName);   //Load Book

  // Get Title depending on Header
   rBook.title = "";
   if( (pos = eBook->Strings[0].UpperCase().Pos( gutJunk1.UpperCase() )) > 0 )
   {
      rBook.title = (eBook->Strings[0].SubString(pos+gutJunk1.Length(),eBook->Strings[0].Length()).Trim() );
   }
   else if( (pos = eBook->Strings[0].UpperCase().Pos( gutJunk2.UpperCase() )) >0)
   {
      rBook.title = (eBook->Strings[0].SubString(pos+gutJunk2.Length(),eBook->Strings[0].Length()).Trim() );
   }
   else if( (pos = eBook->Strings[0].UpperCase().Pos( gutJunk3.UpperCase() )) >0)
   {
      rBook.title = (eBook->Strings[0].SubString(pos+gutJunk3.Length(),eBook->Strings[0].Length()).Trim() );
   }
   else if( (pos = eBook->Strings[0].UpperCase().Pos( gutJunk4.UpperCase() )) >0)
   {
      rBook.title = (eBook->Strings[0].SubString(pos+gutJunk4.Length(),eBook->Strings[0].Length()).Trim() );
   }
   else if( (pos = eBook->Strings[0].UpperCase().Pos( gutJunk5.UpperCase() )) >0)
   {
      rBook.title = (eBook->Strings[0].SubString(pos+gutJunk5.Length(),eBook->Strings[0].Length()).Trim() );
   }
   else if( (pos = eBook->Strings[0].UpperCase().Pos( gutJunk6.UpperCase() )) >0)
   {
      rBook.title = (eBook->Strings[0].SubString(pos+gutJunk6.Length(),eBook->Strings[0].Length()).Trim() );
   }
   //Find Author Name
   rBook.author = "";
   tStr = eBook->Strings[0].UpperCase();
   if( (pos = tStr.Pos( ", BY" )) > 0 )
   {
      rBook.author = Trim( eBook->Strings[0].SubString( pos+4, eBook->Strings[0].Length() ));
   }
   else if( (pos = tStr.Pos( "BY," )) > 0 )
   {
      rBook.author = Trim( eBook->Strings[0].SubString( pos+3, eBook->Strings[0].Length() ));
   }
   else if( (pos = tStr.Pos( "BY " )) > 0 )
   {
      rBook.author = Trim( eBook->Strings[0].SubString( pos+3, eBook->Strings[0].Length() ));
   }
   else
   {
      tStr = eBook->Strings[1].UpperCase();
      if( (pos = tStr.Pos( ", BY" )) > 0 )
      {
         rBook.author = Trim( eBook->Strings[1].SubString( pos+4, eBook->Strings[1].Length() ));
      }
      else if( (pos = tStr.Pos( "BY," )) > 0 )
      {
         rBook.author = Trim( eBook->Strings[1].SubString( pos+3, eBook->Strings[1].Length() ));
      }
      else if( (pos = tStr.Pos( "BY " )) > 0 )
      {
         rBook.author = Trim( eBook->Strings[1].SubString( pos+3, eBook->Strings[1].Length() ));
      }
   }

   rBook.month = "";
   int i;
   for( i = 0; (i < eBook->Count) && (i<250); i++ )
   {
       if( eBook->Strings[i].Pos("January") > 0 )
       {
         rBook.month = "Jan";
         break;
       }
       else if( eBook->Strings[i].Pos("Feburary")  > 0 )
       {
         rBook.month = "Feb";
         break;
       }
       else if( eBook->Strings[i].Pos("March") > 0 )
       {
         rBook.month = "Mar";
         break;
       }
       else if( eBook->Strings[i].Pos("April") > 0 )
       {
         rBook.month = "Apr";
         break;
       }
       else if( eBook->Strings[i].Pos("May") > 0 )
       {
         rBook.month = "May";
         break;
       }
       else if( eBook->Strings[i].Pos("June") > 0 )
       {
         rBook.month = "Jun";
         break;
       }
       else if( eBook->Strings[i].Pos("July") > 0 )
       {
         rBook.month = "Jul";
         break;
       }
       else if( eBook->Strings[i].Pos("August") > 0 )
       {
         rBook.month = "Aug";
         break;
       }
       else if( eBook->Strings[i].Pos("September") > 0 )
       {
         rBook.month = "Sep";
         break;
       }
       else if( eBook->Strings[i].Pos("October") > 0 )
       {
         rBook.month = "Jan";
       }
       else if( eBook->Strings[i].Pos("November") > 0 )
       {
         rBook.month = "Nov";
         break;
       }
       else if( eBook->Strings[i].Pos("December") > 0 )
       {
         rBook.month = "Dec";
         break;
       }
   }
   pos = eBook->Strings[i].Pos(",");
   if( pos > 0 )
      rBook.year = Trim(eBook->Strings[i].SubString( pos+1,5 ));

   rBook.filename = FName;
   rBook.gutNum = "0";
   delete eBook;
   return rBook;
}

////////////////////////////////////////////////////////////////////////////////
// NAME: WriteBookList
// ARGUEMENTS: AnsiString appPath, application's path
// RETURN: NONE
// PURPOSE: Open Gutenberg provided list and convert to internal format. Then
//          write Book list to file.
////////////////////////////////////////////////////////////////////////////////
void WriteBookList( AnsiString appPath )
{
   TStringList  *bookFile = new TStringList();        //File downloaded from gutenberg
   ofstream out( (appPath+"books.dat").c_str() );     //ouputed internal format
   AnsiString   curLine;                              //current line processing
   char *       token;                                //current line token
   AnsiString   buffer;                               //buffer

   //--- Load Gutenberg provided file ---
   bookFile->LoadFromFile( appPath +"books.txt" );

   //--- Go through Gutenberg file and convert to internal format ---
   for( int i = 0; i < bookFile->Count; i++ )
   {
      Application->ProcessMessages();     //Update GUI

      //--- Set Current line and trim it---
      curLine = Trim(bookFile->Strings[i]);

      if( curLine != "" )
      {  //--- Line is not blank ---

         //--- Get first token ---
         token = strtok( curLine.c_str(), " " );

         if( isMonth( AnsiString( token ) ) )
         {  //--- It is a month this is an entry ---
            BOOK_INFO book;         //This book's info

            //--- Set Month ---
            book.month = token;

            //--- Get Year ---
            token = strtok( NULL, " " );
            if(token)
               book.year = token;
            else
               book.year = "";

            //--- Get Book Title ---
            token = strtok( NULL, "[" );
            if( token )
               book.title = token;
            else
               book.title = "";

            //--- Get Author ---
            //First check for a comma and by
            int start = book.title.Pos(", by");
            if(start > 0 )
               book.author = Trim( book.title.SubString( start+4, book.title.Length() ));
            else
            {
               //If first check fails try just "by"
               start = book.title.Pos("by ");
               if(start > 0 )
                  book.author = Trim(book.title.SubString( start+3, book.title.Length() ));
               else
               {
                  //Finnaly check for comma
                  start = book.title.Pos(",");
                  if(start > 0 )
                     book.author = Trim(book.title.SubString( start+1, book.title.Length() ));
               }
            }

            //--- Add Filename ---
            buffer = Trim( curLine.SubString( 61, 12 ) );
            //#if( buffer != "" && buffer[buffer.Length()] == ']' )
            //#   buffer = Trim( curLine.SubString( curLine.Length()-17, 12 ) );
            //#if( (buffer.Length() >= 1) && (buffer[1] == '[') )
            //#   buffer = Trim( buffer.SubString( 2, buffer.Length() ));

            book.filename = buffer;

            //--- Add Gutenberg Book # ---
            buffer = Trim(curLine.SubString( 74, 8 ));
            if( buffer.Length() >= 1 && isdigit(buffer[1]) )
               book.gutNum = buffer;
            else
               book.gutNum = "0";

            //--- Only output book if it has a title ---
            if( book.title != "" )
            {
               //--- output all info making sure there is something there ---
               out << book.month.c_str() << "\t" << book.year.c_str() << "\t"<<
                   Trim(book.title).c_str() << "\t";
               if( book.author != "" )
                  out << Trim(book.author).c_str() << "\t";
               else
                  out << " \t";

               out << book.filename.c_str() << "\t";
               if( Trim(book.gutNum) == "" )
                  out << "0" << endl;
               else
                  out << book.gutNum.c_str() << endl;
            }
         }
      }
   }
   out.close();
   delete bookFile;
   DeleteFile( appPath+"books.txt" );  //get rid of gutenberg file
}

////////////////////////////////////////////////////////////////////////////////
// NAME: readLibrary
// ARGS: TListViewe *LibList, Listview to display myLibrary
//       AnsiString FileName, Filename of library
//       TProgressBar *pBar, Shows status of reading
// RETURNS: NONE
// PURPOSE: Read in and display library
////////////////////////////////////////////////////////////////////////////////
int  readLibrary( TListView  *LibList, AnsiString FileName, TProgressBar * pBar )
{
   ifstream lib( FileName.c_str() );
   int num;
   TListItem *item;
   char *token;
   char buffer[1000];

   if( lib.fail() )
   {
      ShowMessage( "ERROR: Opening Lib File" );
      return-1;
   }

   lib.getline(buffer, sizeof(buffer) );
   num = StrToInt(buffer);

   if( pBar )
   {
      pBar->Position = 0;
      pBar->Max = num;
   }
   for( int i = 0; i < num; i++ )
   {
      Application->ProcessMessages();
      if( pBar ) pBar->Position = i;
      item = LibList->Items->Add();
      lib.getline(buffer,sizeof(buffer));

      //Get Month
      token = strtok( buffer, "\t" );
      item->Caption = Trim(token);

      //Get Year
      token = strtok( NULL, "\t" );
      item->SubItems->Add(Trim(token));

      //Get Title
      token = strtok( NULL, "\t" );
      item->SubItems->Add(Trim(token));

      //Get Author
      token = strtok( NULL, "\t" );
      item->SubItems->Add(Trim(token));

      //Get Filename
      token = strtok( NULL, "\t" );
      item->SubItems->Add(Trim(token));

      //Get Gutenberg Number
      token = strtok( NULL, "\t\n" );
      item->SubItems->Add(Trim(token));
  }
   lib.close();
   return num;
}

////////////////////////////////////////////////////////////////////////////////
// NAME: saveLibrary
// ARGS: AnsiString dir - directory to store library
//       TListItems *Items- Books to save
// RETURNS: NONE
// PURPOSE: Write a list of books to file
////////////////////////////////////////////////////////////////////////////////
void saveLibrary( AnsiString dir, TListItems *Items )
{
   ofstream lib( (dir+"lib.dat").c_str() );
   AnsiString author;

   if( lib.fail() )
   {
      ShowMessage( "ERROR: Opening Lib File" );
      return;
   }

   lib << Items->Count << endl;
   //Write all Library Entries
   for( int i = 0; i < Items->Count; i++ )
   {
      Application->ProcessMessages();
      //Check If this book has an author if it doesn't then
      //we need to print a space out to library file
      if( Items->Item[i]->SubItems->Strings[AUTHOR] == "" )
         author = " ";
      else
         author = Items->Item[i]->SubItems->Strings[AUTHOR];

      lib << Items->Item[i]->Caption.c_str() << " " << "\t" <<
             Items->Item[i]->SubItems->Strings[YEAR].c_str() << " "<< "\t" <<
             Items->Item[i]->SubItems->Strings[TITLE].c_str()<< " " << "\t" <<
             author.c_str() << " "<<"\t" <<
             Items->Item[i]->SubItems->Strings[VIEW_FILENAME].c_str() << "\t" <<
             Items->Item[i]->SubItems->Strings[GUTNUM].c_str() << endl;
   }

   lib.close();
}

////////////////////////////////////////////////////////////////////////////////
// NAME: ReadBookList
// ARGS: AnsiString appPath, applicaton path
//       TListView *BookList, List to display books available from gutenberg
//       TProgressBar *pBar, Shows Status
// RETURNS: NONE
// PURPOSE: Read in booklist and display
////////////////////////////////////////////////////////////////////////////////
void ReadBookList( AnsiString appPath, TListView *BookList, TProgressBar *pBar )
{
   TStringList *bookFile = new TStringList();
   AnsiString   curLine;
   char *       token;
   AnsiString   buffer;

   bookFile->LoadFromFile( appPath+"books.dat" );
   if( pBar != NULL )
   {
      pBar->Min = 0;
      pBar->Max = bookFile->Count - 1;
   }

   for( int i = 0; i < bookFile->Count; i++ )
   {
      Application->ProcessMessages();
      if(pBar) pBar->Position = i;
      curLine = bookFile->Strings[i];
      curLine = curLine.Trim();
      if( curLine != "" )
      {
         BOOK_INFO book;
         token = strtok( curLine.c_str(), "\t" );
         book.month = token;
         token = strtok( NULL, "\t" );
         book.year = token;
         token = strtok( NULL, "\t" );
         book.title = token;
         token = strtok( NULL, "\t" );
         book.author = token;
         token = strtok( NULL, "\t" );
         book.filename = token;
         token = strtok( NULL, "\t" );
         book.gutNum = token;
         TListItem *lv;
         lv = BookList->Items->Add();
         BookToListItem( lv, book );
      }
   }
   delete bookFile;
}

////////////////////////////////////////////////////////////////////////////////
// NAME: isMonth
// ARGS: AnsiString month - string to determine if is month
// RETURNS: bool, true is month, false is not
// PURPOSE: Determine if a string is a month or not
////////////////////////////////////////////////////////////////////////////////
bool isMonth( AnsiString month )
{
   AnsiString months[] = {"Jan","Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug","Sep","Oct","Nov","Dec" };
   for( int i = 0; i < 12; i++ )
      if( month == months[i] )
         return true;

   return false;
}

////////////////////////////////////////////////////////////////////////////////
// NAME: ListItemToBook
// ARGS: TListItem * Item, Item to convert
//       BOOK_INFO &book, Saves converted item here
// RETURNS: NONE
// PURPOSE: Convert list item to book info
////////////////////////////////////////////////////////////////////////////////
void ListItemToBook( TListItem *Item, BOOK_INFO &book )
{
   book.month     =  Item->Caption;
   book.year      =  Item->SubItems->Strings[YEAR];
   book.title     =  Item->SubItems->Strings[TITLE];
   book.author    =  Item->SubItems->Strings[AUTHOR];
   book.filename  =  Item->SubItems->Strings[VIEW_FILENAME];
   book.gutNum    =  Item->SubItems->Strings[GUTNUM];
}

////////////////////////////////////////////////////////////////////////////////
// NAME: BookTostItem
// ARGS: TListItem * Item, Saves converted book here
//       BOOK_INFO &book, book to convert
// RETURNS: NONE
// PURPOSE: Convert  book info to list item
////////////////////////////////////////////////////////////////////////////////
void BookToListItem( TListItem *Item, BOOK_INFO &book )
{
   Item->Caption  =  book.month;
   Item->SubItems->Add( book.year      );
   Item->SubItems->Add( book.title     );
   Item->SubItems->Add( book.author    );
   Item->SubItems->Add( book.filename  );
   Item->SubItems->Add( book.gutNum    );
}


