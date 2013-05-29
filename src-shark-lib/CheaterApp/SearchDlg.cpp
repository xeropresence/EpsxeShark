#include "SearchDlg.h"
#include "..\rc.h"


SearchDlg::SearchDlg( HWND hwnd, int id )
			:
				_method_box( hwnd, id ),
				_size_box( hwnd, id ),
				_region_box( hwnd, id ),
				_value_box( hwnd, id ),
				_results_list( hwnd, id )
{
	// private var init
	_hWnd = hwnd;
}


void SearchDlg::Build_List()
{
	_method_box.AddString( "EQUAL" );
	_method_box.AddString( "NOT EQUAL" );
	_method_box.AddString( "GREATER" );
	_method_box.AddString( "GREATER EQUAL" );
	_method_box.AddString( "LESS" );
	_method_box.AddString( "LESS EQUAL" );
	_method_box.AddString( "NO CHANGE" );
	_method_box.AddString( "DIFFERENT" );
	_method_box.SetSelection( 0 );

	_size_box.AddString( "1  BYTES" );
	_size_box.AddString( "2  BYTES" );
	_size_box.AddString( "3  BYTES" );
	_size_box.AddString( "4  BYTES" );
	_size_box.SetSelection( 0 );

	_region_box.AddString( "RAM" );
	_region_box.AddString( "SRAM" );
	_region_box.SetSelection( 0 );


	for( int lcv=0; lcv < _cheat_info->num_options; lcv++ )
	{
		//_options_list.AddString( _cheat_info->options_name[ lcv ] );
	}
}


void SearchDlg::Init( CHEAT_INFO &info )
{
	_cheat_info = &info;
}


void SearchDlg::Command( HWND hwnd, int controlID, int command )
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
				//_option_num = _options_list.GetSelection();
				//_option_valid = true;


				PostMessage( _hWnd, WM_CLOSE, 0, 0 );
			}
			break;

		
		case IDC_OPTIONS_CANCEL:
			if( command == BN_CLICKED )
			{
				//_option_valid = false;


				PostMessage( _hWnd, WM_CLOSE, 0, 0 );
			}
			break;


		default:
			break;
    }
}


void SearchDlg::Notify( HWND hwnd, int controlID, int command, LPARAM lParam )
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


BOOL SearchDlg::DialogProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
		case WM_INITDIALOG:
			_method_box.SetHwnd( hwnd, IDC_SEARCH_METHOD );
			_size_box.SetHwnd( hwnd, IDC_SEARCH_DATA_SIZE );
			_region_box.SetHwnd( hwnd, IDC_SEARCH_MEMORY_REGION );
			_value_box.SetHwnd( hwnd, IDC_SEARCH_VALUE );
			_results_list.SetHwnd( hwnd, IDC_SEARCH_LIST );

			_hWnd = hwnd;

			Build_List();
			return TRUE;


    case WM_COMMAND:
			Command(hwnd, LOWORD(wParam), HIWORD (wParam));
			return TRUE;


		case WM_NOTIFY:
			Notify(hwnd, LOWORD(wParam), HIWORD (wParam), lParam);
			return TRUE;


    case WM_DESTROY:
			return TRUE;


    case WM_CLOSE:
			DestroyWindow (hwnd);
			return TRUE;
	}

	return FALSE;
}


static SearchDlg* search_dlg = 0;
static BOOL CALLBACK DialogProc_Callback( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return search_dlg->DialogProc( hwnd, message, wParam, lParam );
}


void SearchDlg::Create( HINSTANCE hInst )
{
	// Only allow -ONE- instance at a time!
	if( search_dlg )
	{
		return;
	}


	// CALLBACK hack!
	search_dlg = this;


	DialogBox ( hInst, MAKEINTRESOURCE (DLG_SEARCH), 0, (DLGPROC) DialogProc_Callback );


	search_dlg = 0;
}
