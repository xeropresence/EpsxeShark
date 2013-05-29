#include "OptionsDlg.h"
#include "..\rc.h"


OptionsDlg::OptionsDlg( HWND hwnd, int id )
			:	
				_options_list( hwnd, id )
{
	// private var init
	_hWnd = hwnd;
}


void OptionsDlg::Build_Options_List()
{
	for( int lcv=0; lcv < _cheat_info->num_options; lcv++ )
	{
		_options_list.AddString( _cheat_info->options_name[ lcv ] );
	}
}


void OptionsDlg::Init( CHEAT_INFO &info )
{
	_cheat_info = &info;
}


int OptionsDlg::Get_Option_Number()
{
	return _option_num;
}


bool OptionsDlg::Is_Valid_Option()
{
	return _option_valid;
}


void OptionsDlg::Command (HWND hwnd, int controlID, int command)
{
	//char statusMessage[64];

	switch (controlID)
	{
		case IDC_OPTIONS_LIST:
			if( command == LBN_SELCHANGE )
			{
				//_game_selected = _options_list.GetSelection();
			}
			break;


		case IDC_OPTIONS_OK:
			if( command == BN_CLICKED )
			{
				// retrieve quantity
				_option_num = _options_list.GetSelection();
				_option_valid = true;


				PostMessage( _hWnd, WM_CLOSE, 0, 0 );
			}
			break;

		
		case IDC_OPTIONS_CANCEL:
			if( command == BN_CLICKED )
			{
				_option_valid = false;


				PostMessage( _hWnd, WM_CLOSE, 0, 0 );
			}
			break;


		default:
			break;
    }
}


void OptionsDlg::Notify (HWND hwnd, int controlID, int command, LPARAM lParam)
{
  //char statusMessage[64];
	//int nSel;


	switch (controlID)
	{
		case IDC_OPTIONS_LIST:
		{
			int code;
			
			code = ((LPNMHDR)lParam)->code;
			/*
			switch( code ) 
			{
			default:
				// unknown code
				if( code == 0xfffffff4 )
					break;

				code = 0;
				break;
			}
			*/
		}
	}
}


BOOL OptionsDlg::DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			_options_list.SetHwnd( hwnd, IDC_OPTIONS_LIST );
			_hWnd = hwnd;


			Build_Options_List();
			_options_list.SetSelection(0);
			return TRUE;


    case WM_COMMAND:
			Command(hwnd, LOWORD(wParam), HIWORD (wParam));
			return TRUE;


		case WM_NOTIFY:
			Notify(hwnd, LOWORD(wParam), HIWORD (wParam), lParam);
			return TRUE;


    case WM_DESTROY:
			//PostQuitMessage(0);
			return TRUE;


    case WM_CLOSE:
			DestroyWindow (hwnd);
			return TRUE;
	}

	return FALSE;
}


static OptionsDlg* options_dlg = 0;
static BOOL CALLBACK DialogProc_Callback (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return options_dlg->DialogProc( hwnd, message, wParam, lParam );
}


void OptionsDlg::Create( HINSTANCE hInst )
{
	// Only allow -ONE- instance at a time!
	if( options_dlg )
	{
		_option_valid = false;
		return;
	}


	// CALLBACK hack!
	options_dlg = this;


	DialogBox ( hInst, MAKEINTRESOURCE (DLG_OPTIONS), 0, (DLGPROC) DialogProc_Callback );


	options_dlg = 0;
}
