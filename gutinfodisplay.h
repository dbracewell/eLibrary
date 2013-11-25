//---------------------------------------------------------------------------

#ifndef gutInfoDisplayH
#define gutInfoDisplayH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmGutInfo : public TForm
{
__published:	// IDE-managed Components
   TPanel *pnlControls;
   TButton *cmdOk;
   TMemo *memInfo;
   void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmGutInfo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGutInfo *frmGutInfo;
//---------------------------------------------------------------------------
#endif
