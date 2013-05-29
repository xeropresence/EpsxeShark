void __declspec(naked) slow_boot_180()
{
	__asm 
	{
			cmp slow_boot,0
			jz old

			mov ds:[0x466000], 0
			jmp done

old:
			; OLD code
			mov ds:[0x466000], al
done:
			push dword ptr 0x00403CA7
			ret
	}
}

void _declspec(naked) GPUinit_Wrapper_180()
{
	__asm {
		mov GPUinit_func, eax
		mov eax, offset GPUinit



		; OLD code
		mov ds:[0x4F05FC], eax

		push 0x004323EB
		ret
	}
}

void _declspec(naked) GPUopen_Wrapper_180()
{
	__asm {
		mov GPUopen_func, eax


/*
.text:0040FAB0                 cmp     byte_468122, 0
.text:0040FAB7                 jz      short loc_40FACE
.text:0040FAB9                 push    2
.text:0040FABB                 call    ds:Sleep
*/
// 1.7.0 hack - get framelimiter address
		mov eax, 0x488D82
		mov framelimit, eax


		mov eax, offset GPUopen



		; OLD code
		; mov     dword_50F03C, eax
		mov ds:[0x4F057C], eax

		; JMP $0042debd
		push 0x0043243D
		ret
	}
}

void _declspec(naked) GPUclose_Wrapper_180()
{
	__asm {
		mov GPUclose_func, eax
		mov eax, offset GPUclose



		; OLD code
		mov ds:[0x4F05C0], eax

		push 0x00432465
		ret
	}
}

void _declspec(naked) GPUshutdown_Wrapper_180()
{
	__asm {
		mov GPUshutdown_func, eax
		mov eax, offset GPUshutdown



		; OLD code
		mov ds:[0x4F0590], eax

		push 0x00432414
		ret
	}
}

void _declspec(naked) GPUwriteStatus_Wrapper_180()
{
	_asm {
		mov GPUwriteStatus_func, eax
		mov eax, offset GPUwriteStatus



		; OLD code
		mov dword ptr ds:[0x4F0A08], eax

		push 0x00432531
		ret
	}
}

void __declspec(naked) GPUsetframelimit_Wrapper_180()
{
	__asm 
	{
		mov GPUsetframelimit_func, eax
		mov eax, offset GPUsetframelimit

		; OLD code
		mov dword ptr ds:[0x4F0A00], eax

		push 0x004326E1
		ret
	}
}

void _declspec(naked) GPUclearDynarec_Wrapper_180()
{
	_asm {
		mov GPUclearDynarec_func, eax
			mov eax, offset GPUclearDynarec


			; OLD code
			mov ds:[0x4F0578], eax

			; JMP $0042E19C
			push 0x0043271C
			ret
	}
}

void _declspec(naked) GPUupdateLace_Wrapper_180()
{
	_asm {
		mov GPUupdateLace_func, eax
			mov eax, offset GPUupdateLace


			; OLD code
			mov ds:[0x4F0574], eax

			push 0x00432625
			ret
	}
}

void _declspec(naked) GPUshowScreenPic_Wrapper_180()
{
	__asm 
	{
			mov GPUshowScreenPic_func, eax
			mov eax, offset GPUshowScreenPic

			; OLD code
			mov ecx, dword ptr ds:[0x4E5B60]

			push 0x004326BC
			ret
	}
}

void __declspec(naked) reset_emu_180()
{
	__asm {
			mov reset_spu,1
			;mov memcard_reset,0


			; old code
			cmp dword ptr ds:0x4F291C,0

			push dword ptr 0x0040BF65
			ret
	}
}

void __stdcall autostart_controllers_180()
{
	u32 *pad1_ptr = (u32 *) 0x4DE918;
	u32 *pad5_ptr = (u32 *) 0x4DE928;
	u32 *pad1_toggle = (u32 *) 0x4E5A90;
	u32 *pad5_toggle = (u32 *) 0x4E5AA0;
	u32 *special_flag = (u32 *) 0x4E5A8C;


	if( autostart_controllers == 0 ) return;


	/*
	none = 0
	digital = 1
	mouse = 2
	guncon = 3
	dualshock = 4
	dualanalog = 5
	? = 6
	negcon = 7
	justifier = 8
	*/

	// turn on pad1
	if( (*pad1_ptr) > 1 ) {
		if( autostart_controllers == 1 ) {
			// dualshock / dualanalog must be off!
			if( (*pad1_ptr) != 4 && (*pad1_ptr) != 5 )
				(*pad1_toggle) = 1;
			else
				(*pad1_toggle) = 0;
		}
		else if( autostart_controllers == 2 )
			(*pad1_toggle) = 0;
		else if( autostart_controllers == 3 )
			(*pad1_toggle) = 1;
	}


	// turn on pad5
	if( (*pad5_ptr) > 1 ) {
		if( autostart_controllers == 1 ) {
			// dualshock / dualanalog must be off!
			if( (*pad5_ptr) != 4 && (*pad5_ptr) != 5 )
				(*pad5_toggle) = 1;
			else
				(*pad5_toggle) = 0;
		}
		else if( autostart_controllers == 2 )
			(*pad5_toggle) = 0;
		else if( autostart_controllers == 3 )
			(*pad5_toggle) = 1;
	}

#if 0
// ###############################
// ###############################
// ###############################

	// dualshock mouse controller

	if( dualshock_mouse_active ) {
		// turn on pad1 - dualshock
		(*pad1_ptr) = 4;
		(*pad1_toggle) = 0;

	
		// turn on pad5 - mouse
		(*pad5_ptr) = 2;
		(*pad5_toggle) = 1;
	}

// ###############################
// ###############################
// ###############################

	(*special_flag) = 0;


	// autostart mouse driver
	if( (*pad1_ptr) == 2 || (*pad5_ptr) == 2 )
		(*special_flag) = 1;


	// autostart guncon driver
	if( (*pad1_ptr) == 3 || (*pad5_ptr) == 3 )
		(*special_flag) = 1;


	// autostart justifier driver
	if( (*pad1_ptr) == 8 || (*pad5_ptr) == 8 )
		(*special_flag) = 1;
#endif
}

void __declspec(naked) cpu_reset_180()
{
	__asm {
			pushad
			pushfd

			;mov tomb1_last_play,0
			;mov tomb1_play_count,0

			;call konami_justifiers_170
			call autostart_controllers_180

			; load redump sbi file
			;call opensbifile_wrapper

			popfd
			popad

			// spu crash on reset fix
			mov reset_spu,1

			; OLD code
			mov dword ptr ds:0x4F2808,eax

			push dword ptr 0x0042DAA5
			ret
	}
}

void __declspec(naked) SPU_PlayCDDA_180()
{
	__asm 
	{
			test eax,eax
			mov dword ptr ds:[0xC75A8C],0
			jz done


			; reset emu - spu crash (bad buffer)
			cmp reset_spu,1
			mov reset_spu,0
			je done


			pushad
			pushfd

			push ecx
			call cdda_attenuation

			popfd
			popad


			; call spuPlayCDDAChannel
			push dword ptr 2352
			push ecx
			call eax

done:
			push dword ptr 0x0040DBF6
			ret
	}
}

void __declspec(naked) MMIO_Write_180()
{
	__asm 
	{
			// addr, rt
			mov temp_var1, eax
			movzx eax, byte ptr [esp+8]
			
			pushad
			pushfd

			push eax
			push temp_var1
			call MMIO_Write

			popfd
			popad

			; OLD code
			mov eax, temp_var1
			cmp eax, dword ptr 0x1f801800

			push 0x0041FBC9
			ret
	}
}

void __declspec(naked) SPU_PlayXA_180()
{
	__asm 
	{
			pushad
			pushfd

			push dword ptr 0xC8E0C0
			call xa_attenuation

			popfd
			popad
			

			; old code
			push dword ptr 0xC8E0C0

			push 0x0040DBA3
			ret
	}
}

void _declspec(naked) SPUopen_Wrapper_180()
{
	__asm 
	{
		mov SPUopen_func, eax
		mov eax, offset SPUopen

		; OLD code
		mov ds:[0xD18C90], eax

		push 0x0040D0B6
		ret
	}
}

void __declspec(naked) SPUsetframelimit_Wrapper_180()
{
	__asm 
	{
			; OLD code
			mov dword ptr ds:[0xD18CB0],eax

			; get spu library address
			mov SPUsetframelimit_func,0
			mov ecx, dword ptr ds:[0x004E045C]
			push offset spu_lib_setframelimit
			push ecx
			call esi
			mov SPUsetframelimit_func,eax

			; OLD code
			mov ecx, dword ptr ds:[0x004E045C]
			push 0x0040D400
			ret
	}
}

void __declspec(naked) pad_startpoll_180()
{
	__asm {
		pushad
		pushfd

		push dword ptr ds:[0x911480]
		call PADStartPoll_func

		popfd
		popad

		// OLD code
		mov byte ptr ds:[0x911481],1

		push dword ptr 0x421548
		ret
	}
}

void __declspec(naked) pad_poll_pad1_180()
{
	__asm {
		// OLD code
		mov temp_var1,eax
		mov temp_var2,ecx

		mov func_ptr,0x421C30
		call func_ptr



		// save result
		mov temp_var3,eax

		pushad
		pushfd

		push 0
		call PADPoll_func

		popfd
		popad



		// OLD code
		mov eax,temp_var3

		push dword ptr 0x4230A1
		ret
	}
}

void __declspec(naked) pad_poll_pad2_180()
{
	__asm {
		// OLD code
		mov temp_var1,eax
		mov temp_var2,ecx

		mov func_ptr,0x421C30
		call func_ptr



		// save result
		mov temp_var3,eax

		pushad
		pushfd

		push 1
		call PADPoll_func

		popfd
		popad

		
		
		// OLD code
		mov eax,temp_var3

		push dword ptr 0x004230C1
		ret
	}
}