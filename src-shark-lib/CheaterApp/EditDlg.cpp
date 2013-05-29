#include <stdio.h>

#include "EditDlg.h"
#include "..\rc.h"


char ini_file[0x100000 * 10];
char text_list[0x100000];
int text_size;

int ini_size, ini_ptr;
int first_ptr, start_ptr, stop_ptr;

EditDlg::EditDlg( HWND hwnd, int id )
			:	
				_game_list( hwnd, id ),
				_choice_box( hwnd, id )
{
	// private var init
	_hWnd = hwnd;
}


#if 0
int EditDlg::Get_Line( int list_ptr )
{
	char *ptr;

	ptr = _file + list_ptr;
	while(1)
	{
		// look for newline chars
		if( ptr[0] == '\r' || ptr[0] == '\n' || ptr[0] == 0 )
		{
			while( ptr[0] == '\r' || ptr[0] == '\n' || ptr[0] == 0 &&
						 list_ptr < _file_size )
			{
				ptr[0] = 0;
				
				ptr++;
				list_ptr++;
			}
			break;
		}

		ptr++;
		list_ptr++;
	}

	return list_ptr;
}
#endif


void EditDlg::Init( char *name )
{
	FILE *fp;

	fopen_s(&fp, "inis\\shark_codelist.inf", "rb" );
	if( !fp ) return;

	ini_size = fread( ini_file, 1, 0x100000 * 10, fp );
	fclose(fp);


	_game_name = name;


	// search for matching name
	first_ptr = -1;
	start_ptr = 0;
	while(1) {
		if( start_ptr == ini_size ) break;


		// check game name
		if( ini_file[ start_ptr ] == '#' ) {
			bool valid;
			int lcv;

			// start of real list
			if( first_ptr == -1 ) first_ptr = start_ptr;

			stop_ptr = start_ptr+1;
			valid = true;
			lcv = 0;
			
			while(1) {
				// end of file
				if( stop_ptr >= ini_size ) {
					valid = false;
					break;
				}

				// done
				if( (unsigned int) lcv >= strlen(_game_name) ) {
					if( ini_file[ stop_ptr ] != '#' &&
							ini_file[ stop_ptr ] != '\r' &&
							ini_file[ stop_ptr ] != '\n' )
						valid = false;
					break;
				}

				// name mismatch
				if( ini_file[ stop_ptr ] != _game_name[ lcv ] ) {
					valid = false;
					break;
				}

				stop_ptr++;
				lcv++;
			}


			// finished
			if( valid == true ) {
				// PEC - game ID
				if( ini_file[ stop_ptr ] == '#' ) stop_ptr++;


				while( ini_file[ stop_ptr ] != '#' ) {
					if( stop_ptr >= ini_size ) break;


					// look for EOF or next game (#)
					while(1) {
						// end of file
						if( stop_ptr >= ini_size ) break;

						// done
						if( ini_file[ stop_ptr ] == '\n' ) {
							stop_ptr++;
							break;
						}

						stop_ptr++;
					}
				}

				memcpy( text_list, ini_file + start_ptr, stop_ptr - start_ptr );
				
				text_size = stop_ptr - start_ptr;
				text_list[ text_size ] = 0;
				break;
			} else {
				// start over
				start_ptr = stop_ptr;
			}
		}


		// look for newline flags or EOF
		while(1) {
			// end of file
			if( start_ptr >= ini_size ) {
				break;
			}

			// done
			if( ini_file[ start_ptr ] == '\n' ) {
				start_ptr++;
				break;
			}

			start_ptr++;
		}
	}


	// set string
	_game_file = text_list;
}


void EditDlg::Command (HWND hwnd, int controlID, int command)
{
	//char statusMessage[64];

	switch (controlID)
	{
		case IDC_EDIT_LIST:
			if( command == LBN_SELCHANGE )
			{
				//_game_selected = _Edit_list.GetSelection();
			}
			break;


		case IDC_EDIT_SAVE:
			if( command == BN_CLICKED ) {
				// add
				if( _choice_box.GetSelection() == 0 ) {
					FILE *fp;

					fopen_s(&fp, "inis\\shark_codelist.inf", "wb" );
					if( fp ) {
						//text_size = _game_list.GetLength();
						_game_list.GetString( text_list, 0x100000 );
						text_size = strlen( text_list );
	
						if( text_list[text_size-1] != '\n' ) {
							text_list[text_size+0] = '\r';
							text_list[text_size+1] = '\n';
							text_list[text_size+2] = '\r';
							text_list[text_size+3] = '\n';
							text_list[text_size+4] = '\r';
							text_list[text_size+5] = '\n';
							text_size += 6;
						}
						else if( text_list[text_size-3] != '\n' ) {
							text_list[text_size+0] = '\r';
							text_list[text_size+1] = '\n';
							text_list[text_size+2] = '\r';
							text_list[text_size+3] = '\n';
							text_size += 4;
						}
						else if( text_list[text_size-5] != '\n' ) {
							text_list[text_size+0] = '\r';
							text_list[text_size+1] = '\n';
							text_size += 2;
						}

						fwrite( ini_file, 1, first_ptr, fp );
						fwrite( text_list, 1, text_size, fp );
						fwrite( ini_file + first_ptr, 1, ini_size - first_ptr, fp );

						fclose(fp);


						// reload
						Init( _game_name );
					}
				}


				// replace
				else if( _choice_box.GetSelection() == 1 ) {
					FILE *fp;

					fopen_s(&fp, "inis\\shark_codelist.inf", "wb" );
					if( fp ) {
						text_size = _game_list.GetLength();
						_game_list.GetString( text_list, text_size );
	
						if( text_list[text_size-1] != '\n' ) {
							text_list[text_size+0] = '\r';
							text_list[text_size+1] = '\n';
							text_list[text_size+2] = '\r';
							text_list[text_size+3] = '\n';
							text_list[text_size+4] = '\r';
							text_list[text_size+5] = '\n';
							text_size += 6;
						}
						else if( text_list[text_size-3] != '\n' ) {
							text_list[text_size+0] = '\r';
							text_list[text_size+1] = '\n';
							text_list[text_size+2] = '\r';
							text_list[text_size+3] = '\n';
							text_size += 4;
						}
						else if( text_list[text_size-5] != '\n' ) {
							text_list[text_size+0] = '\r';
							text_list[text_size+1] = '\n';
							text_size += 2;
						}

						fwrite( ini_file, 1, start_ptr, fp );
						fwrite( text_list, 1, text_size, fp );
						fwrite( ini_file + stop_ptr, 1, ini_size - stop_ptr, fp );

						fclose(fp);


						// reload
						Init( _game_name );
					}
				}


				// remove
				else if( _choice_box.GetSelection() == 2 ) {
					FILE *fp;

					fopen_s(&fp, "inis\\shark_codelist.inf", "wb" );
					if( fp ) {
						text_size = _game_list.GetLength();
						_game_list.GetString( text_list, text_size );

						fwrite( ini_file, 1, start_ptr, fp );
						fwrite( ini_file + stop_ptr, 1, ini_size - stop_ptr, fp );

						fclose(fp);


						// reload
						Init( _game_name );
					}
				}
			}
			break;

		
		case IDC_EDIT_DONE:
			if( command == BN_CLICKED )
			{
				PostMessage( _hWnd, WM_CLOSE, 0, 0 );
			}
			break;


		default:
			break;
    }
}


void EditDlg::Notify (HWND hwnd, int controlID, int command, LPARAM lParam)
{
  //char statusMessage[64];
	//int nSel;


	switch (controlID)
	{
		case IDC_EDIT_LIST:
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


BOOL EditDlg::DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			_game_list.SetHwnd( hwnd, IDC_EDIT_LIST );
			_hWnd = hwnd;


			_game_list.SetString( _game_file );

			
			_choice_box.SetHwnd( hwnd, IDC_EDIT_CHOICE );
			_choice_box.AddString( "Add game" );
			_choice_box.AddString( "Change codes" );
			_choice_box.AddString( "Remove game" );
			_choice_box.SetSelection( 1 );
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


static EditDlg* Edit_dlg = 0;
static BOOL CALLBACK DialogProc_Callback (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Edit_dlg->DialogProc( hwnd, message, wParam, lParam );
}


void EditDlg::Create( HINSTANCE hInst )
{
	// Only allow -ONE- instance at a time!
	if( Edit_dlg )
	{
		return;
	}


	// CALLBACK hack!
	Edit_dlg = this;


	DialogBox( hInst, MAKEINTRESOURCE (DLG_EDIT), 0, (DLGPROC) DialogProc_Callback );


	Edit_dlg = 0;
}
