void _declspec(naked) Time2Addr_Wrapper_152()
{
	_asm {
		; OLD code
		imul eax,0x4b
		add eax,edx


		pushfd
		pushad

		push eax
		call Time2Addr

		popad
		popfd

		mov eax, LBA_local


		; JMP $0042b592
		push 0x42b592
		ret
	}
}


void _declspec(naked) CdlSetLoc_Wrapper_152()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x509ce3
		call CdlSetLoc

		popad
		popfd


		; OLD code
		; mov     al, byte_509CE3
		mov al, ds:[0x509ce3]

		; JMP $0042a5f7
		push 0x42a5f7
		ret
	}
}



void _declspec(naked) saveState1_Wrapper_152()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B2B80
		push dword ptr 0x45A9E4
		call save_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_4031A0
		mov eax,0x4031A0
		call eax

		; JMP $0040A7EC
		push 0x0040A7EC
		ret
	}
}



void _declspec(naked) saveState2_Wrapper_152()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B2B80
		push dword ptr 0x45A9E4
		call save_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_4031A0
		mov eax,0x4031A0
		call eax

		; JMP $0042C216
		push 0x0042C216
		ret
	}
}



void _declspec(naked) loadState1_Wrapper_152()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B2B80
		push dword ptr 0x45A9E4
		call load_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_403320
		mov eax,0x403320
		call eax

		; JMP $40A7A0
		push 0x40A7A0
		ret
	}
}



void _declspec(naked) loadState2_Wrapper_152()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B2B80
		push dword ptr 0x45A9E4
		call load_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_403320
		mov eax,0x403320
		call eax

		; JMP $0042C27F
		push 0x0042C27F
		ret
	}
}



void _declspec(naked) CDRreadTrack_Wrapper_152()
{
	_asm {
		// SPECIAL: Retrieve CDR_SetDVD5( int state )
		mov eax, ds:[0x4FC81C]
		push offset CDR_setDVD5_str
		push eax
		call esi
		mov CDR_setDVD5, eax



		; OLD code
		mov eax, ds:[0x4FC81C]

		; JMP $442EE8D
		push 0x42EE8D
		ret
	}
}



void Attach_152()
{

	// window caption
	static char name[] = " ePSXe - Shark enhanced";
	WriteCode(
		(unsigned char*) 0x0044DD94,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x0044DD94 );

	WriteCode(
		(unsigned char*) 0x00455590,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x00455590 );


/*
.text:0042A5F0                 xor     eax, eax        ; jumptable 0042A53A case 2
--->
.text:0042A5F2                 mov     al, byte_509CE3
--->
.text:0042A5F7                 mov     ebx, 2
.text:0042A5FC                 mov     byte_509D6B, 1
.text:0042A603                 mov     byte_509D6A, 0
.text:0042A60A                 mov     byte_509D6D, 0
*/

	ApplyJmpHook( 
		(unsigned char*) 0x0042a5f2,
		(unsigned char*) &CdlSetLoc_Wrapper_152,
		5-5 );


/*
.text:0042B588                 imul    eax, 3Ch
.text:0042B58B                 add     eax, ecx
-->
.text:0042B58D                 imul    eax, 4Bh
.text:0042B590                 add     eax, edx
-->
.text:0042B592                 retn
*/

	ApplyJmpHook( 
		(unsigned char*) 0x0042b58d,
		(unsigned char*) &Time2Addr_Wrapper_152,
		5-5 );



/*
.text:0040A7D9                 mov     eax, dword_44CDCC
.text:0040A7DE                 add     eax, 0FFFFFFF6h
.text:0040A7E1                 push    eax
.text:0040A7E2                 call    sub_403180
-->
.text:0040A7E7                 call    sub_4031A0
-->
.text:0040A7EC                 mov     ecx, dword_44CDCC
.text:0040A7F2                 add     ecx, 0FFFFFFF6h
.text:0040A7F5                 push    ecx             ; Args
.text:0040A7F6                 push    offset aSavestateDoneD ; " * SaveState Done! (%d)\n"
.text:0040A7FB                 call    sub_435CD0
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040a7e7,
		(unsigned char*) &saveState1_Wrapper_152,
		5-5 );


/*
.text:0042C200                 test    al, al
.text:0042C202                 jnz     loc_42C473
.text:0042C208                 mov     al, byte_509930
.text:0042C20D                 test    al, al
.text:0042C20F                 jz      short loc_42C22F
-->
.text:0042C211                 call    sub_4031A0
-->
.text:0042C216                 push    offset aSavestateDone ; " * SaveState Done! \n"
.text:0042C21B                 call    sub_435CD0
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042C211,
		(unsigned char*) &saveState2_Wrapper_152,
		5-5 );



/*
.text:0040A78C                 mov     byte ptr [esp+4+var_4], al
.text:0040A790                 mov     eax, dword_44CDCC
.text:0040A795                 push    eax
.text:0040A796                 call    sub_403180
-->
.text:0040A79B                 call    sub_403320
-->
.text:0040A7A0                 call    sub_427F40
.text:0040A7A5                 mov     ecx, dword_44CDCC
.text:0040A7AB                 push    ecx             ; Args
.text:0040A7AC                 push    offset aLoadstateDoneD ; " * LoadState Done! (%d)\n"
.text:0040A7B1                 call    sub_435CD0
.text:0040A7B6                 mov     edx, [esp+10h+var_4]
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040A79B,
		(unsigned char*) &loadState1_Wrapper_152,
		5-5 );


/*
.text:0042C271                 mov     eax, dword_4FC808
.text:0042C276                 test    eax, eax
.text:0042C278                 jnz     short loc_42C2AC
-->
.text:0042C27A                 call    sub_403320
-->
.text:0042C27F                 cmp     dword_50B18C, esi
.text:0042C285                 jnz     short loc_42C28C
.text:0042C287                 call    sub_427F40
.text:0042C28C
.text:0042C28C loc_42C28C:                             ; CODE XREF: sub_42D340-10BB.j
.text:0042C28C                 push    offset aLoadstateDone ; " * LoadState Done! \n"
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042C27A,
		(unsigned char*) &loadState2_Wrapper_152,
		5-5 );


/*
.text:0042EE7B                 cmp     eax, ebx
.text:0042EE7D                 mov     dword_504250, eax
.text:0042EE82                 jz      loc_42EF7F
-->
.text:0042EE88                 mov     eax, dword_4FC81C
-->
.text:0042EE8D                 push    offset aCdrreadtrack ; "CDRreadTrack"
.text:0042EE92                 push    eax             ; hModule
.text:0042EE93                 call    esi ; GetProcAddress
.text:0042EE95                 cmp     eax, ebx
.text:0042EE97                 mov     dword_508EE0, eax
.text:0042EE9C                 jz      loc_42EF7F
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042EE88,
		(unsigned char*) &CDRreadTrack_Wrapper_152,
		5-5 );
}
