#ifndef _SearchDlg_H_INCLUDED
#define _SearchDlg_H_INCLUDED

#include <windows.h>
#include <stdio.h>

#include "CheatFile.h"
#include "controls.h"


class SearchDlg
{
public:
    SearchDlg( HWND hwnd = 0, int id = 0 );
    ~SearchDlg ()
    {}


		void Init( CHEAT_INFO &info );
		void Create( HINSTANCE hInst );

		void Build_List();


		BOOL  DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void	Command (HWND hwnd, int controlID, int command);
    void	Notify (HWND hwnd, int controlID, int command, LPARAM lParam);


private:
		ComboBox _method_box;
		ComboBox _size_box;
		ComboBox _region_box;
		Edit _value_box;
		ListBox	_results_list;
		CHEAT_INFO* _cheat_info;

		bool _search_valid;
		HWND _hWnd;
};

#endif // _MAIN_H_INCLUDED
