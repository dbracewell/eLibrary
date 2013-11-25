////////////////////////////////////////////////////////////////////////////////
// bookmark.h
// David Bracewell
// 5/2001
////////////////////////////////////////////////////////////////////////////////

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <vcl.h>
#include <iniFiles.hpp>
#include <vector.h>
#include "books.h"

typedef struct {
   TMenuItem * menu;
   int pos;
   int lpp;
   AnsiString filename;
} BOOKMARK;

void writeBookmarks(AnsiString FileName);
void readBookmarks(AnsiString FileName, TMenuItem *parent );
void addBookmark( BOOK_INFO &book, int lpp, int page,TMenuItem *parent );


#endif