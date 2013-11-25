#ifndef _BOOKS_H
#define _BOOKS_H _BOOKS_H

#include <fstream.h>
#include <vcl.h>
#include <ComCtrls.hpp>
#include <vector.h>
const    int   MONTH    =     0;
const    int   YEAR     =     0;
const    int   TITLE    =     1;
const    int   AUTHOR   =     2;
const    int   VIEW_FILENAME =     3;
const    int   GUTNUM   =     4;
const    int   BOOKLIST =     0;
const    int   SEARCHLIST  =  1;
const    int   LIBLIST     =  2;

typedef struct {
   AnsiString month;
   AnsiString year;
   AnsiString title;
   AnsiString author;
   AnsiString filename;
   AnsiString gutNum;
} BOOK_INFO;

void ReadBookList( AnsiString appPath, TListView *BookList,TProgressBar *pBar = NULL  );
void WriteBookList( AnsiString appPath );
void ListItemToBook( TListItem *Item, BOOK_INFO &book );
void BookToListItem( TListItem *Item, BOOK_INFO &book );
bool isMonth( AnsiString month );
void saveLibrary( AnsiString dir,TListItems *Items );
int  readLibrary( TListView  *LibList, AnsiString FileName, TProgressBar * pBar = NULL );
void bookToHTML( AnsiString FName,AnsiString book,AnsiString bookName,TFont *font, TColor back, int lpp,TProgressBar   *pBar  );
void bookToPages( AnsiString FName, vector<AnsiString> &book, int lpp, TProgressBar *pBar, bool AddNewLine, bool StripHeader );
BOOK_INFO getBookInfo( AnsiString FName );
void GetBookHeading( ifstream &book );

#endif
