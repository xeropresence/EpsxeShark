#ifndef _CHEATERAPP_H_INCLUDED
#define _CHEATERAPP_H_INCLUDED

#include <windows.h>
#include <stdio.h>

#include "controls.h"

#include "OptionsDlg.h"
#include "SearchDlg.h"
#include "EditDlg.h"

#include "CheatFile.h"
#include "Cheat_Engine\Shark_Cheater.h"




class CheaterApp
{
public:
	CheaterApp(HWND hwnd = 0);
	~CheaterApp ()
	{}


	void Run( HINSTANCE hInst );
	void Run_Thread( HINSTANCE hInst );
	void Destroy();

	bool Is_Active();
	void Minimize();
	void Init_Emu( UINT32 system, UINT32 *ptr_list, UINT32 *size_list );

	void Update_Memory();
	void Send_Cheats();

	int  Get_Dynarec_Count();
	void Set_Dynarec_Count( int num );

	BOOL DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


protected:
	void Init_App( HWND hwnd );

	void Load_Database();
	void Load_Init( int ini_file );
	void Save_Init( int ini_file );

	void Show_Game_List();
	void Show_Cheats_List();
		
	void Dlg_Command (HWND hwnd, int controlID, int command);
	void Dlg_Notify (HWND hwnd, int controlID, int command, LPARAM lParam);


	bool _hide;
	bool _active;
	int _dynarec_count;
	int _ini_current;


	Button _ini_1;
	Button _ini_2;
	Button _ini_3;

	Edit _cheat_notes;
	ListBox	_game_list;
	OptionsDlg _options_dlg;
	SearchDlg _search_dlg;
	EditDlg _edit_dlg;


	TreeView	_cheat_list;
	HTREEITEM	_tree_ptr[30000];
	int _tree_checkbox[30000];
	int _treeview_draw_lag;
	int _treeview_select_lag;


	Shark_Cheater _shark_cheater;
	CheatFile	_cheat_file;

	CODES_LIST _codes_list;

	int	_game_selected;
	int	_cheat_selected;
	int	_send_cheats;


	HINSTANCE _hInst;
	HWND _hWnd;
	char _str_line[10000], _str_temp[10000];
	char _hex_editor[10000];
};

#endif // _MAIN_H_INCLUDED
