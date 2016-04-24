//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#define _UNICODE
#define UNICODE
#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "Util.h"
#include <sstream>
#include <windows.h>
//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit(HANDLE hModule)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );

    ShortcutKey *shKey = new ShortcutKey();
    shKey->_isCtrl = true;
    shKey->_isShift = true;
    shKey->_isAlt = false;
    shKey->_key = 69; //E key code
    setCommand(0, TEXT("Execute Selected"), hello, shKey, false);
    //setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

string GetDocumentTextBytes(HWND curScintilla) {

    int length = ::SendMessageA(curScintilla, SCI_GETSELTEXT, 0, 0) + 1;
    char buffLen[20];
    itoa (length,buffLen,10);


    char* selectedText = new char[length];

    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)selectedText);

    return string(selectedText);

    //return hexStr(sb);


}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
     // Get the current scintilla

    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    string commandSt = GetDocumentTextBytes(curScintilla);

    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
    int countText = ::SendMessageA(curScintilla, SCI_GETLENGTH, 0, 0) + 1;

    int length = (int) ::SendMessageA(curScintilla, SCI_GETLENGTH, 0, 0) + 1;
    ::SendMessageA(curScintilla,SCI_SETTEXT,0,(LPARAM)ExecuteWinProcess(commandSt.c_str()));

}

void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}
