#include <windows.h>
#include <stdio.h>

HANDLE dll_thread;

#define PROC_NAME "ePSXe.180"
#define DLL_NAME "shark.dll"


// Structures for creating the process
STARTUPINFO Startupinfo = {0};
PROCESS_INFORMATION processinfo = {0};
LPVOID pLibRemote;
FARPROC pLoadLibrary;
HMODULE hKernel32;
HANDLE hThread;



int delay_injection;
int ePSXe_version;



#include "misc.h"



bool InjectDLL(HANDLE hProcess, const char* dll){
	DWORD dwWritten;
	LPVOID pStringInRemoteProcess;

	if(!hProcess) return false;
	pStringInRemoteProcess = VirtualAllocEx(hProcess, 0, strlen(dll)+1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if(pStringInRemoteProcess == NULL) return false;

	// NOTE: Avira Malware detection (HEUR/Malware - add file exception)
	WriteProcessMemory(hProcess, pStringInRemoteProcess, (void *) dll, strlen(dll)+1, &dwWritten);
	if(dwWritten != strlen(dll)+1) return false;

	dll_thread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), pStringInRemoteProcess, 0, 0);

	return true;
}



void read_ini()
{
	char line[512];


	FILE *fp;
	fopen_s(&fp, "inis\\ePSXe_shark.ini", "r" );
	if( !fp )
		return;



	fgets( line, 512, fp );
	sscanf_s( line, "Delay_injection = %d", &delay_injection );
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
									 LPTSTR lpCmdLine, int nCmdShow)
{

/*
	// check epsxe unpacked
	ePSXe_version = detect_version();
	if( ePSXe_version == 0 ) {
		unpack_exe();
	}


	// insert jmp point into exe
	SaveApp();
	StallApp();
	*/



	delay_injection = 100;
	read_ini();
	timeBeginPeriod(1);

	
	BOOL result = FALSE;
	
	// Hardcoded just for a demo, you will need to use a game/program of
	// your own to test. It is not a good idea to use stuff in system32
	// due to the DEP enabled on those apps.
	
	/*
	char* exeString = 
		"\"C:\\Program Files\\Windows NT\\Pinball\\PINBALL.EXE\" -quick";
		char* workingDir = "C:\\Program Files\\Windows NT\\Pinball";
	*/
	
	char exeString[MAX_PATH] = {""};
	char workingDir[MAX_PATH] = {""};
	char cmdline[MAX_PATH] = {""};
		
	GetCurrentDirectory(MAX_PATH,workingDir);
	strcat_s( exeString, "\"" );
	strcat_s( exeString, workingDir );
	strcat_s( exeString, "\\" );
	strcat_s( exeString, PROC_NAME );
	strcat_s( exeString, "\" " );
		
	strcat_s( exeString, lpCmdLine );
		
	// Need to set this for the structure
	Startupinfo.cb = sizeof(STARTUPINFO);
	
		
	
	// Load the Target in suspended mode + cmdline parameters
	if( !CreateProcessA( NULL, exeString, 0, 0, false,
		0, 0, workingDir, &Startupinfo, &processinfo) )
	{
		MessageBox(0, "Process could not be loaded!", "Error", MB_ICONERROR);
		return -1;
	}



	Sleep( delay_injection );
	SuspendThread( processinfo.hThread );



	// Inject the DLL, the export function is named 'Initialize'
	if( !InjectDLL(processinfo.hProcess, DLL_NAME) )
	{
		MessageBox(0, "Injection failed!", "Error", MB_ICONERROR);
		return -1;
	}


	// Resume process execution
	ResumeThread(processinfo.hThread);


	
	return 0;
}
