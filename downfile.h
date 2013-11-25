//---------------------------------------------------------------------------

#ifndef downFileH
#define downFileH
//---------------------------------------------------------------------------
#include "ftp.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmDownload : public TForm
{
private:	// User declarations
   SERVER_INFO *FServer;
   AnsiString  FFileName;
   AnsiString  FYear;
   AnsiString  FLocal;
   bool        bookList;
__published:	// IDE-managed Components
   TAnimate *CopyAnimation;
   TLabel *lblFile;
   TTimer *Timer1;
   void __fastcall Timer1Timer(TObject *Sender);
public:		// User declarations
   __fastcall TfrmDownload(TComponent* Owner, AnsiString file, AnsiString year,AnsiString cap, SERVER_INFO *server = NULL);
   __fastcall TfrmDownload(TComponent* Owner, AnsiString file, SERVER_INFO *server );
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDownload *frmDownload;
//---------------------------------------------------------------------------
#endif
