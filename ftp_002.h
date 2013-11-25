#ifndef _FTP_H
#define _FTP_H _FTP_H

#include <vcl.h>

/*******************************************************************************
* T Y P E  D E F I N I T I O N S																 *
*******************************************************************************/
typedef struct {
   AnsiString sAppName;
   AnsiString server;
   AnsiString proxy;
   AnsiString username;
   AnsiString password;
   AnsiString directory;
} SERVER_INFO;

void copyServer( SERVER_INFO &lhs, SERVER_INFO &rhs );
bool ftpGetFile( SERVER_INFO sInfo, AnsiString ftpFile, AnsiString localFile );
TStringList * ftpGetFileList( SERVER_INFO sInfo, AnsiString filter );
AnsiString  ftpFindFile( SERVER_INFO sInfo, AnsiString file );


void HTTP_GetFile(SERVER_INFO sInfo, AnsiString file, AnsiString localfile);


#endif
