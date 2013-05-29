#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "CheatFile.h"
#include "../rc.h"



int CheatFile::Get_Line( int list_ptr )
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


void CheatFile::Load_File( char *file_name )
{
	FILE *fp;

	fopen_s(&fp, file_name, "rb" );
	if( !fp ) return;

	_file_size = fread( _file, 1, 0x100000 * 10, fp );
	fclose( fp );
}


void CheatFile::Build_Game_List()
{
	int list_ptr;
	bool pec_hack;

	_game_count = 0;

	//_game_list.ClearSelection();

	list_ptr = 0;
	while( list_ptr < _file_size )
	{
		pec_hack = false;

		// new game entry
		if( _file[ list_ptr ] == '#' )
		{
			// add name ptr to master list
			_game_index[ _game_count ] = list_ptr;
			_game_count++;

			pec_hack = true;
		}

	
		list_ptr = Get_Line( list_ptr );


		// PEC hack - <name>#SLUS-00000
		if( pec_hack &&
				_file[ list_ptr-13 ] == '#' && 
				_file[ list_ptr-12 ] == 'S' &&
				_file[ list_ptr-11 ] == 'L' &&
				_file[ list_ptr-9 ]  == 'S' &&
				_file[ list_ptr-8 ]  == '-' )
		{
			_file[ list_ptr-13 ] = 0;
		}
	}


	// EOF
	_game_index[ _game_count ] = list_ptr;
}


int CheatFile::Get_Num_Games()
{
	return _game_count;
}


char* CheatFile::Get_Game_Name( int game_num )
{
	return _file + _game_index[ game_num ] + 1;
}


void CheatFile::Build_Cheat_List( int game_num )
{
	int list_ptr;
	int stop_ptr;

	_cheat_count = 0;


	// invalid index
	if( game_num >= _game_count ) return;

	list_ptr = _game_index[ game_num ];
	stop_ptr = _game_index[ game_num+1 ];


	while( list_ptr < stop_ptr )
	{
		// new cheat entry
		if( _file[ list_ptr ] == '"' )
		{
			Set_Cheat_Quantity( _cheat_count, 0 );

			
			// add name ptr to master list
			_cheat_index[ _cheat_count ] = list_ptr;
			_cheat_count++;
		}
		
		
		list_ptr = Get_Line( list_ptr );
	}


	// EOF
	_cheat_index[ _cheat_count ] = list_ptr;
}


int CheatFile::Get_Num_Cheats()
{
	return _cheat_count;
}


char* CheatFile::Get_Cheat_Name( int num )
{
	char *str;
	
	// generic name
	str = _file + _cheat_index[ num ] + 1;

	// sub-directory name
	if( str[0] == '\\' && str[1] == '\\' )
		str += 2;

	return str;
}


void CheatFile::Get_Cheat_Info( int num, CHEAT_INFO &info )
{
	int list_ptr;
	int notes_lines;

	/*
	"cheat name
	; comment
	@id
	; comment
	80 xxxxxx xxxx
	; comment
	$option - line 1
	$option - line 2
	$option - line 3
	; comment
	.notes - line 1
	.notes - line 2
	; comment
	"next cheat
	*/

	// init
	list_ptr = _cheat_index[ num ];
	notes_lines = 0;
	
	memset( &info, 0, sizeof(CHEAT_INFO) );


	while( list_ptr < _cheat_index[ num+1 ] )
	{
		switch( _file[ list_ptr ] )
		{
			// name
			case '"':
				info.name = _file + list_ptr + 1;


				// check for sub-directory
				if( info.name[0] == '\\' && info.name[1] == '\\' )
				{
					info.subdir = 1;

					info.name += 2;
				}
				break;


			// ID
			case '@':
				sscanf_s( _file + list_ptr + 1, "%d", &info.id );
				break;


			// options
			case '$':
				{
					// option value
					sscanf_s( _file + list_ptr, "$%X", &info.option_value[ info.num_options ] );


					// option name
					int lcv2;

					lcv2 = 0;
					while( _file[ list_ptr + lcv2 ] != ' ' )
						lcv2++;
					while( _file[ list_ptr + lcv2 ] == ' ' )
						lcv2++;

					info.options_name[ info.num_options ] = _file + list_ptr + lcv2;


					info.num_options++;
				}
				break;


			// notes
			case '.':
				notes_lines++;

				// start of cheat comments
				if( notes_lines == 1 )
				{
					strcpy_s( _cheat_notes, _file + list_ptr + 1 );

					info.notes = _cheat_notes;
				}

				// notes 1 += '\r\n' + notes 2
				else
				{
					strcat_s( _cheat_notes, "\r\n" );
					strcat_s( _cheat_notes, _file + list_ptr + 1 );
				}
				break;


			// misc - comment, code, other
			default:
				break;
		}


		list_ptr = Get_Line( list_ptr );
	}
}


void CheatFile::Set_Cheat_Quantity( int num, UINT32 value )
{
	_cheat_quantity[ num ] = value;
}


UINT32 CheatFile::Get_Cheat_Quantity( int num )
{
	return _cheat_quantity[ num ];
}


bool CheatFile::Valid_Code( char *file_ptr )
{
	// format:
	// HEX opcode / HEX code / value + ????

	//////////////////////////////////////
	// #1

	// check code syntax
	while( file_ptr[0] != ' ' )
	{
		file_ptr++;

		if( file_ptr[-1] >= '0' && file_ptr[-1] <= '9' )
			continue;

		if( file_ptr[-1] >= 'A' && file_ptr[-1] <= 'F' )
			continue;

		if( file_ptr[-1] >= 'a' && file_ptr[-1] <= 'f' )
			continue;

		return false;
	}


	// whitespace
	while( file_ptr[0] == ' ' )
		file_ptr++;


	//////////////////////////////////////
	// #2

	// check code syntax
	while( file_ptr[0] != ' ' )
	{
		file_ptr++;

		if( file_ptr[-1] >= '0' && file_ptr[-1] <= '9' )
			continue;

		if( file_ptr[-1] >= 'A' && file_ptr[-1] <= 'F' )
			continue;

		if( file_ptr[-1] >= 'a' && file_ptr[-1] <= 'f' )
			continue;

		return false;
	}


	// whitespace
	while( file_ptr[0] == ' ' )
		file_ptr++;


	//////////////////////////////////////
	// #3

	// check quantity syntax
	while( file_ptr[0] != '\r' &&
				 file_ptr[0] != '\n' &&
				 file_ptr[0] != '\0' &&
				 file_ptr[0] != ' ' )
	{
		file_ptr++;

		if( file_ptr[-1] >= '0' && file_ptr[-1] <= '9' )
			continue;

		if( file_ptr[-1] >= 'A' && file_ptr[-1] <= 'F' )
			continue;

		if( file_ptr[-1] >= 'a' && file_ptr[-1] <= 'f' )
			continue;

		if( file_ptr[-1] == '?' )
			continue;

		return false;
	}

	return true;
}


void CheatFile::Get_Cheat_Codes( int num, CODES_LIST &codes_list )
{
	int list_ptr;
	int notes_lines;

	/*
	"cheat name
	; comment
	@id
	; comment
	80 xxxxxx xxxx
	; comment
	$option - line 1
	$option - line 2
	$option - line 3
	; comment
	.notes - line 1
	.notes - line 2
	; comment
	"next cheat
	*/

	// init
	list_ptr = _cheat_index[ num ];
	notes_lines = 0;
	
	codes_list.num_codes = 0;


	while( list_ptr < _cheat_index[ num+1 ] )
	{
		if( Valid_Code( _file + list_ptr ) )
		{
			if( sscanf_s( _file+list_ptr, "%X %X %X",
						&codes_list.code[ codes_list.num_codes ],
						&codes_list.address[ codes_list.num_codes ],
						&codes_list.value[ codes_list.num_codes ] ) < 3 )
			{
				// ???? - quantity modifier
				codes_list.value[ codes_list.num_codes ] = _cheat_quantity[ num ];
			}

			codes_list.num_codes++;
		}


		list_ptr = Get_Line( list_ptr );
	}
}
