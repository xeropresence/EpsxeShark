#include "epsxe_170_asm.h"



void Attach_170()
{

	// window caption
	static char name[] = " ePSXe - Shark enhanced";
	WriteCode(
		(unsigned char*) 0x004503E0,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x004503E0 );

	WriteCode(
		(unsigned char*) 0x00451600,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x00451600 );

/*
WinMain
		
.text:00403970                 sub     esp, 408h
81 ec 08 04 00 00

.text:00403976                 mov     ecx, [esp+408h+lpCmdLine]
.text:0040397D                 push    ebx
*/

	{
		static u8 run[2] = { 0x81, 0xec };

		WriteCode(
			(unsigned char*) 0x403970,
			(unsigned char*) run,
			2 );

		DirtyPatch( 0x403970 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0040D017                 call    ds:CreateWindowExA
.text:0040D01D                 test    eax, eax
-->
.text:0040D01F                 mov     hWnd, eax
-->
.text:0040D024                 jnz     short loc_40D038
*/

	// get ePSXe hwnd - for autostart window
	{
		ApplyJmpHook(
			(unsigned char*) 0x0040d01f,
			(unsigned char*) &hwnd_170,
			(0x24 - 0x1f) - 5 );

		DirtyPatch( 0x40d01f );
	}


/*
-->
.text:0042DD02                 call    ds:CreateWindowExA
-->
.text:0042DD08                 test    eax, eax
*/

	// force shark to open first - then ePSXe window
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042dd02,
			(unsigned char*) &force_shark_170,
			(0x08 - 0x02) - 5 );

		DirtyPatch( 0x42dd02 );
	}

// ###############################
// ###############################
// ###############################
// ###############################
		
/*
--->
.text:0042BD86 mov     cl, byte_50F943 (min)           ; jumptable 0042BCDB case 2
--->
.text:0042BD8C mov     al, cl
.text:0042BD8E shr     al, 4
.text:0042BD91 mov     dl, 0Ah
*/

	ApplyJmpHook( 
		(unsigned char*) 0x0042bd86,
		(unsigned char*) &CdlSetLoc_Wrapper_170,
		6-5 );

	DirtyPatch( 0x42bd86 );



/*
.text:0042ADF2                 lea     eax, [eax+esi*4]

; normal way to do (minutes + seconds) * 75
--->
.text:0042ADF5                 imul    eax, 4Bh

; add frames
.text:0042ADF8                 add     eax, edx
--->
.text:0042ADFA                 pop     esi
*/

	ApplyJmpHook( 
		(unsigned char*) 0x0042adf5,
		(unsigned char*) &Time2Addr_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42adf5 );
	DirtyPatch( 0x42adf8 );


// ###############################
// ###############################
// ###############################
// ###############################

/*
// check screen enable / disable

.text:0042DF9C                 mov     edx, dword_4C4D30
.text:0042DFA2                 push    offset aGpuwritestatus ; "GPUwriteStatus"
.text:0042DFA7                 push    edx             ; hModule
.text:0042DFA8                 call    esi ; GetProcAddress
.text:0042DFAA                 test    eax, eax
-->
.text:0042DFAC                 mov     dword_50F4C8, eax
-->
.text:0042DFB1                 jnz     short loc_42DFC5
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042dfac,
		(unsigned char*) &GPUwriteStatus_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42dfac );



/*
// framelimiter

.text:0042E14F                 push    offset aGpusetframelim ; GPUsetframelimit
.text:0042E154                 push    edx             ; hModule
.text:0042E155                 mov     dword_50F060, eax
.text:0042E15A                 call    esi ; GetProcAddress
-->
.text:0042E15C                 mov     dword_50F4C0, eax
-->
.text:0042E161                 mov     eax, dword_4C4D30
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042e15c,
		(unsigned char*) &GPUsetframelimit_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42e15c );



// spu framelimiter

/*
-->
.text:0040D69A                 mov     ecx, hModule
-->
.text:0040D6A0                 push    offset aSpuasync ; "SPUasync"
.text:0040D6A5                 push    ecx             ; hModule
.text:0040D6A6                 mov     dword_C01210, eax
.text:0040D6AB                 call    esi ; GetProcAddress
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0040d69a,
			(unsigned char*) &SPUsetframelimit_Wrapper_170,
			6-5 );

		DirtyPatch( 0x40d69a );



		char nop[5] = { (char) 0x90, (char) 0x90, (char) 0x90, (char) 0x90, (char) 0x90 };
		WriteCode(
			(unsigned char*) 0x40d6a6,
			(unsigned char*) nop,
			5 );

		DirtyPatch( 0x40d6a6 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042DEA8                 mov     edx, dword_4C4D30
.text:0042DEAE                 push    offset aGpuopen ; "GPUopen"
.text:0042DEB3                 push    edx             ; hModule
.text:0042DEB4                 call    esi ; GetProcAddress
.text:0042DEB6                 test    eax, eax
-->
.text:0042DEB8                 mov     dword_50F03C, eax
-->
.text:0042DEBD                 jnz     short loc_42DED1
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042deb8,
		(unsigned char*) &GPUopen_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42deb8 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042E090                 mov     edx, dword_4C4D30
.text:0042E096                 push    offset aGpuupdatelace ; "GPUupdateLace"
.text:0042E09B                 push    edx             ; hModule
.text:0042E09C                 call    esi ; GetProcAddress
.text:0042E09E                 test    eax, eax
-->
.text:0042E0A0                 mov     dword_50F034, eax
-->
.text:0042E0A5                 jnz     short loc_42E0B9
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042e0a0,
		(unsigned char*) &GPUupdateLace_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42e0a0 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0040D21F                 push    eax
.text:0040D220                 call    sub_404BA0
-->
.text:0040D225                 call    sub_404BC0
-->
.text:0040D22A                 mov     ecx, Args
.text:0040D230                 add     ecx, 0FFFFFFF6h
.text:0040D233                 push    ecx             ; Args
.text:0040D234                 push    offset aSavestateDoneD ; " * SaveState Done! (%d)\n"
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040d225,
		(unsigned char*) &saveState1_Wrapper_170,
		5-5 );

	DirtyPatch( 0x40d225 );


/*
.text:0042FDB8                 cmp     byte_50F430, al
.text:0042FDBE                 jz      short loc_42FDE8
.text:0042FDC0                 mov     dword_4C4D24, 70h
-->
.text:0042FDCA                 call    sub_404BC0
-->
.text:0042FDCF                 push    offset aSavestateDone ; " * SaveState Done! \n"
.text:0042FDD4                 call    sub_439240
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042fdca,
		(unsigned char*) &saveState2_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42fdca );


/*
.text:0040D1DA                 push    eax
.text:0040D1DB                 call    sub_404BA0
-->
.text:0040D1E0                 call    sub_404D50
-->
.text:0040D1E5                 call    sub_4209A0
.text:0040D1EA                 mov     ecx, Args
.text:0040D1F0                 push    ecx             ; Args
.text:0040D1F1                 push    offset aLoadstateDoneD ; " * LoadState Done! (%d)\n"
.text:0040D1F6                 call    sub_439240
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040d1e0,
		(unsigned char*) &loadState1_Wrapper_170,
		5-5 );

	DirtyPatch( 0x40d1e0 );



/*
.text:0042FE34                 cmp     dword_4C4D4C, 0
.text:0042FE3B                 jnz     short loc_42FE54
-->
.text:0042FE3D                 call    sub_404D50
-->
.text:0042FE42                 push    offset aLoadstateDone ; " * LoadState Done! \n"
.text:0042FE47                 call    sub_439240
.text:0042FE4C                 mov     byte_4C4B30, bl
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042fe3d,
		(unsigned char*) &loadState2_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42fe3d );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042E12E                 push    offset aGpushowscreenp ; "GPUshowScreenPic"
.text:0042E133                 push    eax             ; hModule
.text:0042E134                 call    esi ; GetProcAddress
-->
.text:0042E136                 mov     ecx, dword_4C4D30
-->
.text:0042E13C                 push    offset aGpucursor ; "GPUcursor"
.text:0042E141                 push    ecx             ; hModule
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042e136,
		(unsigned char*) &GPUshowScreenPic_Wrapper_170,
		6-5 );

	DirtyPatch( 0x42e136 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:00430E65                 mov     eax, dword_4C4D54
-->
.text:00430E6A                 push    offset aCdrreadtrack ; "CDRreadTrack"
.text:00430E6F                 push    eax             ; hModule
.text:00430E70                 call    esi ; GetProcAddress
.text:00430E72                 test    eax, eax
.text:00430E74                 mov     dword_50E460, eax
.text:00430E79                 jz      loc_430F73
.text:00430E7F                 mov     ecx, dword_4C4D54
*/

	ApplyJmpHook(
		(unsigned char*) 0x00430e65,
		(unsigned char*) &CDRreadTrack_Wrapper_170,
		5-5 );

	DirtyPatch( 0x430e65 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042DE7F                 mov     ecx, dword_4C4D30
.text:0042DE85                 push    offset aGpushutdown ; "GPUshutdown"
.text:0042DE8A                 push    ecx             ; hModule
.text:0042DE8B                 call    esi ; GetProcAddress
.text:0042DE8D                 test    eax, eax
-->
.text:0042DE8F                 mov     dword_50F050, eax
-->
.text:0042DE94                 jnz     short loc_42DEA8
.text:0042DE96                 push    offset aGpushutdown ; "GPUshutdown"
.text:0042DE9B                 push    offset aGetprocaddress ; " * GetProcAddress error %s\n"
.text:0042DEA0                 call    loc_439340
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042DE8F,
		(unsigned char*) &GPUshutdown_Wrapper_170,
		5-5 );

	DirtyPatch( 0x42de8f );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0040BF77                 push    offset FileName
.text:0040BF7C                 push    offset aPsxIsos_bin_is ; "PSX ISOs (*.BIN, *.ISO, *.IMG, *.CUE, *"...
.text:0040BF81                 push    offset aOpenPsxIso ; "Open PSX ISO"
-->
.text:0040BF86                 call    sub_405EB0
-->
.text:0040BF8B                 add     esp, 14h
.text:0040BF8E                 test    eax, eax
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040BF86,
		(unsigned char*) &RunISO_Wrapper_170,
		5-5 );

	DirtyPatch( 0x40bf86 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:00438425                 push    0               ; Origin
.text:00438427                 push    edx             ; Offset
.text:00438428                 push    ecx             ; File
.text:00438429                 call    _fseek
.text:0043842E                 add     esp, 0Ch
-->

.text:00438431 loc_438431:                             ; CODE XREF: sub_438370+AD.j
.text:00438431                 mov     edx, dword_508480
.text:00438437                 mov     eax, dword_508488
*/

	ApplyJmpHook(
		(unsigned char*) 0x00438425,
		(unsigned char*) &CdlReadTrack_ISO_Wrapper_170,
		(0x31 - 0x25) - 5 );

	DirtyPatch( 0x438425 );
	DirtyPatch( 0x438427 );
	DirtyPatch( 0x438428 );
	DirtyPatch( 0x438429 );
	DirtyPatch( 0x43842e );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042E192                 call    esi ; GetProcAddress
.text:0042E194                 test    eax, eax
.text:0042E196                 pop     edi
-->
.text:0042E197                 mov     dword_50F038, eax
-->
.text:0042E19C                 pop     esi
.text:0042E19D                 jz      short loc_42E1A6
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042E197,
		(unsigned char*) &GPUclearDynarec_Wrapper_170,
		(0x9C - 0x97) - 5 );

	DirtyPatch( 0x42e197 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0040D180 loc_40D180:                             ; CODE XREF: sub_40D0B0+A0.j
.text:0040D180                 push    0               ; bShow
.text:0040D182                 call    ds:ShowCursor
.text:0040D188                 mov     edx, hWnd

	
.text:0042DD1E loc_42DD1E:                             ; CODE XREF: sub_42DCA0+6F.j
.text:0042DD1E                 push    0               ; bShow
.text:0042DD20                 call    ds:ShowCursor

	
.text:00430F5D                 push    0               ; bShow
.text:00430F5F                 call    esi ; ShowCursor
*/

	if( show_cursor )
	{
		static char showcursor[1] = { 1 };
		WriteCode(
			(unsigned char*) 0x40d180+1,
			(unsigned char*) showcursor,
			1 );

		DirtyPatch( 0x40d180 );


		WriteCode(
			(unsigned char*) 0x42dd1e+1,
			(unsigned char*) showcursor,
			1 );

		DirtyPatch( 0x42dd1e );


		WriteCode(
			(unsigned char*) 0x430f5d+1,
			(unsigned char*) showcursor,
			1 );

		DirtyPatch( 0x430f5d );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042A97C                 add     dword_510A40, 4

; run opcode (!!)
.text:0042A983                 shr     esi, 1Ah
.text:0042A986                 mov     ecx, off_45EFF8[esi*4]
.text:0042A98D                 call    ecx ; sub_429EA0
*/
	if( opcode_tracer )
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042A97C,
			(unsigned char*) &Tracer_Wrapper_170,
			(0x83 - 0x7C) - 5 );

		DirtyPatch( 0x42a97c );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:00420A7C                 mov     eax, dword_4C4B3C
.text:00420A81                 add     eax, esi

; call spu_async every 32 hsync	
.text:00420A83                 test    al, 1Fh
.text:00420A85                 mov     dword_4C4B3C, eax
.text:00420A8A                 jnz     short loc_420A9E
	
	
.text:00420A8C                 mov     eax, dword_45EC8C
; call spu_async every 32 hsync	
.text:00420A91                 shl     eax, 5
.text:00420A94                 push    eax
.text:00420A95                 call    dword_4C4AE8
.text:00420A9B                 add     esp, 4
*/

	{
		static char async_cycles[1] = { 1 };
		static char async_rate[1] = { 1 };

		async_cycles[0] = apu_cycles-1;
		async_rate[0] = apu_rate;

		WriteCode(
			(unsigned char*) 0x420A83+1,
			(unsigned char*) async_cycles,
			1 );

		DirtyPatch( 0x420a83 );


		WriteCode(
			(unsigned char*) 0x420A91+2,
			(unsigned char*) async_rate,
			1 );

		DirtyPatch( 0x420a91 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
 ; enable spu cdda playing

-->
.text:0041F1F0                 push    0
-->
.text:0041F1F2                 mov     dword_4C4AC0, offset loc_40D830
.text:0041F1FC                 mov     dword_4C4AC4, offset loc_40DD60
.text:0041F206                 mov     dword_4C4AC8, offset loc_40D9F0
*/

	{
		static char spu_cddaplay[1] = { 1 };

		WriteCode(
			(unsigned char*) 0x41f1f0+1,
			(unsigned char*) spu_cddaplay,
			1 );

		DirtyPatch( 0x41f1f0 );
	}


/*
-->
.text:0040DE85                 test    eax, eax
.text:0040DE87                 mov     dword_B5E7EC, 0
.text:0040DE91                 jz      short loc_40DE96
.text:0040DE93                 push    ecx
.text:0040DE94                 call    eax ; dword_C011E4
-->
.text:0040DE96
.text:0040DE96 loc_40DE96:                             ; CODE XREF: sub_40DE60+1E.j
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040DE85,
		(unsigned char*) &SPU_PlayCDDA_170,
		(0x96 - 0x85) - 5 );

	DirtyPatch( 0x40de85 );
	DirtyPatch( 0x40de87 );
	DirtyPatch( 0x40de91 );
	DirtyPatch( 0x40de93 );
	DirtyPatch( 0x40de94 );

/*
.text:0041F560                 mov     eax, [esp+arg_0]

-->
.text:0041F564                 cmp     eax, 1F801800h
-->

.text:0041F569                 ja      short loc_41F5B9
.text:0041F56B                 jz      short loc_41F5AA
.text:0041F56D                 cmp     eax, 1F801040h
.text:0041F572                 jz      short loc_41F59B
.text:0041F574                 cmp     eax, 1F8010F6h
.text:0041F579                 jnz     loc_41F605
*/

	ApplyJmpHook(
		(unsigned char*) 0x0041f564,
		(unsigned char*) &MMIO_Write_170,
		(0x69 - 0x64) - 5 );

	DirtyPatch( 0x41f564 );


/*
.text:0040DE3E                 push    offset dword_B76E20
.text:0040DE43                 call    dword_C01200
.text:0040DE49
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040de3e,
		(unsigned char*) &SPU_PlayXA_170,
		(0x43 - 0x3e) - 5 );

	DirtyPatch( 0x40de3e );

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042BF27                 mov     cl, byte_50F9D0
-->
.text:0042BF2D                 add     al, 1
-->
.text:0042BF2F                 mov     byte_50F9D0, cl
.text:0042BF35                 mov     byte_50F9D1, al
*/

	// cdda iso - first 1-sec speech cut (Chronicles of the Sword)
	{
		static char value[1] = { 0 };

		WriteCode(
			(unsigned char*) 0x42bf2d+1,
			(unsigned char*) value,
			1 );

		DirtyPatch( 0x42bf2d );
	}


/*
.text:00438796                 mov     ecx, 4Bh

--->
.text:0043879B                 div     ecx
.text:0043879D                 mov     eax, [esp+18h]

; stop time frames ($0a)
.text:004387A1                 mov     [eax], dl
--->

.text:004387A3                 retn
*/

	// cdda iso - do not stop 2-sec short of next track pregap (Rayman)
	ApplyJmpHook(
		(unsigned char*) 0x0043879b,
		(unsigned char*) &RunISO_CDDA_Fix_170,
		(0xa3 - 0x9b) - 5 );

	DirtyPatch( 0x43879b );
	DirtyPatch( 0x43879d );
	DirtyPatch( 0x4387a1 );


/*
.text:00438B4D                 shl     al, 4
.text:00438B50                 add     al, dl

; local subq time

; (***) should be +2
.text:00438B52                 mov     [ebp+3], al
.text:00438B62                 mov     [ebp+4], al
.text:00438B7A                 mov     [ebp+5], al
*/

	// cdda - fix local subq times (Chronicles of the Sword)
	{
		static char value[3] = { 2,3,4 };

		WriteCode(
			(unsigned char*) 0x438b52+2,
			(unsigned char*) value+0,
			1 );

		DirtyPatch( 0x438b52 );


		WriteCode(
			(unsigned char*) 0x438b62+2,
			(unsigned char*) value+1,
			1 );

		DirtyPatch( 0x438b62 );


		WriteCode(
			(unsigned char*) 0x438b7a+2,
			(unsigned char*) value+2,
			1 );

		DirtyPatch( 0x438b7a );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0040C0ED                 call    sub_4198B0
-->
.text:0040C0F2                 call    dword_4C4B1C
.text:0040C0F8                 push    0               ; nExitCode
.text:0040C0FA                 call    ds:PostQuitMessage
*/

	// switch iso
	ApplyJmpHook(
		(unsigned char*) 0x0040c0ed,
		(unsigned char*) &switch_disc_170,
		(0xf2 - 0xed) - 5 );

	DirtyPatch( 0x40c0ed );


/*
-->
.text:0042B6A0                 cmp     byte_50F9C5, 0
-->
.text:0042B6A7                 jnz     short locret_42B714
.text:0042B6A9                 cmp     byte_50F9C6, 0
.text:0042B6B0                 jnz     short loc_42B6BB
*/

	// cdrom interrupt timer
	ApplyJmpHook(
		(unsigned char*) 0x0042b6a0,
		(unsigned char*) &cdrom_irq_timer_170,
		(0xa7 - 0xa0) - 5 );

	DirtyPatch( 0x42b6a0 );


/*
-->
.text:0042A050                 or      dword_510BC0, 4
-->
.text:0042A057                 retn
*/

	// cdrom lid state - output
	ApplyJmpHook(
		(unsigned char*) 0x0042a050,
		(unsigned char*) &cdrom_lid_output_170,
		(0x57 - 0x50) - 5 );

	DirtyPatch( 0x42a050 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0042C802                 call    dword_4C4AF8
-->
.text:0042C808                 mov     dl, byte_50F9CA
.text:0042C80E                 movzx   eax, [esp+1Ch+var_B]
*/

	// getTN - stop reading flag
	ApplyJmpHook(
		(unsigned char*) 0x0042c802,
		(unsigned char*) &gettn_fix_170,
		(0x08 - 0x02) - 5 );

	DirtyPatch( 0x42c802 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:00409DFE                 push    400h            ; cchMax
-->
.text:00409E03                 push    offset byte_C0FB40 ; lpString
.text:00409E08                 push    232Ah           ; nIDDlgItem
*/

	// memcard - flag auto-eject
	ApplyJmpHook(
		(unsigned char*) 0x00409dfe,
		(unsigned char*) &memcard_swap_170,
		(0xe03 - 0xdfe) - 5 );

	DirtyPatch( 0x409dfe );


/*
.text:0043B4F6                 mov     byte_5078A3, 5Ah
-->
.text:0043B4FD                 mov     byte_5078A2, al
-->
.text:0043B502
*/

	// memcard - restart card
	ApplyJmpHook(
		(unsigned char*) 0x0043b4fd,
		(unsigned char*) &memcard_eject_170,
		(0x502 - 0x4fd) - 5 );

	DirtyPatch( 0x43b4fd );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0040C1FB                 cmp     dword_510B5C, 0 ; jumptable 0040BDEB case 40009
-->
.text:0040C202                 jz      loc_40CC70      ; jumptable 0040BCC2 cases 6,8
*/

	// reset emu - spu crash
	ApplyJmpHook(
		(unsigned char*) 0x0040c1fb,
		(unsigned char*) &reset_emu_170,
		(0x202 - 0x1fb) - 5 );

	DirtyPatch( 0x40c1fb );


/*
-->
.text:0043746C                 lea     ecx, [esp+114h+var_104]
.text:00437470                 push    ecx
-->
.text:00437471                 mov     [esp+118h+var_104], eax
.text:00437475                 mov     al, byte_4C5D94[esi]
*/

	// emu - cd exe name
	ApplyJmpHook(
		(unsigned char*) 0x0043746c,
		(unsigned char*) &cd_name_170,
		(0x71 - 0x6c) - 5 );

	DirtyPatch( 0x43746c );
	DirtyPatch( 0x437470 );

// ###############################
// ###############################
// ###############################
// ###############################

// .text:0042C27A                 mov     word_45F118, 1C2h

	// longer cdlpause time - InuYasha / Gundam Battle Assault 2
	{
		static short value[1] = { 0x1c2 * 2 };

		WriteCode(
			(unsigned char*) 0x42c27a+7,
			(unsigned char*) value,
			2 );

		DirtyPatch( 0x42c27a );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0043832E                 movzx   ecx, al
-->
.text:00438331                 mov     ecx, dword_5084AC[ecx*8]
.text:00438338                 add     ecx, 96h
-->
.text:0043833E                 mov     eax, 1B4E81B5h
*/
	
	// GetTD - track 0 (iso only)
	ApplyJmpHook(
		(unsigned char*) 0x00438331,
		(unsigned char*) &gettd_iso_track0_170,
		(0x3e - 0x31) - 5 );

	DirtyPatch( 0x438331 );
	DirtyPatch( 0x438338 );


/*
.text:0043831A                 jbe     short loc_43832E
-->
.text:0043831C                 movzx   eax, al
.text:0043831F                 push    eax             ; Args
.text:00438320                 push    offset aErrorOverflowI ; " * Error: overflow in GetTD %d\n"
-->
.text:00438325                 call    sub_439240
*/
	
	// GetTD - track N (overflow - iso only)
	ApplyJmpHook(
		(unsigned char*) 0x0043831f,
		(unsigned char*) &gettd_iso_trackN_170,
		(0x25 - 0x1c) - 5 );

	DirtyPatch( 0x43831c );
	DirtyPatch( 0x43831f );
	DirtyPatch( 0x438320 );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0043026E                 movzx   edx, word_4BDB8C[eax]
-->
.text:00430275                 test    dx, dx
.text:00430278                 jz      short loc_43028C
*/

	// L3/R3 - dualanalog + dualshock only
	ApplyJmpHook(
		(unsigned char*) 0x0043026e,
		(unsigned char*) &joypad_dualshock_keys_170,
		(0x75 - 0x6e) - 5 );

	DirtyPatch( 0x43026e );


/*
-->
.text:0042D755                 sub     eax, 0
.text:0042D758                 jz      short loc_42D77D
.text:0042D75A                 sub     eax, 1
-->
.text:0042D75D                 mov     eax, [esp+arg_4]
*/

	// port2 - pad1 - mouse data
	ApplyJmpHook(
		(unsigned char*) 0x0042d755,
		(unsigned char*) &port2_mouse_170,
		(0x5d - 0x55) - 5 );

	DirtyPatch( 0x42d755 );
	DirtyPatch( 0x42d758 );
	DirtyPatch( 0x42d75a );


/*
.text:0042A7C6                 mov     dword_510B44, eax
.text:0042A7CB                 mov     dword_510A44, eax
-->
.text:0042A7D0                 mov     dword_510A48, eax
-->
.text:0042A7D5                 retn
*/

	// system reset - do extra init
	ApplyJmpHook(
		(unsigned char*) 0x0042a7d0,
		(unsigned char*) &cpu_reset_170,
		(0xd5 - 0xd0) - 5 );

	DirtyPatch( 0x42a7d0 );


/*
.text:0043A4A5                 movzx   ecx, byte_4C6DE4
.text:0043A4AC                 push    ecx
.text:0043A4AD                 push    edx
-->
.text:0043A4AE                 push    edi
-->
.text:0043A4AF                 call    sub_42D7D0
*/

	// konami justifier - send pad # (push eax)
	{
		static char value[1] = { 0x50 };

		WriteCode(
			(unsigned char*) 0x43a4ae,
			(unsigned char*) value,
			1 );

		DirtyPatch( 0x43a4ae );
	}


/*
-->
.text:0042D7D0                 movzx   eax, [esp+arg_0]
-->
.text:0042D7D5                 push    ebx
.text:0042D7D6                 xor     ebx, ebx
.text:0042D7D8                 sub     eax, ebx
*/
	
	// port2 - konami justifier
	ApplyJmpHook(
		(unsigned char*) 0x0042d7d0,
		(unsigned char*) &port2_justifier_170,
		(0xd5 - 0xd0) - 5 );

	DirtyPatch( 0x42d7d0 );


/*
.text:0042D8D2                 cmp     [esp+4+arg_8], bl
-->
.text:0042D8D6                 jz      short loc_42D92A
-->
.text:0042D8D8                 and     word ptr [ecx+3], 0FFC8h
.text:0042D8DE                 movzx   eax, word ptr [ecx+3]
*/

	// konami justifier - use current data (jnz)
	if(0)
	{
		static char value[1] = { 0x75 };

		WriteCode(
			(unsigned char*) 0x42d8d6,
			(unsigned char*) value,
			1 );

		DirtyPatch( 0x42d8d6 );
	}



/*

.text:0042D816                 mov     byte ptr [ecx+1], 31h
.text:0042D81A                 mov     byte ptr [ecx+2], 5Ah
-->
.text:0042D81E                 mov     word ptr [ecx+3], 0C03Fh
-->
.text:0042D824                 mov     [ecx+8], bl
.text:0042D827                 mov     [ecx+7], bl
*/

	// konami justifier - default button state
	{
		static short value[1] = { (short) 0xffff };

		WriteCode(
			(unsigned char*) 0x42d81e+4,
			(unsigned char*) value,
			2 );

		DirtyPatch( 0x42d81e );
	}


/*
.text:0043B152                 movzx   eax, al
-->
.text:0043B155                 cmp     dword_4C4C60[eax*4], 1
.text:0043B15D                 jnz     loc_43B34C      ; default
-->
.text:0043B163                 movzx   eax, [esp+14h+var_1]
*/

	// sio $45 - get status - arg #5 (dualshock-only led)
	ApplyJmpHook(
		(unsigned char*) 0x0043b155,
		(unsigned char*) &sio_cmd45_dualshock_led_170,
		(0x63 - 0x55) - 5 );

	DirtyPatch( 0x43b155 );
	DirtyPatch( 0x43b15d );

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0042B38C                 mov     byte ptr [esi+2], 80h
-->
.text:0042B390                 shl     al, 4
.text:0042B393                 add     al, dl
.text:0042B395                 mov     [esi+1], al
*/
	// repplay - index # (0 or 1) - use 1 for now
	{
		static char value[1] = { 0x01 };

		WriteCode(
			(unsigned char*) 0x42b38c+3,
			(unsigned char*) value,
			1 );

		DirtyPatch( 0x42b38c );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
; analog lx-ly
.text:0042D0C8                 movzx   ecx, byte ptr dword_4C4C90
.text:0042D0CF                 add     cl, 80h
.text:0042D0D2                 mov     [eax+7], cl
.text:0042D0D5                 movzx   edx, byte ptr dword_4C4CB0
.text:0042D0DC                 add     dl, 80h
.text:0042D0DF                 mov     [eax+8], dl

; analog rx-ry
.text:0042D0E2                 movzx   ecx, byte ptr dword_4C4CD0
.text:0042D0E9                 add     cl, 80h
.text:0042D0EC                 mov     [eax+5], cl
.text:0042D0EF                 movzx   edx, byte ptr dword_4C4CF0
.text:0042D0F6                 add     dl, 80h
.text:0042D0F9                 mov     [eax+6], dl

.text:0042D0FC                 retn
*/

	// analog mode - fix center value ($80 -> $7f)
	{
		static char value[1] = { 0x7f };

		// digital joypad
		WriteCode(
			(unsigned char*) 0x43a573+6,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x43a573+7,
			(unsigned char*) value,
			1 );	
		WriteCode(
			(unsigned char*) 0x43a573+8,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x43a573+9,
			(unsigned char*) value,
			1 );

		DirtyPatch( 0x43a573 );
		


		/*
		// analog red - case 0
		WriteCode(
			(unsigned char*) 0x42d0cf+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d0dc+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d0e9+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d0f6+2,
			(unsigned char*) value,
			1 );


		// analog red - case 1
		WriteCode(
			(unsigned char*) 0x42d127+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d134+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d141+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d14e+2,
			(unsigned char*) value,
			1 );


		// analog red - case 2
		WriteCode(
			(unsigned char*) 0x42d17f+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d18c+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d199+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d1a6+2,
			(unsigned char*) value,
			1 );


		// analog red - case 3
		WriteCode(
			(unsigned char*) 0x42d1d7+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d1e4+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d1f1+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d1fe+2,
			(unsigned char*) value,
			1 );


		// analog red - case 4
		WriteCode(
			(unsigned char*) 0x42d22f+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d23c+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d249+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d256+2,
			(unsigned char*) value,
			1 );


		// analog red - case 5
		WriteCode(
			(unsigned char*) 0x42d287+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d294+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d2a1+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d2ae+2,
			(unsigned char*) value,
			1 );


		// analog red - case 6
		WriteCode(
			(unsigned char*) 0x42d2df+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d2ec+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d2f9+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d306+2,
			(unsigned char*) value,
			1 );


		// analog red - case 7
		WriteCode(
			(unsigned char*) 0x42d337+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d344+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d351+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d35e+2,
			(unsigned char*) value,
			1 );


		// analog red - default
		WriteCode(
			(unsigned char*) 0x42d37b+3,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d37f+3,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d383+3,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d387+3,
			(unsigned char*) value,
			1 );
		*/

	



		// digital joypad
		WriteCode(
			(unsigned char*) 0x43a077+4,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x43a077+5,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x43a077+6,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x43a077+7,
			(unsigned char*) value,
			1 );

		DirtyPatch( 0x43a077 );
		

	

		/*
		// analog green - case 0
		WriteCode(
			(unsigned char*) 0x42d426+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d433+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d43f+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d44f+2,
			(unsigned char*) value,
			1 );


		// analog green - case 1
		WriteCode(
			(unsigned char*) 0x42d487+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d494+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d4a0+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d4b0+2,
			(unsigned char*) value,
			1 );


		// analog green - case 2
		WriteCode(
			(unsigned char*) 0x42d4e8+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d4f5+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d501+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d511+2,
			(unsigned char*) value,
			1 );


		// analog green - case 3
		WriteCode(
			(unsigned char*) 0x42d549+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d556+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d562+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d572+2,
			(unsigned char*) value,
			1 );


		// analog green - case 4
		WriteCode(
			(unsigned char*) 0x42d5aa+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d5b7+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d5c3+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d5d3+2,
			(unsigned char*) value,
			1 );


		// analog green - case 5
		WriteCode(
			(unsigned char*) 0x42d60b+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d618+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d624+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d634+2,
			(unsigned char*) value,
			1 );


		// analog green - case 6
		WriteCode(
			(unsigned char*) 0x42d66c+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d679+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d685+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d695+2,
			(unsigned char*) value,
			1 );


		// analog green - case 7
		WriteCode(
			(unsigned char*) 0x42d6cd+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d6da+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d6e6+2,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d6f6+2,
			(unsigned char*) value,
			1 );


		// analog green - default
		WriteCode(
			(unsigned char*) 0x42d714+3,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d718+3,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d71c+3,
			(unsigned char*) value,
			1 );
		WriteCode(
			(unsigned char*) 0x42d720+3,
			(unsigned char*) value,
			1 );
		*/
	}



/*
// analog red
.text:0043A09B                 call    sub_42D090
.text:0043A592                 call    sub_42D090


// analog green
.text:0043A0A2                 call    sub_42D3E0
.text:0043A5A1                 call    sub_42D3E0

	
// stack still valid after call
.text:0042D0A5                 mov     eax, [esp+arg_4] ; jumptable 0042D09E case 0
.text:0042D0A9                 mov     byte ptr [eax], 0
.text:0042D0AC                 mov     byte ptr [eax+1], 73h
.text:0042D0B0                 mov     byte ptr [eax+2], 5Ah
*/

	// add analog deadzones
	ApplyJmpHook(
		(unsigned char*) 0x0043a09b,
		(unsigned char*) &analog_deadzone_red1_170,
		(0xa7 - 0xa2) - 5 );

	DirtyPatch( 0x43a09b );
		

	ApplyJmpHook(
		(unsigned char*) 0x0043a592,
		(unsigned char*) &analog_deadzone_red2_170,
		(0xa7 - 0xa2) - 5 );

	DirtyPatch( 0x43a592 );


	ApplyJmpHook(
		(unsigned char*) 0x0043a0a2,
		(unsigned char*) &analog_deadzone_green1_170,
		(0xa7 - 0xa2) - 5 );

	DirtyPatch( 0x43a0a2 );


	ApplyJmpHook(
		(unsigned char*) 0x0043a5a1,
		(unsigned char*) &analog_deadzone_green2_170,
		(0xa7 - 0xa2) - 5 );

	DirtyPatch( 0x43a5a1 );



/*
-->
.text:00412B51                 mov     eax, dword_4C0930
-->
.text:00412B56                 mov     ecx, dword_459638
.text:00412B5C                 cmp     eax, ecx
*/

	ApplyJmpHook(
		(unsigned char*) 0x00412b51,
		(unsigned char*) &mouse_sensitivity_170,
		(0x56 - 0x51) - 5 );

	DirtyPatch( 0x412b51 );



/*
-->
.text:0041117C                 lea     eax, [eax+eax*2]
-->
.text:0041117F                 mov     dword_4C0930, eax

	
-->
.text:00411184                 mov     eax, [esp+14h+var_10]
.text:00411188                 lea     ecx, [eax+eax*2]
-->
.text:0041118B                 movzx   eax, [esp+14h+var_7]

(..)

.text:00411199                 mov     dword_4C0934, ecx
.text:0041119F                 movzx   ecx, [esp+14h+var_6]
*/

	// raw mouse data - no sensitivity scaling
	{
		static u8 value[3] = { 0x90, 0x90, 0x90 };

		WriteCode(
			(unsigned char*) 0x41117c,
			(unsigned char*) value,
			3 );

		DirtyPatch( 0x41117c );
	}


	// mov ecx,eax
	{
		static u8 value[3] = { 0x8b, 0xc8, 0x90 };

		WriteCode(
			(unsigned char*) 0x411188,
			(unsigned char*) value,
			3 );

		DirtyPatch( 0x411188 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0040C1DF                 cmp     dword_510B5C, 0 ; jumptable 0040BDEB case 40008
-->
.text:0040C1E6                 jz      loc_40CC70      ; jumptable 0040BCC2 cases 6,8
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040c1df,
		(unsigned char*) &reload_ini_170,
		(0xe6 - 0xdf) - 5 );

	DirtyPatch( 0x40c1df );


// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0043A500                 call    sub_42CD70
-->
.text:0043A505                 movzx   eax, [esp+1Ch+var_1]
.text:0043A50A                 mov     edx, [esp+1Ch+arg_8]
*/

	// negcon - change controller ID for special commands
	{
		ApplyJmpHook(
			(unsigned char*) 0x0043a500,
			(unsigned char*) &negcon_read_170,
			(0x05 - 0x00) - 5 );

		DirtyPatch( 0x43a500 );
	}


// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:004038FE                 mov     ecx, [ebx+4]
.text:00403901                 mov     eax, ecx
-->
.text:00403903                 lea     esi, [eax+1]
*/

	// gui + loadbin - set iso name
	{
		ApplyJmpHook(
			(unsigned char*) 0x004038fe,
			(unsigned char*) &loadbin_registry_170,
			(0x903 - 0x8fe) - 5 );

		DirtyPatch( 0x4038fe );
		DirtyPatch( 0x403901 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0043A433                 call    sub_42F4F0
-->
.text:0043A438                 movzx   eax, [esp+1Ch+var_1]
.text:0043A43D                 mov     ecx, [esp+1Ch+arg_8]
*/

	// guncon - sio 43 detection
	{
		ApplyJmpHook(
			(unsigned char*) 0x0043a433,
			(unsigned char*) &guncon_sio43_170,
			(0x38 - 0x33) - 5 );

		DirtyPatch( 0x43a433 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:004111AC                 shr     edx, 4
-->
.text:004111AF                 mov     dword_4BFAC4, edx
-->
.text:004111B5
.text:004111B5 loc_4111B5:                             ; CODE XREF: sub_4110D0+43.j
.text:004111B5                                         ; sub_4110D0+50.j
.text:004111B5                 mov     ecx, [esp+14h+var_4]
.text:004111B9                 xor     ecx, esp
*/

	// mouse wheel detection
	{
		ApplyJmpHook(
			(unsigned char*) 0x004111af,
			(unsigned char*) &mousewheel_170,
			(0xb5 - 0xaf) - 5 );

		DirtyPatch( 0x004111af );
	}



/*
-->
.text:00412B4C                 call    sub_4110D0
-->
.text:00412B51                 mov     eax, dword_4C0930
.text:00412B56                 mov     ecx, dword_459638
*/

	// dualshock - analog mouse poll
	{
		ApplyJmpHook(
			(unsigned char*) 0x00412b4c,
			(unsigned char*) &dualshock_mouse_poll_170,
			(0x51 - 0x4c) - 5 );

		DirtyPatch( 0x00412b4c );
	}



/*
-->
.text:0042D0AC                 mov     byte ptr [eax+1], 73h
.text:0042D0B0                 mov     byte ptr [eax+2], 5Ah
-->

; digital 1-2
.text:0042D0B4                 movzx   ecx, byte ptr dword_4BDB4C+1
*/

	// dualshock - analog mouse
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042d0ac,
			(unsigned char*) &dualshock_mouse_170,
			(0xb4 - 0xac) - 5 );

		DirtyPatch( 0x0042d0ac );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0043A2AC                 shr     eax, 1
.text:0043A2AE                 or      al, 80h
.text:0043A2B0                 mov     [ecx+1], al
-->
.text:0043A2B3
.text:0043A2B3 locret_43A2B3:                          ; CODE XREF: sub_43A240+6A.j
.text:0043A2B3                 retn
*/

	// multitap port2 - keep ID
	{
		static u8 value[3] = { 0x90, 0x90, 0x90 };

		WriteCode(
			(unsigned char*) 0x43a2ac,
			(unsigned char*) value,
			2 );
		WriteCode(
			(unsigned char*) 0x43a2ae,
			(unsigned char*) value,
			2 );
		WriteCode(
			(unsigned char*) 0x43a2b0,
			(unsigned char*) value,
			3 );

		DirtyPatch( 0x43a2ac );
		DirtyPatch( 0x43a2ae );
		DirtyPatch( 0x43a2b0 );
	}



/*
.text:00439FBD                 movzx   eax, [esp+20h+arg_C] ; jumptable 00439FB6 case 0
.text:00439FC2                 mov     cl, [esp+20h+arg_8]
-->
.text:00439FC6                 mov     byte_4C6DA4[eax], cl
-->
.text:00439FCC loc_439FCC:                             ; CODE XREF: sub_439F90+24.j
*/

	// no controller ID
	{
		ApplyJmpHook(
			(unsigned char*) 0x439fc6,
			(unsigned char*) &sio_multitap_no_controller_170,
			(0xcc - 0xc6) - 5 );

		DirtyPatch( 0x439fc6 );
	}



/*
.text:0043A7A9                 movzx   eax, [esp+20h+var_1]
-->
.text:0043A7AE                 mov     al, byte ptr (dword_507BE2+1)[eax+ebp]
-->
.text:0043A7B5                 add     esp, 0Ch
.text:0043A7B8                 pop     edi
.text:0043A7B9                 pop     esi
*/

	// multitap - config commands ($43,45,47,4c)
	{
		ApplyJmpHook(
			(unsigned char*) 0x43a7ae,
			(unsigned char*) &sio_multitap_45_170,
			(0xb5 - 0xae) - 5 );

		DirtyPatch( 0x43a7ae );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0042CDB0                 movzx   ecx, byte ptr dword_4C4C90
.text:0042CDB7                 add     cl, 80h
.text:0042CDBA                 mov     [eax+5], cl

.text:0042CDBD                 movzx   edx, byte ptr dword_4C4CD0
.text:0042CDC4                 mov     [eax+6], dl

.text:0042CDC7                 movzx   ecx, byte ptr dword_4C4CF0
.text:0042CDCE                 mov     [eax+7], cl

.text:0042CDD1                 movzx   edx, byte ptr dword_4C4CB0
.text:0042CDD8                 mov     [eax+8], dl
-->
.text:0042CDDB                 retn
*/

	// clamp down before writing value, flip I+II
	{
		ApplyJmpHook(
			(unsigned char*) 0x42cdb0,
			(unsigned char*) &negcon_clamp_pad1_170,
			(0xdb - 0xb0) - 5 );

		DirtyPatch( 0x42cdb0 );
		DirtyPatch( 0x42cdb7 );
		DirtyPatch( 0x42cdba );
		DirtyPatch( 0x42cdc4 );
		DirtyPatch( 0x42cdc7 );
		DirtyPatch( 0x42cdce );
		DirtyPatch( 0x42cdd1 );
		DirtyPatch( 0x42cdd8 );
	}



//.text:0042CDB0                 movzx   ecx, byte ptr dword_4C4C90
//.text:0042CDB7                 add     cl, 80

	// input testing
	if(0)
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042cdb0,
			(unsigned char*) &negcon_test_170,
			(0xb7 - 0xb0) - 5 );

		DirtyPatch( 0x42cdb0 );
	}


// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:00439B8E                 mov     byte_507BE1, 2
-->
.text:00439B95                 add     byte_507BE2[esi], 1
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x00439b8e,
			(unsigned char*) &sio_memcard_none,
			(0x95 - 0x8e) - 5 );

		DirtyPatch( 0x42cd8e );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0042C6C1                 call    dword_4C4B18
-->
.text:0042C6C7                 mov     al, byte_50F9CD
.text:0042C6CC                 add     esp, 10h
.text:0042C6CF                 cmp     al, bl
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042c6c1,
			(unsigned char*) &cdrom_sbi_1,
			(0xc7 - 0xc1) - 5 );

		DirtyPatch( 0x42c6c1 );
	}



/*
-->
.text:0042C718                 call    dword_4C4B18
-->
.text:0042C71E                 mov     al, byte_50F9CD
.text:0042C723                 add     esp, 10h
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042c718,
			(unsigned char*) &cdrom_sbi_2,
			(0x1e - 0x18) - 5 );

		DirtyPatch( 0x42c718 );
	}



/*
-->
.text:0042C75E                 call    dword_4C4B18
-->
.text:0042C764                 mov     al, byte_50F9CD
.text:0042C769                 add     esp, 10h
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042C75E,
			(unsigned char*) &cdrom_sbi_3,
			(0x64 - 0x5e) - 5 );

		DirtyPatch( 0x42C75E );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:0043A446                 mov     dword ptr [ecx], 9
-->
.text:0043A44C                 mov     al, byte ptr (dword_507BE2+1)[eax+ebp]
.text:0043A453                 pop     ebp
*/

	// guncon - invalid command handling
	{
		ApplyJmpHook(
			(unsigned char*) 0x0043A446,
			(unsigned char*) &guncon_cmd_response_170,
			(0x4c - 0x46) - 5 );

		DirtyPatch( 0x43A446 );
	}



/*
-->
.text:0043A3CC                 mov     dword ptr [edx], 7
-->
.text:0043A3D2                 mov     al, byte ptr (dword_507BE2+1)[eax+ebp]
.text:0043A3D9                 pop     ebp
*/

	// mouse - invalid command handling
	{
		ApplyJmpHook(
			(unsigned char*) 0x0043A3CC,
			(unsigned char*) &mouse_cmd_response_170,
			(0xd2 - 0xcc) - 5 );

		DirtyPatch( 0x43A3CC );
	}



/*
.text:0043A011                 cmp     bl, 42h
.text:0043A014                 jz      short loc_43A01B
-->
.text:0043A016                 mov     byte ptr [esp+20h+var_14+1], 0F1h
-->
.text:0043A01B                 cmp     bl, 45h
.text:0043A01E                 jnz     short loc_43A034
*/

	// multitap - no config
	{
		ApplyJmpHook(
			(unsigned char*) 0x0043A016,
			(unsigned char*) &multitap_cmd_response_170,
			(0x1b - 0x16) - 5 );

		DirtyPatch( 0x43a016 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:00419D14                 jnb     short loc_419D22
-->
.text:00419D16                 and     eax, 0FFFh
-->
.text:00419D1B                 mov     al, byte ptr dword_B4C420[eax]
*/


	// memory read - dcache penalty (1 cycle)
	{
		ApplyJmpHook(
			(unsigned char*) 0x00419d16,
			(unsigned char*) &memory_read_penalty_dcache_1_170,
			(0x1b - 0x16) - 5 );

		DirtyPatch( 0x419d16 );



		ApplyJmpHook(
			(unsigned char*) 0x00419d66,
			(unsigned char*) &memory_read_penalty_dcache_2_170,
			(0x6b - 0x66) - 5 );

		DirtyPatch( 0x419d66 );



		ApplyJmpHook(
			(unsigned char*) 0x00419dc6,
			(unsigned char*) &memory_read_penalty_dcache_3_170,
			(0xcb - 0xc6) - 5 );

		DirtyPatch( 0x419dc6 );
	}



/*
.text:00419E16                 mov     cl, byte ptr [esp+arg_4]
-->
.text:00419E1A                 and     eax, 0FFFh
-->
.text:00419E1F                 mov     byte ptr dword_B4C420[eax], cl
*/

	// memory write - dcache penalty (1 cycle)
	{
		ApplyJmpHook(
			(unsigned char*) 0x00419e1a,
			(unsigned char*) &memory_write_penalty_dcache_1_170,
			(0x1f - 0x1a) - 5 );

		DirtyPatch( 0x419e1a );



		ApplyJmpHook(
			(unsigned char*) 0x00419efb,
			(unsigned char*) &memory_write_penalty_dcache_2_170,
			(0x100 - 0xfb) - 5 );

		DirtyPatch( 0x419efb );



		ApplyJmpHook(
			(unsigned char*) 0x00419fda,
			(unsigned char*) &memory_write_penalty_dcache_3_170,
			(0xdf - 0xda) - 5 );

		DirtyPatch( 0x419fda );
	}



/*
.text:0041A20D                 jnb     short loc_41A220
.text:0041A20F                 mov     dl, byte ptr [esp+arg_4]
-->
.text:0041A213                 and     ecx, 0FFFh
-->
.text:0041A219                 mov     byte ptr dword_B4C420[ecx], dl
*/

	// memory - dcache penalty (1 cycle)
	{
		ApplyJmpHook(
			(unsigned char*) 0x0041a213,
			(unsigned char*) &memory_penalty_dcache_1_170,
			(0x19 - 0x13) - 5 );

		DirtyPatch( 0x41a213 );
	}



/*
-->
.text:00426BA7                 mov     dword ptr [ecx], 1F800000h
-->
.text:00426BAD                 mov     ecx, dword_4C4B2C
.text:00426BB3                 add     eax, 80h
.text:00426BB8                 add     ecx, 4
.text:00426BBB                 mov     dword_4C4B2C, ecx
*/

	// dynarec - read dcache penalty (1 cycle)
	{
		ApplyJmpHook(
			(unsigned char*) 0x00426ba7,
			(unsigned char*) &dynarec_memory_penalty_dcache_1_170,
			(0xad - 0xa7) - 5 );

		DirtyPatch( 0x426bad );



		ApplyJmpHook(
			(unsigned char*) 0x00426e98,
			(unsigned char*) &dynarec_memory_penalty_dcache_2_170,
			(0x9e - 0x98) - 5 );

		DirtyPatch( 0x426e98 );



		ApplyJmpHook(
			(unsigned char*) 0x00427189,
			(unsigned char*) &dynarec_memory_penalty_dcache_3_170,
			(0x8f - 0x89) - 5 );

		DirtyPatch( 0x427189 );
	}



	// dynarec - write dcache penalty (1 cycle)
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042747A,
			(unsigned char*) &dynarec_memory_penalty_dcache_4_170,
			(0x80 - 0x7a) - 5 );

		DirtyPatch( 0x42747A );



		ApplyJmpHook(
			(unsigned char*) 0x00427A70,
			(unsigned char*) &dynarec_memory_penalty_dcache_5_170,
			(0x76 - 0x70) - 5 );

		DirtyPatch( 0x427A70 );





		ApplyJmpHook(
			(unsigned char*) 0x0042807A,
			(unsigned char*) &dynarec_memory_penalty_dcache_6_170,
			(0x80 - 0x7a) - 5 );

		DirtyPatch( 0x42807A );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:004286E1                 and     dword_81AAB4, 0FEFFFFFFh
-->
.text:004286EB                 push    1
.text:004286ED                 call    sub_42A070
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x004286E1,
			(unsigned char*) &dma_irq_mdecout_170,
			(0xeb - 0xe1) - 5 );

		DirtyPatch( 0x4286E1 );
	}


/*
-->
.text:0040D803                 and     dword_81AAD8, 0FEFFFFFFh
-->
.text:0040D80D                 push    4
.text:0040D80F                 mov     byte_510E16, 0FFh
.text:0040D816                 call    sub_42A070
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0040D803,
			(unsigned char*) &dma_irq_spu_170,
			(0x0d - 0x03) - 5 );

		DirtyPatch( 0x40D803 );
	}



	{
		ApplyJmpHook(
			(unsigned char*) 0x0040f0bb,
			(unsigned char*) &dma_irq_spu2_170,
			(0xc2 - 0xbb) - 5 );

		DirtyPatch( 0x40f0bb );
	}


/*
-->
.text:0042B816                 push    3
.text:0042B818                 mov     byte_5109E4, 1
.text:0042B81F                 call    sub_42A070
.text:0042B824                 add     esp, 4
-->
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042b816,
			(unsigned char*) &dma_irq_cdrom_170,
			(0x24 - 0x16) - 5 );

		DirtyPatch( 0x0042b816 );
	}


/*
-->
.text:0042E834                 and     dword_81AAC0, 0FEFFFFFFh
-->
.text:0042E83E                 push    2
.text:0042E840                 call    sub_42A070
.text:0042E845                 add     esp, 4
.text:0042E848                 mov     dword_510A94, 80000000h
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042e834,
			(unsigned char*) &dma_irq_gpu_170,
			(0x3e - 0x34) - 5 );

		DirtyPatch( 0x0042e834 );
	}



/*
===>
.text:0042E897                 and     dword_81AAC0, 0FEFFFFFFh
===>
.text:0042E8A1                 push    2
.text:0042E8A3                 call    sub_42A070
.text:0042E8A8                 mov     dword_510A94, 80000000h
.text:0042E8B2                 add     esp, 4
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042e897,
			(unsigned char*) &dma_irq_gpu2_170,
			(0xa1 - 0x97) - 5 );

		DirtyPatch( 0x0042e897 );
	}


/*
-->
.text:0042E995                 and     dword_81AAC0, 0FEFFFFFFh
-->
.text:0042E99F                 push    2
.text:0042E9A1                 call    sub_42A070
.text:0042E9A6                 add     esp, 4
.text:0042E9A9                 pop     edi
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042e995,
			(unsigned char*) &dma_irq_gpu3_170,
			(0x9f - 0x95) - 5 );

		DirtyPatch( 0x0042e995 );
	}



/*
-->
.text:0042E9E5                 and     dword_81AAC0, 0FEFFFFFFh
-->
.text:0042E9EF                 push    2
.text:0042E9F1                 call    sub_42A070
.text:0042E9F6                 add     esp, 4
.text:0042E9F9                 pop     edi
.text:0042E9FA                 pop     esi
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042e9e5,
			(unsigned char*) &dma_irq_gpu4_170,
			(0xef - 0xe5) - 5 );

		DirtyPatch( 0x0042e9e5 );
	}



/*
===>
.text:0042EABA                 push    2
.text:0042EABC                 call    sub_42A070
.text:0042EAC1                 add     esp, 4
===>
.text:0042EAC4                 and     dword_81AAC0, 0FEFFFFFFh
.text:0042EACE                 mov     byte_510E14, 0FFh
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042EABA,
			(unsigned char*) &dma_irq_gpu5_170,
			(0xc4 - 0xba) - 5 );

		DirtyPatch( 0x0042EABA );
	}



/*
--->
.text:0042EAF3                 and     dword_81AAF0, 0FEFFFFFFh
--->
.text:0042EAFD                 push    6
.text:0042EAFF                 mov     byte_510E15, 0FFh
.text:0042EB06                 call    sub_42A070
.text:0042EB0B                 pop     ecx
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042EAF3,
			(unsigned char*) &dma_irq_gpuotc_170,
			(0xfd - 0xf3) - 5 );

		DirtyPatch( 0x0042EAF3 );
	}



/*
-->
.text:0042EBCA                 and     ecx, 0FEFFFFFFh
-->
.text:0042EBD0                 push    6
.text:0042EBD2                 mov     dword_81AAF0, ecx
.text:0042EBD8                 call    sub_42A070
.text:0042EBDD                 add     esp, 4
.text:0042EBE0                 mov     byte_510E15, 0FFh
*/

	{
		ApplyJmpHook(
			(unsigned char*) 0x0042EBCA,
			(unsigned char*) &dma_irq_gpuotc2_170,
			(0xd0 - 0xca) - 5 );

		DirtyPatch( 0x0042EBCA );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0041FF20                 mov     eax, [esp+arg_0]
-->
.text:0041FF24                 lea     ecx, [eax-1F801C00h]
-->
.text:0041FF2A                 cmp     ecx, 2EFh
.text:0041FF30                 ja      short loc_41FF3C
.text:0041FF32                 mov     [esp+arg_0], eax
*/

	if( cdrom_tracer )
	{
		ApplyJmpHook(
			(unsigned char*) 0x0041ff24,
			(unsigned char*) &MMIO_read_170,
			(0x2a - 0x24) - 5 );

		DirtyPatch( 0x0041ff24 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:00438BD7                 mov     eax, ecx
.text:00438BD9                 cdq
.text:00438BDA                 mov     ecx, esi
.text:00438BDC                 idiv    ecx
-->
.text:00438BDE                 shl     al, 4
.text:00438BE1                 add     al, dl
-->
.text:00438BE3                 mov     [ebp+7], al
*/

	// GetlocP - fake subq - track # (BCD)
	{
		ApplyJmpHook(
			(unsigned char*) 0x00438bde,
			(unsigned char*) &Cdrom_getlocp_track_170,
			(0xe3 - 0xde) - 5 );

		DirtyPatch( 0x00438bde );
	}



/*
.text:0042C293                 mov     word_45F114, bx
.text:0042C29A                 mov     word_45F118, bx
-->
.text:0042C2A1                 mov     byte_45F112, al
-->
.text:0042C2A6                 pop     ebx
*/

	// Pause - already stopped (short time)
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042c2a1,
			(unsigned char*) &Cdrom_pause_short_170,
			(0xa6 - 0xa1) - 5 );

		DirtyPatch( 0x0042c2a1 );
	}



/*
.text:0042C060 loc_42C060:                             ; CODE XREF: sub_42BC80+21B.j
.text:0042C060                 mov     byte_50F9CA, bl
.text:0042C066                 mov     byte_50F9CD, 80h

; Actua Soccer 2 - CDDA says $02 for acknowledge, $82 later after -physical- read
.text:0042C06D                 mov     al, 82h
*/

	// Play - no cdda flag yet on acknowledge
	{
		static u8 flag[1] = { 0x02 };

		WriteCode(
			(unsigned char*) 0x42c06d+1,
			(unsigned char*) flag,
			1 );

		DirtyPatch( 0x42c06d );
	}



/*
; check flag stop
	
-->
.text:0042B4F9                 test    byte_50F9CF, 2
-->
.text:0042B500                 jz      short loc_42B578
*/

	// Repplay - check flag $4 (turn off DataReady)
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042b4f9,
			(unsigned char*) &cdrom_repplay_flag_170,
			(0x500 - 0x4f9) - 5 );

		DirtyPatch( 0x0042b4f9 );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
.text:0042D7AE loc_42D7AE:                             ; CODE XREF: sub_42D750+58.j
-->
.text:0042D7AE                 mov     cl, byte ptr dword_4C4C80
-->
.text:0042D7B4                 mov     [eax+5], cl
.text:0042D7B7                 mov     dl, byte ptr dword_4C4C84
.text:0042D7BD                 mov     [eax+6], dl
.text:0042D7C0                 retn
*/

	// Mouse wheel converter
	if( mousewheel_converter )
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042d7ae,
			(unsigned char*) &mouse_wheel_converter_170,
			(0x7b4 - 0x7ae) - 5 );

		DirtyPatch( 0x0042d7ae );
	}

// ###############################
// ###############################
// ###############################
// ###############################

/*
-->
.text:00439BDE                 mov     byte_507BE1, 1
-->
.text:00439BE5                 add     byte_507BE2[esi], 1
*/

	// pad plugins - startpoll
	ApplyJmpHook(
		(unsigned char*) 0x00439bde,
		(unsigned char*) &pad_startpoll_170,
		(0xbe5 - 0xbde) - 5 );

	DirtyPatch( 0x00439bde );


/*
.text:0043B3F9                 push    0
.text:0043B3FB                 push    ecx
-->
.text:0043B3FC                 call    sub_43A2C0
-->
.text:0043B401                 add     esp, 0Ch
*/

	// pad plugins - poll pad1
	ApplyJmpHook(
		(unsigned char*) 0x0043b3fc,
		(unsigned char*) &pad_poll_pad1_170,
		(0x401 - 0x3fc) - 5 );

	DirtyPatch( 0x0043b3fc );



/*
.text:0043B419                 push    1
.text:0043B41B                 push    ecx
-->
.text:0043B41C                 call    sub_43A2C0
-->
.text:0043B421                 add     esp, 0Ch
.text:0043B424                 retn
*/

	// pad plugins - poll pad2
	ApplyJmpHook(
		(unsigned char*) 0x0043b41c,
		(unsigned char*) &pad_poll_pad2_170,
		(0x421 - 0x41c) - 5 );

	DirtyPatch( 0x0043b41c );



/*
.text:0042DE62                 call    esi ; GetProcAddress
.text:0042DE64                 test    eax, eax
-->
.text:0042DE66                 mov     dword_50F0BC, eax
-->
.text:0042DE6B                 jnz     short loc_42DE7F
*/

	// gpuinit
	ApplyJmpHook(
		(unsigned char*) 0x42DE66,
		(unsigned char*) &GPUinit_Wrapper_170,
		(0xe6b - 0xe66) - 5 );

	DirtyPatch( 0x42DE66 );



/*
.text:0042DEDC                 call    esi ; GetProcAddress
.text:0042DEDE                 test    eax, eax
-->
.text:0042DEE0                 mov     dword_50F080, eax
-->
.text:0042DEE5                 jnz     short loc_42DEF9
.text:0042DEE7                 push    offset aGpuclose ; "GPUclose"
*/

	// gpuclose
	ApplyJmpHook(
		(unsigned char*) 0x42DEE0,
		(unsigned char*) &GPUclose_Wrapper_170,
		(0xee5 - 0xee0) - 5 );

	DirtyPatch( 0x42DEE0 );



/*
.text:0042E181                 mov     edx, dword_4C4D30
.text:0042E187                 push    offset aGpucleardynare ; "GPUclearDynarec"
.text:0042E18C                 push    edx             ; hModule
-->
.text:0042E18D                 mov     dword_50EE88, eax
-->
.text:0042E192                 call    esi ; GetProcAddress
*/

	// visual vibration - pad interface
	ApplyJmpHook(
		(unsigned char*) 0x42e18d,
		(unsigned char*) &GPUvisualVibration_Wrapper_170,
		(0x192 - 0x18d) - 5 );

	DirtyPatch( 0x42e18d );



/*
.text:0042E149                 mov     edx, dword_4C4D30
.text:0042E14F                 push    offset aGpusetframelim ; "GPUsetframelimit"
.text:0042E154                 push    edx             ; hModule
-->
.text:0042E155                 mov     dword_50F060, eax
-->
.text:0042E15A                 call    esi ; GetProcAddress
*/

	// gun cursor - pad interface
	ApplyJmpHook(
		(unsigned char*) 0x42e155,
		(unsigned char*) &GPUcursor_Wrapper_170,
		(0x15a - 0x155) - 5 );

	DirtyPatch( 0x42e155 );

// ################################
// ################################
// ################################
// ################################

/*
-->
.text:0042B52B                 mov     byte_50F9C5, 4 (dataend)
-->
.text:0042B532                 mov     byte_50F900, 2 (output)
.text:0042B539                 mov     byte_50F940, 1
.text:0042B540                 mov     byte_50F941, 0
*/

	// Tomb Raider 1 - cdda music track change + resume hack
	if( tomb1_cdda_hack || tomb2_cdda_hack ) {
		ApplyJmpHook(
			(unsigned char*) 0x42b52b,
			(unsigned char*) &CDDA_tomb_raider1_cdda_resume_hack_170,
			(0x532 - 0x52b) - 5 );

		DirtyPatch( 0x42b52b );

	

/*
.text:0042BFAC                 cmp     byte_5109FB, al
.text:0042BFB2                 jnz     short loc_42C017
*/
		/*
		// skip auto-cdda stop (iso only)
		// - interferes with Tomb Raider 2 music loop
		ApplyJmpHook(
			(unsigned char*) 0x42bfac,
			(unsigned char*) 0x42c017,
			0 );
		*/
	}



/*
.text:00431573                 movzx   ecx, byte ptr [esp+7]
.text:00431578                 push    edx
.text:00431579                 push    eax
.text:0043157A                 push    ecx
-->
.text:0043157B                 call    dword_50E488
-->
.text:00431581                 pop     ecx
.text:00431582                 retn
*/

	// repplay stop - set correct msf time (ex. Tomb Raider 1)
	ApplyJmpHook(
		(unsigned char*) 0x43157B,
		(unsigned char*) &cdrom_plugin_cdda_stop_170,
		(0x581 - 0x57b) - 5 );

	DirtyPatch( 0x43157b );


/*
.text:00431151                 push    esi
--->
.text:00431152                 mov     esi, dword_50E470
--->
.text:00431158                 test    esi, esi
.text:0043115A                 jz      short loc_431196
*/

	// load state - cdrplay + cdrreadcdda (don't mix them together!)
	ApplyJmpHook(
		(unsigned char*) 0x431152,
		(unsigned char*) &cdrom_cdda_load_170,
		(0x158 - 0x152) - 5 );

	DirtyPatch( 0x431152 );






/*
-->
.text:0042B502                 mov     dl, byte_5109FA
-->
.text:0042B508                 cmp     dl, byte_50F9D0
.text:0042B50E                 jnz     short loc_42B578
*/

	// on cdda restart, sometimes track goes over stop point
	// - repplay every 10 frames
	ApplyJmpHook(
		(unsigned char*) 0x42b502,
		(unsigned char*) &cdrom_cdda_repplay_stop_over_170,
		(0x508 - 0x502) - 5 );

	DirtyPatch( 0x42b502 );



/*
.text:0042B37D                 movzx   edi, [esp+14h+var_A]
-->
.text:0042B382                 mov     eax, edi
.text:0042B384                 cdq
.text:0042B385                 mov     ecx, 0Ah
-->
.text:0042B38A                 idiv    ecx
.text:0042B38C                 mov     byte ptr [esi+2], 80h
*/

	// Repplay - track # (cd plugin only)
	ApplyJmpHook(
		(unsigned char*) 0x42b382,
		(unsigned char*) &repplay_track_cdrom_plugin_170,
		(0x38a - 0x382) - 5 );

	DirtyPatch( 0x42b382 );


// ################################
// ################################
// ################################
// ################################

/*
.text:0042BE35 mov     byte_50F983, 2
.text:0042BE3C mov     byte_50F9C3, 1
-->
.text:0042BE43 mov     word_45F114, 0Fh
-->
.text:0042BE4C pop     ebx
.text:0042BE4D add     esp, 0Ch
*/

	// Setloc - store cdda frames time
	// - fix repplay playback (Mega Man X3 password)
	ApplyJmpHook(
		(unsigned char*) 0x42be43,
		(unsigned char*) &setloc_repplay_fix1_170,
		(0xe4c - 0xe43) - 5 );

	DirtyPatch( 0x42be43 );



/*
-->
.text:0042B35B                 movzx   edx, byte_50F9D2
-->
.text:0042B362                 lea     ecx, [esp+14h+var_A]
.text:0042B366                 push    ecx
.text:0042B367                 movzx   ecx, byte_50F9D1
.text:0042B36E                 push    edx
.text:0042B36F                 movzx   edx, byte_50F9D0
.text:0042B376                 push    ecx
.text:0042B377                 push    edx
.text:0042B378                 call    eax ; dword_4C4B24
.text:0042B37A                 add     esp, 10h
*/

	// Setloc + play (no seek) fix
	// - fix repplay playback (Mega Man X3 password)
	ApplyJmpHook(
		(unsigned char*) 0x42b35b,
		(unsigned char*) &setloc_repplay_fix2_170,
		(0x362 - 0x35b) - 5 );

	DirtyPatch( 0x42b35b );
}
