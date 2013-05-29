#ifndef _SHARK_BASE_H
#define _SHARK_BASE_H



#include "shark_engine.h"


class Shark_Base : virtual public Shark_Engine
{
public:
	Shark_Base()
		: Shark_Engine()
	{
		_slide_size = 0;
		_slide_runs = 0;
		_slide_addr = 0;
		_slide_value = 0;

		_code_block = 0;
	}


	void Convert_Code( UINT32 opcode, UINT32 address, UINT32 value );


private:
	// MISC
	void Convert_Cheat_RESET();
	void Convert_Cheat_SLIDE();
	void Convert_Cheat_SLIDE_INC();
	void Convert_Cheat_SLIDE_DEC();
	void Convert_Cheat_CODE_BLOCK();

	// MEMORY WRITE
	void Convert_Cheat_WRITE( int size );
	void Convert_Cheat_WRITE_NEXT( int size );

	// CONDITIONAL
	void Convert_Cheat_EQUAL( int size );
	void Convert_Cheat_NOTEQUAL( int size );
	void Convert_Cheat_LESS( int size );
	void Convert_Cheat_GREATER( int size );



	SHARK_CODE_TYPE _shark_code;

	int _slide_size;
	int _slide_runs;
	int _slide_addr;
	int _slide_value;

	int _code_block;
};


#endif
