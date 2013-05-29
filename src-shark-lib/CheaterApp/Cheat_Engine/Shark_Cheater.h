#ifndef _SHARK_CHEATER_H
#define _SHARK_CHEATER_H


#define SYSTEM_PS1 1
#define SYSTEM_GBC 2


#include "shark_base.h"


class Shark_Cheater : public Shark_Base
{
public:
	Shark_Cheater()
		: Shark_Base() {}


	void Set_System( UINT32 system );
	void Set_Memory( UINT32 *ptr_list, UINT32 *size_list );

	void Convert_Cheat( UINT32 opcode, UINT32 address, UINT32 value );

	void Dump_Memory();
	void Edit_Memory( char *path );
	void Load_Memory();


private:
	void Dump_File( char *name, void *ptr, int size );
	void Load_File( char *name, void *ptr, int size );

	UINT32 Get_Memory_Ptr( UINT32 addr );

	UINT32 Map_PS1( UINT32 addr );
	UINT32 Map_GBC( UINT32 addr );


	UINT32 _system;

	UINT32 _emu_ram;
	UINT32 _emu_rom;
	UINT32 _emu_sram;
	UINT32 _emu_ram2;
	UINT32 _emu_ram3;

	UINT32 _ram_size;
	UINT32 _rom_size;
	UINT32 _sram_size;
};


#endif
