#include<wininet.h>
#include<dstring.h>
#include<classes.hpp>

typedef struct SERVER_INFO_STRUCT{
   AnsiString sAppName;
   AnsiString server;
   AnsiString username;
   AnsiString password;
} SERVER_INFO;

bool ftpGetFile( SERVER_INFO sInfo, AnsiString ftpFile, AnsiString localFile );
TStringList * ftpGetFileList( SERVER_INFO sInfo, AnsiString Dir, AnsiString filter );

bool ftpGetFile( SERVER_INFO sInfo, AnsiString ftpFile, AnsiString localFile )
{
   HINTERNET hSession;    //Session
   HINTERNET hFTP;        //FTP Connection
   bool bResult;

   hSession = InternetOpen(
                           sInfo.sAppName.c_str(),
                           INTERNET_OPEN_TYPE_PRECONFIG,
                           NULL,
                           NULL,
                           0 );
   try
   {
      hFTP = InternetConnect(
                             hSession,
                             sInfo.server.c_str(),
                             INTERNET_INVALID_PORT_NUMBER,
                             sInfo.username.c_str(),
                             sInfo.password.c_str(),
                             INTERNET_SERVICE_FTP,
                             0,
                             (DWORD)0);
                             
      bResult = ::FtpGetFile(
            hFTP,        // Handle from an InternetConnect call
            ftpFile.c_str(),
            localFile.c_str(),
            FALSE,
            FILE_ATTRIBUTE_NORMAL,
            FTP_TRANSFER_TYPE_BINARY,
            0);
   }
   __finally
   {
      InternetCloseHandle( hSession );
      InternetCloseHandle( hFTP );
   }

   return bResult;
}

TStringList * ftpGetFileList( SERVER_INFO sInfo, AnsiString Dir, AnsiString filter )
{
   TStringList *fileList;
   fileList = new TStringList();
   HINTERNET hSession;    //Session
   HINTERNET hFTP;        //FTP Connection

   hSession = InternetOpen(
                           sInfo.sAppName.c_str(),
                           INTERNET_OPEN_TYPE_PRECONFIG,
                           NULL,
                           NULL,
                           0 );
   try
   {
      hFTP = InternetConnect(
                             hSession,
                             sInfo.server.c_str(),
                             INTERNET_INVALID_PORT_NUMBER,
                             sInfo.username.c_str(),
                             sInfo.password.c_str(),
                             INTERNET_SERVICE_FTP,
                             0,
                            (DWORD)0);

      if( Dir != "" )
         FtpSetCurrentDirectory(hFTP, Dir.c_str() );                            
      WIN32_FIND_DATA sWFD;
      bool bResult = true;
      HINTERNET hFileConnection;

      hFileConnection = ::FtpFindFirstFile(
                    hFTP,
                    filter.c_str(),
                    &sWFD,
                    0,
                    0);
      if (hFileConnection != (HINTERNET)NULL)
      {
         while (bResult)
         {
            fileList->Add( sWFD.cFileName );
            bResult = ::InternetFindNextFile(
                           hFileConnection,
                           &sWFD);
         }
      }
   }
   __finally
   {
      InternetCloseHandle( hSession );
      InternetCloseHandle( hFTP );
   }

   return fileList;
}
