#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#define WIN32_MEAN_AND_LEAN



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
PADconfigure					PAD_configure[2];
PADinit								PAD_init[2];
PADshutdown						PAD_shutdown[2];
PADopen								PAD_open[2];
PADclose							PAD_close[2];
PADquery							PAD_query[2];
PADstartPoll					PAD_startPoll[2];
PADpoll								PAD_poll[2];
PADregisterCursor			PAD_registerCursor[2];
PADregisterVibration	PAD_registerVibration[2];




#include <stdio.h>



void LoadConfig_Pad()
{
	FILE *fp;


	pad_name[0][0] = 0;
	pad_name[1][0] = 0;

	fopen_s(&fp, "inis\\ePSXe_shark_pads.ini", "r" );
	if( fp ) {
		fscanf_s( fp, "PORT1 = %s\n", &pad_name[0] );
		fscanf_s( fp, "PORT2 = %s\n", &pad_name[1] );

		fclose( fp );
	}
}


void LoadPadPlugins()
{
	char str[1024];


	LoadConfig_Pad();

	for( int lcv = 0; lcv < 2; lcv++ ) {
		if( strcmp( pad_name[ lcv ], "ePSXe internal PAD" ) == 0 ) continue;
		if( pad_name[lcv][0] == 0 ) continue;


		strcpy_s( str, "plugins\\" );
		strcat_s( str, pad_name[lcv] );

		pad_plugin[lcv] = LoadLibrary( str );
		if( pad_plugin[lcv] == 0 ) continue;


		pad_plugin_active[lcv] = 1;


		PAD_init[lcv] = (PADinit)(GetProcAddress( pad_plugin[lcv], "PADinit" ));
		PAD_shutdown[lcv] = (PADshutdown)(GetProcAddress( pad_plugin[lcv], "PADshutdown" ));
		PAD_open[lcv] = (PADopen)(GetProcAddress( pad_plugin[lcv], "PADopen" ));
		PAD_close[lcv] = (PADclose)(GetProcAddress( pad_plugin[lcv], "PADclose" ));
		PAD_configure[lcv] = (PADconfigure)(GetProcAddress( pad_plugin[lcv], "PADconfigure" ));
		PAD_query[lcv] = (PADquery)(GetProcAddress( pad_plugin[lcv], "PADquery" ));
		PAD_startPoll[lcv] = (PADstartPoll)(GetProcAddress( pad_plugin[lcv], "PADstartPoll" ));
		PAD_poll[lcv] = (PADpoll)(GetProcAddress( pad_plugin[lcv], "PADpoll" ));
		PAD_registerCursor[lcv] = (PADregisterCursor)(GetProcAddress( pad_plugin[lcv], "PADregisterCursor" ));
		PAD_registerVibration[lcv] = (PADregisterVibration)(GetProcAddress( pad_plugin[lcv], "PADregisterVibration" ));


		if( PAD_registerCursor[lcv] && GPUcursor_func )
			PAD_registerCursor[lcv]( GPUcursor_func );

		if( PAD_registerVibration[lcv] && GPUvisualVibration_func )
			PAD_registerVibration[lcv]( GPUvisualVibration_func );
	}
}


void PADInitDriver()
{
	for( int lcv = 0; lcv < 2; lcv++ ) {
		if( pad_plugin_active[lcv] == 0 ) continue;

		if( PAD_init[lcv] )
			PAD_init[lcv]( lcv+1 );
	}


	// turn off ePSXe pads
	// - note: must be standard pads for hack to work

	switch(ePSXe_version)
	{
	case 170:
		if( pad_plugin_active[0] ) *( (char *) 0x4bdcb0 ) = 1;
		if( pad_plugin_active[1] ) *( (char *) 0x4bdcc0 ) = 1;
		break;

	case 180:
		if( pad_plugin_active[0] ) *( (char *) 0x4DE918 ) = 1;
		if( pad_plugin_active[1] ) *( (char *) 0x4DE928 ) = 1;
		break;
	}  
}


void PADOpenDriver()
{
	for( int lcv = 0; lcv < 2; lcv++ ) {
		if( pad_plugin_active[lcv] == 0 ) continue;

		if( PAD_open[lcv] )
			PAD_open[lcv]( hWWindow );
	}
}


void PADCloseDriver()
{
	for( int lcv = 0; lcv < 2; lcv++ ) {
		if( pad_plugin_active[lcv] == 0 ) continue;

		if( PAD_close[lcv] )
			PAD_close[lcv]();
	}
}


void PADShutdownDriver()
{
	for( int lcv = 0; lcv < 2; lcv++ ) {
		if( pad_plugin_active[lcv] == 0 ) continue;

		if( PAD_shutdown[lcv] )
			PAD_shutdown[lcv]();
	}
}


void __stdcall PADStartPoll_func( int padno )
{
	// byte value
	padno &= 0xff;


	// padno = 0 or 1
	if( pad_plugin_active[ padno ] ) {
		if( PAD_startPoll[ padno ] )
			PAD_startPoll[ padno ]( padno+1 );
	}
}


void __stdcall PADPoll_func( int padno )
{
	// padno = 0 or 1
	if( pad_plugin_active[ padno ] ) {
		if( PAD_poll[ padno ] )
			temp_var3 = PAD_poll[ padno ]( temp_var2 );

		switch(ePSXe_version)
		{
		case 180:
			if( padno == 0 && *((char *) 0x911482) == 1 ) {
				*((char *) 0x47F7B0) = 1 + 2 + (temp_var3 & 0x0f) * 2;


				// Wipeout 3 fix
				if( temp_var2 == 0x43 && temp_var3 == 0x23 ) {
					temp_var3 = 0x79;
				}
			}

			if( padno == 1 && *((char *) 0x9114C3) == 1 ) {
				*((char *) 0x47F7B0) = 1 + 2 + (temp_var3 & 0x0f) * 2;


				// Wipeout 3 fix
				if( temp_var2 == 0x43 && temp_var3 == 0x23 ) {
					temp_var3 = 0x79;
				}
			}

			break;

		case 170:
			if( padno == 0 && *((char *) 0x507be2) == 1 ) {
				*((char *) 0x4be2ec) = 1 + 2 + (temp_var3 & 0x0f) * 2;


				// Wipeout 3 fix
				if( temp_var2 == 0x43 && temp_var3 == 0x23 ) {
					temp_var3 = 0x79;
				}
			}

			if( padno == 1 && *((char *) 0x507c23) == 1 ) {
				*((char *) 0x4be2ec) = 1 + 2 + (temp_var3 & 0x0f) * 2;


				// Wipeout 3 fix
				if( temp_var2 == 0x43 && temp_var3 == 0x23 ) {
					temp_var3 = 0x79;
				}
			}

			break;
		} //switch
	}
}
