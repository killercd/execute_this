#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <tchar.h>
#include <windows.h>

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>

//#include <strsafe.h>

#define BUFSIZE 4096
using namespace std;
HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;

void CreateChildProcess(void);
void WriteToPipe(void);
const char* ReadFromPipe(void);
void ErrorExit(PTSTR);

const wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

/*
HWND getCurrentScintillaHandle() {
    int currentEdit;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	return (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
}
*/

const char* ExecuteWinProcess(LPCSTR command)
{

    string myString;
    string tempFileName = "tmpFile.out";
    string returnBuffer ="";
    char tempPath[256];
    GetTempPathA(255,tempPath);
    myString.append("cmd.exe /c \"");

    myString.append((char*)command);
    myString.append("\" > ");
    myString.append((char*)tempPath);
    myString.append(tempFileName);

    if(strlen((char*)command)>0){
        system(myString.c_str());
    }
    string outFile = tempPath;
    outFile.append(tempFileName);
    ifstream pipeFile(outFile.c_str());
    string line = "";

    if(pipeFile.is_open()){
        while(getline(pipeFile,line)){
            returnBuffer.append(line+'\n');
        }
        pipeFile.close();
    }

    return returnBuffer.c_str();

}


void ErrorExit(PTSTR lpszFunction)

// Format a readable error message, display a message box,
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );


    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR));

    /*StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
*/

    MessageBox(NULL, (LPCTSTR)lpszFunction, TEXT("Function"), MB_OK);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}

#endif // UTIL_H_INCLUDED
