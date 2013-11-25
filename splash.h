//---------------------------------------------------------------------------

#ifndef splashH
#define splashH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "eLib.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <FileCtrl.hpp>

//---------------------------------------------------------------------------
class TsplashScreen : public TForm
{
__published:	// IDE-managed Components
   TImage *ImgGutenberg;
   TImage *ImgELibrary;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TBevel *Bevel1;
   TBevel *Bevel2;
   TLabel *lblProg;
   TTimer *Timer;
   TProgressBar *ProgressBar;
   TFileListBox *books;
   void __fastcall TimerTimer(TObject *Sender);
private:
   AnsiString appPath;
   void __fastcall SearchForBooks();
   void __fastcall loadBookList();
   void __fastcall loadMyLibrary();
   void __fastcall loadSettings();
   void __fastcall LoadAll();	// User declarations

public:		// User declarations
   __fastcall TsplashScreen(TComponent* Owner);
       

};
//---------------------------------------------------------------------------
extern PACKAGE TsplashScreen *splashScreen;
//---------------------------------------------------------------------------
#endif
