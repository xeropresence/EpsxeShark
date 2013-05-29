#ifndef _SHARKAPP_H
#define _SHARKAPP_H



#define UINT32 unsigned int 
#define UINT16 unsigned short
#define UINT8 unsigned char

#define INT32 int 
#define INT16 short
#define INT8 char


#define SYSTEM_PS1 1
#define SYSTEM_GBC 2


class SharkApp
{
public:
	void Run( HINSTANCE hInst );
	void Run_Thread( HINSTANCE hInst );
	void Destroy();

	bool Is_Active();
	void Minimize();
	void Init_Emu( UINT32 system, UINT32 *ptr_list, UINT32 *size_list );

	void Update_Memory();
	void Send_Cheats();

	int  Get_Dynarec_Count();
	void Set_Dynarec_Count( int num );
};

#endif
