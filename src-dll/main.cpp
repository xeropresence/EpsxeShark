#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0503

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <process.h>

//#define _CRT_SECURE_NO_WARNINGS

#pragma warning (disable:4244)
//#pragma comment (lib, "shark_cheater.lib")

CRITICAL_SECTION cs;

//#define DEBUG_APP

#ifdef DEBUG_APP
FILE *fp_debug;
#endif

// ===============================
// ===============================
// ===============================

#include "SharkApp.h"
#include "globals.h"


SharkApp shark_cheater;


#define EPSXE_INI_VERSION "180_5"
#define EPSXE_INI_HACKS_VERSION "180_1"

// ===============================
// ===============================
// ===============================

void RunISO_Wrapper()
{
	strcpy_s( (char *) 0xC0A6C0, 1024, last_file );
}

void Wait_Shark()
{
	if( shark_cheater_boot ) {
		while( shark_cheater.Is_Active() == 0 )
			Sleep(10);
	}
}


void Write_ISO_Name( char *name )
{
	// check if file exists
	HKEY myKey;
	unsigned long size;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\epsxe\\config",0,KEY_ALL_ACCESS,&myKey)==ERROR_SUCCESS)
  {
		size = strlen(name);
		
		//RegQueryValueEx(myKey,,0,&type,(LPBYTE)&last_file,&size);
		RegSetValueEx(myKey, "IsoDirectory", 0, REG_SZ, (LPBYTE) name, size);

		RegCloseKey(myKey);
	}
}


unsigned int WINAPI Run_ISO(void *dummy)
{
#if 0
	fp_log = fopen("log.txt","a");
	fprintf( fp_log, "KEY   %02X %02X %02X\n", start_gpu );
	fclose(fp_log);
#endif
	
	Wait_Shark();

	if( run_iso_mode == 0 ) return 0;

	// wait for ePSXe window to open
	while( ePSXe_hwnd == 0 ) Sleep(10);

	// check if file exists
	HKEY myKey;
	unsigned long size, type;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\epsxe\\config",0,KEY_ALL_ACCESS,&myKey)==ERROR_SUCCESS)
  {
		size = 1024;
		RegQueryValueEx(myKey,"IsoDirectory",0,&type,(LPBYTE)&last_file,&size);

		RegCloseKey(myKey);
  }


	FILE *fp;
	fopen_s(&fp, last_file, "rb" );
	if( fp ) fclose( fp );
	else last_file[0] = 0;


	// file not found
	if( last_file[0] == 0 &&
			( run_iso_mode == 4 || run_iso_mode == 5 ) )
	{
		run_iso_mode = 3;
	}



	// GUI - Run ISO (confirm)
	if( run_iso_mode == 1 || run_iso_mode == 4 )
	{
		strcat_s( temp_str, "Run last game?\n" );
		if( run_iso_mode == 4 )
			strcat_s( temp_str, last_file );
		else
			strcat_s( temp_str, "CDROM plugin" );


		if( MessageBox( NULL, temp_str, "Autostart", MB_YESNO | MB_TOPMOST ) == IDNO )
		{
			run_iso_mode = 0;
		}
		else
		{
			if( run_iso_mode == 1 ) run_iso_mode = 2;
			if( run_iso_mode == 4 ) run_iso_mode = 5;
		}
	}


	// GUI - Run CDROM (autorun)
	if( run_iso_mode == 2 )
	{
		PostMessage( ePSXe_hwnd, 0x111, 0x9c41, 0 );
	}


	// GUI - Run ISO (dialog)
	if( run_iso_mode == 3 || run_iso_mode == 5 )
	{
		PostMessage( ePSXe_hwnd, 0x111, 0x9c43, 0 );
	}


	return 0;
}



void Start_Autorun()
{
	if( ePSXe_version == 170 ) {
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = 0;
		sa.lpSecurityDescriptor = NULL;
		_beginthreadex(&sa,NULL,Run_ISO,NULL,NULL,NULL);
		//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Run_ISO, NULL, 0, 0 ); 
	}
}

// ===============================
// ===============================
// ===============================

#ifdef _DEBUG
bool dirty_patch[0x400000];
#endif

void DirtyPatch( int location )
{
#ifdef _DEBUG
	if( dirty_patch[ location - 0x400000 ] == true ) {
		char str[256];

		sprintf_s( str, "%X", location );
		MessageBox( NULL, str, "PATCH 2x", MB_OK );
		return;
	}

	dirty_patch[ location - 0x400000 ] = true;
#endif
}

void WriteCodeRVA(void* code, void* location, int length, HMODULE hMod = hHost)
{
	unsigned char* ptr = (unsigned char*)hMod + (unsigned long)code;
	DWORD oldProtect;

	VirtualProtect(ptr, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(ptr,location,length);
	VirtualProtect(ptr, length, oldProtect, &oldProtect);
	FlushInstructionCache(hMod,ptr,length);
}

void WriteCode(unsigned char* code, unsigned char* location, int length, HMODULE hMod = hHost)
{
	DWORD oldProtect;
	VirtualProtect(code, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(code,location,length);
	VirtualProtect(code, length, oldProtect, &oldProtect);
	FlushInstructionCache(hMod,code,length);
}


void ApplyJmpHook(unsigned char* code, unsigned char* location, int nops = 0, HMODULE hMod = hHost)
{
	DWORD oldProtect;
	VirtualProtect(code, 5 + nops, PAGE_EXECUTE_READWRITE, &oldProtect);
	code[0] = 0xE9;
	*(int*)(code+1) = (int)location - ((int)code + 5);
	if(nops > 0) memset(code + 5, 0x90, nops);
	VirtualProtect(code, 5 + nops, oldProtect, &oldProtect);
	FlushInstructionCache(hMod,code,5+nops);
}

// ===============================
// ===============================
// ===============================

extern void Load_INI();


#include "rc.h"
#include "plugins.h"


#include "epsxe_spu.h"
#include "epsxe_cdr.h"
#include "epsxe_gpu.h"
#include "epsxe_cpu.h"
#include "epsxe_sio.h"


#include "epsxe_180.h"
#include "epsxe_170.h"
#include "epsxe_160.h"
#include "epsxe_152.h"


void Load_Negcon_INI( char *name )
{
	char line[512];
	char str[8][512];

	FILE *fp;
	fopen_s(&fp, name, "r" );


	if( !fp ) return;


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "DEADZONE = %d %d",
		&negcon_twist_deadzone_1, &negcon_twist_deadzone_2 );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "CENTER = %d",
		&negcon_twist_option_center );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "FLIP = %d",
		&negcon_twist_option_flip );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "THRESHOLD = %d %d",
		&negcon_twist_threshold_1, &negcon_twist_threshold_2 );



	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SENSITIVITY = %d",
		&negcon_twist_sensitivity );






	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "DEADZONE = %d",
		&negcon_I_deadzone );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "CENTER = %d",
		&negcon_I_option_center );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "FLIP = %d",
		&negcon_I_option_flip );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "THRESHOLD = %d",
		&negcon_I_threshold );



	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SENSITIVITY = %d",
		&negcon_I_sensitivity );





	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "DEADZONE = %d",
		&negcon_II_deadzone );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "CENTER = %d",
		&negcon_II_option_center );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "FLIP = %d",
		&negcon_II_option_flip );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "THRESHOLD = %d",
		&negcon_II_threshold );



	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SENSITIVITY = %d",
		&negcon_II_sensitivity );








	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "DEADZONE = %d",
		&negcon_L_deadzone );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "CENTER = %d",
		&negcon_L_option_center );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "FLIP = %d",
		&negcon_L_option_flip );


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "THRESHOLD = %d",
		&negcon_L_threshold );



	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SENSITIVITY = %d",
		&negcon_L_sensitivity );

	
	


	fclose( fp );
}



void Load_Dualshock_Mouse_INI( char *name )
{
	char line[512], temp_str[512];
	char str[8][512];
	int lcv;

	FILE *fp;
	fopen_s(&fp, name, "r" );


	if( !fp ) return;


	str[0][0] = 0;
	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "MOUSE = %s %s", temp_str, str[0] );


	if( strcmp( temp_str, "LEFT" ) == 0 )
		dualshock_mouse_stick = 1;
	else if( strcmp( temp_str, "RIGHT" ) == 0 )
		dualshock_mouse_stick = 2;
	else if( strcmp( temp_str, "D-PAD" ) == 0 )
		dualshock_mouse_stick = 3;


	dualshock_mouse_snap = 0;
	dualshock_mouse_snap_x = 0;
	dualshock_mouse_snap_y = 0;
	if( strcmp( str[0], "SNAP" ) == 0 )
		dualshock_mouse_snap = 1;


	toggle_dualshock_mouse[0][0] = dualshock_mouse_stick;
	toggle_dualshock_mouse[0][1] = dualshock_mouse_stick;
	toggle_dualshock_mouse[1][0] = dualshock_mouse_stick;
	toggle_dualshock_mouse[1][1] = dualshock_mouse_stick;
	toggle_dualshock_mouse[2][0] = dualshock_mouse_stick;
	toggle_dualshock_mouse[2][1] = dualshock_mouse_stick;




	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "D-PAD = %s", temp_str );

	if( strcmp( temp_str, "LEFT" ) == 0 )
		dualshock_dpad_stick = 1;
	else if( strcmp( temp_str, "RIGHT" ) == 0 )
		dualshock_dpad_stick = 2;
	else if( strcmp( temp_str, "D-PAD" ) == 0 )
		dualshock_dpad_stick = 3;

	toggle_dualshock_dpad[0][0] = dualshock_dpad_stick;
	toggle_dualshock_dpad[0][1] = dualshock_dpad_stick;
	toggle_dualshock_dpad[1][0] = dualshock_dpad_stick;
	toggle_dualshock_dpad[1][1] = dualshock_dpad_stick;
	toggle_dualshock_dpad[2][0] = dualshock_dpad_stick;
	toggle_dualshock_dpad[2][1] = dualshock_dpad_stick;


	toggle_dualshock_allow[0] = 0;
	toggle_dualshock_allow[1] = 0;
	toggle_dualshock_allow[2] = 0;



	for( lcv = 0; lcv < 8; lcv++ )
		str[lcv][0] = 0;

	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "LEFT = %s %s %s %s %s %s %s %s",
		str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7] );


	// check keymaps
	dualshock_button_left = 0;
	for( lcv = 0; lcv < 8; lcv++ ) {
		if( strcmp( str[lcv], "L2" ) == 0 ) dualshock_button_left |= 0x0001;
		else if( strcmp( str[lcv], "R2" ) == 0 ) dualshock_button_left |= 0x0002;
		else if( strcmp( str[lcv], "L1" ) == 0 ) dualshock_button_left |= 0x0004;
		else if( strcmp( str[lcv], "R1" ) == 0 ) dualshock_button_left |= 0x0008;

		else if( strcmp( str[lcv], "TRIANGLE" ) == 0 ) dualshock_button_left |= 0x0010;
		else if( strcmp( str[lcv], "CIRCLE" ) == 0 ) dualshock_button_left |= 0x0020;
		else if( strcmp( str[lcv], "CROSS" ) == 0 ) dualshock_button_left |= 0x0040;
		else if( strcmp( str[lcv], "SQUARE" ) == 0 ) dualshock_button_left |= 0x0080;

		else if( strcmp( str[lcv], "SELECT" ) == 0 ) dualshock_button_left |= 0x0100;
		else if( strcmp( str[lcv], "L3" ) == 0 ) dualshock_button_left |= 0x0200;
		else if( strcmp( str[lcv], "R3" ) == 0 ) dualshock_button_left |= 0x0400;
		else if( strcmp( str[lcv], "START" ) == 0 ) dualshock_button_left |= 0x0800;

		else if( strcmp( str[lcv], "UP" ) == 0 ) dualshock_button_left |= 0x1000;
		else if( strcmp( str[lcv], "RIGHT" ) == 0 ) dualshock_button_left |= 0x2000;
		else if( strcmp( str[lcv], "DOWN" ) == 0 ) dualshock_button_left |= 0x4000;
		else if( strcmp( str[lcv], "LEFT" ) == 0 ) dualshock_button_left |= 0x8000;

		else if( strcmp( str[lcv], "TOGGLE_MOUSE_LEFT" ) == 0 ) toggle_dualshock_mouse[0][1] = 1;
		else if( strcmp( str[lcv], "TOGGLE_MOUSE_RIGHT" ) == 0 ) toggle_dualshock_mouse[0][1] = 2;
		else if( strcmp( str[lcv], "TOGGLE_MOUSE_DPAD" ) == 0 ) toggle_dualshock_mouse[0][1] = 3;

		else if( strcmp( str[lcv], "TOGGLE_DPAD_LEFT" ) == 0 ) toggle_dualshock_dpad[0][1] = 1;
		else if( strcmp( str[lcv], "TOGGLE_DPAD_RIGHT" ) == 0 ) toggle_dualshock_dpad[0][1] = 2;
		else if( strcmp( str[lcv], "TOGGLE_DPAD_DPAD" ) == 0 ) toggle_dualshock_dpad[0][1] = 3;
	}

	if( toggle_dualshock_mouse[0][0] != toggle_dualshock_mouse[0][1] )
		toggle_dualshock_allow[0] = 1;

	if( toggle_dualshock_dpad[0][0] != toggle_dualshock_dpad[0][1] )
		toggle_dualshock_allow[0] = 1;





	for( lcv = 0; lcv < 8; lcv++ )
		str[lcv][0] = 0;

	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "RIGHT = %s %s %s %s %s %s %s %s",
		str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7] );


	// check keymaps
	dualshock_button_right = 0;
	for( lcv = 0; lcv < 8; lcv++ ) {
		if( strcmp( str[lcv], "L2" ) == 0 ) dualshock_button_right |= 0x0001;
		else if( strcmp( str[lcv], "R2" ) == 0 ) dualshock_button_right |= 0x0002;
		else if( strcmp( str[lcv], "L1" ) == 0 ) dualshock_button_right |= 0x0004;
		else if( strcmp( str[lcv], "R1" ) == 0 ) dualshock_button_right |= 0x0008;

		else if( strcmp( str[lcv], "TRIANGLE" ) == 0 ) dualshock_button_right |= 0x0010;
		else if( strcmp( str[lcv], "CIRCLE" ) == 0 ) dualshock_button_right |= 0x0020;
		else if( strcmp( str[lcv], "CROSS" ) == 0 ) dualshock_button_right |= 0x0040;
		else if( strcmp( str[lcv], "SQUARE" ) == 0 ) dualshock_button_right |= 0x0080;

		else if( strcmp( str[lcv], "SELECT" ) == 0 ) dualshock_button_right |= 0x0100;
		else if( strcmp( str[lcv], "L3" ) == 0 ) dualshock_button_right |= 0x0200;
		else if( strcmp( str[lcv], "R3" ) == 0 ) dualshock_button_right |= 0x0400;
		else if( strcmp( str[lcv], "START" ) == 0 ) dualshock_button_right |= 0x0800;

		else if( strcmp( str[lcv], "UP" ) == 0 ) dualshock_button_right |= 0x1000;
		else if( strcmp( str[lcv], "RIGHT" ) == 0 ) dualshock_button_right |= 0x2000;
		else if( strcmp( str[lcv], "DOWN" ) == 0 ) dualshock_button_right |= 0x4000;
		else if( strcmp( str[lcv], "LEFT" ) == 0 ) dualshock_button_right |= 0x8000;

		else if( strcmp( str[lcv], "TOGGLE_MOUSE_LEFT" ) == 0 ) toggle_dualshock_mouse[1][1] = 1;
		else if( strcmp( str[lcv], "TOGGLE_MOUSE_RIGHT" ) == 0 ) toggle_dualshock_mouse[1][1] = 2;
		else if( strcmp( str[lcv], "TOGGLE_MOUSE_DPAD" ) == 0 ) toggle_dualshock_mouse[1][1] = 3;

		else if( strcmp( str[lcv], "TOGGLE_DPAD_LEFT" ) == 0 ) toggle_dualshock_dpad[1][1] = 1;
		else if( strcmp( str[lcv], "TOGGLE_DPAD_RIGHT" ) == 0 ) toggle_dualshock_dpad[1][1] = 2;
		else if( strcmp( str[lcv], "TOGGLE_DPAD_DPAD" ) == 0 ) toggle_dualshock_dpad[1][1] = 3;
	}

	if( toggle_dualshock_mouse[1][0] != toggle_dualshock_mouse[1][1] )
		toggle_dualshock_allow[1] = 1;

	if( toggle_dualshock_dpad[1][0] != toggle_dualshock_dpad[1][1] )
		toggle_dualshock_allow[1] = 1;




	for( lcv = 0; lcv < 8; lcv++ )
		str[lcv][0] = 0;

	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "MIDDLE = %s %s %s %s %s %s %s %s",
		str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7] );


	// check keymaps
	dualshock_button_middle = 0;
	for( lcv = 0; lcv < 8; lcv++ ) {
		if( strcmp( str[lcv], "L2" ) == 0 ) dualshock_button_middle |= 0x0001;
		else if( strcmp( str[lcv], "R2" ) == 0 ) dualshock_button_middle |= 0x0002;
		else if( strcmp( str[lcv], "L1" ) == 0 ) dualshock_button_middle |= 0x0004;
		else if( strcmp( str[lcv], "R1" ) == 0 ) dualshock_button_middle |= 0x0008;

		else if( strcmp( str[lcv], "TRIANGLE" ) == 0 ) dualshock_button_middle |= 0x0010;
		else if( strcmp( str[lcv], "CIRCLE" ) == 0 ) dualshock_button_middle |= 0x0020;
		else if( strcmp( str[lcv], "CROSS" ) == 0 ) dualshock_button_middle |= 0x0040;
		else if( strcmp( str[lcv], "SQUARE" ) == 0 ) dualshock_button_middle |= 0x0080;

		else if( strcmp( str[lcv], "SELECT" ) == 0 ) dualshock_button_middle |= 0x0100;
		else if( strcmp( str[lcv], "L3" ) == 0 ) dualshock_button_middle |= 0x0200;
		else if( strcmp( str[lcv], "R3" ) == 0 ) dualshock_button_middle |= 0x0400;
		else if( strcmp( str[lcv], "START" ) == 0 ) dualshock_button_middle |= 0x0800;

		else if( strcmp( str[lcv], "UP" ) == 0 ) dualshock_button_middle |= 0x1000;
		else if( strcmp( str[lcv], "RIGHT" ) == 0 ) dualshock_button_middle |= 0x2000;
		else if( strcmp( str[lcv], "DOWN" ) == 0 ) dualshock_button_middle |= 0x4000;
		else if( strcmp( str[lcv], "LEFT" ) == 0 ) dualshock_button_middle |= 0x8000;

		else if( strcmp( str[lcv], "TOGGLE_MOUSE_LEFT" ) == 0 ) toggle_dualshock_mouse[2][1] = 1;
		else if( strcmp( str[lcv], "TOGGLE_MOUSE_RIGHT" ) == 0 ) toggle_dualshock_mouse[2][1] = 2;
		else if( strcmp( str[lcv], "TOGGLE_MOUSE_DPAD" ) == 0 ) toggle_dualshock_mouse[2][1] = 3;

		else if( strcmp( str[lcv], "TOGGLE_DPAD_LEFT" ) == 0 ) toggle_dualshock_dpad[2][1] = 1;
		else if( strcmp( str[lcv], "TOGGLE_DPAD_RIGHT" ) == 0 ) toggle_dualshock_dpad[2][1] = 2;
		else if( strcmp( str[lcv], "TOGGLE_DPAD_DPAD" ) == 0 ) toggle_dualshock_dpad[2][1] = 3;
	}

	if( toggle_dualshock_mouse[2][0] != toggle_dualshock_mouse[2][1] )
		toggle_dualshock_allow[2] = 1;

	if( toggle_dualshock_dpad[2][0] != toggle_dualshock_dpad[2][1] )
		toggle_dualshock_allow[2] = 1;


		

	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SNAP_TOGGLE = %X", &dualshock_mouse_snap_key );


	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SENSITIVITY = %f", &dualshock_mouse_sensitive );


	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "MOUSE_DEADZONE_SENSITIVITY = %f", &dualshock_mouse_deadzone );


	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SNAP_SPEED = %d", &dualshock_mouse_snap_speed );


	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "SENSITIVITY = %f", &dualshock_mouse_snap_sensitive );


	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "MOUSE_DEADZONE_SENSITIVITY = %f", &dualshock_mouse_snap_deadzone );


	do {
		fgets( line, 512, fp );
	} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
	sscanf_s( line, "MAX = %d", &dualshock_wheel_max );


	{
		static int once = 0;

		if( once == 0 ) {
			once = 1;

			do {
				fgets( line, 512, fp );
			} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
			sscanf_s( line, "START = %d", &dualshock_wheel );

			dualshock_wheel--;
		} else {
			int temp;

			do {
				fgets( line, 512, fp );
			} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
			sscanf_s( line, "START = %d", &temp );
		}
	}


	for( lcv = 0; lcv < dualshock_wheel_max; lcv++ ) {
		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "UP = $%X", &dualshock_mouse_up[lcv] );

		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "DOWN = $%X", &dualshock_mouse_down[lcv] );

		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "LEFT = $%X", &dualshock_mouse_left[lcv] );

		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "RIGHT = $%X", &dualshock_mouse_right[lcv] );


		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "UP-LEFT = $%X", &dualshock_mouse_up_left[lcv] );

		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "UP-RIGHT = $%X", &dualshock_mouse_up_right[lcv] );

		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "DOWN-LEFT = $%X", &dualshock_mouse_down_left[lcv] );

		do {
			fgets( line, 512, fp );
		} while( line[0] == ';' || line[0] == 0x0d || line[0] == 0x0a );
		sscanf_s( line, "DOWN-RIGHT = $%X", &dualshock_mouse_down_right[lcv] );
	}


	fclose( fp );
}


void Load_Hacks_INI()
{
	char line[512];
	char temp_str[512];
	FILE *fp;
	
	
	fopen_s(&fp, "inis\\ePSXe_shark_hacks.ini", "r" );

	if( fp ) {
		temp_str[0] = 0;
		
		fgets( line, 512, fp );
		sscanf_s( line, "version = %s", &temp_str );

		fgets( line, 512, fp );

		
		if( strcmp( temp_str, EPSXE_INI_HACKS_VERSION ) != 0 ) {
			fclose(fp);
			fp = 0;
		}
	}


	if( !fp ) {
		fopen_s(&fp, "inis\\ePSXe_shark_hacks.ini", "w" );
		if( !fp ) return;


		fprintf( fp, "version = %s\n", EPSXE_INI_HACKS_VERSION );
		fprintf( fp, "\n" );
		fprintf( fp, "tomb_raider_1_cdda = 0\n" );
		fprintf( fp, "tomb_raider_2_cdda = 0\n" );
		fprintf( fp, "rebel_assault_2_usa = 0\n" );
		fclose(fp);


		fopen_s(&fp, "inis\\ePSXe_shark_hacks.ini", "r" );
		if( !fp ) return;

		fgets( line, 512, fp );
		fgets( line, 512, fp );
	}



	fgets( line, 512, fp );
	sscanf_s( line, "tomb_raider_1_cdda = %d", &tomb1_cdda_hack );

	fgets( line, 512, fp );
	sscanf_s( line, "tomb_raider_2_cdda = %d", &tomb2_cdda_hack );

	fgets( line, 512, fp );
	sscanf_s( line, "rebel_assault_2_usa = %d", &rebel2_gpudma_hack );



	fclose(fp);
}


void Load_INI()
{
	char line[512];
	char file1[512], temp_str[512];
	FILE *fp;
	
	
	fopen_s(&fp, "inis\\ePSXe_shark.ini", "r" );

	if( fp ) {
		fgets( line, 512, fp );
		fgets( line, 512, fp );

		temp_str[0] = 0;
		fgets( line, 512, fp );
		sscanf_s( line, "version = %s", &temp_str );

		if( strcmp( temp_str, EPSXE_INI_VERSION ) != 0 ) {
			fclose(fp);
			fp = 0;
		}
	}


	if( !fp ) {
		CreateDirectoryA("inis",NULL);
		fopen_s(&fp, "inis\\ePSXe_shark.ini", "w" );
		if( !fp ) return;


		fprintf( fp, "delay_injection = 100\n" );
		fprintf( fp, "\n" );
		fprintf( fp, "version = %s\n", EPSXE_INI_VERSION );
		fprintf( fp, "timer_precise = 1\n" );
		fprintf( fp, "fast_start = 0\n" );
		fprintf( fp, "autorun = 0\n" );
		fprintf( fp, "spu_framelimit = 1\n" );
		fprintf( fp, "throttle_fps_off = 100000\n" );
		fprintf( fp, "max_fps_key = 0x08  (BACKSPACE)\n" );
		fprintf( fp, "move_window = 0\n" );
		fprintf( fp, "slow_boot = 0\n" );
		fprintf( fp, "opengl2_vramauto = 0\n" );
		fprintf( fp, "\n" );
		fprintf( fp, "shark_cheater = 0\n" );
		fprintf( fp, "autostart_controllers = 0\n" );
		fprintf( fp, "dvd5_mode = 0\n" );
		fprintf( fp, "disable_memcards = 0\n" );
		fprintf( fp, "mouse_sensitive = 100.0 100.0\n" );
		fprintf( fp, "guncon_sensitive = 100.0 100.0\n" );
		fprintf( fp, "\n" );
		fprintf( fp, "show_cursor = 0\n" );
		fprintf( fp, "apu_cycles = 16 4\n" );
		fprintf( fp, "dualshock_mouse = 0\n" );
		fprintf( fp, "dualshock_mouse_ini = ape_escape.ini\n" );
		fprintf( fp, "negcon_ini = none\n" );
		fprintf( fp, "\n" );
		fprintf( fp, "opcode_tracer = 0\n" );
		fprintf( fp, "cdrom_tracer = 0\n" );
		fprintf( fp, "\n" );
		fprintf( fp, "analog_deadzone_left = 0 0 0 0\n" );
		fprintf( fp, "analog_deadzone_right = 0 0 0 0\n" );
		fprintf( fp, "konami_justifier1 = 0\n" );
		fprintf( fp, "konami_justifier2 = 0\n" );
		fprintf( fp, "mousewheel_converter = 0\n" );

		fclose(fp);

		
		fopen_s(&fp, "inis\\ePSXe_shark.ini", "r" );
		if( !fp ) return;

		fgets( line, 512, fp );
		fgets( line, 512, fp );
		fgets( line, 512, fp );
	}



	fgets( line, 512, fp );
	sscanf_s( line, "timer_precise = %d", &timer_precise );


	fgets( line, 512, fp );
	sscanf_s( line, "fast_start = %d", &start_gpu );
	if( start_gpu > 0 ) {
		static int once = 0;

		if( once == 0 ) {
			start_gpu = 0;

			once = 1;
		}
		else
			start_gpu = 200;
	}
	else
		start_gpu = 200;


	fgets( line, 512, fp );
	sscanf_s( line, "autorun = %d", &run_iso_mode );

	fgets( line, 512, fp );
	sscanf_s( line, "spu_framelimit = %d", &spu_framelimit );

	fgets( line, 512, fp );
	sscanf_s( line, "throttle_fps_off = %f", &fFrameRateHz );
	if( fFrameRateHz > 0 )
		sleep_throttle = 1;


	fgets( line, 512, fp );
	sscanf_s( line, "max_fps_key = %X", &throttle_max_key );

	fgets( line, 512, fp );
	sscanf_s( line, "move_window = %d", &move_window );

	fgets( line, 512, fp );
	sscanf_s( line, "slow_boot = %d", &slow_boot );

	fgets( line, 512, fp );
	sscanf_s( line, "opengl2_vramauto = %d", &opengl2_vramauto );

	fgets( line, 512, fp );


	fgets( line, 512, fp );
	sscanf_s( line, "shark_cheater = %d", &shark_cheater_boot );


	fgets( line, 512, fp );
	sscanf_s( line, "autostart_controllers = %d", &autostart_controllers );


	fgets( line, 512, fp );
	sscanf_s( line, "dvd5_mode = %d", &DVD5_mode );


	fgets( line, 512, fp );
	sscanf_s( line, "disable_memcards = %d", &disable_memcards );


	fgets( line, 512, fp );
	sscanf_s( line, "mouse_sensitive = %f %f",
		&mouse_sensitive_x, &mouse_sensitive_y );


	fgets( line, 512, fp );
	sscanf_s( line, "guncon_sensitive = %f %f",
		&guncon_sensitive_x, &guncon_sensitive_y );


	fgets( line, 512, fp );


	fgets( line, 512, fp );
	sscanf_s( line, "show_cursor = %d", &show_cursor );


	fgets( line, 512, fp );
	sscanf_s( line, "apu_cycles = %d %d", &apu_cycles, &apu_rate );


	fgets( line, 512, fp );
	sscanf_s( line, "dualshock_mouse = %d",
		&dualshock_mouse_active );


	fgets( line, 512, fp );
	sscanf_s( line, "dualshock_mouse_ini = %s",
		temp_str );

	if( dualshock_mouse_active ) {
		strcpy_s( file1, "inis\\shark_presets\\" );
		strcat_s( file1, temp_str );

		Load_Dualshock_Mouse_INI( file1 );
	}


	fgets( line, 512, fp );
	sscanf_s( line, "negcon_ini = %s",
		temp_str );

	strcpy_s( file1, "inis\\shark_presets\\" );
	strcat_s( file1, temp_str );

	Load_Negcon_INI( file1 );


	fgets( line, 512, fp );


	fgets( line, 512, fp );
	sscanf_s( line, "opcode_tracer = %d %X", &opcode_tracer, &opcode_addr );


	fgets( line, 512, fp );
	sscanf_s( line, "cdrom_tracer = %d",
		&cdrom_tracer );


	fgets( line, 512, fp );


	fgets( line, 512, fp );
	sscanf_s( line, "analog_deadzone_left = %d %d %d %d",
		&analog_deadzone_lx_1, &analog_deadzone_lx_2,
		&analog_deadzone_ly_1, &analog_deadzone_ly_2 );


	fgets( line, 512, fp );
	sscanf_s( line, "analog_deadzone_right = %d %d %d %d",
		&analog_deadzone_rx_1, &analog_deadzone_rx_2,
		&analog_deadzone_ry_1, &analog_deadzone_ry_2 );


	fgets( line, 512, fp );
	sscanf_s( line, "konami_justifier1 = %d", &konami_justifier1 );


	fgets( line, 512, fp );
	sscanf_s( line, "konami_justifier2 = %d", &konami_justifier2 );
	

	fgets( line, 512, fp );
	sscanf_s( line, "mousewheel_converter = %d", &mousewheel_converter );


	fclose(fp);
}

static int detect_version()
{
	unsigned char *mem;
	unsigned int value;


	// base EXE address
	mem = (unsigned char *) 0x400000 + 0x2e197;
	value = * ((unsigned long *) mem);

	if( value == (unsigned int) 0x83067902 )
		return 180;

	if( value == (unsigned int) 0x50f038a3 )
		return 170;

	if( value == (unsigned int) 0x00008f35 )
		return 160;

	if( value == (unsigned int) 0x9090c300 )
		return 152;

	return 0;
}

void Attach_DVD5_Hooks()
{
	ePSXe_version = detect_version();

	if( ePSXe_version == 180 )
	{
		Attach_180();
	}
	else if( ePSXe_version == 170 )
	{
		Attach_170();
	}
	else if( ePSXe_version == 160 )
	{
		Attach_160();
	}
	else if( ePSXe_version == 152 )
	{
		Attach_152();
	}
}

DWORD WINAPI Inject_Code()
{
#ifdef DEBUG_APP
	fp_debug = fopen( "log-dvd5.txt", "w" );
#endif

	EnterCriticalSection(&cs);

	Load_INI();
	Load_Hacks_INI();

	if( timer_precise )
		timeBeginPeriod(1);

	Attach_DVD5_Hooks();

	if( shark_cheater_boot )
	{
		// Shark Cheater stuff
		UINT32 ptr_list[20], size_list[20];
		switch( ePSXe_version )
		{
		case 180:
			// RAM
			ptr_list[0] = 0xA52EA0;
			size_list[0] = 0x200000;
			break;
		case 170:
			// RAM
			ptr_list[0] = 0x94C020;
			size_list[0] = 0x200000;
			break;

		case 160:
			ptr_list[0] = 0x5B6E40;
			size_list[0] = 0x200000;
			break;

		case 152:
			ptr_list[0] = 0x5B5C40;
			size_list[0] = 0x200000;
			break;

		default:
			break;
		}

		shark_cheater.Init_Emu( SYSTEM_PS1, ptr_list, size_list );
		shark_cheater.Run_Thread( hThis );
	}

	Start_Autorun();
	LeaveCriticalSection(&cs);
	return 0;
}

// Define the DLL's main function
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call){

	case DLL_PROCESS_ATTACH:
		hThis = hModule;
		hHost = GetModuleHandle(NULL);

		//this will cause problem !
		//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Inject_Code, NULL, 0, 0 ); 
        InitializeCriticalSection(&cs);
		Inject_Code();
		break;


	case DLL_PROCESS_DETACH:
		//FreeLibraryAndExitThread( hModule, 0 );
		//MessageBox(0, "Process could not be loaded!", "Error", MB_ICONERROR);
        DeleteCriticalSection(&cs);
		break;
	};

	return TRUE;
}
