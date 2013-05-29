#include <stdio.h>


int pad1_select;
int pad2_select;



// PCSX-reloaded modify

typedef unsigned long (CALLBACK* PSEgetLibType)(void);
typedef unsigned long (CALLBACK* PSEgetLibVersion)(void);
typedef char *(CALLBACK* PSEgetLibName)(void);


// PAD Functions
typedef long (CALLBACK* PADopen)(HWND);
typedef long (CALLBACK* PADconfigure)(void);
typedef void (CALLBACK* PADabout)(void);
typedef long (CALLBACK* PADinit)(long);
typedef long (CALLBACK* PADshutdown)(void);	
typedef long (CALLBACK* PADtest)(void);		
typedef long (CALLBACK* PADclose)(void);
typedef long (CALLBACK* PADquery)(void);
typedef long (CALLBACK* PADkeypressed)(void);
typedef unsigned char (CALLBACK* PADstartPoll)(int);
typedef unsigned char (CALLBACK* PADpoll)(unsigned char);
typedef void (CALLBACK* PADsetSensitive)(int);
typedef void (CALLBACK* PADregisterVibration)(void (CALLBACK *callback)(unsigned long, unsigned long));
typedef void (CALLBACK* PADregisterCursor)(void (CALLBACK *callback)(int, int, int));
typedef void (CALLBACK* PADhandleHID)(LPARAM);
typedef void (CALLBACK* PADscreenResolution)(int, int);



// PAD function pointers
HINSTANCE pad_plugin[500];
char pad_name[500][512];
char pad_current[2][512];
int pad_count;

PADconfigure					PAD_configure[500];




// PCSX-reloaded
BOOL OnConfigurePluginsDialog(HWND hW)
{
	WIN32_FIND_DATA FindData;
	HANDLE Find;
	HINSTANCE Lib;
	PSEgetLibType    PSE_GetLibType;
	PSEgetLibName    PSE_GetLibName;
	PSEgetLibVersion PSE_GetLibVersion;
	HWND hWC_PAD1;
	HWND hWC_PAD2;
	char str[1024];


	hWC_PAD1 = GetDlgItem( hW,IDC_PAD_PORT1 );
	ComboBox_AddString( hWC_PAD1, "ePSXe internal pad" );
	
	hWC_PAD2 = GetDlgItem( hW,IDC_PAD_PORT2 );
	ComboBox_AddString( hWC_PAD2, "ePSXe internal pad" );

	ComboBox_SetCurSel( hWC_PAD1, 0 );
	ComboBox_SetCurSel( hWC_PAD2, 0 );

	strcpy_s( pad_name[0], "internal" );
	strcpy_s( pad_name[1], "internal" );
	pad_count = 1;


	Find = FindFirstFile( "plugins\\*.dll", &FindData);
	while(1) {
		if( Find == INVALID_HANDLE_VALUE ) break;


		strcpy_s( str, "plugins\\" );
		strcat_s( str, FindData.cFileName );

		Lib = LoadLibrary( str );
		pad_plugin[ pad_count ] = Lib;


		if( Lib ) {
			PSE_GetLibType = (PSEgetLibType) GetProcAddress((HMODULE)Lib,"PSEgetLibType");
			PSE_GetLibName = (PSEgetLibName) GetProcAddress((HMODULE)Lib,"PSEgetLibName");
			PSE_GetLibVersion = (PSEgetLibVersion) GetProcAddress((HMODULE)Lib,"PSEgetLibVersion");
			PAD_configure[ pad_count ] = (PADconfigure)(GetProcAddress( Lib, "PADconfigure" ));


			if( PAD_configure[ pad_count ] && PSE_GetLibType && PSE_GetLibName && PSE_GetLibVersion ) {
				unsigned long version = PSE_GetLibVersion();
				long type;

				sprintf_s( str, "%s %d.%d", PSE_GetLibName(), (int)(version>>8)&0xff, (int)version&0xff);
				type = PSE_GetLibType();


				// PAD_configure = PAD plugin
				strcpy_s( pad_name[ pad_count ], FindData.cFileName );

				ComboBox_AddString( hWC_PAD1, str );
				ComboBox_AddString( hWC_PAD2, str );


				if( strcmp( pad_current[0], FindData.cFileName ) == 0 ) 
					ComboBox_SetCurSel( hWC_PAD1, pad_count );
				if( strcmp( pad_current[1], FindData.cFileName ) == 0 ) 
					ComboBox_SetCurSel( hWC_PAD2, pad_count );

				pad_count++;
			}
		}

		if( FindNextFile( Find, &FindData ) == 0 ) break;
	}

	if( Find != INVALID_HANDLE_VALUE ) FindClose( Find );


	return TRUE;
}


void LoadConfig_Pad()
{
	FILE *fp;


	fopen_s(&fp, "inis\\ePSXe_shark_pads.ini", "r" );
	if( fp ) {
		fscanf_s( fp, "PORT1 = %s\n", &pad_current[0] );
		fscanf_s( fp, "PORT2 = %s\n", &pad_current[1] );

		fclose( fp );
	}
}


void SaveConfig_Pad()
{
	FILE *fp;


	fopen_s(&fp, "inis\\ePSXe_shark_pads.ini", "w" );
	if( fp ) {
		fprintf( fp, "PORT1 = %s\n", pad_name[ pad1_select ] );
		fprintf( fp, "PORT2 = %s\n", pad_name[ pad2_select ] );

		fclose( fp );
	}
}


void GetConfig_Pad( HWND hWnd )
{
	HWND hWC;


	hWC = GetDlgItem( hWnd,IDC_PAD_PORT1 );
	pad1_select = ComboBox_GetCurSel( hWC );


	hWC = GetDlgItem( hWnd,IDC_PAD_PORT2 );
	pad2_select = ComboBox_GetCurSel( hWC );


	if( pad1_select < 0 ) pad1_select = 0;
	if( pad2_select < 0 ) pad2_select = 0;
}



BOOL CALLBACK PluginDlgProc( const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	switch (msg)
	{
	case WM_INITDIALOG:
		LoadConfig_Pad();

		OnConfigurePluginsDialog( hWnd );
		return TRUE;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_PAD_OK:
			GetConfig_Pad( hWnd );
			SaveConfig_Pad();

			EndDialog( hWnd,TRUE );
			return TRUE;


		case ID_PAD_CANCEL:
			EndDialog( hWnd,FALSE );
			return TRUE;


		case IDC_PAD_CONFIG1:
			GetConfig_Pad( hWnd );

			if( PAD_configure[ pad1_select ] )
				PAD_configure[ pad1_select ]();
			return TRUE;


		case IDC_PAD_CONFIG2:
			GetConfig_Pad( hWnd );

			if( PAD_configure[ pad2_select ] )
				PAD_configure[ pad2_select ]();
			return TRUE;
		}
	}

	return FALSE;
}
