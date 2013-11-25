//---------------------------------------------------------------------------

#include <vcl.h>
#include "splash.h"
#pragma hdrstop
USERES("eLibrary.res");
USELIB("wininet.lib");
USEFORM("eLib.cpp", mainForm);
USEFORM("splash.cpp", splashScreen);
USEFORM("about.cpp", AboutBox);
USEFORM("downFile.cpp", frmDownload);
USEFORM("editbook.cpp", frmEditBook);
USEUNIT("books.cpp");
USEUNIT("Settings.cpp");
USEFORM("options.cpp", frmOptions);
USE("eLibrary.todo", ToDo);
USEFORM("fullscreen.cpp", frmFullScreen);
USEUNIT("bookmark.cpp");
USEFORM("RemBookmarks.cpp", frmRemBookmarks);
USEFORM("gutInfoDisplay.cpp", frmGutInfo);
USEFORM("find.cpp", frmFind);
USEUNIT("ftp.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->Title = "eLibrary";
       Application->CreateForm(__classid(TmainForm), &mainForm);
         Application->CreateForm(__classid(TfrmFullScreen), &frmFullScreen);
         Application->CreateForm(__classid(TfrmFind), &frmFind);
         TsplashScreen *splash = new TsplashScreen(Application);
       splash->ShowModal();
       delete splash;

       //Start Program
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
