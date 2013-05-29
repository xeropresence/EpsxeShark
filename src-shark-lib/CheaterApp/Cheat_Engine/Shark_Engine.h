#ifndef SHARK_ENGINE_H_INCLUDED
#define SHARK_ENGINE_H_INCLUDED


#define UINT8 unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned int


struct SHARK_CODE_TYPE
{
	UINT32 addr;
	UINT32 value;
	UINT8 type;
	UINT8 size;
};


enum SHARK_OPCODE_TYPE
{ RESET, NOP_SLIDE, NOP_CODE_BLOCK, WRITE,
	EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL };


class Shark_Engine
{
public:
	Shark_Engine()
	{
		_code_list_size = 0;
		_code_ptr = 0;
		_write_skip = 0;
		_code_block_skip = 0;
	}
	virtual ~Shark_Engine() {}


	void Clear_List();
	void Update_Memory();
	

protected:
	void Add_Code( SHARK_CODE_TYPE &code );
	void Add_Code_Slide( int addr, int value, int runs, int codes );


private:
	void Code_Reset();
	void Code_Nop_Slide();
	void Code_Nop_Code_Block();
	void Code_Write();

	void Code_Equal();
	void Code_Not_Equal();
	void Code_Greater();
	void Code_Less();
	void Code_Greater_Equal();
	void Code_Less_Equal();



	SHARK_CODE_TYPE _shark_codes[ 256000 ];

	int _code_list_size;
	int _code_ptr;
	int _write_skip;
	int _code_block_skip;
};


#endif
