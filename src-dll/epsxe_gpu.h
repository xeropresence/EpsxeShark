#include "gl\gl.h"
#include "wglext.h"
#include <stdlib.h>
#include <stdio.h>

void __stdcall GPUsetframelimit( int option )
{
#if 0
	fopen_s(&fp_log, "log.txt","a");
	fputs("GPUsetframelimit\n", fp_log );
	fclose(fp_log);
#endif

	if( GPUsetframelimit_func ) {
		__asm
		{
			// call REAL routine
			push option
			call GPUsetframelimit_func
		}
	}
}


unsigned int WINAPI Fast_Startup(void *dummy)
{
#if 0
	fp_log = fopen("log.txt","a");
	fprintf( fp_log, "KEY   %02X %02X %02X\n", start_gpu );
	fclose(fp_log);
#endif


	if( start_gpu < 100 )
	{
		static int once = 0;


		// unknown reason - start_gpu == 0 on emu open again
		if( once == 1 ) {
			start_gpu = 200;
			return 0;
		}
		once = 1;


		start_gpu = 10;
		max_fps_toggle = 1;


		// turn off framelimiter
		(*framelimit) = 0;
		GPUsetframelimit(0);
	}
	else
	{
		start_gpu = 100;
		max_fps_toggle = 0;

		
		// turn on framelimiter
		(*framelimit) = 1;
		GPUsetframelimit(1);
	}


	return 0;
}




/////////////////////////////////
// PeoPS OpenGL 1.78

void PCFrameCap( int framelimit )
{
	static DWORD curticks, lastticks, _ticks_since_last_update;
	static DWORD TicksToWait = 0;
	static LARGE_INTEGER  CurrentTime;
	static LARGE_INTEGER  LastTime;
	BOOL Waiting = TRUE;
	DWORD sleep_target;

	if( fFrameRateHz <= 0 )
		return;

	if( max_fps_toggle == 1 )
		return;


	sleep_target = fFrameRateHz;
	if( framelimit == 1 ) sleep_target = 60+50;


	if (QueryPerformanceFrequency (&liCPUFrequency))
		bIsPerformanceCounter = TRUE;
	else
		bIsPerformanceCounter = FALSE;



	static DWORD ticks_per_ms[10+1];
	static DWORD last_frequency = 0;

	if(bIsPerformanceCounter &&
		 last_frequency != liCPUFrequency.LowPart)
	{
		last_frequency = liCPUFrequency.LowPart;
		ticks_per_ms[1] = (liCPUFrequency.LowPart / 1000);
		ticks_per_ms[2] = ticks_per_ms[1] * 2;
		ticks_per_ms[3] = ticks_per_ms[1] * 3;
		ticks_per_ms[4] = ticks_per_ms[1] * 4;
		ticks_per_ms[5] = ticks_per_ms[1] * 5;
		ticks_per_ms[6] = ticks_per_ms[1] * 6;
		ticks_per_ms[7] = ticks_per_ms[1] * 7;
		ticks_per_ms[8] = ticks_per_ms[1] * 8;
		ticks_per_ms[9] = ticks_per_ms[1] * 9;
		ticks_per_ms[10] = ticks_per_ms[1] * 10;
	}



	while (Waiting)
	{
		if(bIsPerformanceCounter)
		{
			QueryPerformanceCounter(&CurrentTime);
			_ticks_since_last_update = CurrentTime.LowPart - LastTime.LowPart;

			//------------------------------------------------//
			curticks = timeGetTime();
			if(_ticks_since_last_update>(liCPUFrequency.LowPart>>1))   
			{
				if(curticks < lastticks)
					_ticks_since_last_update = TicksToWait+1;
				else
					_ticks_since_last_update = (liCPUFrequency.LowPart * (curticks - lastticks))/1000;
      }
			//------------------------------------------------//

			if ((_ticks_since_last_update > TicksToWait) ||
					(CurrentTime.LowPart < LastTime.LowPart))
			{
				Waiting = FALSE;

				lastticks=curticks;

				LastTime.HighPart = CurrentTime.HighPart;
				LastTime.LowPart = CurrentTime.LowPart;
				TicksToWait = (liCPUFrequency.LowPart / sleep_target);
      }
			else if( sleep_throttle > 0 )
			{
				DWORD sleep_count;
				DWORD ticks_left;
				ticks_left = TicksToWait - _ticks_since_last_update;

				if( ticks_left < ticks_per_ms[1] )
					sleep_count = 1;
				else if( ticks_left < ticks_per_ms[2] )
					sleep_count = 2;
				else if( ticks_left < ticks_per_ms[3] )
					sleep_count = 3;
				else if( ticks_left < ticks_per_ms[4] )
					sleep_count = 4;
				else if( ticks_left < ticks_per_ms[5] )
					sleep_count = 5;
				else if( ticks_left < ticks_per_ms[6] )
					sleep_count = 6;
				else if( ticks_left < ticks_per_ms[7] )
					sleep_count = 7;
				else if( ticks_left < ticks_per_ms[8] )
					sleep_count = 8;
				else if( ticks_left < ticks_per_ms[9] )
					sleep_count = 9;
				else
					sleep_count = 10;

				Sleep( sleep_count );

#if 0
			fp_log = fopen("log.txt","a");
			fprintf( fp_log, "TICKS %d | %d | %d %d | %d %d | %d\n",
				sleep_count,
				CurrentTime.LowPart - LastTime.LowPart,
				CurrentTime.HighPart, CurrentTime.LowPart,
				LastTime.HighPart, LastTime.LowPart,
				TicksToWait
				);
			fclose(fp_log);
#endif
			}
    }
   else
    {
     curticks = timeGetTime();
     _ticks_since_last_update = curticks - lastticks;
     if ((_ticks_since_last_update > TicksToWait) || 
         (curticks < lastticks))
      {
       Waiting = FALSE;
       lastticks = curticks;
       TicksToWait = (1000 / (DWORD)sleep_target);
      }
    }
  }
}




void __stdcall GPUclearDynarec( unsigned char *func )
{
	clear_dynarec = func;


	// call REAL routine
	__asm {
		push func
		call GPUclearDynarec_func
	}
}


void __stdcall GPUupdateLace()
{
	// load state - gpu black screen hack (screen disabled)
	if( gpu_loadstate_hack ) {
		gpu_loadstate_hack = 0;


		extern void __stdcall GPUwriteStatus( unsigned long gdata );
		GPUwriteStatus( 0x03000000 );
	}



	// long gpu dma transfers (vram copy) hack
	if( rebel2_gpudma_hack && ePSXe_version == 170 ) {
		if( *((unsigned short *) (0x94C020 + 0x3bb94)) == 0x0001 )
			*((unsigned short *) (0x94C020 + 0x3bb9e)) = 0x1000;
	}
	if( rebel2_gpudma_hack && ePSXe_version == 180 ) {
		if( *((unsigned short *) (0xA52EA0 + 0x3bb94)) == 0x0001 )
			*((unsigned short *) (0xA52EA0 + 0x3bb9e)) = 0x1000;
	}


	if( ePSXe_version >= 170 )
	{
		// boot state - load state
		if( start_gpu == -10 )
		{
			start_gpu = 100;
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = 0;
			sa.lpSecurityDescriptor = NULL;
			_beginthreadex(&sa,NULL,Fast_Startup,NULL,NULL,NULL);
			//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Fast_Startup, NULL, 0, 0 ); 
			while( framelimiter_mutex == 1 ) Sleep(1);
		}


		if( start_gpu >= 100 )
			PCFrameCap( (*framelimit) );
	}


	// apply Shark Cheater cheats
	if( shark_cheater.Get_Dynarec_Count() )
	{
		if( clear_dynarec )
		{
			// clear dynamic recompiler tables
			__asm {
				call clear_dynarec
			}
		}
	}

	shark_cheater.Update_Memory();



	// call REAL routine
	__asm {
		call GPUupdateLace_func
	}
}



void __stdcall GPUwriteStatus( unsigned long gdata )
{
	unsigned long lCommand=(gdata>>24)&0xff;


	// boot state - speed hack on
	if( start_gpu == 10 )
	{
#if 0
		if( lCommand == 0x03 )
		{
			fp_log = fopen("log.txt","a");
			fprintf( fp_log, "DISPLAY %d\n", gdata&1 );
			fclose(fp_log);
		}
#endif


		// DISPLAY turned on - deactivate speed hack
		if( lCommand == 0x03 && (gdata & 1)==0 )
		{
			start_gpu = 100;
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = 0;
			sa.lpSecurityDescriptor = NULL;
			_beginthreadex(&sa,NULL,Fast_Startup,NULL,NULL,NULL);
			//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Fast_Startup, NULL, 0, 0 ); 
		}
	}


	// call REAL routine
	__asm
	{
		push gdata
		call GPUwriteStatus_func
	}
}



LRESULT CALLBACK key_winproc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static int nAltState = 0;


	switch(message)
  {
	case WM_KEYDOWN:
#if 0
	fp_log = fopen("log.txt","a");
	fprintf( fp_log, "KEYDOWN--   %02X %02X %02X\n", wParam, lParam, start_gpu );
	fclose(fp_log);
#endif


		// discover key code
		if(0)
		{
			static FILE *fp = 0;

			if( !fp )
				fopen_s(&fp, "log.txt", "w" );

			if(fp)
				fprintf( fp, "%d\n", wParam );
		}


		// key already pressed
		if( szGPUKeys[ wParam ] )
			break;


		szGPUKeys[ wParam ] = 1;


		// unlimited FPS
		if( wParam == throttle_max_key ) {

			// on-to-off, F4-to-off (full)
			if( max_fps_toggle == 0 ) {
				max_fps_toggle = 1;

				if( (*framelimit) == 1 ) {
					// turn off framelimiter
					(*framelimit) = 0;
					GPUsetframelimit(0);
				}

				break;
			}

			// off-to-on
			else if( max_fps_toggle == 1 ) {
				// turn on framelimiter
				(*framelimit) = 1;
				GPUsetframelimit(1);


				max_fps_toggle = 0;
				start_gpu = 200;
				break;
			}
		}
		//f
		if (wParam == 70)
		{
						// on-to-off, F4-to-off (full)
			if( max_fps_toggle == 0 ) {
				max_fps_toggle = 1;

				if( (*framelimit) == 1 ) {
					// turn off framelimiter
					(*framelimit) = 0;
					//GPUsetframelimit(0);
					SPUsetframelimit(0);
				}

				break;
			}

			// off-to-on
			else if( max_fps_toggle == 1 ) {
				// turn on framelimiter
				(*framelimit) = 1;
				//GPUsetframelimit(1);
				SPUsetframelimit(1);


				max_fps_toggle = 0;
				start_gpu = 200;
				break;
			}
		}
		//g
		if (wParam == 71)
		{
			SPUsetframelimit(0);
			break;
		}
		//h
		if (wParam == 72)
		{
			SPUsetframelimit(1);
			break;
		}




		if( wParam == dualshock_mouse_snap_key )
		{
			// dualshock mouse - snap toggle
			if( dualshock_mouse_active ) {
				dualshock_mouse_snap++;
				dualshock_mouse_snap &= 1;


				dualshock_mouse_snap_x = 0;
				dualshock_mouse_snap_y = 0;
			}
			break;
		}


		if( wParam == VK_F3 )
		{
			// disable fast-forward
			if( start_gpu < 200 ) {
				start_gpu = -10;
			}


			//shark_cheater.Set_Dynarec_Count(20);
			break;
		}


		if( wParam == VK_F4 )
		{
#if 0
	fp_log = fopen("log.txt","a");
	fprintf( fp_log, "KEY2   %02X %02X %02X\n", wParam, lParam, start_gpu );
	fclose(fp_log);
#endif
			// received message
			if( framelimiter_mutex )
				framelimiter_mutex = 0;


			// ignore fast start - send straight to emu
			if( start_gpu < 100 )
				break;


			// de-activate max throttle
			// - off to on
			if( (*framelimit) == 0 ) {
				max_fps_toggle = 0;
			}

			break;
		}


		if( wParam == VK_F11 )
		{
			if( opcode_tracer == 1 )
			{
#if 0
	fp_log = fopen("log.txt","a");
	fprintf( fp_log, "KEY2   %02X %02X %02X\n", wParam, lParam, start_gpu );
	fclose(fp_log);
#endif

				if( start_tracer == 0 )
					start_tracer = 1;
				else
					start_tracer = 0;
			}


			if( cdrom_tracer )
			{
				if( cdrom_tracer_active == 0 )
					cdrom_tracer_active = 1;
				else
					cdrom_tracer_active = 0;
			}
			break;
		}
		break;


	case WM_SYSKEYUP:
    //if(wParam==VK_RETURN) bChangeWinMode=TRUE;
		break;


  case WM_KEYUP:
#if 0
		fp_log = fopen("log.txt","a");
		fprintf( fp_log, "KEYUP   %02X %02X %02X\n", wParam, lParam, wParam );
		fclose(fp_log);
#endif


		// key released
		szGPUKeys[ wParam ] = 0;

		break;
  }


	return CallWindowProc( pOldWinProc, hwnd, message, wParam, lParam );
}
int GetVideoMemoryAMD()
{
	PFNWGLGETGPUIDSAMDPROC wglGetGPUIDsAMD= 
		(PFNWGLGETGPUIDSAMDPROC)wglGetProcAddress("wglGetGPUIDsAMD");

	PFNWGLGETGPUINFOAMDPROC wglGetGPUInfoAMD= 
		(PFNWGLGETGPUINFOAMDPROC)wglGetProcAddress("wglGetGPUInfoAMD");

	if(wglGetGPUIDsAMD && wglGetGPUInfoAMD) 
	{
		UINT n = wglGetGPUIDsAMD(0, 0);
		UINT *ids = (UINT*)malloc(n*sizeof(UINT));
		size_t total_mem_mb = 0;
		wglGetGPUIDsAMD(n, ids);
		wglGetGPUInfoAMD(ids[0], WGL_GPU_RAM_AMD, GL_UNSIGNED_INT, sizeof(size_t),&total_mem_mb);

		int iVRamSize = total_mem_mb;
		if(iVRamSize>1024) iVRamSize = 1024;
		return iVRamSize;
	}
	return 0;
};

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

int GetVideoMemoryNV()
{
	GLint total_mem_kb = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,&total_mem_kb);

	int iVRamSize = total_mem_kb/1024;
	if(iVRamSize>1024) iVRamSize = 1024;
	return iVRamSize;
}

void OpenGL2_VRAMAuto()
{
	if(!opengl2_vramauto) return;

	// fix video memory auto detection in OpenGL2 Plugin
	HMODULE hPlug = GetModuleHandle("gpuPeteOpenGL2.dll");
	if(!hPlug) return;

	static const unsigned char check[]= {0x83, 0xEC, 0x0C, 0x33, 0xC0, 0x53, 0x66, 0xA1, 0x5A, 0xFA};
	unsigned char* CheckTextureMemory = (unsigned char*)hPlug + 0x448B0;
	int err = 0;

	// check CheckTextureMemory code to make sure this is gpuPeteOpenGL2 2.9 plugin
	DWORD oldProtect;
	VirtualProtect(CheckTextureMemory, sizeof(check), PAGE_EXECUTE_READ, &oldProtect);
	for(int i = 0; i < sizeof(check); i++)
		if(check[i] != *(CheckTextureMemory+i)) err = 1;
	VirtualProtect(CheckTextureMemory, sizeof(check), oldProtect, &oldProtect);

	if(err) return;
	int iVRamSize = GetVideoMemoryAMD();
	if(!iVRamSize) iVRamSize = GetVideoMemoryNV();

	if(iVRamSize)
	{
		WriteCodeRVA((void*)0x50224,&iVRamSize,sizeof(iVRamSize),hPlug);

		//this is need to change texture limit
		int result=0;
		__asm
		{
			call CheckTextureMemory
				mov result,eax
		}
	}
}

long __stdcall GPUopen(HWND hwndGPU)                    
{
	long result;

	hWWindow = hwndGPU;                                   // store hwnd globally

	if( shark_cheater_boot ) 
	{
		Wait_Shark();

		if( shark_cheater_boot == 2 )
			shark_cheater.Minimize();
	}



#if 0
	fp_log = fopen("log.txt","a");
	fprintf( fp_log, "GPUopen\n" );
	fclose(fp_log);
#endif


	if(pOldWinProc)
	{
		SetWindowLong(hWWindow,GWL_WNDPROC,              // set old proc
									(long)pOldWinProc);
		pOldWinProc = 0;
	}

	//if(!pOldWinProc)
	{
		pOldWinProc = (WNDPROC)GetWindowLong(hWWindow, GWL_WNDPROC );
		SetWindowLong(hWWindow, GWL_WNDPROC, (long)key_winproc);
	}

	__asm
	{
		// call REAL routine
		push hwndGPU
		call GPUopen_func

		mov result,eax
	}

	// move window
	if(move_window)
	{
		RECT wr;
		int x=0,y=0,width=0,height=0;
		GetWindowRect(hwndGPU,&wr);

		width = (wr.right - wr.left); 
		height = (wr.bottom - wr.top);

		if(move_window == 2)
			y -= GetSystemMetrics(SM_CYCAPTION);

		MoveWindow(hwndGPU,x,y,width,height,TRUE);
	}

	OpenGL2_VRAMAuto();

	// do after gpu does window
	PADOpenDriver();

	if( ePSXe_version >= 170 && start_gpu == 0 )
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = 0;
		sa.lpSecurityDescriptor = NULL;
		_beginthreadex(&sa,NULL,Fast_Startup,NULL,NULL,NULL);
		//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Fast_Startup, NULL, 0, 0 ); 
		while( framelimiter_mutex == 1 ) Sleep(1);
	}
	else
	{
		start_gpu = 200;
	}



	return result;
}



void __stdcall GPUshutdown()
{
#if 0
	fp_log = fopen("log.txt","a");
	fputs( fp_log, "GPUopen - shutdown\n" );
	fclose(fp_log);
#endif


	PADShutdownDriver();


	if(pOldWinProc)
		SetWindowLong(hWWindow,GWL_WNDPROC,              // set old proc
									(long)pOldWinProc);
	pOldWinProc = 0;

	start_gpu = 100;



	__asm
	{
		// call REAL routine
		call GPUshutdown_func
	}
}


void __stdcall GPUshowScreenPic( unsigned char *pMem )
{
	static int once = 0;


	// check fast boot
	if( once == 0 && start_gpu < 100 ) {
		once = 1;
		return;
	}


	__asm
	{
		// call REAL routine
		push pMem
		call GPUshowScreenPic_func
	}
}



long __stdcall GPUinit()
{
	long result;


	LoadPadPlugins();
	PADInitDriver();


	__asm
	{
		// call REAL routine
		call GPUinit_func

		mov result,eax
	}



	return result;
}



void __stdcall GPUclose()
{
	PADCloseDriver();


	__asm
	{
		// call REAL routine
		call GPUclose_func
	}
}
