////////////////////////////////////////////////////////////////////////////////
// eLib.cpp
// David B. Bracewell
// 3/2001 - Rewrite of Delphi Application
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "eLib.h"
#include "splash.h"
#include "about.h"
#include "FTP.h"
#include "downFile.h"
#include "books.h"
#include <stdio.h>
#include <registry.hpp>
#include <forms.hpp>
#include "editbook.h"
#include "options.h"
#include <vector.h>
#include "fullscreen.h"
#include "bookmarks.h"
#include "RemBookmarks.h"
#include "find.h"
#include <Printers.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TmainForm *mainForm;

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
//True Searching Gutenberg, False Searching My Library
bool GutSearch = true;
//True in Fullscreen mode, false not
bool inFullScreen =false;
//Contains pages of the open book
vector< AnsiString > openBook;
//Current page number
unsigned int curPage;
//All the bookmarks
extern vector<BOOKMARK> bookmarks;
//number of bookmarks
extern int numOfBookmarks ;

////////////////////////////////////////////////////////////////////////////////
// Function Declearations
////////////////////////////////////////////////////////////////////////////////
int __stdcall CustomSortProc(long Item1, long Item2, long ParamSort);

//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// TmainForm
// mainForm Construtor
////////////////////////////////////////////////////////////////////////////////
__fastcall TmainForm::TmainForm(TComponent* Owner)
   : TForm(Owner)
{
   //Set Shortcuts for next page and previous page
   NextPage->ShortCut = ShortCut( VK_NEXT ,TShiftState()<<ssCtrl);
   PreviousPage->ShortCut = ShortCut( VK_PRIOR ,TShiftState()<<ssCtrl);

   //Set Book Reader Variables
   curBook = NULL;
   curPage = 0;

   //Load Default Settings
   mySettings.HTMLFont = new TFont();
   mySettings.AppFont = new TFont();
   mySettings.ReaderFont = new TFont();
   mySettings.appPath = IncludeTrailingBackslash( ExtractFilePath( Application->ExeName ) );
   mySettings.HTMLFont->Assign( this->Font );
   mySettings.AppFont->Assign( this->Font );
   mySettings.ReaderFont->Assign( this->Font);
   mySettings.ReaderBack = clWindow;
   mySettings.HTMLBack = clWhite;
   mySettings.bookDir = mySettings.appPath+"books\\";
   mySettings.server.sAppName = ExtractFileName( Application->ExeName );
   mySettings.server.server = "ftp.cdrom.com";
   mySettings.server.username = "anonymous";
   mySettings.server.password = "email@host.com";
   mySettings.server.directory = "/pub/gutenberg/";
   mySettings.ReaderLpp = 30;
   mySettings.HTMLLpp = 30;

   //Setup Progress Bar to be in Statusbar
   //and get rid of it's border
   pbarStatus->Parent = StatusBar;
   pbarStatus->Top =  4;
   pbarStatus->Left = 2;
   pbarStatus->Width = StatusBar->Panels->Items[0]->Width-4;
   pbarStatus->Height = 13;
   long int ProgressBarStyle;
   ProgressBarStyle = GetWindowLong(pbarStatus->Handle,GWL_EXSTYLE);
   ProgressBarStyle = ProgressBarStyle - WS_EX_STATICEDGE;
   SetWindowLong(pbarStatus->Handle, GWL_EXSTYLE, ProgressBarStyle);
   pbarStatus->Visible = false;

   //Read Bookmarks and assign onClick operator
   readBookmarks( mySettings.appPath+"bookmarks.dat", mnuBookmarks);
   for( unsigned int i = 0; i < bookmarks.size(); i++ )
      bookmarks[i].menu->OnClick = BookmarkClick;

   //Setup search information
   cboSearchType->ItemIndex = 0;
   for( int i = 0; i < 3; i++ )
      sortInfo[i].sorted = false;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuFileExit Click
// Close Application
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuFileExitClick(TObject *Sender)
{  Close(); }
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuHelpAbout Click
// creates and shows an about box
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuHelpAboutClick(TObject *Sender)
{
   TAboutBox *abox = new TAboutBox( Application );
   abox->ShowModal();
   delete abox;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuViewToolbar Click
// shows/hides toolbar
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuViewToolbarClick(TObject *Sender)
{
   ToolBar->Visible = !ToolBar->Visible;
   mnuViewToolbar->Checked = ToolBar->Visible;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// DownloadBookList Execute
// Download Book list
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::DownloadBookListExecute(TObject *Sender)
{

HTTP_GetFile(mySettings.server,"http://www.gutenberg.org/dirs/GUTINDEX.ALL", "C:\\Test.txt");

/*   TfrmDownload *download = new TfrmDownload(Application,"GUTINDEX.ALL",&mySettings.server );
   download->ShowModal();
   delete download;
   StatusBar->Panels->Items[1]->Text =  IntToStr( BookList->Items->Count ) + " Books Available"; */
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// CustomSortProc
// Custom Sort Function for All ListViews. Does Descending Order
////////////////////////////////////////////////////////////////////////////////
int __stdcall CustomSortProc(long Item1, long Item2,  long ParamSort)
{
   TListItem *i1 = (TListItem *)Item1;
   TListItem *i2 = (TListItem *)Item2;

   if( ParamSort == MONTH )
   {
      return -CompareText( i1->Caption, i2->Caption );
   }
   else if( ParamSort == (GUTNUM+1) )
   {
      //Convert The Gutenberg Number string into a number
      //and then compare
      AnsiString s1 = i1->SubItems[0].Strings[GUTNUM];
      AnsiString s2 = i2->SubItems[0].Strings[GUTNUM];

      //Trims any non digits off the end
      if( !s1.IsEmpty() && !isdigit(s1[s1.Length()]) )
         s1 = s1.SubString(1,s1.Length()-1);
      if( !s2.IsEmpty() && !isdigit(s2[s2.Length()]) )
         s2 = s2.SubString(1,s2.Length()-1);

      //Make Sure there is something there
      if( Trim(s1) == "" )
         s1 = "0";
      if( Trim(s2) == "" )
         s2 = "0";
      return StrToInt(s1) < StrToInt(s2);
   }
   else
      return -CompareText( i1->SubItems[0].Strings[ParamSort - 1],
                           i2->SubItems[0].Strings[ParamSort - 1] );
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// ListViewCompare
// Compare Function for All ListView's Alphasort
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::ListViewCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{

   if( curCol == 0 )
      Compare = CompareText(  Item1->Caption,
                              Item2->Caption );
   else if( curCol == (GUTNUM+1) )
   {
      //Convert The Gutemberg Number string into a number
      //and then compare
      AnsiString s1 = Item1->SubItems[0].Strings[GUTNUM];
      AnsiString s2 = Item2->SubItems[0].Strings[GUTNUM];

      //Trims any non digits off the end
      if( !s1.IsEmpty() && !isdigit(s1[s1.Length()]) )
         s1 = s1.SubString(1,s1.Length()-1);
      if( !s2.IsEmpty() && !isdigit(s2[s2.Length()]) )
         s2 = s2.SubString(1,s2.Length()-1);
       if( Trim(s1) == "" )
         s1 = "0";
      if( Trim(s2) == "" )
         s2 = "0";

      Compare = StrToInt(s1) > StrToInt(s2);
   }
   else
      Compare = CompareText( Item1->SubItems[0].Strings[curCol - 1],
                             Item2->SubItems[0].Strings[curCol - 1] );
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuViewGridLines Click
// Turns on and off listviews gridlines
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuViewGridLinesClick(TObject *Sender)
{
   mnuViewGridLines->Checked  = !mnuViewGridLines->Checked;
   BookList->GridLines        =  mnuViewGridLines->Checked;
   SearchList->GridLines      =  mnuViewGridLines->Checked;
   MyLibraryList->GridLines   =  mnuViewGridLines->Checked;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuViewStatusBar Click
// Shows/Hides Statusbar
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuViewStatusBarClick(TObject *Sender)
{
      mnuViewStatusBar->Checked  = !mnuViewStatusBar->Checked;
      StatusBar->Visible         =  mnuViewStatusBar->Checked;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// DownloadBookActionExecute
// Download Currently selceted book
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::DownloadBookActionExecute(TObject *Sender)
{
   TListView * useMe;
   AnsiString filename;
   AnsiString directory;
   AnsiString bookYear;
   AnsiString bookName;

   //--- Determine which listview contains book to download ---
   if( WorkArea->ActivePageIndex == BookListTab->TabIndex )
      useMe = BookList;
   else
      useMe = SearchList;

   //--- if there is no book selected the exit ---
   if( !useMe->Selected )
      return;

   //--- Store old directory info ---
   directory = mySettings.server.directory;

   //--- build information about the book ---
   filename = useMe->Selected->SubItems->Strings[VIEW_FILENAME];
   bookYear = useMe->Selected->SubItems->Strings[YEAR];
   bookName = useMe->Selected->SubItems->Strings[TITLE];

   //--- download the book
   TfrmDownload *download = new TfrmDownload(Application,filename,bookYear,bookName,&mySettings.server );
   download->ShowModal();
   delete download;

   //---- reset to the origrinal directory ---
   mySettings.server.directory = directory;

}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// cmdClearResultsClick
// Clear Items from Search List
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::cmdClearResultsClick(TObject *Sender)
{
   SearchList->Items->Clear();
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// cmdClearCriteriaClcik
// Clears all Critera Edit Boxs
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::cmdClearCriteriaClick(TObject *Sender)
{
   edtSearchAuthor->Text = "";
   edtSearchMonth->Text = "";
   edtSearchYear->Text = "";
   edtSearchGutNum->Text = "";
   edtSearchTitle->Text = "";
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// cmdSearchClick
// Perform Search for books in Book List or My Library based on
// given criteria
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::cmdSearchClick(TObject *Sender)
{
   TListView *use;      //The Listview to search in
   bool found[5] = {true,true,true,true,true};    //Critera matches
   TListItem *item;      //Item added to Search list
   TListItem *curItem;   //Current Item

   //Determine which Listview to search
   if( cboSearchType->ItemIndex == 0 )
      use = BookList;
   else
      use = MyLibraryList;

   //Set Global GutSearch: True Searching Gutenberg,
   //False Searching My Library
   GutSearch = (use == BookList );

   //Set Popup Menu based on What we are searching
   if( GutSearch )
      SearchList->PopupMenu = BookListPopUp;
   else
      SearchList->PopupMenu = popMyLibrary;

   //Begin the Search List Update and clear any previous results
   SearchList->Items->BeginUpdate();
   SearchList->Items->Clear();
   Screen->Cursor = crHourGlass;

   //Go Through all Items
   for( int i = 0; i < use->Items->Count;  i++ )
   {
      curItem = use->Items->Item[i];

      //Check that The current Item matches all criteria
      if( edtSearchAuthor->Text != "" )
      {
        if( curItem->SubItems->Strings[AUTHOR].UpperCase().Pos(edtSearchAuthor->Text.UpperCase())  )
         found[0] = true;
        else
         found[0] = false;
      }

      if( edtSearchTitle->Text != "" )
      {
         if( curItem->SubItems->Strings[TITLE].UpperCase().Pos(edtSearchTitle->Text.UpperCase()) )
           found[1] = true;
         else
            found[1] = false;
      }

      if( edtSearchYear->Text != "" )
      {
         if( curItem->SubItems->Strings[YEAR].UpperCase().Pos(edtSearchYear->Text.UpperCase()) )
           found[2] = true;
         else
            found[2] = false;
      }

      if( edtSearchGutNum->Text != "" )
      {
         if( curItem->SubItems->Strings[GUTNUM].UpperCase().Pos(edtSearchGutNum->Text.UpperCase()) )
           found[3] = true;
         else
            found[3] = false;
      }

      if( edtSearchMonth->Text != "" )
      {
         if( curItem->Caption.UpperCase().Pos(edtSearchMonth->Text.UpperCase()) )
           found[4] = true;
         else
            found[4] = false;
      }

      //If all of the matches are true then add this item to
      //the Search List
      if( found[0] && found[1] && found[2] && found[3] && found[4] )
      {
         item = SearchList->Items->Add();
         item->Assign( use->Items->Item[i] );
      }
   }

   //End updating Search List
   SearchList->Items->EndUpdate();
   Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// DownloadBookListUpdate
// Default update procedure for Download Book list and Download Book
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::DownloadBookListUpdate(TObject *Sender)
{
   bool result;
   result = WorkArea->ActivePageIndex == 0 ||
            (WorkArea->ActivePageIndex == 1 && GutSearch);

   ((TAction*)Sender)->Enabled = result;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// SortListView
// Determine what sort to do and perform it
////////////////////////////////////////////////////////////////////////////////
void TmainForm::SortListView( TListView * sortMe, // ListView to sort
                              int col,            // Column of list view
                              int listId)         // id for listview
{
   //--Show Hourglass cursor --
   Screen->Cursor = crHourGlass;

   if(   (!sortInfo[listId].sorted) ||    //The Listview has not been sorted
         (sortInfo[listId].col != col) || //The column clicked does not equal the one already sorted
         (!sortInfo[listId].ascend) )     //The column is sorted in reverse order
   {
      //--- Sort Column in Ascending order ---
      sortInfo[listId].sorted = true;
      sortInfo[listId].col = col;
      sortInfo[listId].ascend = true;
      curCol = sortInfo[listId].col;
      sortMe->AlphaSort();
   }
   else
   {
      //--- Sort Column in Descending order ---
      sortInfo[listId].col = col;
      sortInfo[listId].ascend = false;
      curCol = sortInfo[listId].col;
      sortMe->CustomSort( CustomSortProc,col);
   }

   //-- Restore cursor
   Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// FormClose
// Write Bookmarks, Library, and Settings. Also free memory for bookmarks and
// fonts.
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
   //--- Write Bookmarks and then free the memory for the menues ---
   writeBookmarks( mySettings.appPath+"bookmarks.dat" );
   for( unsigned int i =0; i < bookmarks.size(); i++)
   {
      if( bookmarks[i].menu )
      {
         mnuBookmarks->Delete( mnuBookmarks->IndexOf(bookmarks[i].menu) );
         delete bookmarks[i].menu;
      }
   }

   //--- Free memory for Fonts ---
   delete mySettings.HTMLFont;
   delete mySettings.AppFont;
   delete mySettings.ReaderFont;

   //-- Write Settings and save my Library
   WriteRegSettings( mySettings, this );
   saveLibrary( mySettings.appPath,MyLibraryList->Items );
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// popSearchForAuthorClick
// Searches for author of selected book in my Library list
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::DeleteBookExecute(TObject *Sender)
{
   if( MyLibraryList->Selected ) //Make sure something is selected
   {
      //Check if file exists if it does then delete file
      //and then remove from list view
      AnsiString FName = mySettings.bookDir + MyLibraryList->Selected->SubItems->Strings[VIEW_FILENAME];
      if( FileExists(FName) )
         DeleteFile(FName);
      MyLibraryList->Items->Delete( MyLibraryList->Selected->Index );
      StatusBar->Panels->Items[2]->Text = IntToStr( MyLibraryList->Items->Count ) + " In Library";
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// EditBookExecute
// Create and show edit book dialog and if changed display changes
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::EditBookExecute(TObject *Sender)
{
   //--- if nothing is selected exit ---
   if( !MyLibraryList->Selected ) return;

   BOOK_INFO b;   // Book to be edited

   //--- Get Book Information ---
   ListItemToBook( MyLibraryList->Selected, b );

   //--- Create and Show Dialog
   TfrmEditBook *eB= new TfrmEditBook(Application, b);
   if( eB->ShowModal() == mrOk )
   {
      //--- Make Changes if user choose Ok ---
      MyLibraryList->Selected->Caption = eB->cboMonth->Items->Strings[ eB->cboMonth->ItemIndex ];
      MyLibraryList->Selected->SubItems->Strings[YEAR] = eB->edtYear->Text.Trim();
      MyLibraryList->Selected->SubItems->Strings[TITLE] = eB->edtTitle->Text.Trim();
      MyLibraryList->Selected->SubItems->Strings[AUTHOR] = eB->edtAuthor->Text.Trim();
      MyLibraryList->Selected->SubItems->Strings[GUTNUM] = eB->edtGutNum->Text.Trim();
   }
   delete eB; //Free the dialog's memory
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// DeleteBookUpdate
// Default Update for Edit Book Action and Delete Book Action
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::DeleteBookUpdate(TObject *Sender)
{
   //-- Cast the sender as a TAction to determine which it is ---
   ((TAction*)Sender)->Enabled = WorkArea->ActivePageIndex == MyLibraryTab->TabIndex;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// popSearchForAuthorClick
// Searches for author of selected book in my Library list
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::popSearchForAuthorClick(TObject *Sender)
{
   if( MyLibraryList->Selected )
   {
      //--- Clear all previouse search ---
      cmdClearCriteria->Click();
      cmdClearResults->Click();

      //--- Set search's author name ---
      edtSearchAuthor->Text = MyLibraryList->Selected->SubItems->Strings[AUTHOR];

      //--- Display Search Page, Set Search to Gutenberg, and search ---
      WorkArea->ActivePage = SearchTab;
      cboSearchType->ItemIndex = 0;
      cmdSearch->Click();
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuSearchForAuthorClick
// Searches for author of selected book in booklist
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuBookSearchForAuthorClick(TObject *Sender)
{
   if( BookList->Selected )
   {
      //--- Clear all previouse search ---
      cmdClearCriteria->Click();
      cmdClearResults->Click();

      //--- Set search's author name ---
      edtSearchAuthor->Text = BookList->Selected->SubItems->Strings[AUTHOR];

      //--- Display Search Page, Set Search to Gutenberg, and search ---
      WorkArea->ActivePage = SearchTab;
      cboSearchType->ItemIndex = 0;
      cmdSearch->Click();
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// ExportToHTMLExecute
// Export to HTML file
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::ExportToHTMLExecute(TObject *Sender)
{
   AnsiString BookFile,BookName;

   //--- Get Correct Book Title and filename ---
   if( WorkArea->ActivePageIndex == MyLibraryTab->TabIndex )
   {
      if( !MyLibraryList->Selected ) return;
      BookFile =  mySettings.bookDir+MyLibraryList->Selected->SubItems->Strings[VIEW_FILENAME];
      BookName =  MyLibraryList->Selected->SubItems->Strings[TITLE];
   }
   else
   {
      if( !SearchList->Selected ) return;
      BookFile =  mySettings.bookDir+SearchList->Selected->SubItems->Strings[VIEW_FILENAME];
      BookName =  SearchList->Selected->SubItems->Strings[TITLE];
   }

   ExportDialog->FileName = BookName;
   //--- Display Export(Save) Dialog ---
   if( ExportDialog->Execute() )
   {
      //--- Show Progress Bar and set cursor to hourglass ---
      pbarStatus->Visible = true;
      Screen->Cursor = crHourGlass;

      //--- Export book to an HTML file ---
      bookToHTML( ExportDialog->FileName,
               BookFile,
               BookName,
               mySettings.HTMLFont,
               mySettings.HTMLBack,
               mySettings.HTMLLpp, pbarStatus);

      //--- Reset cursor, hide progress bar, and open browser to HTML file ---
      Screen->Cursor = crDefault;
      pbarStatus->Visible = false;
      ShellExecute(Application->Handle,"open",ExportDialog->FileName.c_str(),"",
                   mySettings.appPath.c_str(),SW_MAXIMIZE);
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// ExportToHTMLUpdate
// Update for Export and Read Book menues
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::ExportToHTMLUpdate(TObject *Sender)
{
   bool result;
   result = WorkArea->ActivePageIndex == MyLibraryTab->TabIndex ||
            ( (WorkArea->ActivePageIndex == SearchTab->TabIndex) && !GutSearch);

   ExportToHTML->Enabled = result;
   ReadBook->Enabled = result;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// popBookListPopup
// Default updates for Book list popup
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::BookListPopUpPopup(TObject *Sender)
{
 mnuBookSearchForAuthor->Enabled = ( WorkArea->ActivePageIndex == BookListTab->TabIndex );
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// popMyLibraryPopup
// Default updates for my library popup
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::popMyLibraryPopup(TObject *Sender)
{
   popSearchForAuthor->Enabled = ( WorkArea->ActivePageIndex == MyLibraryTab->TabIndex );   
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuViewOptionsClick
// Create and Display an options dialog
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuViewOptionsClick(TObject *Sender)
{
   TfrmOptions *options;
   options  = new TfrmOptions(Application, mySettings);
   if( options->ShowModal() == mrOk )
      CopySettings( mySettings, options->FSettings );

   delete options;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// ReadBookExecute
// Setup the reader for the book to read
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::ReadBookExecute(TObject *Sender)
{
   BOOK_INFO book;
   TListItem *LBook;

   //--- Determine which list to use to find the book to open ---
   if( WorkArea->ActivePageIndex == MyLibraryTab->TabIndex )
      LBook = MyLibraryList->Selected;
   else
      LBook = SearchList->Selected;

   //--- If there is no book then return ---
   if( !LBook )
      return;

   //--- Set the open book (for Bookmark) to the book being opened ---
   curBook = LBook;

   //--- Convert the ListItem to a book info struct and then open it ---
   ListItemToBook( LBook, book );
   OpenBook( book, mySettings.ReaderLpp );

   //---- Make a 15pixel margin in the reader ---
   if( mySettings.ReaderLpp != 0){
   TRect r;
   memBook->Perform( EM_GETRECT, 0, (LPARAM)&r);
   r.Left   =  15;
   r.Top    =  15;
   r.Right  -= 15;
   r.Bottom -= 15;
   memBook->Perform( EM_SETRECT, 0, (LPARAM)&r);
   }
   else{
       FullScreenExecute(this);
   }

}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// NextPageExecute
// Go to next page if we can
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::NextPageExecute(TObject *Sender)
{
   if( (curPage+1) < openBook.size() )
   {
      curPage++;
      memBook->Lines->Text = openBook[curPage];
      edtPageInfo->Text = IntToStr(curPage+1) + " of " + IntToStr(openBook.size());
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// edtPageInfoKeyPress
// Allow user only to enter digits or backspace, delete, return keys
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::PreviousPageExecute(TObject *Sender)
{
   if( int(curPage-1) >= 0 )
   {
      curPage--;
      memBook->Lines->Text = openBook[curPage];
      edtPageInfo->Text = IntToStr(curPage+1) + " of " + IntToStr(openBook.size());
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// CloseBookExecute
// Close Book
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::CloseBookExecute(TObject *Sender)
{
   //--- Set that there is no open book ---
   curBook = NULL;

   //--- If in fullscreen mode then exit ---
   if( inFullScreen )
      FullScreenExecute( Sender );

   //--- Reset page, book, and find information ---
   curPage = 0;
   openBook.clear();
   frmFind->cmdClearClick(Sender);

   //--- Hide all Reader information ---
   WorkArea->ActivePageIndex = MyLibraryTab->TabIndex;
   ReaderTab->TabVisible = false;
   mnuBookReader->Visible = false;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// NAME: GoToPage
// ARGUEMENTS: unsigned int pageNum, page number to go to
// RETURNS: NONE
// PURPOSE: Go to specified page
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::GoToPage(unsigned int pageNum)
{
   if( pageNum < openBook.size() )
   {
      curPage = pageNum;
      memBook->Lines->Text = openBook[curPage];
      memBook->SelStart = 0;
      edtPageInfo->Text = IntToStr(curPage+1) + " of " + IntToStr(openBook.size());
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// edtPageInfoKeyPress
// Allow user only to enter digits or backspace, delete, return keys
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::edtPageInfoKeyPress(TObject *Sender, char &Key)
{
   if( !isdigit(Key) && (Key != VK_BACK && Key != VK_DELETE && Key != VK_RETURN) )
   {
      //-- Key wasn't valid
      Key = '\0';
   }
   else if( Key == VK_RETURN )
   {
      //-- Otherwise go to the page
      GoToPage(StrToInt(edtPageInfo->Text)-1);
      memBook->SetFocus();
   }
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// edtPageInfoExit
// Redisplay the page info text
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::edtPageInfoExit(TObject *Sender)
{
   edtPageInfo->Text = IntToStr(curPage+1) + " of " + IntToStr(openBook.size());
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuFullScreenExecute
// Show or Hide Fullscreen Mode
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::FullScreenExecute(TObject *Sender)
{
   if( inFullScreen )
   {
      //-- We are in fullscreen mode so we need to exit
      //-- Indicate FullScreen mode is off, set the reader
      //-- back to the mainForm's Reader Tab. Hide the
      //-- Exit Fullscreen button and hide the fullscreen
      //-- Form.
      inFullScreen = false;
      pnlReader->Parent = ReaderTab;
      cmdExitFull->Visible = false;
      frmFullScreen->Hide();
   }
   else
   {
      //-- We are not in fullscreen mode so turn it on
      //-- Show the exit fullscreen button. Setup the
      //-- Fullscreen form and assign the reader to it.
      //-- Show the form
      inFullScreen = true;
      cmdExitFull->Visible = true;
      pnlReader->Parent = frmFullScreen;
      frmFullScreen->SetBounds(0,0,Screen->DesktopWidth,Screen->DesktopHeight);
      frmFullScreen->Show();
   }

   //--- Make a 15 pixel margin in the reader ---
   TRect r;
   memBook->Perform( EM_GETRECT, 0, (LPARAM)&r);
   r.Left   =  15;
   r.Top    =  15;
   r.Right  -= 15;
   r.Bottom -= 15;
   memBook->Perform( EM_SETRECT, 0, (LPARAM)&r);
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// FullScreenUpdate --Action OnUpdate Event--
// Common Update Event for BookReader Menus
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::FullScreenUpdate(TObject *Sender)
{
   NextPage->Enabled = ReaderTab->TabVisible;
   PreviousPage->Enabled = ReaderTab->TabVisible;
   FullScreen->Enabled = ReaderTab->TabVisible;
   CloseBook->Enabled = ReaderTab->TabVisible;   
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// cmdLastPageClick
// Go To Last Page
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::cmdLastPageClick(TObject *Sender)
{
   GoToPage( openBook.size() - 1 );
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// cmdFirstPageClick
// Go To First Page
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::cmdFirstPageClick(TObject *Sender)
{
   GoToPage(0);    
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// edtPageInfoEnter
// Clear Text so user can enter page number
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::edtPageInfoEnter(TObject *Sender)
{
   edtPageInfo->Text = "";
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookMouseWheelDownClick
// Go TO Next Page
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::memBookMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
   NextPageExecute( Sender );   
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookMouseWheelUpClick
// Go TO Previous Page
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::memBookMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
   PreviousPageExecute(Sender);   
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookmarksClick
// Determine wheter Add Bookmark and Remove Bookmark Menus should be enabled
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuBookmarksClick(TObject *Sender)
{
   mnuBookmarkAddBookmark->Enabled = ReaderTab->TabVisible;
   mnuRemoveBookmarks->Enabled = (bookmarks.size() >0);
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookmarkAddBookmarkClick
// Default handler for Bookmark OnClick. Opens bookmark.
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuBookmarkAddBookmarkClick(TObject *Sender)
{
   BOOK_INFO bmBook;    //Book to Bookmark

   //-- Convert ListItem to Book and then add the bookmark ---
   ListItemToBook( curBook, bmBook );
   addBookmark( bmBook, memBook->Tag, curPage,mnuBookmarks);

   //-- Assign OnClick handler to new bookmark
   bookmarks[numOfBookmarks-1].menu->OnClick = BookmarkClick;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookmarkClick
// Default handler for Bookmark OnClick. Opens bookmark.
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::BookmarkClick(TObject * Sender)
{
   unsigned int index = ((TMenuItem*)(Sender))->Tag -1;
   BOOK_INFO book;

   //--- If there is a book open then close it ---
   if( openBook.size() > 0 )
      CloseBookExecute(Sender);

   //--- Setup Book Information used to open book ---
   book.title = bookmarks[index].menu->Caption;
   book.filename = bookmarks[index].filename;

   //--- Find book in my Library and select. This allows further bookmarks ---
   for( int i = 0; i < MyLibraryList->Items->Count; i++ )
   {
      if( book.filename == MyLibraryList->Items->Item[i]->SubItems->Strings[VIEW_FILENAME] )
      {
         curBook = MyLibraryList->Items->Item[i];
         break;
      }
   }

   //--- Open book and go to the bookmarked page ---
   OpenBook( book, bookmarks[index].lpp );
   GoToPage( bookmarks[index].pos  );
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// NAME: OpenBook
// ARGUEMENTS: BOOK_INFO book, The book to be opened
//             int lpp, The number of lines per page
// RETURNS: NONE
// PURPOSE: To open book specified by the book argument.
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::OpenBook(BOOK_INFO book, int lpp)
{
   //--- Show Progress Bar and change cursor to  hourglass ---
   pbarStatus->Visible = true;
   Screen->Cursor = crHourGlass;

   //--- Show Book's Title in Reader Tab's Caption ---
   ReaderTab->Caption = Trim(book.title);

   if( lpp == 0 ){
       int ScreenHeight = Screen->Height - 200;
       lpp = ScreenHeight / abs(memBook->Font->Height);
   }

   //Generate pages
   bookToPages(   mySettings.bookDir + book.filename,
                  openBook,
                  lpp,
                  pbarStatus,
                  mySettings.addNewLine,
                  mySettings.StripHeader );

   //--- Set Current Page to the beginning and display it ---
   curPage = 0;
   memBook->Lines->Text = openBook[curPage];

   //--- Save the lines per page used for bookmark purposes ---
   memBook->Tag = lpp;

   //--- Display page information in book navigation panel ---
   edtPageInfo->Text = IntToStr(curPage+1) + " of " + IntToStr(openBook.size());

   //--- Hide Progress Bar and Show the Reader Tab and Book Reader menu
   //--- Then display the Reader and set cursor back to default
   pbarStatus->Visible = false;
   ReaderTab->TabVisible = true;
   mnuBookReader->Visible = true;
   WorkArea->ActivePageIndex = ReaderTab->TabIndex;
   Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuRemoveBookmarksClick
// Create and show a remove bookmark dialog box
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuRemoveBookmarksClick(TObject *Sender)
{
   TfrmRemBookmarks *remB = new TfrmRemBookmarks( Application );
   remB->ShowModal();
   delete remB;
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// SearchListDblClick
// Decide if we should download the selected book or Read it and then do it.
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::SearchListDblClick(TObject *Sender)
{
   if( GutSearch )
      DownloadBookActionExecute(Sender);
   else
      ReadBookExecute(Sender);
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookReaderGoToPageClick
// Get what page the user wishes to go to and go their
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuBookReaderGoToPageClick(TObject *Sender)
{
   int PageNum;
   AnsiString InputString = InputBox("Go To Page", "Page #", IntToStr(curPage+1));

   //--- User Canceled or wants to go to same page as displayed ---
   if( InputString == IntToStr(curPage+1) )
      return;

   try
   {
      PageNum = StrToInt(InputString);
      GoToPage(PageNum-1);
   }__finally{;}
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// mnuBookReaderFindClick
// Show Find Dialog
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::mnuBookReaderFindClick(TObject *Sender)
{
   frmFind->Show();
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// actPrintPageExecute
// Display Print Dialog and Print Pages
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::actPrintBookExecute(TObject *Sender)
{
    if( MyLibraryList->Selected == NULL )
        return;


   int startPage, endPage;
   BOOK_INFO book;
   vector<AnsiString> PrintedBook;
   int lpp = 100;

   lpp = (Printer()->PageHeight - 400) / Printer()->Canvas->TextHeight("TEST"); 
   ListItemToBook(MyLibraryList->Selected, book );
   bookToPages(mySettings.bookDir + book.filename,PrintedBook,lpp,pbarStatus,true,false);

   //--- Setup page range information --------------
   PrintDialog->MinPage = 1;
   PrintDialog->MaxPage = PrintedBook.size();
   //-----------------------------------------------

   if( PrintDialog->Execute() )
   {
      //--- Setup the print range ----------
      if( PrintDialog->PrintRange == prPageNums )
      {  //User wants to print only a subrange
         startPage = PrintDialog->FromPage;
         endPage   = PrintDialog->ToPage;
      }
      else
      {  //user wants to print entire book
         startPage = 1;
         endPage   = PrintedBook.size();
      }
      //-----------------------------------------------

    Printer()->Title = book.title;
    Printer()->BeginDoc();
      //--- Print the pages -----
      for( int i = startPage; i <= endPage; i++ )
      {
         //tPrinter->Lines->Text = openBook[i-1];
         //tPrinter->Print("Page" + IntToStr(i));

         TRect rect = Rect(400,400,100,100);
         rect.Top += 4*Printer()->Canvas->TextHeight("ABC");
         rect.Right  = Printer()->PageWidth - 800;
         rect.Bottom = Printer()->PageHeight -800;
         Printer()->Canvas->TextOut( 400,400,"Page " + IntToStr(Printer()->PageNumber) );
         DrawText(Printer()->Canvas->Handle,PrintedBook[i-1].c_str(),PrintedBook[i-1].Length(),&rect,DT_EDITCONTROL | DT_LEFT	);
         if( i < endPage )
            Printer()->NewPage();
      }
      //-----------------------------------------------

      Printer()->EndDoc();

   }    
}
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// ListViewColumnClick
// Generic ColumnClick for BookList, SearchList, and MyLibraryList
////////////////////////////////////////////////////////////////////////////////
void __fastcall TmainForm::ListViewColumnClick(TObject * Sender, TListColumn * Column)
{
   TListView *SortMe = ((TListView*)Sender);
   int ListId;

   //--- Choose which listview this is ---
   if(   SortMe == SearchList )
      ListId = SEARCHLIST;
   else if( SortMe == MyLibraryList )
      ListId = LIBLIST;
   else
      ListId = BOOKLIST;

   //--- Sort ---
   SortListView( SortMe, Column->Index, ListId );
}
void __fastcall TmainForm::actPrintBookUpdate(TObject *Sender)
{
    PrintBook1->Enabled = WorkArea->ActivePageIndex == 2;    
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::memBookKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if( Key == VK_RETURN	 )
        NextPageExecute(Sender);    
}
//---------------------------------------------------------------------------

