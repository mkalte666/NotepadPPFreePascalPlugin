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

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;
	toolbarIcons icon_compile;
	toolbarIcons icon_run;
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
    setCommand(0, TEXT("Compile with FreePascal"), compile, NULL, false);
    setCommand(1, TEXT("Run Program"), run, NULL, false);
	setCommand(2, TEXT("About.."), help, NULL, false);
	
	

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

void createToolbar()
{
	icon_compile.hToolbarBmp = (HBITMAP) LoadImage( NULL, L"./plugins/resource/cmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[0]._cmdID, (LPARAM)&icon_compile);
	icon_run.hToolbarBmp = (HBITMAP) LoadImage( NULL, L"./plugins/resource/run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[1]._cmdID, (LPARAM)&icon_run);
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void compile()
{
   TCHAR path[MAX_PATH];
	TCHAR pathNpp[MAX_PATH];
	char path_out[MAX_PATH];
	char pathNpp_out[MAX_PATH];
	int currentEdit;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	HWND curScint = (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

	
	// A message to Notepad++ to get a multibyte string (if ANSI mode) or a wide char string (if Unicode mode)
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, 0, (LPARAM)path);
	::SendMessage(nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)pathNpp);
	int encoding = (int)::SendMessage(curScint, SCI_GETCODEPAGE, 0, 0);
	WideCharToMultiByte(encoding, 0, path, -1, path_out, MAX_PATH, NULL, NULL);
	WideCharToMultiByte(encoding, 0, pathNpp, -1, pathNpp_out, MAX_PATH, NULL, NULL);
	char *out = new char[sizeof(path_out)+255+MAX_PATH+MAX_PATH];
	sprintf(out, "%s/plugins/resource/cmp.bat %s", pathNpp_out, path_out);
	system(out);
	
	
	//::SendMessage(curScint, SCI_REPLACESEL, 0, (LPARAM)out);
}

void run()
{
    TCHAR path[MAX_PATH];
	TCHAR pathNpp[MAX_PATH];
	char path_out[MAX_PATH];
	char pathNpp_out[MAX_PATH];
	int currentEdit;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	HWND curScint = (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

	
	// A message to Notepad++ to get a multibyte string (if ANSI mode) or a wide char string (if Unicode mode)
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, 0, (LPARAM)path);
	::SendMessage(nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)pathNpp);
	int encoding = (int)::SendMessage(curScint, SCI_GETCODEPAGE, 0, 0);
	WideCharToMultiByte(encoding, 0, path, -1, path_out, MAX_PATH, NULL, NULL);
	WideCharToMultiByte(encoding, 0, pathNpp, -1, pathNpp_out, MAX_PATH, NULL, NULL);
	char *out = new char[sizeof(path_out)+255+MAX_PATH];
	sprintf(out, "%s/plugins/resource/run.bat %s.exe", pathNpp_out, path_out);
	system(out);
}

 
void help()
{
	::MessageBox(NULL, TEXT("Notepad++ Pascal Plugin\n by Malte Kieﬂling \n If you have questions, email me:\n mkalte666@googlemail.com"), TEXT("Notepad++ FreePascal Plugin"), MB_OK);
} 