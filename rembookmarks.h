//---------------------------------------------------------------------------

#ifndef RemBookmarksH
#define RemBookmarksH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmRemBookmarks : public TForm
{
__published:	// IDE-managed Components
   TListBox *lstBookmarks;
   TButton *cmdOk;
   TButton *cmdRemove;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall cmdRemoveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmRemBookmarks(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRemBookmarks *frmRemBookmarks;
//---------------------------------------------------------------------------
#endif
