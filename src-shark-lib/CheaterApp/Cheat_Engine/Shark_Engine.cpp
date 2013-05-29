#include <memory.h>
#include "shark_engine.h"


#define _CUR_CODE _shark_codes[_code_ptr]


void Shark_Engine::Clear_List()
{
	_code_list_size = 0;
	_code_ptr = 0;
	_write_skip = 0;
}


void Shark_Engine::Add_Code( SHARK_CODE_TYPE &code )
{
	memcpy( &_CUR_CODE, &code, sizeof(SHARK_CODE_TYPE) );
	
	_code_list_size++;
	_code_ptr++;
}


void Shark_Engine::Add_Code_Slide( int addr, int value, int runs, int codes )
{
	int inc_total;


	// already performed at least '1 run'
	runs--;
	inc_total = 0;

	while( runs-- )
	{


		memcpy( &_CUR_CODE, &_CUR_CODE - codes, sizeof(SHARK_CODE_TYPE) * codes );

		// make changes: offset, increment
		for( int lcv=0; lcv<codes; lcv++ )
		{
			_CUR_CODE.addr += addr;

			// adds to previous value
			_CUR_CODE.value += value;

			_code_list_size++;
			_code_ptr++;
		}
	}
}


void Shark_Engine::Update_Memory()
{
	_code_ptr = 0;
	while( _code_ptr < _code_list_size )
	{
		switch( _CUR_CODE.type )
		{
		// MISC
		case RESET: Code_Reset(); break;
		case NOP_SLIDE: Code_Nop_Slide(); break;
		case NOP_CODE_BLOCK: Code_Nop_Code_Block(); break;


		// MEMORY
		case WRITE: Code_Write(); break;


		// CONDITIONALS
		case EQUAL: Code_Equal(); break;
		case NOT_EQUAL: Code_Not_Equal();	break;

		case GREATER: Code_Greater();	break;
		case GREATER_EQUAL: Code_Greater_Equal(); break;

		case LESS: Code_Less();	break;
		case LESS_EQUAL: Code_Less_Equal(); break;


		default: break;
		}


		// run next code
		_code_ptr++;
	}
}


void Shark_Engine::Code_Reset()
{
	// first code of -THIS- cheat
	_write_skip = 0;
	_code_block_skip = 0;
}


void Shark_Engine::Code_Nop_Slide()
{
	// conditional failed - bypass write codes
	if( _write_skip )
	{
		// ex. 1 * (5 write runs)
		_write_skip *= _CUR_CODE.value;
	}
}


void Shark_Engine::Code_Nop_Code_Block()
{
	// conditional failed - bypass write codes
	if( _write_skip )
	{
		// START write block
		if( _CUR_CODE.value == 0 )
		{
			_code_block_skip++;
		}

		// END write block
		else if( _CUR_CODE.value == 1 )
		{
			_code_block_skip--;

			_write_skip--;
		}
	}
}


void Shark_Engine::Code_Write()
{
	// conditional failed - bypass code block
	if( _code_block_skip )
	{
		return;
	}

	// conditional failed - bypass write codes
	if( _write_skip )
	{
		_write_skip--;
		return;
	}



	switch( _CUR_CODE.size )
	{
		case 1:
			*((UINT8 *) _CUR_CODE.addr) = (UINT8) _CUR_CODE.value;
			break;

		case 2:
			*((UINT16 *) _CUR_CODE.addr) = (UINT16) _CUR_CODE.value;
			break;

		case 3:
			*((UINT16 *) _CUR_CODE.addr) = (UINT16) _CUR_CODE.value;
			*((UINT8 *) (_CUR_CODE.addr+2)) = (UINT8) (_CUR_CODE.value>>16);
			break;

		case 4:
			*((UINT32 *) _CUR_CODE.addr) = (UINT32) _CUR_CODE.value;
			break;

		default:
			break;
	}
}


void Shark_Engine::Code_Equal()
{
	// conditional failed - bypass write codes
	if( _write_skip ) return;


	bool flag;

	switch( _CUR_CODE.size )
	{
		case 1:
			flag = ( (*((UINT8 *) _CUR_CODE.addr)) == (UINT8) _CUR_CODE.value );
			break;

		case 2:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) == (UINT16) _CUR_CODE.value );
			break;

		case 3:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) == (UINT16) _CUR_CODE.value );
			flag &= ( (*((UINT8 *) (_CUR_CODE.addr+2))) == (UINT8) (_CUR_CODE.value>>16) );
			break;

		case 4:
			flag = ( (*((UINT32 *) _CUR_CODE.addr)) == (UINT32) _CUR_CODE.value );
			break;

		default:
			break;
	}


	// conditional failed - bypass write codes
	if( !flag )
	{
		_write_skip = 1;
	}
}


void Shark_Engine::Code_Not_Equal()
{
	// conditional failed - bypass write codes
	if( _write_skip ) return;


	bool flag;

	switch( _CUR_CODE.size )
	{
		case 1:
			flag = ( (*((UINT8 *) _CUR_CODE.addr)) != (UINT8) _CUR_CODE.value );
			break;

		case 2:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) != (UINT16) _CUR_CODE.value );
			break;

		case 3:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) != (UINT16) _CUR_CODE.value );
			flag &= ( (*((UINT8 *) (_CUR_CODE.addr+2))) != (UINT8) (_CUR_CODE.value>>16) );
			break;

		case 4:
			flag = ( (*((UINT32 *) _CUR_CODE.addr)) != (UINT32) _CUR_CODE.value );
			break;

		default:
			break;
	}


	// conditional failed - bypass write codes
	if( !flag )
	{
		_write_skip = 1;
	}
}


void Shark_Engine::Code_Greater()
{
	// conditional failed - bypass write codes
	if( _write_skip ) return;


	bool flag;

	switch( _CUR_CODE.size )
	{
		case 1:
			flag = ( (*((UINT8 *) _CUR_CODE.addr)) > (UINT8) _CUR_CODE.value );
			break;

		case 2:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) > (UINT16) _CUR_CODE.value );
			break;

		case 3:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) > (UINT16) _CUR_CODE.value );
			flag &= ( (*((UINT8 *) (_CUR_CODE.addr+2))) > (UINT8) (_CUR_CODE.value>>16) );
			break;

		case 4:
			flag = ( (*((UINT32 *) _CUR_CODE.addr)) > (UINT32) _CUR_CODE.value );
			break;

		default:
			break;
	}


	// conditional failed - bypass write codes
	if( !flag )
	{
		_write_skip = 1;
	}
}


void Shark_Engine::Code_Less()
{
	// conditional failed - bypass write codes
	if( _write_skip ) return;


	bool flag;

	switch( _CUR_CODE.size )
	{
		case 1:
			flag = ( (*((UINT8 *) _CUR_CODE.addr)) < (UINT8) _CUR_CODE.value );
			break;

		case 2:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) < (UINT16) _CUR_CODE.value );
			break;

		case 3:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) < (UINT16) _CUR_CODE.value );
			flag &= ( (*((UINT8 *) (_CUR_CODE.addr+2))) < (UINT8) (_CUR_CODE.value>>16) );
			break;

		case 4:
			flag = ( (*((UINT32 *) _CUR_CODE.addr)) < (UINT32) _CUR_CODE.value );
			break;

		default:
			break;
	}


	// conditional failed - bypass write codes
	if( !flag )
	{
		_write_skip = 1;
	}
}


void Shark_Engine::Code_Greater_Equal()
{
	// conditional failed - bypass write codes
	if( _write_skip ) return;


	bool flag;

	switch( _CUR_CODE.size )
	{
		case 1:
			flag = ( (*((UINT8 *) _CUR_CODE.addr)) >= (UINT8) _CUR_CODE.value );
			break;

		case 2:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) >= (UINT16) _CUR_CODE.value );
			break;

		case 3:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) >= (UINT16) _CUR_CODE.value );
			flag &= ( (*((UINT8 *) (_CUR_CODE.addr+2))) >= (UINT8) (_CUR_CODE.value>>16) );
			break;

		case 4:
			flag = ( (*((UINT32 *) _CUR_CODE.addr)) >= (UINT32) _CUR_CODE.value );
			break;

		default:
			break;
	}


	// conditional failed - bypass write codes
	if( !flag )
	{
		_write_skip = 1;
	}
}


void Shark_Engine::Code_Less_Equal()
{
	// conditional failed - bypass write codes
	if( _write_skip ) return;


	bool flag;

	switch( _CUR_CODE.size )
	{
		case 1:
			flag = ( (*((UINT8 *) _CUR_CODE.addr)) <= (UINT8) _CUR_CODE.value );
			break;

		case 2:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) <= (UINT16) _CUR_CODE.value );
			break;

		case 3:
			flag = ( (*((UINT16 *) _CUR_CODE.addr)) <= (UINT16) _CUR_CODE.value );
			flag &= ( (*((UINT8 *) (_CUR_CODE.addr+2))) <= (UINT8) (_CUR_CODE.value>>16) );
			break;

		case 4:
			flag = ( (*((UINT32 *) _CUR_CODE.addr)) <= (UINT32) _CUR_CODE.value );
			break;

		default:
			break;
	}


	// conditional failed - bypass write codes
	if( !flag )
	{
		_write_skip = 1;
	}
}
