#include "Settings.H"
#include "FTP.H"
#include "elib.h"
#include <graphics.hpp>
#include <registry.hpp>

void CopySettings( Settings &lhs, Settings &rhs )
{
   copyServer( lhs.server, rhs.server );
   lhs.bookDir = rhs.bookDir;
   lhs.HTMLFont->Assign( rhs.HTMLFont );
   lhs.AppFont->Assign( rhs.AppFont );
   lhs.ReaderFont->Assign( rhs.ReaderFont );
   lhs.ReaderBack = rhs.ReaderBack;              
   lhs.HTMLBack = rhs.HTMLBack;
   lhs.HTMLLpp = rhs.HTMLLpp;
   lhs.ReaderLpp =rhs.ReaderLpp;
   lhs.addNewLine = rhs.addNewLine;
   lhs.StripHeader = rhs.StripHeader;
}

void WriteRegSettings( Settings & save, TForm *form)
{
   TmainForm * mainForm;
   mainForm = (TmainForm*)form;
   TRegistry *reg = new TRegistry;
   try
   {
      reg->RootKey = HKEY_LOCAL_MACHINE;
      if( reg->OpenKey("Software\\LN3GS Productions\\eLibrary", true ) )
      {
         reg->WriteBool("Maximized", mainForm->WindowState == wsMaximized );
         reg->WriteInteger("Width", mainForm->Width);
         reg->WriteInteger("Height", mainForm->Height);
         reg->WriteInteger("Left", mainForm->Left);
         reg->WriteInteger("Top", mainForm->Top);
         reg->WriteBool("GridLines", mainForm->mnuViewGridLines->Checked );
         reg->WriteBool("Toolbar", mainForm->mnuViewToolbar->Checked );
         reg->WriteBool("Statusbar", mainForm->mnuViewStatusBar->Checked );
         reg->WriteString("Server", save.server.server );
         reg->WriteString("ServerDir", save.server.directory );
         reg->WriteString("Password", save.server.password );
         reg->WriteString("Proxy", save.server.proxy );
         reg->WriteString("ListFontName", save.AppFont->Name );
         reg->WriteInteger("ListFontSize", save.AppFont->Size );
         reg->WriteString("BookDir", save.bookDir );
         reg->WriteString("HTMLFont", save.HTMLFont->Name );
         reg->WriteInteger("HTMLFontSize", save.HTMLFont->Size );
         reg->WriteString("HTMLFontColor", ColorToString(save.HTMLFont->Color) );
         reg->WriteString("HTMLBackColor", ColorToString(save.HTMLBack) );
         reg->WriteInteger("HTMLLpp", save.HTMLLpp );
         reg->WriteString("ReaderFont", save.ReaderFont->Name );
         reg->WriteInteger("ReaderFontSize", save.ReaderFont->Size );
         reg->WriteString("ReaderFontColor", ColorToString(save.ReaderFont->Color) );
         reg->WriteString("ReaderBackColor", ColorToString(save.ReaderBack) );
         reg->WriteInteger("ReaderLpp", save.ReaderLpp );
         reg->WriteBool("ClearNewLine", save.addNewLine);
         reg->WriteBool("StripHeader", save.StripHeader);
      }
   }
   __finally
   {  delete reg; }
}













