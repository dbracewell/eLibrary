//---------------------------------------------------------------------------

#ifndef fullscreenH
#define fullscreenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmFullScreen : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
   __fastcall TfrmFullScreen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFullScreen *frmFullScreen;
//---------------------------------------------------------------------------
#endif
