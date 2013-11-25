#include "ftp.h"
#include <wininet.h>
#include <fstream>

/*******************************************************************************
* NAME:	ftpGetFile																				 *
* ARGUMENTS:	sInfo: Information on server												 *
*					ftpFile:	File to download													 *
*					localFile:	Name to save local file as									 *
* RETURNS:	true: File downloaded															 *
*				false: File not downloaded														 *
* PURPOSE:	Download a a file from an FTP Server										 *		
*******************************************************************************/
bool ftpGetFile( SERVER_INFO sInfo, AnsiString ftpFile, AnsiString localFile )
{
   HINTERNET hSession;    	//Session
   HINTERNET hFTP;        	//FTP Connection
   bool bResult;				//Function Result
   
   //Open an internet Session
   hSession = InternetOpen(
                           sInfo.sAppName.c_str(),
                           INTERNET_OPEN_TYPE_PRECONFIG,
                           NULL,
                           NULL,
                           0 );
   try
   {
      //Connect to FTP server
      hFTP = InternetConnect(
                             hSession,
                             sInfo.server.c_str(),
                             INTERNET_INVALID_PORT_NUMBER,
                             sInfo.username.c_str(),
                             sInfo.password.c_str(),
                             INTERNET_SERVICE_FTP,
                              INTERNET_FLAG_PASSIVE,
                             (DWORD)0);


      //try and get file
      bResult = ::FtpGetFile(
            hFTP,        // Handle from an InternetConnect call
            (sInfo.directory + ftpFile).c_str(),
            localFile.c_str(),
            FALSE,
            FILE_ATTRIBUTE_NORMAL,
            FTP_TRANSFER_TYPE_BINARY,
            0);


     if( !bResult){
     LPSTR lpMsgBuf;
if (!FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL ))
{
   // Handle the error.
   return false;
}

// Process any inserts in lpMsgBuf.
// ...

// Display the string.
MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );

// Free the buffer.
LocalFree( lpMsgBuf );

      return false;
     }
     }
   __finally
   {
      InternetCloseHandle( hSession );
      InternetCloseHandle( hFTP );
   }

   return bResult;
}

/*******************************************************************************
* NAME:	ftpGetFileList																			 *
* ARGUMENTS:	sInfo: Information on server												 *
*					filter:	File fileter  i.e. *.zip for all zip files				 *
* RETURNS:	A TStringList thats contains all the files that match the filter   *
* PURPOSE:	Find files on FTP server that match user defined filter				 *		
*******************************************************************************/
TStringList * ftpGetFileList( SERVER_INFO sInfo,  AnsiString filter )
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

      if( sInfo.directory != "" )
         FtpSetCurrentDirectory(hFTP, sInfo.directory.c_str() );

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
            Application->ProcessMessages();

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

/*******************************************************************************
* NAME:	ftpFindFile																				 *
* ARGUMENTS:	sInfo: Information on server												 *
*					file:	File to find															 *
* RETURNS:	AnsiString containg filename													 *
* PURPOSE:	Find just one file matching a given expressing i.e. pete*.txt		 *		
*******************************************************************************/
AnsiString  ftpFindFile( SERVER_INFO sInfo, AnsiString file )
{
   AnsiString  rFile;
   HINTERNET hSession;    //Session
   HINTERNET hFTP;        //FTP Connection
   WIN32_FIND_DATA sWFD;
   HINTERNET hFileConnection;



   hSession = InternetOpen(
                           sInfo.sAppName.c_str(),
                           INTERNET_OPEN_TYPE_PRECONFIG,
                           NULL,
                           NULL,
                           0 );
   try
   {
      Application->ProcessMessages();
      hFTP = InternetConnect(
                             hSession,
                             sInfo.server.c_str(),
                             INTERNET_INVALID_PORT_NUMBER,
                             sInfo.username.c_str(),
                             sInfo.password.c_str(),
                             INTERNET_SERVICE_FTP,
                             0,
                             0);

      Application->ProcessMessages();
      if( sInfo.directory != "" )
         FtpSetCurrentDirectory(hFTP, sInfo.directory.c_str() );

      Application->ProcessMessages();
      hFileConnection = ::FtpFindFirstFile(
                    hFTP,
                   file.c_str(),
                    &sWFD,
                    0,
                    0);

      if (hFileConnection != (HINTERNET)NULL)
         rFile = sWFD.cFileName;

   }
   __finally
   {
      InternetCloseHandle( hSession );
      InternetCloseHandle( hFTP );
   }

   return rFile;

}

/*******************************************************************************
* NAME:	copyServer																				 *
* ARGUMENTS:	lhs:	destination															 	 *
*					rhs:	source																	 *
* RETURNS:	NONE																					 *
* PURPOSE:	Download a a file from an FTP Server										 *
*******************************************************************************/
void copyServer( SERVER_INFO &lhs, SERVER_INFO &rhs )
{
      lhs = rhs;
/*      lhs.sAppName    =  rhs.sAppName;
      lhs.server      =  rhs.server;
      lhs.proxy       =  rhs.proxy;
      lhs.username    =  rhs.username;
      lhs.password    =  rhs.password;
      lhs.directory   =  rhs.directory; */
}


void HTTP_GetFile(SERVER_INFO sInfo, AnsiString file, AnsiString localFile)
{
	HINTERNET hINet, hFile;
   std::ofstream output(localFile.c_str());

	hINet = InternetOpen("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
   AnsiString result;
	if ( !hINet )
	{
   	Application->MessageBox("Error","",MB_OK);
		return;
	}

	hFile = InternetOpenUrl( hINet, file.c_str(), NULL, 0, 0, 0 ) ;

	if ( hFile )
	{
		CHAR buffer[1024];
		DWORD dwRead;
		while ( InternetReadFile( hFile, buffer, 1023, &dwRead ) )
		{
			if ( dwRead == 0 )
				break;
         result += AnsiString(buffer);
         Application->ProcessMessages();
		}
		InternetCloseHandle( hFile );
	}
	else
	{
		Application->MessageBox("Error","",MB_OK);
	}

	InternetCloseHandle( hINet );

   output << result.c_str() << std::endl;

   output.close();
}
