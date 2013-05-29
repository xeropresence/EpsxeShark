#define WIN32_LEAN_AND_MEAN // this will assume smaller exe

#include <windows.h>
#include <stdio.h>

#include "shark_cheater.h"


void Shark_Cheater::Set_System( UINT32 system )
{
	_system = system;
}


void Shark_Cheater::Dump_File( char *name, void *ptr, int size )
{
	FILE *fp;

	fopen_s(&fp, name, "wb" );
	if( !fp ) return;

	fwrite( ptr, 1, size, fp );
	fclose( fp );
}


void Shark_Cheater::Load_File( char *name, void *ptr, int size )
{
	FILE *fp;

	fopen_s(&fp, name, "rb" );
	if( !fp ) return;

	fread( ptr, 1, size, fp );
	fclose( fp );
}


void Shark_Cheater::Dump_Memory()
{
	if( _emu_ram )
		Dump_File( "dump-ram.bin", (void *) _emu_ram, _ram_size );

	if( _emu_sram )
		Dump_File( "dump-sram.bin", (void *) _emu_sram, _sram_size );
}


void Shark_Cheater::Edit_Memory( char *path )
{
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	int result;
	char str_temp[2048];


	strcpy_s( str_temp, path );
	
	if( _emu_sram )
	{
		strcat_s( str_temp, " " );
		strcat_s( str_temp, "dump-sram.bin" );
	}
	
	if( _emu_ram )
	{
		strcat_s( str_temp, " " );
		strcat_s( str_temp, "dump-ram.bin" );
	}


	si.cb = sizeof(STARTUPINFO);

	// Try to load our process
	result = CreateProcessA(NULL, str_temp, NULL, NULL, FALSE,
													NULL, NULL, NULL, &si, &pi);
	if(!result)
	{
		MessageBox(0, "Hex editor could not be loaded!", "Error", MB_ICONERROR);
	}
	else
	{
		WaitForSingleObject( pi.hProcess,INFINITE );
	}
}


void Shark_Cheater::Load_Memory()
{
	switch( _system )
	{
		case SYSTEM_PS1:
			Load_File( "dump-ram.bin", (void *) _emu_ram, _ram_size );
			break;


		case SYSTEM_GBC:
			Load_File( "dump-ram.bin", (void *) _emu_ram, _ram_size );
			Load_File( "dump-sram.bin", (void *) _emu_sram, _sram_size );
			break;
	}
}


void Shark_Cheater::Set_Memory( UINT32 *ptr_list, UINT32 *size_list )
{
	_ram_size = 0;
	_sram_size = 0;
	_rom_size = 0;

	_emu_ram = 0;
	_emu_sram = 0;
	_emu_rom = 0;


	switch( _system )
	{
		case SYSTEM_PS1:
			_emu_ram = ptr_list[0];

			_ram_size = size_list[0];
			break;


		case SYSTEM_GBC:
			_emu_ram = ptr_list[0];
			_emu_rom = ptr_list[1];
			_emu_sram = ptr_list[2];

			_ram_size = size_list[0];
			_rom_size = size_list[1];
			_sram_size = size_list[2];
			break;
	}
}


UINT32 Shark_Cheater::Map_PS1( UINT32 addr )
{
	UINT32 map;

	/*
	Memory Map (PS1)

	RAM = [80:000000-80:1FFFFF]
	*/

	map = addr >> 24;

	if( map == 0x80 )
	{
		addr = _emu_ram + ( addr & 0x1fffff );
	}

	return addr;
}


UINT32 Shark_Cheater::Map_GBC( UINT32 addr )
{
	UINT32 map;

	/*
	 Memory Map (GBC)

	 RAM = [80:000000-80:01FFFF]
	 ROM = [C0:000000-C0:FFFFFF]
	SRAM = [40:000000-40:01FFFF]
	*/

	map = addr >> 24;

	switch( map )
	{
		case 0x80:
			addr = _emu_ram + ( addr & 0xffffff );
			break;

		case 0xC0:
			addr = _emu_rom + ( addr & 0xffffff );
			break;

		case 0x40:
			addr = _emu_sram + ( addr & 0xffffff );
			break;
	}

	return addr;
}


UINT32 Shark_Cheater::Get_Memory_Ptr( UINT32 addr )
{
	switch( _system )
	{
		case SYSTEM_PS1:
			addr = Map_PS1(addr);
			break;

		case SYSTEM_GBC:
			addr = Map_GBC(addr);
			break;
	}

	return addr;
}


void Shark_Cheater::Convert_Cheat( UINT32 opcode, UINT32 address, UINT32 value )
{
	if( !_emu_ram ) return;


	Convert_Code( opcode, Get_Memory_Ptr(address), value );
}
