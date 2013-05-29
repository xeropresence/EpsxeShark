#ifndef _CHEATFILE_H
#define _CHEATFILE_H


#define UINT32 unsigned int 


struct CHEAT_INFO
{
	char *name;
	int id;
	int subdir;

	int num_options;
	UINT32 option_value[2000];
	char *options_name[2000];

	char *notes;
};


struct CODES_LIST
{
	int num_codes;

	UINT32 code[100000];
	UINT32 address[100000];
	UINT32 value[100000];
};


class CheatFile
{
public:
	CheatFile()
	{
		_file_size = 0;
		_game_count = 0;
	}


	void Load_File( char *file_name );

	// ==============================
	// ==============================
	// ==============================

	void Build_Game_List();

	int Get_Num_Games();
	char *Get_Game_Name( int game_num );

	// ==============================
	// ==============================
	// ==============================

	void Build_Cheat_List( int game_num );

	void Set_Cheat_Quantity( int num, UINT32 value );
	UINT32 Get_Cheat_Quantity( int num );

	int Get_Num_Cheats();
	char *Get_Cheat_Name( int num );
	void Get_Cheat_Info( int num, CHEAT_INFO &info );

	void Get_Cheat_Codes( int num, CODES_LIST &codes_list );

	// ==============================
	// ==============================
	// ==============================

private:
	int Get_Line( int list_ptr );
	bool Valid_Code( char *file_ptr );


	char _file[ 0x100000 * 10 ];
	int _file_size;

	int _game_index[ 10000 ];
	int _game_count;
	int _game_selected;

	int _cheat_index[ 30000 ];
	int _cheat_count;
	char _cheat_notes[ 30000 ];

	UINT32 _cheat_quantity[ 30000 ];
};


#endif

