#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501


#include <windows.h>
#include "CheaterApp\CheaterApp.h"
#include "SharkApp.h"


static CheaterApp _app;


void SharkApp::Run_Thread( HINSTANCE hInst )
{
	_app.Run_Thread( hInst );
}


void SharkApp::Run( HINSTANCE hInst )
{
	_app.Run( hInst );
}


void SharkApp::Destroy()
{
	_app.Destroy();
}


void SharkApp::Send_Cheats()
{
	_app.Send_Cheats();
}


bool SharkApp::Is_Active()
{
	return _app.Is_Active();
}


void SharkApp::Minimize()
{
	_app.Minimize();
}


void SharkApp::Init_Emu( UINT32 system, UINT32 *ptr_list, UINT32 *size_list )
{
	_app.Init_Emu( system, ptr_list, size_list );
}


void SharkApp::Update_Memory()
{
	_app.Update_Memory();
}


int SharkApp::Get_Dynarec_Count()
{
	return _app.Get_Dynarec_Count();
}


void SharkApp::Set_Dynarec_Count( int num )
{
	_app.Set_Dynarec_Count( num );
}
