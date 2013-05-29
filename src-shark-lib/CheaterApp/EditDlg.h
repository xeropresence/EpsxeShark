#ifndef _EDITDLG_H_INCLUDED
#define _EDITDLG_H_INCLUDED

#include <windows.h>
#include <stdio.h>

#include "CheatFile.h"
#include "controls.h"


class EditDlg
{
public:
    EditDlg( HWND hwnd = 0, int id = 0 );
    ~EditDlg ()
    {}


		void Init( char *name );
		void Create( HINSTANCE hInst );

		void Build_Cheat_List();
		int Get_Option_Number();
		bool Is_Valid_Option();

		BOOL  DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void	Command (HWND hwnd, int controlID, int command);
    void	Notify (HWND hwnd, int controlID, int command, LPARAM lParam);


private:
		Edit _game_list;
		ComboBox _choice_box;


		char *_game_name;
		char *_game_file;

		HWND _hWnd;
};

#endif // _MAIN_H_INCLUDED
