void _declspec(naked) Time2Addr_Wrapper_160()
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


		; JMP $0042c702
		push 0x42c702
		ret
	}
}


void _declspec(naked) CdlSetLoc_Wrapper_160()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x50aec3
		call CdlSetLoc

		popad
		popfd


		; OLD code
		; mov     al, byte_50AEC3
		mov al, ds:[0x50aec3]

		; JMP $0042b7ee
		push 0x42b7ee
		ret
	}
}



void _declspec(naked) saveState1_Wrapper_160()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B3D80
		push dword ptr 0x45b8d0
		call save_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_4035A0
		mov eax,0x4035A0
		call eax

		; JMP $0040B436
		push 0x0040B436
		ret
	}
}



void _declspec(naked) saveState2_Wrapper_160()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B3D80
		push dword ptr 0x45b8d0
		call save_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_4035A0
		mov eax,0x4035A0
		call eax

		; JMP $0042D80F
		push 0x0042D80F
		ret
	}
}



void _declspec(naked) loadState1_Wrapper_160()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B3D80
		push dword ptr 0x45b8d0
		call load_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_403720
		mov eax,0x403720
		call eax

		; JMP $40B3EA
		push 0x40B3EA
		ret
	}
}



void _declspec(naked) loadState2_Wrapper_160()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x8B3D80
		push dword ptr 0x45b8d0
		call load_dvd5_state

		popad
		popfd



		; OLD code
		; call    sub_403720
		mov eax,0x403720
		call eax

		; JMP $0042D878
		push 0x0042D878
		ret
	}
}



void _declspec(naked) CDRreadTrack_Wrapper_160()
{
	_asm {
		// SPECIAL: Retrieve CDR_SetDVD5( int state )
		mov eax, ds:[0x4FD9B0]
		push offset CDR_setDVD5_str
		push eax
		call esi
		mov CDR_setDVD5, eax



		; OLD code
		mov eax, ds:[0x4FD9B0]

		; JMP $4303E3
		push 0x4303E3
		ret
	}
}



void __declspec(naked) Tracer_Wrapper_160()
{
	_asm {
		pushfd
		pushad

		call Opcode_Tracer_Run_160

		popad
		popfd



		; OLD code
		mov dword ptr ds:[0x50c260], edi

		; JMP $42b3bb
		push 0x42b3bb
		ret
	}
}



void Attach_160()
{

	// window caption
	static char name[] = " ePSXe - Shark enhanced";
	WriteCode(
		(unsigned char*) 0x0044EF04,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x0044EF04 );

	WriteCode(
		(unsigned char*) 0x0045666C,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x0045666C );

/*
.text:0042B7E7                 xor     eax, eax        ; jumptable 0042B743 case 2
--->
.text:0042B7E9                 mov     al, byte_50AEC3
--->
.text:0042B7EE                 mov     edx, 2
.text:0042B7F3                 mov     byte_50AF4B, 1
.text:0042B7FA                 mov     byte_50AF4A, bl
.text:0042B800                 mov     byte_50AF4D, bl
.text:0042B806                 mov     byte_50AF49, dl
.text:0042B80C                 push    eax
.text:0042B80D                 call    sub_42B6A0
*/

	ApplyJmpHook( 
		(unsigned char*) 0x0042b7e9,
		(unsigned char*) &CdlSetLoc_Wrapper_160,
		5-5 );


/*
.text:0042C6F8                 imul    eax, 3Ch
.text:0042C6FB                 add     eax, ecx
-->
.text:0042C6FD                 imul    eax, 4Bh
.text:0042C700                 add     eax, edx
-->
.text:0042C702                 retn
*/

	ApplyJmpHook( 
		(unsigned char*) 0x0042c6fd,
		(unsigned char*) &Time2Addr_Wrapper_160,
		5-5 );



/*
.text:0040B42B                 push    eax
.text:0040B42C                 call    sub_403580
-->
.text:0040B431                 call    sub_4035A0
-->
.text:0040B436                 mov     ecx, dword_44DF24
.text:0040B43C                 add     ecx, 0FFFFFFF6h
.text:0040B43F                 push    ecx             ; Args
.text:0040B440                 push    offset aSavestateDoneD ; " * SaveState Done! (%d)\n"
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040b431,
		(unsigned char*) &saveState1_Wrapper_160,
		5-5 );


/*
.text:0042D7F9                 test    al, al
.text:0042D7FB                 jnz     loc_42D9EB
.text:0042D801                 mov     al, byte_50AAD0
.text:0042D806                 test    al, al
.text:0042D808                 jz      short loc_42D828
-->
.text:0042D80A                 call    sub_4035A0
-->
.text:0042D80F                 push    offset aSavestateDone ; " * SaveState Done! \n"
.text:0042D814                 call    sub_437290
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042D80A,
		(unsigned char*) &saveState2_Wrapper_160,
		5-5 );



/*
.text:0040B3DA                 mov     eax, dword_44DF24
.text:0040B3DF                 push    eax
.text:0040B3E0                 call    sub_403580
-->
.text:0040B3E5                 call    sub_403720
-->
.text:0040B3EA                 call    sub_4291E0
.text:0040B3EF                 mov     ecx, dword_44DF24
.text:0040B3F5                 push    ecx             ; Args
.text:0040B3F6                 push    offset aLoadstateDoneD ; " * LoadState Done! (%d)\n"
.text:0040B3FB                 call    sub_437290
*/

	ApplyJmpHook(
		(unsigned char*) 0x0040B3E5,
		(unsigned char*) &loadState1_Wrapper_160,
		5-5 );


/*
.text:0042D868                 jz      short loc_42D8B0
.text:0042D86A                 mov     eax, dword_4FD99C
.text:0042D86F                 test    eax, eax
.text:0042D871                 jnz     short loc_42D897
-->
.text:0042D873                 call    sub_403720
-->
.text:0042D878                 push    offset aLoadstateDone ; " * LoadState Done! \n"
.text:0042D87D                 call    sub_437290
*/

	ApplyJmpHook(
		(unsigned char*) 0x0042D873,
		(unsigned char*) &loadState2_Wrapper_160,
		5-5 );



/*
-->
.text:004303DE                 mov     eax, dword_4FD9B0
-->
.text:004303E3                 push    offset aCdrreadtrack ; "CDRreadTrack"
.text:004303E8                 push    eax             ; hModule
.text:004303E9                 call    esi ; GetProcAddress
.text:004303EB                 cmp     eax, ebx
.text:004303ED                 mov     dword_50A080, eax
.text:004303F2                 jz      loc_4304D5
.text:004303F8                 mov     ecx, dword_4FD9B0
*/

	ApplyJmpHook(
		(unsigned char*) 0x004303DE,
		(unsigned char*) &CDRreadTrack_Wrapper_160,
		5-5 );



/*
.text:0042B3A9                 mov     edi, dword_50C260
.text:0042B3AF                 add     edi, 4
.text:0042B3B2                 shr     esi, 1Ah
.text:0042B3B5                 mov     dword_50C260, edi

; run opcode (!!)
.text:0042B3BB                 call    off_455E80[esi*4] ; jumptable 0042AC78 cases 1,5,10,11,14,15,20-23,28-31,40,41,44-63
*/

	if( opcode_tracer )
	{
		ApplyJmpHook(
			(unsigned char*) 0x0042B3B5,
			(unsigned char*) &Tracer_Wrapper_160,
			(0xBB - 0xB5) - 5 );
	}
}
