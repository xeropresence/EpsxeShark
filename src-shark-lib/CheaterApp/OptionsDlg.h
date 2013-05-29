#ifndef _OPTIONSDLG_H_INCLUDED
#define _OPTIONSDLG_H_INCLUDED

#include <windows.h>
#include <stdio.h>

#include "CheatFile.h"
#include "controls.h"


class OptionsDlg
{
public:
    OptionsDlg( HWND hwnd = 0, int id = 0 );
    ~OptionsDlg ()
    {}


		void Init( CHEAT_INFO &info );
		void Create( HINSTANCE hInst );

		void Build_Options_List();
		int Get_Option_Number();
		bool Is_Valid_Option();


		BOOL  DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void	Command (HWND hwnd, int controlID, int command);
    void	Notify (HWND hwnd, int controlID, int command, LPARAM lParam);


private:
		ListBox	_options_list;
		CHEAT_INFO* _cheat_info;

		int _option_num;
		bool _option_valid;

		HWND _hWnd;
};

#endif // _MAIN_H_INCLUDED
