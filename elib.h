//---------------------------------------------------------------------------

#ifndef eLibH
#define eLibH
//---------------------------------------------------------------------------
#include "FTP.H"
#include "books.h"
#include "Settings.h"
#include <ActnList.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>

////////////////////////////////////////////////////////////////////////////////
// SORT_STRUCT
// Contains sort information for listviews
////////////////////////////////////////////////////////////////////////////////
typedef struct SORT_STRUCT{
   bool sorted;
   int  col;
   bool ascend;
} SORTTYPE;
//---------------------------------------------------------------------------

class TmainForm : public TForm
{
__published:	// IDE-managed Components
   TStatusBar *StatusBar;
   TPageControl *WorkArea;
   TTabSheet *BookListTab;
   TTabSheet *SearchTab;
   TTabSheet *MyLibraryTab;
   TListView *BookList;
   TPanel *pnlSearchCritera;
   TPanel *PnlSearchList;
   TActionList *ActionList1;
   TMainMenu *MainMenu;
   TMenuItem *mnuFile;
   TMenuItem *mnuFileExit;
   TMenuItem *mnuHelp;
   TMenuItem *mnuHelpAbout;
   TImageList *ActiveImages;
   TImageList *InactiveImages;
   TMenuItem *mnuView;
   TMenuItem *mnuViewToolbar;
   TAction *DownloadBookList;
   TMenuItem *mnuDwnDwnBookList;
   TPopupMenu *BookListPopUp;
   TMenuItem *DownloadBookListPopUp;
   TMenuItem *mnuViewGridLines;
   TMenuItem *mnuViewStatusBar;
   TListView *SearchList;
   TListView *MyLibraryList;
   TLabel *Label1;
   TEdit *edtSearchAuthor;
   TLabel *Label2;
   TEdit *edtSearchTitle;
   TLabel *Label3;
   TEdit *edtSearchMonth;
   TLabel *Label4;
   TEdit *edtSearchYear;
   TLabel *Label5;
   TEdit *edtSearchGutNum;
   TLabel *Label6;
   TComboBox *cboSearchType;
   TButton *cmdSearch;
   TButton *cmdClearResults;
   TButton *cmdClearCriteria;
   TAction *DownloadBookAction;
   TMenuItem *mnuFileDownloadBook;
   TMenuItem *DownloadBookPopUp;
   TImageList *HotImages;
   TMenuItem *N1;
   TAction *DeleteBook;
   TPopupMenu *popMyLibrary;
   TMenuItem *popDeleteBook;
   TAction *EditBook;
   TMenuItem *popEditBook;
   TMenuItem *N2;
   TMenuItem *mnuFileEditBook;
   TMenuItem *mnuFileDeleteBook;
   TMenuItem *mnuViewOptions;
   TMenuItem *popSearchForAuthor;
   TMenuItem *N4;
   TMenuItem *mnuBookSearchForAuthor;
   TAction *ExportToHTML;
   TSaveDialog *ExportDialog;
   TMenuItem *mnuFileExportToHTML;
   TMenuItem *N5;
   TMenuItem *popExportToHTML;
   TAction *ReadBook;
   TTabSheet *ReaderTab;
   TProgressBar *pbarStatus;
   TMenuItem *ReadBook1;
   TMenuItem *mnuBookReader;
   TMenuItem *mnuBookReaderNextPage;
   TMenuItem *mnuBookReaderPreviousPage;
   TMenuItem *mnuBookReaderGoToPage;
   TAction *NextPage;
   TAction *PreviousPage;
   TAction *CloseBook;
   TMenuItem *N6;
   TMenuItem *mnuBookReaderClose;
   TMenuItem *MyLibrary1;
   TToolBar *ToolBar;
   TToolButton *tbDownloadList;
   TToolButton *tbDownloadBook;
   TToolButton *ToolButton7;
   TToolButton *tbReadBook;
   TToolButton *tbExportToHTML;
   TToolButton *tbEditBook;
   TMenuItem *popReadBook;
   TAction *FullScreen;
   TToolButton *ToolButton1;
   TMenuItem *N7;
   TPanel *pnlReader;
   TRichEdit *memBook;
   TPanel *pnlNavBar;
   TEdit *edtPageInfo;
   TButton *cmdExitFull;
   TMenuItem *N8;
   TPopupMenu *popReader;
   TMenuItem *popNextPage;
   TMenuItem *popPrevPage;
   TMenuItem *N9;
   TMenuItem *popFullScreen;
   TMenuItem *N10;
   TMenuItem *popClose;
   TMenuItem *N11;
   TMenuItem *mnuBkReaderFullScreen;
   TSpeedButton *cmdNextPage;
   TSpeedButton *cmdLastPage;
   TSpeedButton *cmdPreviousPage;
   TSpeedButton *cmdFirstPage;
   TToolButton *ToolButton3;
   TMenuItem *mnuBookmarks;
   TMenuItem *mnuBookmarkAddBookmark;
   TMenuItem *N3;
   TMenuItem *popAddToBookmarks;
   TMenuItem *mnuRemoveBookmarks;
   TMenuItem *N13;
   TMenuItem *mnuBookReaderFind;
   TMenuItem *popGoToPage;
   TMenuItem *N14;
   TMenuItem *popFind;
   TAction *actPrintBook;
   TPrintDialog *PrintDialog;
   TMenuItem *PrintBook1;
   TMenuItem *Print1;
    TButton *Button1;
    TMenuItem *N12;
   void __fastcall mnuFileExitClick(TObject *Sender);
   void __fastcall mnuHelpAboutClick(TObject *Sender);
   void __fastcall mnuViewToolbarClick(TObject *Sender);
   void __fastcall DownloadBookListExecute(TObject *Sender);
   void __fastcall ListViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall mnuViewGridLinesClick(TObject *Sender);
   void __fastcall mnuViewStatusBarClick(TObject *Sender);
   void __fastcall DownloadBookActionExecute(TObject *Sender);
   void __fastcall cmdClearResultsClick(TObject *Sender);
   void __fastcall cmdClearCriteriaClick(TObject *Sender);
   void __fastcall cmdSearchClick(TObject *Sender);
   void __fastcall DownloadBookListUpdate(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall DeleteBookExecute(TObject *Sender);
   void __fastcall EditBookExecute(TObject *Sender);
   void __fastcall DeleteBookUpdate(TObject *Sender);
   void __fastcall popSearchForAuthorClick(TObject *Sender);
   void __fastcall mnuBookSearchForAuthorClick(TObject *Sender);
   void __fastcall ExportToHTMLExecute(TObject *Sender);
   void __fastcall ExportToHTMLUpdate(TObject *Sender);
   void __fastcall BookListPopUpPopup(TObject *Sender);
   void __fastcall popMyLibraryPopup(TObject *Sender);
   void __fastcall mnuViewOptionsClick(TObject *Sender);
   void __fastcall ReadBookExecute(TObject *Sender );
   void __fastcall NextPageExecute(TObject *Sender);
   void __fastcall PreviousPageExecute(TObject *Sender);
   void __fastcall CloseBookExecute(TObject *Sender);
   void __fastcall edtPageInfoKeyPress(TObject *Sender, char &Key);
   void __fastcall edtPageInfoExit(TObject *Sender);
   void __fastcall FullScreenExecute(TObject *Sender);
   void __fastcall FullScreenUpdate(TObject *Sender);
   void __fastcall cmdLastPageClick(TObject *Sender);
   void __fastcall cmdFirstPageClick(TObject *Sender);
   void __fastcall edtPageInfoEnter(TObject *Sender);
   void __fastcall memBookMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
   void __fastcall memBookMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
   void __fastcall mnuBookmarksClick(TObject *Sender);
   void __fastcall mnuBookmarkAddBookmarkClick(TObject *Sender);
   void __fastcall mnuRemoveBookmarksClick(TObject *Sender);
   void __fastcall SearchListDblClick(TObject *Sender);
   void __fastcall mnuBookReaderGoToPageClick(TObject *Sender);
   void __fastcall mnuBookReaderFindClick(TObject *Sender);
   void __fastcall actPrintBookExecute(TObject *Sender);
   void __fastcall ListViewColumnClick(TObject * Sender, TListColumn * Column);
    void __fastcall actPrintBookUpdate(TObject *Sender);
    void __fastcall memBookKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
   void SortListView(TListView * sortMe, int col, int listId);
   void __fastcall BookmarkClick(TObject * Sender);
   void __fastcall OpenBook(BOOK_INFO book, int lpp);
public:		// User declarations
   __fastcall TmainForm(TComponent* Owner);
   void __fastcall GoToPage(unsigned int pageNum);   

   SORTTYPE sortInfo[3];      // Sort Info for listviews
   int curCol;                //Current collumn being sorted
   Settings mySettings;       //settings for application
   TListItem *curBook;        //current open book info

};
//---------------------------------------------------------------------------
extern PACKAGE TmainForm *mainForm;
//---------------------------------------------------------------------------
#endif
