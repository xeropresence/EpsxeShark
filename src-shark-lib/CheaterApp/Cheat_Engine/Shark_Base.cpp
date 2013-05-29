#include "shark_base.h"


#define _CUR_CHEAT _shark_code


unsigned int next_address = 0;


void Shark_Base::Convert_Code( UINT32 opcode, UINT32 address, UINT32 value )
{
	_CUR_CHEAT.addr = address;
	_CUR_CHEAT.value = value;


	// START of cheat
	if( opcode == -1 && address == -1 && value == -1 )
	{
		Convert_Cheat_RESET();
		return;
	}


	switch( opcode )
	{
		// MISC
		case 0x20: Convert_Cheat_SLIDE(); break;
		case 0x21: Convert_Cheat_SLIDE_INC(); break;
		case 0x22: Convert_Cheat_SLIDE_DEC(); break;
		case 0x3F: Convert_Cheat_CODE_BLOCK(); break;


		// 8-BIT
		case 0x40: Convert_Cheat_WRITE(1); break;
		case 0x4F: Convert_Cheat_WRITE_NEXT(1); break;
		case 0x50: Convert_Cheat_EQUAL(1); break;
		case 0x51: Convert_Cheat_NOTEQUAL(1); break;
		case 0x52: Convert_Cheat_LESS(1); break;
		case 0x53: Convert_Cheat_GREATER(1); break;


		// 16-BIT
		case 0x80: Convert_Cheat_WRITE(2); break;
		case 0x8F: Convert_Cheat_WRITE_NEXT(2); break;
		case 0x90: Convert_Cheat_EQUAL(2); break;
		case 0x91: Convert_Cheat_NOTEQUAL(2); break;
		case 0x92: Convert_Cheat_LESS(2); break;
		case 0x93: Convert_Cheat_GREATER(2); break;


		// 32-BIT
		case 0xC0: Convert_Cheat_WRITE(4); break;
		case 0xCF: Convert_Cheat_WRITE_NEXT(4); break;
		case 0xD0: Convert_Cheat_EQUAL(4); break;
		case 0xD1: Convert_Cheat_NOTEQUAL(4); break;
		case 0xD2: Convert_Cheat_LESS(4); break;
		case 0xD3: Convert_Cheat_GREATER(4); break;


		default: break;
	}
}


// FIRST code of cheat
void Shark_Base::Convert_Cheat_RESET()
{
	_CUR_CHEAT.type = RESET;


	Add_Code( _CUR_CHEAT );

	// =================================
	// =================================
	// =================================

	// init
	_slide_runs = 0;
	_code_block = 0;
}


void Shark_Base::Convert_Cheat_WRITE( int size )
{
	_CUR_CHEAT.type = WRITE;
	_CUR_CHEAT.size = size;


	Add_Code( _CUR_CHEAT );


	next_address = _CUR_CHEAT.addr + size;

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}


	// handle slide codes - STOP!
	if( _code_block == 0 && _slide_runs )
	{
		Add_Code_Slide( _slide_addr, _slide_value, _slide_runs, _slide_size );

		_slide_runs = 0;
	}
}


void Shark_Base::Convert_Cheat_WRITE_NEXT( int size )
{
	_CUR_CHEAT.addr = next_address;
	_CUR_CHEAT.type = WRITE;
	_CUR_CHEAT.size = size;


	Add_Code( _CUR_CHEAT );

	next_address = _CUR_CHEAT.addr + size;

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}


	// handle slide codes - STOP!
	if( _code_block == 0 && _slide_runs )
	{
		Add_Code_Slide( _slide_addr, _slide_value, _slide_runs, _slide_size );

		_slide_runs = 0;
	}
}


/*
20 xxxxxxxx yyyyyyyy

y = runs, x = offset bump


ex.
; 2 enemies
20 80000010 0002

; code check
90 80080000 3040
90 80080002 9080

; HP write
93 80080005 0050
80 80080005 0001

===>
===>

90 80080000 3040
90 80080002 9080
93 93080005 0050
80 80080005 0001

90 80080010 3040
90 80080012 9080
93 83080015 0050
80 80080015 0001


addr = 10 (offset bump)
size = 4 (# opcodes)
runs = 2 (# write commands)
*/

void Shark_Base::Convert_Cheat_SLIDE()
{
	_slide_runs = _CUR_CHEAT.value;
	_slide_addr = _CUR_CHEAT.addr & 0xffffff;

	_slide_value = 0;
	_slide_size = 0;

	// =================================
	// =================================
	// =================================

	_CUR_CHEAT.value = _slide_runs;
	_CUR_CHEAT.type = NOP_SLIDE;


	Add_Code( _CUR_CHEAT );
}


/*
21 xxxxxxxx yyyyzzzzz

xxxx = addr bump
yyyy = inc bump
zzzz = run count
*/

void Shark_Base::Convert_Cheat_SLIDE_INC()
{
	_slide_value = ( _CUR_CHEAT.value >> 16 ) & 0xffff;
	_slide_runs = _CUR_CHEAT.value & 0xffff;
	_slide_addr = _CUR_CHEAT.addr & 0xffffff;

	_slide_size = 0;

	// =================================
	// =================================
	// =================================

	_CUR_CHEAT.value = _slide_runs;
	_CUR_CHEAT.type = NOP_SLIDE;


	Add_Code( _CUR_CHEAT );
}


/*
22 xxxxxxxx yyyyzzzzz

xxxx = addr bump
yyyy = dec bump
zzzz = run count
*/

void Shark_Base::Convert_Cheat_SLIDE_DEC()
{
	_slide_value = ( _CUR_CHEAT.value >> 16 ) & 0xffff;
	_slide_runs = _CUR_CHEAT.value & 0xffff;
	_slide_addr = _CUR_CHEAT.addr & 0xffffff;

	_slide_size = 0;

	_slide_value = -1 * _slide_value;

	// =================================
	// =================================
	// =================================

	_CUR_CHEAT.value = _slide_runs;
	_CUR_CHEAT.type = NOP_SLIDE;


	Add_Code( _CUR_CHEAT );
}



/*
3F xxxxxxxx yyyy

0000 = start code block
0001 = end code block


ex.
; field check
90 801F0080 0300
90 801F0081 0301

; random battles - off
3F 00000000 0000
80 801F0082 0333
80 801F0083 CCCC
80 801F0084 DDDD
80 801F0085 EEEE
80 801F0086 FFFF
3F 00000000 0001

===>
===>

90 801F0080 0300
90 801F0081 0301
80 801F0082 0333

90 801F0080 0300
90 801F0081 0301
80 801F0083 CCCC

90 801F0080 0300
90 801F0081 0301
80 801F0084 DDDD

90 801F0080 0300
90 801F0081 0301
80 801F0085 EEEE

90 801F0080 0300
90 801F0081 0301
80 801F0086 FFFF
*/

void Shark_Base::Convert_Cheat_CODE_BLOCK()
{
	_CUR_CHEAT.type = NOP_CODE_BLOCK;


	Add_Code( _CUR_CHEAT );

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}


	if( _CUR_CHEAT.value == 0 )
	{
		_code_block = 1;
	}


	if( _CUR_CHEAT.value == 1 && _slide_runs )
	{
		Add_Code_Slide( _slide_addr, _slide_value, _slide_runs, _slide_size );

		_code_block = 0;
		_slide_runs = 0;
	}
}


void Shark_Base::Convert_Cheat_EQUAL( int size )
{
	_CUR_CHEAT.type = EQUAL;
	_CUR_CHEAT.size = size;


	Add_Code( _CUR_CHEAT );

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}
}


void Shark_Base::Convert_Cheat_NOTEQUAL( int size )
{
	_CUR_CHEAT.type = NOT_EQUAL;
	_CUR_CHEAT.size = size;


	Add_Code( _CUR_CHEAT );

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}
}


void Shark_Base::Convert_Cheat_LESS( int size )
{
	_CUR_CHEAT.type = LESS;
	_CUR_CHEAT.size = size;


	Add_Code( _CUR_CHEAT );

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}
}


void Shark_Base::Convert_Cheat_GREATER( int size )
{
	_CUR_CHEAT.type = GREATER;
	_CUR_CHEAT.size = size;


	Add_Code( _CUR_CHEAT );

	// =================================
	// =================================
	// =================================

	// handle slide codes
	if( _slide_runs )
	{
		_slide_size++;
	}
}
