#include <new.h>

#include "main.h"
#include "SharkApp.h"


HINSTANCE TheInstance;


int NewHandler (size_t size)
{
    throw WinException ( "Out of memory" );
    return 0;
}



static SharkApp shark_cheater;
static UINT8 Ram[0x200000];


int WINAPI WinMain
   (HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
	UINT32 ptr_list[20], size_list[20];


	TheInstance = hInst;
	_set_new_handler (&NewHandler);


	// emu memory
	ptr_list[0] = (UINT32) Ram;
	size_list[0] = 0x200000;

	shark_cheater.Init_Emu( SYSTEM_PS1, ptr_list, size_list );


	// Dragoon testing
	memset( Ram, 0xff, 0x200000 );


	//*((unsigned int *) (Ram+0x2693c)) = 0x8c42a39c;
	//Ram[ 0x2693c ] = 0x9c;
	//Ram[ 0x2693c+1 ] = 0xa3;
	//Ram[ 0x2693c+2 ] = 0x2a;
	//Ram[ 0x2693c+3 ] = 0x8c;



	//shark_cheater.Run( hInst );
	shark_cheater.Run_Thread( hInst );


	// wait for app to start
	while( !shark_cheater.Is_Active() )
	{ 
		Sleep(10);
	}

	// now wait for it to finish
	while( shark_cheater.Is_Active() )
	{
		Sleep(10);

		shark_cheater.Update_Memory();




#if 0
		static int lcv = 0;
		if( ++lcv == 20 )
			shark_cheater.Destroy();
#endif
	}

	return 0;
}
