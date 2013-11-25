//----------------------------------------------------------------------------
#ifndef aboutH
#define aboutH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
   TPanel *pnlButton;
   TButton *OKButton;
   TPageControl *PageControl1;
   TTabSheet *AboutTab;
   TPanel *Panel1;
   TImage *ImgGutenberg;
   TLabel *Label3;
   TLabel *Label2;
   TLabel *Label1;
   TImage *ImgELibrary;
   TTabSheet *GPLTab;
   TMemo *Memo1;
   TLabel *Label4;
   TLabel *Label5;
   void __fastcall Label4Click(TObject *Sender);
   void __fastcall Label5Click(TObject *Sender);
private:
public:
	virtual __fastcall TAboutBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TAboutBox *AboutBox;
//----------------------------------------------------------------------------
#endif    
