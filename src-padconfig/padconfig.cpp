#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#include "rc.h"
#include "plugin.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
									 LPTSTR lpCmdLine, int nCmdShow)
{
	DialogBox( hInstance, MAKEINTRESOURCE(DLG_PAD),
		GetActiveWindow(),(DLGPROC) PluginDlgProc );

	return 0;
}
