#include <vcl.h>
#include "FTP.H"
#include "elib.h"

#ifndef SETTINGS_H
#define SETTINGS_H SETTINGS_H

typedef  struct{
   AnsiString appPath;
   AnsiString bookDir;
   SERVER_INFO server;
   TFont *HTMLFont;
   TFont *AppFont;
   TFont *ReaderFont;
   TColor ReaderBack;
   TColor HTMLBack;
   unsigned int HTMLLpp, ReaderLpp;
   bool addNewLine;
   bool StripHeader;
}Settings;

void WriteRegSettings(  Settings & save, TForm *form);
void CopySettings( Settings &lhs, Settings &rhs );

#endif