#include "CheaterApp.h"
#include "..\rc.h"



//======================Handles================================================//
#define WIN32_LEAN_AND_MEAN // this will assume smaller exe



CheaterApp::CheaterApp (HWND hwnd)
			:	
				_cheat_notes (hwnd, 0),
				_game_list (hwnd, 0),
				_cheat_list (hwnd, 0),
				_options_dlg (hwnd, 0),
				_search_dlg (hwnd, 0),
				_ini_1 (hwnd,0),
				_ini_2 (hwnd,0),
				_ini_3 (hwnd,0)
{
	// private var init
	_hWnd = hwnd;
	_game_selected = 0;
	_send_cheats = 0;
	_treeview_draw_lag = 0;
	_treeview_select_lag = 0;
	_active = false;
	_dynarec_count = 0;
}


bool cheat_used[100000];
void CheaterApp::Load_Init( int ini_file )
{
	CHEAT_INFO info;
	FILE *fp=0;

	
	switch( ini_file )
	{
		case 1: fopen_s(&fp, "inis\\shark_codelist.ini", "r" ); break;
		case 2: fopen_s(&fp, "inis\\shark_codelist2.ini", "r" ); break;
		case 3: fopen_s(&fp, "inis\\shark_codelist3.ini", "r" ); break;
	}
	if( !fp ) return;


	// cleanup
	_cheat_list.SetFocus();
	_cheat_notes.ClearSelection();


	memset( cheat_used, 0, sizeof(cheat_used) );

	while( !feof(fp) )
	{
		fgets( _str_line, 10000, fp );
		if( feof(fp) ) break;


		_str_temp[0] = 0;
		sscanf_s( _str_line, "%s", _str_temp );

		// enable / disable cheats
		if( strcmp( _str_temp, "SEND" ) == 0 )
		{
			sscanf_s( _str_line + strlen( "SEND = "), "%d", &_send_cheats );
		}


		// hex editor
		else if( strcmp( _str_temp, "EDITOR" ) == 0 )
		{
			char *str;
			str = _str_line + strlen( "EDITOR = " );

			// remove whitespace
			while( str[ strlen(str)-1 ] == '\r' ||
						 str[ strlen(str)-1 ] == '\n' )
				str[ strlen(str)-1 ] = 0;

			// copy name
			strcpy_s( _hex_editor, str );
		}


		// game select
		else if( strcmp( _str_temp, "GAME" ) == 0 )
		{
			char *str;
			str = _str_line + strlen( "GAME = " );

			// remove whitespace
			while( str[ strlen(str)-1 ] == '\r' ||
						 str[ strlen(str)-1 ] == '\n' )
				str[ strlen(str)-1 ] = 0;


			// search for name
			for( int lcv = 0; lcv < _cheat_file.Get_Num_Games(); lcv++ )
			{
				if( strcmp( str, _cheat_file.Get_Game_Name(lcv) ) == 0 )
				{
					_game_selected = lcv;
					break;
				}
			}


			// show current game (make visible)
			_game_list.SetSelection( _game_selected );
			Show_Cheats_List();
		}


		// cheat select
		else if( strcmp( _str_temp, "CHEAT" ) == 0 )
		{
			char *str;
			UINT32 value;

			str = _str_line + strlen( "CHEAT = " );

			// remove whitespaces
			while( str[ strlen(str)-1 ] == '\r' ||
						 str[ strlen(str)-1 ] == '\n' )
				str[ strlen(str)-1 ] = 0;


			// find cheat name
			int lcv = 0;
			for( lcv = 0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
			{
				// check duplicate name used
				if( cheat_used[ lcv ] ) continue;

				// check name match
				if( strcmp( _cheat_file.Get_Cheat_Name(lcv), str ) == 0 )
					break;
			}

			// cheat not found
			if( lcv == _cheat_file.Get_Num_Cheats() ) continue;


			// do not use this cheat again
			cheat_used[ lcv ] = 1;

			// =================================
			// =================================
			// =================================

			// get cheat value
			fgets( _str_line, 10000, fp );
			sscanf_s( _str_line, "OPTION = $%X", &value );


			// find option number
			_cheat_file.Get_Cheat_Info( lcv, info );
			int option_num = 0;
			for( option_num = 0; option_num < info.num_options; option_num++ )
			{
				if( info.option_value[ option_num ] == value ) break;
			}


			// found match
			if( info.num_options != option_num )
			{
				// 'cheat name' + '   (=> ' + 'option name' + ')'
				strcpy_s( _str_line, " " );
				strcat_s( _str_line, info.name );
				strcat_s( _str_line, "    (=>  " );
				strcat_s( _str_line, info.options_name[ option_num ] );
				strcat_s( _str_line, ")" );


				// update cheat name (treeview)
				_cheat_file.Set_Cheat_Quantity( lcv, value );
				_cheat_list.SetText( _tree_ptr[lcv], _str_line );
			}

			// =================================
			// =================================
			// =================================

			// get checkbox state
			fgets( _str_line, 10000, fp );
			sscanf_s( _str_line, "STATE = %X", &value );


			// update state (0 or 1)
			_cheat_list.SetCheckbox( _tree_ptr[lcv], value );
			_tree_checkbox[ lcv ] = value;
			value = _cheat_list.GetCheckbox( _tree_ptr[lcv] );
		}
	}


	fclose( fp );

	// ******************************************************** //
	// ******************************************************** //
	// ******************************************************** //

	// takes a lot of time for the checkbox to become 'active'
	_treeview_draw_lag = 30;



	// cheat engine status
	if( _send_cheats == 0 )
		_cheat_notes.SetString( "Cheats currently:\r\n\r\nOFF - OFF - OFF" );
	else
	{
		Send_Cheats();

		_cheat_notes.SetString( "Cheats currently:\r\n\r\nON - ON - ON" );
	}
}


void CheaterApp::Save_Init( int ini_file )
{
	FILE *fp=0;

	switch( ini_file )
	{
		case 1: fopen_s(&fp, "inis\\shark_codelist.ini", "w" ); break;
		case 2: fopen_s(&fp, "inis\\shark_codelist2.ini", "w" ); break;
		case 3: fopen_s(&fp, "inis\\shark_codelist3.ini", "w" ); break;
	}
	if( !fp ) return;


	fprintf( fp, "SEND = %d\n", _send_cheats );
	fprintf( fp, "EDITOR = %s\n", _hex_editor );
	fprintf( fp, "GAME = %s\n", _cheat_file.Get_Game_Name(_game_selected) );

	for( int lcv=0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
	{
		fprintf( fp, "\n" );
		fprintf( fp, "CHEAT = %s\n", _cheat_file.Get_Cheat_Name( lcv ) );
		fprintf( fp, "OPTION = $%X\n", _cheat_file.Get_Cheat_Quantity( lcv ) );
		fprintf( fp, "STATE = %X\n", _tree_checkbox[ lcv ] );
	}

	fclose( fp );
}


void CheaterApp::Show_Game_List()
{
	// rebuild list
	_game_selected = 0;
	_cheat_file.Build_Game_List();

	
	LockWindowUpdate( _hWnd );

	_game_list.ClearSelection();
	for( int lcv = 0; lcv < _cheat_file.Get_Num_Games(); lcv++ )
	{
		char *str;

		str = _cheat_file.Get_Game_Name( lcv );
		_game_list.AddString( str );
	}

	LockWindowUpdate(0);
}


void CheaterApp::Show_Cheats_List()
{
	CHEAT_INFO info;


	// rebuild list
	_cheat_selected = -1;
	_cheat_file.Build_Cheat_List( _game_selected );


	LockWindowUpdate( _hWnd );


		int i=0;
	_cheat_list.Clear();
	for( int lcv = 0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
	{
		_cheat_file.Get_Cheat_Info( lcv, info );


		// init
		_tree_checkbox[ lcv ] = 0;


		// build default name
		strcpy_s( _str_line, " " );
		strcat_s( _str_line, info.name );


		// deal with options selection
		if( info.num_options )
		{
			// 'cheat name' + '   (=> ' + 'option name' + ')'
			strcat_s( _str_line, "    (=>  " );
			strcat_s( _str_line, info.options_name[ 0 ] );
			strcat_s( _str_line, ")" );

			// set options value = '$xxxx name'
			_cheat_file.Set_Cheat_Quantity( lcv, info.option_value[0] );
		}


		if( info.subdir == 0 )
			_tree_ptr[lcv] = _cheat_list.AddString( _str_line );
		else
			_tree_ptr[lcv] = _cheat_list.AddStringSub( _str_line );
	}


	LockWindowUpdate(0);
}


void CheaterApp::Load_Database()
{
	// load database info
	_cheat_file.Load_File( "inis\\shark_codelist.inf" );
	
	Show_Game_List();
	Show_Cheats_List();


	// load save data
	Load_Init(1);
}


void CheaterApp::Send_Cheats()
{
	_shark_cheater.Clear_List();


#if 0
	// -- TESTING ONLY --
	_shark_cheater.Convert_Cheat( -1, -1, -1 );
	_shark_cheater.Convert_Cheat( 0x80, 0x801DAFC4, 0x52EC );
	_shark_cheater.Convert_Cheat( 0x8F,	0x801DAFC4, 0x0800 );
	_shark_cheater.Convert_Cheat( 0xCF, 0x801DAFC4, 0x080052EC );
	_shark_cheater.Convert_Cheat( 0xCF, 0x00000000, 0xFEED0000 );
#endif



	for( int lcv=0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
	{
		// check cheat active
		if( _tree_checkbox[ lcv ] )
		{
			_cheat_file.Get_Cheat_Codes( lcv, _codes_list );

						
			// init code
			_shark_cheater.Convert_Cheat( -1, -1, -1 );

			for( int lcv2=0; lcv2 < _codes_list.num_codes; lcv2++ )
			{
				_shark_cheater.Convert_Cheat(
					_codes_list.code[lcv2], _codes_list.address[lcv2], _codes_list.value[lcv2] );
			}
		}
	}



	Set_Dynarec_Count(20);
	//_gameshark.Update_Memory();
}


void CheaterApp::Dlg_Command (HWND hwnd, int controlID, int command)
{
	//char statusMessage[64];

	switch (controlID)
	{
		case IDC_GAME_LIST:
			if( command == LBN_SELCHANGE )
			{
				_game_selected = _game_list.GetSelection();


				// rebuild cheat list for new game
				Show_Cheats_List();
			}
			break;

	
		case IDC_RELOAD_LIST:
			if( command == BN_CLICKED )
			{
				Save_Init( _ini_current );


				// Reload entire file
				Load_Database();
			}
			break;


		case IDC_SEARCH_MEMORY:
			{
#if 1
				CHEAT_INFO info;

				_search_dlg.Init( info );
				_search_dlg.Create( _hInst );
#endif
			}
			break;


		case IDC_EDIT_MEMORY:
			{
				_shark_cheater.Dump_Memory();
				_shark_cheater.Edit_Memory( _hex_editor );
				_shark_cheater.Load_Memory();
			}
			break;


		case IDC_SEND_CHEATS:
			if( command == BN_CLICKED )
			{
				_cheat_notes.SetString( "Cheats currently:\r\n\r\nON - ON - ON" );
				_send_cheats = 1;


				Send_Cheats();
				Save_Init( _ini_current );
			}
			break;


		case IDC_STOP_CHEATS:
			if( command == BN_CLICKED )
			{
				_cheat_notes.SetString( "Cheats currently:\r\n\r\nOFF - OFF - OFF" );
				_send_cheats = 0;


				Save_Init( _ini_current );
			}
			break;


		case IDC_ADD_CHEATS:
			{
				_edit_dlg.Init( _cheat_file.Get_Game_Name( _game_selected ) );
				_edit_dlg.Create( _hInst );


				// Reload entire file
				Load_Database();
			}
			break;


		case IDC_INI_1:
			{
				_ini_1.SetPushButton(1);
				_ini_2.SetPushButton(0);
				_ini_3.SetPushButton(0);


				Save_Init( _ini_current );
				Load_Init( 1 );
				_ini_current = 1;
			}
			break;

		
		case IDC_INI_2:
			{
				_ini_1.SetPushButton(0);
				_ini_2.SetPushButton(1);
				_ini_3.SetPushButton(0);


				Save_Init( _ini_current );
				Load_Init( 2 );
				_ini_current = 2;
			}
			break;

		
		case IDC_INI_3:
			{
				_ini_1.SetPushButton(0);
				_ini_2.SetPushButton(0);
				_ini_3.SetPushButton(1);


				Save_Init( _ini_current );
				Load_Init( 3 );
				_ini_current = 3;
			}
			break;

		
		default:
			break;
    }
}


void CheaterApp::Dlg_Notify (HWND hwnd, int controlID, int command, LPARAM lParam)
{
	HTREEITEM Selected;

  //char statusMessage[64];
	//int nSel;


	switch (controlID)
	{
		case IDC_CHEAT_LIST:
		{
			int code;
			
			code = ((LPNMHDR)lParam)->code;
			switch( code ) 
			{
			case NM_SETCURSOR:
				{
					// NOTE: Workaround for NM_CLICK treeview -UPDATE- lag
					//
					// NOTE: Ugly workaround

					if( _treeview_select_lag )
					{
						CHEAT_INFO info;
						int changed, id, parent_id;

						_treeview_draw_lag = 0;
						_treeview_select_lag--;


						// find changed checkbox
						int lcv=0;
						for( lcv=0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
						{
							// no change in state
							if( _tree_checkbox[ lcv ] == _cheat_list.GetCheckbox( _tree_ptr[lcv] ) )
								continue;


							// change in state!
							_tree_checkbox[ lcv ] = _cheat_list.GetCheckbox( _tree_ptr[lcv] );
							changed = lcv;
							break;
						}

						// not found
						if( lcv == _cheat_file.Get_Num_Cheats() ) break;



						// find parent ID
						parent_id = changed;
						while(1)
						{
							_cheat_file.Get_Cheat_Info( parent_id, info );
							if( info.subdir == 0 ) break;

							parent_id--;
						}



						// checked -> unchecked
						if( _tree_checkbox[ changed ] == 0 )
						{
							int state, lcv2;


							///////////////////////////////////////
							// subchildren handling

							// check boundary
							if( parent_id == _cheat_file.Get_Num_Cheats() ) break;

							// check no subchildren
							_cheat_file.Get_Cheat_Info( parent_id+1, info );
							if( info.subdir == 0 ) break;



							// PARENT = untick children
							if( parent_id == changed )
							{
								lcv2 = parent_id+1;
								state = 0;
								while(1)
								{
									_cheat_file.Get_Cheat_Info( lcv2, info );
									if( info.subdir == 0 ) break;

									_cheat_list.SetCheckbox( _tree_ptr[ lcv2 ], 0 );
									_tree_checkbox[ lcv2 ] = 0;

									lcv2++;
								}
							}
							else
							{
								// check for 'uncheck' possible
								state = 0;
								lcv2 = parent_id+1;
								while(1)
								{
									_cheat_file.Get_Cheat_Info( lcv2, info );
									if( info.subdir == 0 ) break;

									if( _tree_checkbox[ lcv2 ] )
										state = 1;

									lcv2++;
								}
							}


							// uncheck parent of subchildren
							if( state == 0 )
							{
								_cheat_list.SetCheckbox( _tree_ptr[ parent_id ], 0 );

								_tree_checkbox[ parent_id ] = 0;
							}

							break;
						}

						/////////////////////////////////////////////////
						/////////////////////////////////////////////////
						/////////////////////////////////////////////////

						// unchecked -> checked
						//else if( _tree_checkbox[ changed ] == 1 )


						while(1)
						{
							int state, lcv2;


							///////////////////////////////////////
							// subchildren handling

							// check boundary
							if( parent_id == _cheat_file.Get_Num_Cheats() ) break;

							// check no subchildren
							_cheat_file.Get_Cheat_Info( parent_id+1, info );
							if( info.subdir == 0 ) break;



							// check for 'check' possible
							state = 0;
							lcv2 = parent_id+1;
							while(1)
							{
								_cheat_file.Get_Cheat_Info( lcv2, info );
								if( info.subdir == 0 ) break;

								if( _tree_checkbox[ changed ] )
									state = 1;

								lcv2++;
							}


							// check parent of subchildren
							if( state == 1 )
							{
								_cheat_list.SetCheckbox( _tree_ptr[ parent_id ], 1 );

								_tree_checkbox[ parent_id ] = 1;
							}


							// PARENT = tick children
							if( parent_id == changed )
							{
								lcv2 = parent_id+1;
								state = 1;
								while(1)
								{
									_cheat_file.Get_Cheat_Info( lcv2, info );
									if( info.subdir == 0 ) break;

									_cheat_list.SetCheckbox( _tree_ptr[ lcv2 ], 1 );
									_tree_checkbox[ lcv2 ] = 1;

									lcv2++;
								}
							}

							break;
						}
							


						while(1)
						{
							// no group ID
							_cheat_file.Get_Cheat_Info( changed, info );
							if( info.id == 0 ) break;



							// GROUP ID - mutually exclusive checkboxes
							id = info.id;
							for( lcv = 0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
							{
								// avoid current box
								if( lcv == changed ) continue;

								// check same group ID
								_cheat_file.Get_Cheat_Info( lcv, info );
								if( info.id != id ) continue;


								// uncheck box
								_tree_checkbox[ lcv ] = 0;
								_cheat_list.SetCheckbox( _tree_ptr[ lcv ], 0 );
							}


							break;
						}
					}
				}
				break;


			case TVN_SELCHANGING:
				break;


			case NM_CUSTOMDRAW:
				// checkbox state - refreshes after certain time
				//
				// NOTE: Ugly workaround

				if( _treeview_select_lag == 0 && _treeview_draw_lag > 0 )
				{
					_treeview_draw_lag--;


					for( int lcv = 0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
						_cheat_list.SetCheckbox( _tree_ptr[lcv], _tree_checkbox[lcv] );
				}
				break;


			case TVN_SELCHANGED:
				{
					CHEAT_INFO info;


					// get cursor item
					Selected = (HTREEITEM) SendMessage( _cheat_list.Hwnd(), TVM_GETNEXTITEM, TVGN_CARET, 0 );
					if( !Selected ) break;



					// find node # (root-level only)
					int lcv=0;
					for( lcv=0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
					{
						if( _tree_ptr[lcv] == Selected ) break;
					}

					// check for new selected item
					if( lcv == _cheat_selected ) break;
					_cheat_selected = lcv;



					// update notes
					_cheat_file.Get_Cheat_Info( lcv, info );
					_cheat_notes.SetString( info.notes );
				}
				break;


			case NM_CLICK:
				// checkbox state - refreshes after certain time
				//
				// NOTE: Ugly workaround

				_treeview_select_lag = 20;
				break;


			case NM_RCLICK:
			case NM_DBLCLK:
			{
				CHEAT_INFO info;


				Selected = (HTREEITEM)SendMessage
					( _cheat_list.Hwnd(), TVM_GETNEXTITEM, TVGN_CARET, 0 );
				if( !Selected ) break;


				// find node # (root-level only)
				int lcv=0;
				for( lcv=0; lcv < _cheat_file.Get_Num_Cheats(); lcv++ )
				{
					if( _tree_ptr[lcv] == Selected ) break;
				}


				// show options for this cheat
				_cheat_file.Get_Cheat_Info( lcv, info );
				if( info.num_options > 0 )
				{
					_options_dlg.Init( info );
					_options_dlg.Create( _hInst );

					// get new option quantity
					if( _options_dlg.Is_Valid_Option() )
					{
						int num;


						num = _options_dlg.Get_Option_Number();


						// 'cheat name' + '   (=> ' + 'option name' + ')'
						strcpy_s( _str_line, " " );
						strcat_s( _str_line, info.name );
						strcat_s( _str_line, "    (=>  " );
						strcat_s( _str_line, info.options_name[ num ] );
						strcat_s( _str_line, ")" );

						// show new name
						_cheat_list.SetText( _tree_ptr[lcv], _str_line );


						// set options value
						_cheat_file.Set_Cheat_Quantity( lcv, info.option_value[ num ] );
					}
				}
			}
			break;


			default:
				// unknown code
				if( code == 0xfffffff4 )
					break;

				code = 0;
				break;
			}
		}
		break;
	}
}


void CheaterApp::Init_App( HWND hwnd )
{
	_hWnd = hwnd;


	// reset window handles
	_cheat_notes.SetHwnd( hwnd, IDC_CHEAT_NOTES );
	_game_list.SetHwnd( hwnd, IDC_GAME_LIST );
	_cheat_list.SetHwnd( hwnd, IDC_CHEAT_LIST );

	_ini_1.SetHwnd( hwnd, IDC_INI_1 );
	_ini_2.SetHwnd( hwnd, IDC_INI_2 );
	_ini_3.SetHwnd( hwnd, IDC_INI_3 );
	_ini_1.SetPushButton( 1 );
	_ini_current = 1;


	// Attach icon to main dialog
	HICON hIcon = LoadIcon (_hInst, MAKEINTRESOURCE (DLG_ICON));
	SendMessage (_hWnd, WM_SETICON, WPARAM (TRUE), LPARAM (hIcon));
	hIcon = LoadIcon (_hInst, MAKEINTRESOURCE (DLG_ICON_S));
	SendMessage (_hWnd, WM_SETICON, WPARAM (FALSE), LPARAM (hIcon));



	// Other initializations..
	Load_Database();
}


void CheaterApp::Update_Memory()
{
	if( _send_cheats )
		_shark_cheater.Update_Memory();
}


void CheaterApp::Init_Emu( UINT32 system, UINT32 *ptr_list, UINT32 *size_list )
{
	_shark_cheater.Set_System( system );
	_shark_cheater.Set_Memory( ptr_list, size_list );
}



bool CheaterApp::Is_Active()
{
	return _active;
}



void CheaterApp::Minimize()
{
	ShowWindow( _hWnd, SW_MINIMIZE );
}



int CheaterApp::Get_Dynarec_Count()
{
	if( _dynarec_count > 0 )
	{
		_dynarec_count--;
	}

	return _dynarec_count;
}


void CheaterApp::Set_Dynarec_Count( int count )
{
	_dynarec_count = count;
}


BOOL CheaterApp::DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SHOWWINDOW:
			_active = true;
			return TRUE;


    case WM_INITDIALOG:
			Init_App( hwnd );
			return TRUE;


    case WM_COMMAND:
			Dlg_Command(hwnd, LOWORD(wParam), HIWORD (wParam));
			return TRUE;


		case WM_NOTIFY:
			Dlg_Notify(hwnd, LOWORD(wParam), HIWORD (wParam), lParam);
			return TRUE;


    case WM_DESTROY:
			Save_Init( _ini_current );


			PostQuitMessage(0);
			return TRUE;


    case WM_CLOSE:
			Save_Init( _ini_current );


			DestroyWindow (hwnd);
			return TRUE;
	}
   
	return FALSE;
}


static CheaterApp *control = 0;
static BOOL CALLBACK DialogProc_Callback (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return control->DialogProc( hwnd, message, wParam, lParam );
}


void CheaterApp::Destroy()
{
	PostMessage( _hWnd, WM_CLOSE, NULL, NULL );

	while( Is_Active() )
	{
		Sleep(10);
	}
}


void CheaterApp::Run( HINSTANCE hInst )
{
	HWND hDialog;


	// C++ dialog - hack!
	if( _active )
		return;

	control = this;


	if( hInst != 0 )
		_hInst = hInst;


	hDialog = CreateDialog (_hInst, MAKEINTRESOURCE (DLG_MAIN), 0, (DLGPROC)DialogProc_Callback);
	if( hDialog == 0 )
	{
		char buf [100];
		wsprintf (buf, "Error 0x%x", GetLastError ());
		MessageBox (0, buf, "CreateDialog", MB_ICONEXCLAMATION | MB_OK);
		return;
	}



	MSG  msg;
	int status;
	
	while ((status = GetMessage (&msg, 0, 0, 0)) != 0)
	{
		if (status == -1)
			return;

		if (!IsDialogMessage (hDialog, &msg))
		{
      TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
	}



	control = 0;
	_active = false;
}


static void WINAPI DlgThread()
{
	control->Run( 0 );
}


void CheaterApp::Run_Thread( HINSTANCE hInst )
{
	// C++ dialog - hack!
	if( _active )
		return;

	control = this;


	_hInst = hInst;



	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DlgThread, NULL, 0, 0 );
}
