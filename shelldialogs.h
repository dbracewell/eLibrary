#include <shlobj.hpp>

AnsiString BrowseForFolder( AnsiString Title, TForm *Application )
{
   TBrowseInfo p;
   PItemIDList lpItemId;
   char DisplayName[MAX_PATH];
   char TempPath[MAX_PATH];
   AnsiString result="";

   for( int i = 0; i< MAX_PATH; i++)
      DisplayName[i] = '\n';

   p.hwndOwner = Application->Handle;
   p.pszDisplayName = DisplayName;
   p.lpszTitle = Title.c_str();
   p.ulFlags = 0;
   p.lpfn = NULL;
   p.pidlRoot = NULL;

   lpItemId = SHBrowseForFolder(&p);
   if( lpItemId != NULL )
   {
    SHGetPathFromIDList(lpItemId, TempPath);
    result = IncludeTrailingBackslash(TempPath);
    GlobalFree(lpItemId);
   }

   return result;
}