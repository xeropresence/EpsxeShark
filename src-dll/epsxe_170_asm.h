void _declspec(naked) CdlReadTrack_ISO_Wrapper_170()
{
	_asm {
		pushfd
		pushad

		push 0
		push edx
		push ecx
		call CdlReadTrack_ISO

		popad
		popfd
	

		; OLD code

		; JMP $00438431
		push 0x438431
		ret
	}
}



void _declspec(naked) CdlSetLoc_Wrapper_170()
{
	_asm {
		pushfd
		pushad

		push dword ptr 0x50f943
		call CdlSetLoc

		popad
		popfd


		; OLD code
		; mov cl, byte ptr [0x0050f943]
		mov cl, ds:[0x50f943]

		; JMP $0042bd8c
		push 0x42bd8c
		ret
	}
}


void _declspec(naked) Time2Addr_Wrapper_170()
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


		; JMP $0042adfa
		push 0x42adfa
		ret
	}
}



void _declspec(naked) GPUwriteStatus_Wrapper_170()
{
	_asm {
		mov GPUwriteStatus_func, eax
		mov eax, offset GPUwriteStatus



		; OLD code
		; mov     dword_50F4C8, eax
		mov dword ptr ds:[0x50f4c8], eax

		; JMP $0042dfb1
		push 0x42dfb1
		ret
	}
}



void __declspec(naked) GPUsetframelimit_Wrapper_170()
{
	_asm {
		mov GPUsetframelimit_func, eax
		mov eax, offset GPUsetframelimit



		; OLD code
		mov dword ptr ds:[0x50f4c0], eax

		; JMP $0042e161
		push 0x42e161
		ret
	}
}



void __declspec(naked) SPUsetframelimit_Wrapper_170()
{
	_asm {
		; OLD code
		mov dword ptr ds:[0xc01210],eax


		
		; get spu library address
		mov SPUsetframelimit_func,0
		mov ecx, dword ptr ds:[0x4bf718]
		push offset spu_lib_setframelimit
		push ecx
		call esi
		mov SPUsetframelimit_func,eax




		; OLD code
		mov ecx, dword ptr ds:[0x4bf718]

		; JMP $0040D6A0
		push 0x40D6A0
		ret
	}
}



void _declspec(naked) GPUopen_Wrapper_170()
{
	_asm {
		mov GPUopen_func, eax


/*
.text:0040FAB0                 cmp     byte_468122, 0
.text:0040FAB7                 jz      short loc_40FACE
.text:0040FAB9                 push    2
.text:0040FABB                 call    ds:Sleep
*/
// 1.7.0 hack - get framelimiter address
		mov eax, 0x468122
		mov framelimit, eax


		mov eax, offset GPUopen



		; OLD code
		; mov     dword_50F03C, eax
		mov ds:[0x50f03c], eax

		; JMP $0042debd
		push 0x42debd
		ret
	}
}



void _declspec(naked) GPUupdateLace_Wrapper_170()
{
	_asm {
		mov GPUupdateLace_func, eax
		mov eax, offset GPUupdateLace


		; OLD code
		; mov     dword_50F034, eax
		mov ds:[0x50f034], eax

		; JMP $0042E0A5
		push 0x42E0A5
		ret
	}
}




void _declspec(naked) GPUclearDynarec_Wrapper_170()
{
	_asm {
		mov GPUclearDynarec_func, eax
		mov eax, offset GPUclearDynarec


		; OLD code
		; mov     dword_50F038, eax
		mov ds:[0x50F038], eax

		; JMP $0042E19C
		push 0x42E19C
		ret
	}
}



void _declspec(naked) saveState1_Wrapper_170()
{
	_asm {
		; OLD code
		; call    sub_404BC0
		mov eax,0x404bc0
		call eax


		
		pushfd
		pushad

		push dword ptr 0xb76e20
		call SaveXA_Data

		push dword ptr 0xc10740
		push dword ptr 0x4BF6E5
		call save_dvd5_state

		popad
		popfd



		; JMP $0040d22a
		push 0x40d22a
		ret
	}
}



void _declspec(naked) saveState2_Wrapper_170()
{
	_asm {
		; OLD code
		; call    sub_404BC0
		mov eax,0x404bc0
		call eax


		
		
		pushfd
		pushad

		push dword ptr 0xb76e20
		call SaveXA_Data

		push dword ptr 0xc10740
		push dword ptr 0x4BF6E5
		call save_dvd5_state

		popad
		popfd



		; JMP $0042FDCF
		push 0x0042FDCF
		ret
	}
}



void _declspec(naked) loadState1_Wrapper_170()
{
	_asm {
		; OLD code
		; call    sub_404d50
		mov eax,0x404d50
		call eax


		
		pushfd
		pushad

		push dword ptr 0xc10740
		push dword ptr 0x4BF6E5
		call load_dvd5_state

		push dword ptr 0xb76e20
		call LoadXA_Data

		popad
		popfd


		mov gpu_loadstate_hack,1


		; JMP $0x40d1e5
		push 0x40d1e5
		ret
	}
}



void _declspec(naked) loadState2_Wrapper_170()
{
	_asm {
		; OLD code
		; call    sub_404d50
		mov eax,0x404d50
		call eax




		pushfd
		pushad

		push dword ptr 0xc10740
		push dword ptr 0x4BF6E5
		call load_dvd5_state

		push dword ptr 0xb76e20
		call LoadXA_Data

		popad
		popfd



		mov gpu_loadstate_hack,1


		; JMP $0042fe42
		push 0x0042fe42
		ret
	}
}

void _declspec(naked) GPUshowScreenPic_Wrapper_170()
{
	_asm {
		mov GPUshowScreenPic_func, eax
		mov eax, offset GPUshowScreenPic



		; OLD code
		mov ecx, dword ptr ds:[0x4c4d30]

		push 0x42E13C
		ret
	}
}

void _declspec(naked) GPUshutdown_Wrapper_170()
{
	_asm {
		mov GPUshutdown_func, eax
		mov eax, offset GPUshutdown



		; OLD code
		mov ds:[0x50f050], eax

		; JMP $42DE94
		push 0x42DE94
		ret
	}
}



void _declspec(naked) CDRreadTrack_Wrapper_170()
{
	_asm {
		// SPECIAL: Retrieve CDR_SetDVD5( int state )
		mov eax, ds:[0x4C4D54]
		push offset CDR_setDVD5_str
		push eax
		call esi
		mov CDR_setDVD5, eax



		; OLD code
		mov eax, ds:[0x4C4D54]

		; JMP $430e6a
		push 0x430e6a
		ret
	}
}



void __declspec(naked) RunISO_Wrapper_170()
{
	_asm {
		mov eax,run_iso_mode
		cmp eax,5
		jz AUTORUN_ISO


; NORMAL_ISO:
		; OLD code - OpenFileName
		mov eax, dword ptr 0x405eb0
		call eax
		jmp EXIT


AUTORUN_ISO:
		; autorun ISO
		call RunISO_Wrapper

		; set return code
		mov eax,1


EXIT:
		; JMP $40bf8b
		push 0x40bf8b
		ret
	}
}



void __declspec(naked) RunISO_CDDA_Fix_170()
{
	__asm {
		; OLD code
		div ecx
		mov eax, [esp+0x18]
		mov [eax],dl



		pushad
		pushfd

		; stack not valid - reverse byte ptr
		mov edx, eax
		sub edx, 1

		; seconds data
		mov bl,[edx]

		; lba -> msf CDDA stop time (+2 sec)
		add bl,2
		mov [edx],bl

		; check overflow (60 min+)
		cmp bl,(0x3c-1)
		jna exit_proc



		; seconds adjust
		sub bl,0x3c
		mov [edx],bl

		; minutes adjust
		sub edx,1
		mov bl,[edx]
		add bl,1
		mov [edx],bl



exit_proc:
		popfd
		popad

		
		push dword ptr 0x4387a3
		ret
	}
}



void __declspec(naked) Tracer_Wrapper_170()
{
	_asm {
		pushfd
		pushad

		call Opcode_Tracer_Run_170

		popad
		popfd



		; OLD code
		add dword ptr ds:[0x510a40], 4

		; JMP $42a983
		push 0x42a983
		ret
	}
}


void __declspec(naked) SPU_PlayXA_170()
{
	_asm {
		pushad
		pushfd

		push dword ptr 0xb76e20
		call xa_attenuation

		popfd
		popad



		; old code
		push dword ptr 0xb76e20

		push 0x40de43
		ret
	}
}


void __declspec(naked) SPU_PlayCDDA_170()
{
	_asm {
		test eax,eax
		mov dword ptr ds:[0xb5e7ec],0
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
		; JMP $40de96
		push dword ptr 0x40de96
		ret
	}
}


void __declspec(naked) MMIO_Write_170()
{
	_asm {
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

		; JMP $41f569
		push 0x41f569
		ret
	}
}


void __stdcall switch_disc_core_170()
{
	// NOTE: use static to stop debugger crashes
	// - maybe because auto-init vars aren't compiling right

	static u32 *system_irq = (u32 *) 0x510bc0;
	static u8 *output = (u8 *) 0x50f900;
	static u8 *output_temp = (u8 *) 0x50f983;
	static u8 *output_state = (u8 *) 0x50f940;
	static u8 *cdr_irq = (u8 *) 0x50f9c5;
	static u8 *cdr_command = (u8 *) 0x5109e1;

	static u8 *cdr_open = (u8 *) 0x50f9c9;
	static u8 *cdr_reading = (u8 *) 0x50f9ca;
	static u8 *cdr_rotating = (u8 *) 0x50f9cb;
	static u8 *cdr_play = (u8 *) 0x50f9cd;


	// force cdrom irq
	(*system_irq) |= 4;
	if( (*cdr_irq) == CDRINT_NONE )
		(*cdr_irq) = CDRINT_ACKNOWLEDGE;


	// setup output state
	output_state[0] = 1;
	output_state[1] = 0;

	// set flags - lid open
	output[0] = CDRSTATUS_SHELLOPEN;



	// immediately change state - cd changed
	// - avoid disc corruption errors
	// - ex. changing cd on movie playing

	// lid open - no cd
	(*cdr_reading) |= CDRSTATUS_SHELLOPEN;
	(*cdr_rotating) = 0;

	// stop cdda (vib ribbon)
	(*cdr_play) = 0x00;
}


void __declspec(naked) switch_disc_170()
{
	_asm {
		pushad
		pushfd

		; request lid open
		mov lid_state, 1
		mov lid_timer, 0

		call Lid_Interrupt
		call switch_disc_core_170


		; load redump sbi file
		call opensbifile_wrapper

		popfd
		popad



		; old code
		mov func_ptr, 0x4198b0
		call func_ptr

		push dword ptr 0x40c0f2
		ret
	}
}


void __stdcall cdrom_irq_timer_core_170()
{
	// NOTE: use static to stop debugger crashes
	// - maybe because auto-init vars aren't compiling right

	static u32 *system_irq = (u32 *) 0x510bc0;
	static u8 *output = (u8 *) 0x50f900;
	static u8 *output_temp = (u8 *) 0x50f983;
	static u8 *output_state = (u8 *) 0x50f940;
	static u8 *cdr_irq = (u8 *) 0x50f9c5;
	static u8 *cdr_command = (u8 *) 0x5109e1;

	static u8 *cdr_open = (u8 *) 0x50f9c9;
	static u8 *cdr_reading = (u8 *) 0x50f9ca;
	static u8 *cdr_rotating = (u8 *) 0x50f9cb;
	static u8 *cdr_play = (u8 *) 0x50f9cd;


	if( lid_flag == 1 )
	{
		lid_flag = 0;

		switch( lid_state ) {
			case 0:
				// normal
				break;


			case 1:
				// (internal) start lid open
				break;


			case 2:
				// lid open - no cd
				(*cdr_reading) |= CDRSTATUS_SHELLOPEN;
				(*cdr_rotating) = 0;

				// stop cdda (vib ribbon)
				(*cdr_play) = 0x00;
				break;


			case 3:
				// lid open - cd present
				(*cdr_rotating) = 1;
				break;


			case 4:
				// lid closed - wait
				(*cdr_reading) &= ~CDRSTATUS_SHELLOPEN;
				break;


			case 5:
				// lid closed - seek
				(*cdr_reading) |= CDRSTATUS_SEEK;
				break;


			case 6:
				// lid closed - done
				(*cdr_reading) &= ~CDRSTATUS_SEEK;


				// Metal Gear Solid / Xenogears - needs rotating flag
				(*cdr_rotating) = 1;
				break;


			default:
				break;
		}
	}
}


void __declspec(naked) cdrom_irq_timer_170()
{
	_asm {
		pushad
		pushfd

		; check lid state
		call Lid_Interrupt
		call cdrom_irq_timer_core_170

		popfd
		popad



		; old code
		cmp byte ptr ds:0x50f9c5,0

		push dword ptr 0x42b6a7
		ret
	}
}



void __stdcall cdrom_lid_output_core_170()
{
	// NOTE: use static to stop debugger crashes
	// - maybe because auto-init vars aren't compiling right

	static u32 *system_irq = (u32 *) 0x510bc0;
	static u8 *output = (u8 *) 0x50f900;
	static u8 *output_temp = (u8 *) 0x50f983;
	static u8 *output_state = (u8 *) 0x50f940;
	static u8 *cdr_irq = (u8 *) 0x50f9c5;
	static u8 *cdr_command = (u8 *) 0x5109e1;

	static u8 *cdr_open = (u8 *) 0x50f9c9;
	static u8 *cdr_reading = (u8 *) 0x50f9ca;
	static u8 *cdr_rotating = (u8 *) 0x50f9cb;
	static u8 *cdr_play = (u8 *) 0x50f9cd;


	if( cdrom_tracer_active )
	{
		if( ePSXe_version == 170 )
		{
			fprintf( fp_cdrom, "[CDROM IRQ]\n" );
		}
	}


	switch( lid_state ) {
		case 0:
			// normal
			break;


		case 1:
			// start lid open
			break;


		case 2:
			// lid open - no cd
			if( (*cdr_command) == CdlGetlocP ) {
				memset( output, 0, 16 );
				output_state[0] = 16;
				output_state[1] = 0;


				// default status
				output[0] = (*cdr_reading);
				output[0] |= (*cdr_play);

				output[0] |= CDRSTATUS_SHELLOPEN;
				output[1] = 0x80;
			}


			// check valid disk error cmds
			if( (*cdr_command) != CdlNop &&
					(*cdr_command) != CdlMute &&
					(*cdr_command) != CdlDemute &&
					(*cdr_command) != CdlSetmode &&
					(*cdr_command) != CdlGetmode &&
					(*cdr_command) != CdlSetfilter ) {
				(*cdr_irq) = CDRINT_DISKERROR;

				output[0] |= CDRSTATUS_ERROR;
			}
			break;


		case 3:
			// lid open - cd present
			if( (*cdr_command) == CdlGetlocP ) {
				memset( output, 0, 16 );
				output_state[0] = 16;
				output_state[1] = 0;


				// default status
				output[0] = (*cdr_reading);
				output[0] |= (*cdr_play);

				output[0] |= CDRSTATUS_SHELLOPEN;
				output[0] |= CDRSTATUS_ROTATING;

				output[1] = 0x80;
			}


			// check valid disk error cmds
			if( (*cdr_command) != CdlNop &&
					(*cdr_command) != CdlMute &&
					(*cdr_command) != CdlDemute &&
					(*cdr_command) != CdlSetmode &&
					(*cdr_command) != CdlGetmode &&
					(*cdr_command) != CdlSetfilter ) {
				(*cdr_irq) = CDRINT_DISKERROR;
					output[0] |= CDRSTATUS_ERROR;
			}
			break;


		case 4:
			// lid closed - wait
			if( (*cdr_command) == CdlGetlocP ) {
				memset( output, 0, 16 );
				output_state[0] = 16;
				output_state[1] = 0;


				// default status
				output[0] = (*cdr_reading);
				output[0] |= (*cdr_play);
			}


			// check valid disk error cmds
			if( (*cdr_command) != CdlNop &&
					(*cdr_command) != CdlMute &&
					(*cdr_command) != CdlDemute &&
					(*cdr_command) != CdlSetmode &&
					(*cdr_command) != CdlGetmode &&
					(*cdr_command) != CdlSetfilter ) {
				(*cdr_irq) = CDRINT_DISKERROR;

				output[0] |= CDRSTATUS_ERROR;
			}
			break;


		case 5:
			// lid closed - seek
			if( (*cdr_command) == CdlGetlocP ) {
				memset( output, 0, 16 );
				output_state[0] = 16;
				output_state[1] = 0;


				// default status
				output[0] = (*cdr_reading);
				output[0] |= (*cdr_play);

				output[0] |= CDRSTATUS_SEEK;
				output[0] |= CDRSTATUS_ROTATING;
			}
			break;


		case 6:
			// lid closed - done
			if( (*cdr_command) == CdlGetlocP ) {
				memset( output, 0, 16 );
				output_state[0] = 16;
				output_state[1] = 0;


				// default status
				output[0] = (*cdr_reading);
				output[0] |= (*cdr_play);

				output[0] |= CDRSTATUS_ROTATING;
			}
			break;


		default:
			break;
	}


	if( lid_state >= 2 ) {
		memcpy( output_temp, output, 16 );
	}
}


void __declspec(naked) cdrom_lid_output_170()
{
	_asm {
		pushad
		pushfd

		call cdrom_lid_output_core_170

		popfd
		popad



		; old code
		or dword ptr ds:0x510bc0,4

		push dword ptr 0x42a057
		ret
	}
}



void __declspec(naked) gettn_fix_170()
{
	_asm {
		mov byte ptr ds:0x50f9ca,0


		; old code
		call dword ptr ds:0x4c4af8

		push dword ptr 0x42c808
		ret
	}
}


void __declspec(naked) memcard_swap_170()
{
	__asm {
		pushad
		pushfd

		call Restart_Memcards

		popfd
		popad


		; old code
		push dword ptr 0x400

		push dword ptr 0x409e03
		ret
	}
}


void __declspec(naked) memcard_eject_170()
{
	__asm {
		; old code
		mov byte ptr ds:0x5078a2,al


		pushad
		pushfd

		push dword ptr 0x5078a2
		call Reset_Memcards

		popfd
		popad


		push dword ptr 0x43b502
		ret
	}
}



void __declspec(naked) reset_emu_170()
{
	__asm {
		mov reset_spu,1
		mov memcard_reset,0


		; old code
		cmp dword ptr ds:0x510b5c,0

		push dword ptr 0x40c202
		ret
	}
}


void __stdcall cd_name_core_170()
{
	static u8 *cd_name = (u8 *) 0x4c5d88;
	static int lcv;


	// BOOT = cdrom:\<name>.<ext>;1
	// - <name> = 8-chars fixed
	// - <ext> = 2-chars fixed
	lcv = 0;
	while( cd_name[lcv] != '.' && cd_name[lcv] != ';' ) lcv++;


	if( cd_name[lcv] == '.' ) {
		// cdro(m:\)ARCDX.EX(E;1)
		cd_name[ lcv + 3 ] = 0;
	} else if( cd_name[lcv] == ';' ) {
		// cdr(om:\BOOT);(1)
		cd_name[ lcv + 0 ] = '.';
		cd_name[ lcv + 2 ] = 0;
	}


	strcpy_s( temp_str, (const char *)(cd_name + lcv - 8) );

	// chop directory names (POLICE\NAUTS\SLPS_000.00;1)
	// - skip '(\)PSXMAIN.EX'
	lcv = 1;
	while( lcv < 8 ) {
		if( temp_str[lcv] == '\\' ) temp_str[lcv] = '_';
		if( temp_str[lcv] == '/' ) temp_str[lcv] = '_';
		if( temp_str[lcv] == ':' ) temp_str[lcv] = '_';
		if( temp_str[lcv] == ';' ) temp_str[lcv] = '_';

		lcv++;
	}


	name_ptr = temp_str;
}


void __declspec(naked) cd_name_170()
{
	__asm {
		pushfd
		pushad

		call cd_name_core_170

		popad
		popfd



		; fixed exe namepos
		push dword ptr name_ptr


		push dword ptr 0x437471
		ret
	}
}


void __declspec(naked) gettd_iso_track0_170()
{
	__asm {
		cmp al,0
		jnz Track01


		; Track 00 - cd size
		mov ecx, dword ptr 0x5084b0[ecx * 8]

		; + 0 sec (no msf)
		add ecx,0x00
		jmp Done


Track01:
		; Track 01-99
		mov ecx, dword ptr 0x5084ac[ecx * 8]

		; + 2 sec (msf)
		add ecx,0x96


Done:
		push dword ptr 0x43833e
		ret
	}
}



void __declspec(naked) gettd_iso_trackN_170()
{
	__asm {
		; set Track XX (over) -> Track 01
		mov al,1


		; run normal code
		push dword ptr 0x43832e
		ret
	}
}



void __declspec(naked) joypad_dualshock_keys_170()
{
	__asm {
		; note:
		; eax = 00, 20, 40, 60, 80, a0, c0, e0
		; ebx = 00, 10, 20, 30, 40, 50, 60, 70
		; 4bdcb0 / b4 / b8 / bc / c0 / c4 / c8 / cc


		push ebx
		shr ebx,4
		shl ebx,2
		and ebx,0xff


		; ignore non-dualshock (analog) controllers
		cmp byte ptr 0x4bdcb0[ebx],3
		jz dualshock_run

		cmp byte ptr 0x4bdcb0[ebx],4
		jz dualshock_run



		pop ebx

		; non-dualshock = no L3/R3
		push dword ptr 0x4302aa
		ret



dualshock_run:
		pop ebx


		; run normal code
		movzx edx, word ptr 0x4bdb8c[eax]

		push dword ptr 0x430275
		ret
	}
}



void __declspec(naked) port2_mouse_170()
{
	__asm {
		; check pad1 / pad5
		cmp eax,0
		jz mouse_valid

		cmp eax,4
		jz mouse_valid



		; bad mouse - no data
		sub eax,1


		push dword ptr 0x42d75d
		ret



mouse_valid:
		push dword ptr 0x42d77d
		ret
	}
}



void __stdcall autostart_controllers_170()
{
	u32 *pad1_ptr = (u32 *) 0x4bdcb0;
	u32 *pad5_ptr = (u32 *) 0x4bdcc0;
	u32 *pad1_toggle = (u32 *) 0x4c4c60;
	u32 *pad5_toggle = (u32 *) 0x4c4c70;
	u32 *special_flag = (u32 *) 0x4c4c5c;


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
}



void __stdcall konami_justifiers_170()
{
	u32 *pad1_ptr = (u32 *) 0x4bdcb0;
	u32 *pad5_ptr = (u32 *) 0x4bdcc0;
	u32 *pad1_toggle = (u32 *) 0x4c4c60;
	u32 *pad5_toggle = (u32 *) 0x4c4c70;
	u32 *special_flag = (u32 *) 0x4c4c5c;


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
	if( konami_justifier1 == 1 ) {
		(*pad1_ptr) = 8;
		(*special_flag) = 1;
	}


	// turn on pad5
	if( konami_justifier2 == 1 ) {
		(*pad5_ptr) = 8;
		(*special_flag) = 1;
	}
}



void __declspec(naked) cpu_reset_170()
{
	__asm {
		pushad
		pushfd

		mov tomb1_last_play,0
		mov tomb1_play_count,0

		call konami_justifiers_170
		call autostart_controllers_170

		; load redump sbi file
		call opensbifile_wrapper

		popfd
		popad



		; OLD code
		mov dword ptr ds:0x510a48,eax

		push dword ptr 0x42a7d5
		ret
	}
}



void __declspec(naked) port2_justifier_170()
{
	__asm {
		; OLD code
		movzx eax, [esp+4]


		; check port2 (pad5)
		cmp eax,4
		jnz done

		; set to correct value (4 -> 0)
		mov eax,0



done:
		push dword ptr 0x42d7d5
		ret
	}
}



void __declspec(naked) sio_cmd45_dualshock_led_170()
{
	__asm {
		; check dualshock / dualanalog pads
		cmp dword ptr ds:[0x4bdcb0+eax*4],4
		jz check_led

		cmp dword ptr ds:[0x4bdcb0+eax*4],5
		jz check_led


		; OLD code - skip
		push dword ptr 0x43b34c
		ret



check_led:
		; now check dualshock - analog on
		cmp dword ptr ds:[0x4c4c60+eax*4],1
		jz dualshock_led


		; OLD code - skip
		push dword ptr 0x43b34c
		ret



dualshock_led:
		; OLD code
		push dword ptr 0x43b163
		ret
	}
}



void __declspec(naked) analog_deadzone_red1_170()
{
	__asm {
		; OLD code
		mov func_ptr, 0x42d090
		call func_ptr


		; save output addr
		mov eax, dword ptr [esp+4]


		pushad
		pushfd

		; now add deadzone
		push eax
		call analog_deadzone

		popfd
		popad



		; OLD code
		push dword ptr 0x43a09b+5
		ret
	}
}



void __declspec(naked) analog_deadzone_red2_170()
{
	__asm {
		; OLD code
		mov func_ptr, 0x42d090
		call func_ptr


		; save output addr
		mov eax, dword ptr [esp+4]


		pushad
		pushfd

		; now add deadzone
		push eax
		call analog_deadzone

		popfd
		popad



		; OLD code
		push dword ptr 0x43a592+5
		ret
	}
}



void __declspec(naked) analog_deadzone_green1_170()
{
	__asm {
		; OLD code
		mov func_ptr, 0x42d3e0
		call func_ptr


		; save output addr
		mov eax, dword ptr [esp+4]


		pushad
		pushfd

		; now add deadzone
		push eax
		call analog_deadzone

		popfd
		popad



		; OLD code
		push dword ptr 0x43a0a2+5
		ret
	}
}



void __declspec(naked) analog_deadzone_green2_170()
{
	__asm {
		; OLD code
		mov func_ptr, 0x42d3e0
		call func_ptr


		; save output addr
		mov eax, dword ptr [esp+4]


		pushad
		pushfd

		; now add deadzone
		push eax
		call analog_deadzone

		popfd
		popad



		; OLD code
		push dword ptr 0x43a5a1+5
		ret
	}
}



void __declspec(naked) mouse_sensitivity_170()
{
	__asm {
		pushad
		pushfd

		call mouse_sensitivity

		popfd
		popad



		; OLD code
		mov eax, ds:[0x4c0930]


		push dword ptr 0x412B56
		ret
	}
}



void __declspec(naked) reload_ini_170()
{
	__asm {
		pushad
		pushfd

		call Load_INI

		popfd
		popad



		; OLD code
		cmp ds:[0x510b5c],0


		push dword ptr 0x40c1e6
		ret
	}
}



void __declspec(naked) hwnd_170()
{
	__asm {
		; store hwnd internally
		mov ePSXe_hwnd,eax


		pushad
		pushfd

		call Wait_Shark

		popfd
		popad

		
		
		; OLD code
		mov ds:[0x4bf6e8],eax


		push dword ptr 0x40d024
		ret
	}
}



void __declspec(naked) force_shark_170()
{
	__asm {
		pushad
		pushfd

		call Wait_Shark

		popfd
		popad





		; OLD code
		call ds:0x44d248


		push dword ptr 0x42dd08
		ret
	}
}



void __declspec(naked) negcon_read_170()
{
	__asm {
		; save output buffer
		mov temp_var1, ecx


		; OLD code
		; - read controller data
		mov func_ptr,0x42cd70
		call func_ptr




		pushad
		pushfd

		push temp_var1
		push ebx
		call Negcon_Commands

		popfd
		popad




		; OLD code
		push dword ptr 0x43a505
		ret
	}
}


void __declspec(naked) loadbin_registry_170()
{
	__asm {
		; OLD code
		mov ecx,[ebx+4]
		mov eax,ecx


		pushad
		pushfd

		; send iso namepos to registry
		push ecx
		call Write_ISO_Name

		popfd
		popad



		; OLD code
		push dword ptr 0x403903
		ret
	}
}


void __declspec(naked) guncon_sio43_170()
{
	__asm {
		; save output buffer
		mov temp_var1, eax


		; OLD code
		; - read controller data
		mov func_ptr,0x42f4f0
		call func_ptr




		pushad
		pushfd

		push temp_var1
		push ebx
		call Guncon_Commands

		popfd
		popad




		; OLD code
		push dword ptr 0x43a438
		ret
	}
}



void __declspec(naked) mousewheel_170()
{
	__asm {
		; OLD code
		mov ds:[0x4BFAC4], edx


		; load mouse clicks
		mov ecx,[esp + 0x14 - 0x08]
		mov mouse_left, ecx

		mov ecx,[esp + 0x14 - 0x07]
		mov mouse_right, ecx

		mov ecx,[esp + 0x14 - 0x06]
		mov mouse_middle, ecx


		; load mousewheel data
		mov ecx,[esp + 0x14 - 0x0c]
		mov mousewheel_z, ecx



		; OLD code
		push dword ptr 0x4111B5
		ret
	}
}



void __declspec(naked) dualshock_mouse_170()
{
	__asm {
		pushad
		pushfd


		; map to analog
		call dualshock_analog_mouse

		popfd
		popad



		; OLD code
		mov byte ptr ds:[eax+1], 0x73
		mov byte ptr ds:[eax+2], 0x5a

		push dword ptr 0x42d0b4
		ret
	}
}



void __declspec(naked) dualshock_mouse_poll_170()
{
	__asm {
		; get mouse data
		mov func_ptr, 0x4110d0
		call func_ptr


		
		pushad
		pushfd

		call dualshock_mouse_poll

		popfd
		popad



		; OLD code
		push dword ptr 0x412b51
		ret
	}
}



void __declspec(naked) sio_multitap_no_controller_170()
{
	__asm {
		; bad ID = $ff / $ff (Frogger 2)
		mov byte ptr ds:[edi],0xff
		mov byte ptr ds:[edi+1],0xff


		; OLD code
		mov byte ptr ds:[0x4c6da4+eax],cl
		mov al,0xff

		
		push dword ptr 0x439fd5
		ret
	}
}



void __declspec(naked) sio_multitap_45_170()
{
	__asm {
		; not needed - pop ecx, ebx coming up
		;pushad
		;pushfd


		; rampage 2 - 45/47/4c

		; check sio command
		mov ebx, [esp+0x14+0x04]
		mov cl,0xf3
		
		cmp bl,0x45
		jnz sio_47

		mov byte ptr ds:[0x507be2+1+eax+ebp],cl

		; ID + $5a + [$ff + $ff]
		mov byte ptr ds:[0x507be2+3+eax+ebp],0xff
		mov byte ptr ds:[0x507be2+4+eax+ebp],0xff
		jmp sio_done


sio_47:
		cmp bl,0x47
		jnz sio_4c

		mov byte ptr ds:[0x507be2+1+eax+ebp],cl
		jmp sio_done

		; testing only - not needed yet
		mov byte ptr ds:[0x507be2+2+eax+ebp],0x5a
		mov byte ptr ds:[0x507be2+3+eax+ebp],0x00
		mov byte ptr ds:[0x507be2+4+eax+ebp],0x00
		mov byte ptr ds:[0x507be2+5+eax+ebp],0x02
		mov byte ptr ds:[0x507be2+6+eax+ebp],0x00
		mov byte ptr ds:[0x507be2+7+eax+ebp],0x01
		mov byte ptr ds:[0x507be2+8+eax+ebp],0x00

		jmp sio_done


sio_4c:
		cmp bl,0x4c
		jnz sio_done

		mov byte ptr ds:[0x507be2+1+eax+ebp],cl



sio_done:
		;popfd
		;popad




		; OLD code
		mov al, byte ptr ds:[0x507be2+1+eax+ebp]


		push dword ptr 0x43a7b5
		ret
	}
}


void __declspec(naked) negcon_test_170()
{
	__asm {
		; setup test values

		; I analog
		mov ecx,-0x20
		mov word ptr ds:[0x4c4cf0],cx

		; II analog
		mov ecx,0x30
		mov word ptr ds:[0x4c4cd0],cx

		; L analog
		mov ecx,-0x80
		mov word ptr ds:[0x4c4cb0],cx

		; steer [+ $80]
		mov ecx,0x40
		mov word ptr ds:[0x4c4c90],cx



		; OLD code
		movzx ecx, byte ptr ds:[0x4c4c90]


		push dword ptr 0x42cdb7
		ret
	}
}



void __declspec(naked) negcon_clamp_pad1_170()
{
	__asm {
		; twist data
		movsx edx, word ptr ds:[0x4c4c90]
		add edx,0x80

		pushad
		pushfd

		push edx
		call sio_negcon_clamp_twist

		popfd
		popad

		mov edx,temp_var2
		mov [eax+5],dl
		


		; fix - flip I --> II
		movsx edx, word ptr ds:[0x4c4cf0]

		pushad
		pushfd

		push edx
		call sio_negcon_clamp_I

		popfd
		popad

		mov edx,temp_var2
		mov [eax+6],dl



		; fix - flip II --> I
		movsx edx, word ptr ds:[0x4c4cd0]

		pushad
		pushfd

		push edx
		call sio_negcon_clamp_II

		popfd
		popad

		mov edx,temp_var2
		mov [eax+7],dl




		; clamp L
		movsx edx, word ptr ds:[0x4c4cb0]

		pushad
		pushfd

		push edx
		call sio_negcon_clamp_L

		popfd
		popad

		mov edx,temp_var2
		mov [eax+8],dl




		; OLD code
		push dword ptr 0x42cddb
		ret
	}
}



void __declspec(naked) sio_memcard_none()
{
	__asm {
		; OLD code
		mov byte ptr ds:0x507be1,2



		pushad
		pushfd

		call Disable_Memcard

		popfd
		popad



		cmp byte ptr ds:[0x507be1],2
		jz DONE


		; disable memcard
		dec byte ptr ds:0x507be2[esi]



DONE:
		; OLD code
		push dword ptr 0x439B95
		ret
	}
}



void __declspec(naked) cdrom_sbi_1()
{
	__asm {
		; OLD code
		call ds:0x4c4b18


		pushad
		pushfd

		push dword ptr 0x50f900
		call checkSBI

		popfd
		popad



		; OLD code
		push dword ptr 0x42C6C7
		ret
	}
}



void __declspec(naked) cdrom_sbi_2()
{
	__asm {
		; OLD code
		call ds:0x4c4b18


		pushad
		pushfd

		push dword ptr 0x50f983
		call checkSBI

		popfd
		popad



		; OLD code
		push dword ptr 0x42C71E
		ret
	}
}



void __declspec(naked) cdrom_sbi_3()
{
	__asm {
		; OLD code
		call ds:0x4c4b18


		pushad
		pushfd

		push dword ptr 0x5109ec
		call checkSBI

		popfd
		popad



		; OLD code
		push dword ptr 0x42C764
		ret
	}
}



void __declspec(naked) guncon_cmd_response_170()
{
	__asm {
		; OLD code

		; $63 = 1 [+2] [+6]
		mov dword ptr [ecx], 9


		pushad
		pushfd

		push ecx
		call Guncon_Cmd_Response

		popfd
		popad




		; OLD code
		push dword ptr 0x43a44c
		ret
	}
}



void __declspec(naked) mouse_cmd_response_170()
{
	__asm {
		; OLD code

		; $12 = 1 [+2] [+4]
		mov dword ptr [edx], 7


		pushad
		pushfd

		push ebx
		push edx
		call Mouse_Cmd_Response

		popfd
		popad




		; OLD code
		push dword ptr 0x43a3d2
		ret
	}
}



void __declspec(naked) multitap_cmd_response_170()
{
	__asm {
		; OLD code - ID
		mov byte ptr [esp+0x20-0x14+1],0xf3


		; Frogger 2 - use ID + invalid cmd (41 / ff)
		mov byte ptr [esp+0x20-0x14+1],0x41
		mov byte ptr [esp+0x20-0x14+2],0xff


		; OLD code
		push dword ptr 0x43A01B
		ret
	}
}



void __declspec(naked) memory_read_penalty_dcache_1_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and eax,0x0fff

		push dword ptr 0x419d1b
		ret
	}
}



void __declspec(naked) memory_read_penalty_dcache_2_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and eax,0x0fff

		push dword ptr 0x419d6b
		ret
	}
}



void __declspec(naked) memory_read_penalty_dcache_3_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and eax,0x0fff

		push dword ptr 0x419dcb
		ret
	}
}



void __declspec(naked) memory_write_penalty_dcache_1_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and eax,0x0fff

		push dword ptr 0x419e1f
		ret
	}
}



void __declspec(naked) memory_write_penalty_dcache_2_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and eax,0x0fff

		push dword ptr 0x419f00
		ret
	}
}



void __declspec(naked) memory_write_penalty_dcache_3_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and eax,0x0fff

		push dword ptr 0x419fdf
		ret
	}
}



void __declspec(naked) memory_penalty_dcache_1_170()
{
	__asm {
		; undo I/O cycle penalty - 1 cycle
		add dword ptr ds:[0x510a50],3



		; OLD code
		and ecx,0x0fff

		push dword ptr 0x41a219
		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_byte_170()
{
	__asm {
		pushad
		pushfd


		; cycle counter
		mov temp_var3, edi


		; address
		mov temp_var2, eax
	}


	// D-cache = 1-cycle (4 -> 1)
	if( temp_var2 >= 0x1f800000 && temp_var2 < 0x1f800400 )
		temp_var3 += 3;

	// BIOS = 8-cycles (4 -> 8)
	if( temp_var2 >= 0xbfc00000 && temp_var2 < 0xbfc80000 )
		temp_var3 -= 4;



	__asm {
		popfd
		popad


		; update cycle count
		mov edi,temp_var3


		; OLD code
		and edx,0x1fc00000
		cmp edx,0x1f800000



		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_word_170()
{
	__asm {
		pushad
		pushfd


		; cycle counter
		mov temp_var3, edi


		; address
		mov temp_var2, eax
	}


	// D-cache = 1-cycle (4 -> 1)
	if( temp_var2 >= 0x1f800000 && temp_var2 < 0x1f800400 )
		temp_var3 += 3;

	// BIOS = 8-cycles (4 -> 8 * 2)
	if( temp_var2 >= 0xbfc00000 && temp_var2 < 0xbfc80000 )
		temp_var3 -= 12;



	__asm {
		popfd
		popad


		; update cycle count
		mov edi,temp_var3


		; OLD code
		and edx,0x1fc00000
		cmp edx,0x1f800000



		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dword_170()
{
	__asm {
		pushad
		pushfd


		; cycle counter
		mov temp_var3, edi


		; address
		mov temp_var2, eax
	}


	// D-cache = 1-cycle (4 -> 1)
	if( temp_var2 >= 0x1f800000 && temp_var2 < 0x1f800400 )
		temp_var3 += 3;

	// BIOS = 8-cycles (4 -> 8 * 4)
	if( temp_var2 >= 0xbfc00000 && temp_var2 < 0xbfc80000 )
		temp_var3 -= 28;



	__asm {
		popfd
		popad


		; update cycle count
		mov edi,temp_var3


		; OLD code
		and edx,0x1fc00000
		cmp edx,0x1f800000



		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dcache_1_170()
{
	__asm {
		; patch dynarec code
		; - mov ecx, (*dynarec_mem_func)
		; - call ecx
		mov dword ptr [ecx-8],0x90909090
		mov dword ptr [ecx-4],0x90909090
		mov dword ptr [ecx],0x90909090




		mov temp_var3, eax
		mov eax, offset dynarec_memory_penalty_byte_170

		mov byte ptr [ecx-8],0xb9
		mov dword ptr [ecx-7], eax
		mov word ptr [ecx-3],0xd1ff

		mov eax,temp_var3




		; OLD code
		push dword ptr 0x426bad
		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dcache_2_170()
{
	__asm {
		; patch dynarec code
		; - mov ecx, (*dynarec_mem_func)
		; - call ecx
		mov dword ptr [ecx-8],0x90909090
		mov dword ptr [ecx-4],0x90909090
		mov dword ptr [ecx],0x90909090




		mov temp_var3, eax
		mov eax, offset dynarec_memory_penalty_word_170

		mov byte ptr [ecx-8],0xb9
		mov dword ptr [ecx-7], eax
		mov word ptr [ecx-3],0xd1ff

		mov eax,temp_var3




		; OLD code
		push dword ptr 0x426e9e
		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dcache_3_170()
{
	__asm {
		mov dword ptr [ecx], 0x1f800000



		; patch dynarec code
		; - mov ecx, (*dynarec_mem_func)
		; - call ecx
		mov dword ptr [ecx-8],0x90909090
		mov dword ptr [ecx-4],0x90909090
		mov dword ptr [ecx],0x90909090




		mov temp_var3, eax
		mov eax, offset dynarec_memory_penalty_dword_170

		mov byte ptr [ecx-8],0xb9
		mov dword ptr [ecx-7], eax
		mov word ptr [ecx-3],0xd1ff

		mov eax,temp_var3




		; OLD code
		push dword ptr 0x42718f
		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dcache_4_170()
{
	__asm {
		; patch dynarec code
		; - mov ecx, (*dynarec_mem_func)
		; - call ecx
		mov dword ptr [ecx-8],0x90909090
		mov dword ptr [ecx-4],0x90909090
		mov dword ptr [ecx],0x90909090




		mov temp_var3, eax
		mov eax, offset dynarec_memory_penalty_byte_170

		mov byte ptr [ecx-8],0xb9
		mov dword ptr [ecx-7], eax
		mov word ptr [ecx-3],0xd1ff

		mov eax,temp_var3




		; OLD code
		push dword ptr 0x427480
		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dcache_5_170()
{
	__asm {
		; patch dynarec code
		; - mov ecx, (*dynarec_mem_func)
		; - call ecx
		mov dword ptr [ecx-8],0x90909090
		mov dword ptr [ecx-4],0x90909090
		mov dword ptr [ecx],0x90909090




		mov temp_var3, eax
		mov eax, offset dynarec_memory_penalty_word_170

		mov byte ptr [ecx-8],0xb9
		mov dword ptr [ecx-7], eax
		mov word ptr [ecx-3],0xd1ff

		mov eax,temp_var3




		; OLD code
		push dword ptr 0x427A76
		ret
	}
}



void __declspec(naked) dynarec_memory_penalty_dcache_6_170()
{
	__asm {
		; patch dynarec code
		; - mov ecx, (*dynarec_mem_func)
		; - call ecx
		mov dword ptr [ecx-8],0x90909090
		mov dword ptr [ecx-4],0x90909090
		mov dword ptr [ecx],0x90909090




		mov temp_var3, eax
		mov eax, offset dynarec_memory_penalty_dword_170

		mov byte ptr [ecx-8],0xb9
		mov dword ptr [ecx-7], eax
		mov word ptr [ecx-3],0xd1ff

		mov eax,temp_var3




		; OLD code
		push dword ptr 0x428080
		ret
	}
}



void __declspec(naked) dma_irq_mdecout_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aab4]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		mov eax, dword ptr ds:[0x4c4b6c]
		add word ptr ds:[0x4c4b6a],0xffff


		push dword ptr 0x428702
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aab4],0xfeffffff

		push dword ptr 0x4286eb
		ret
	}
}



void __declspec(naked) dma_irq_spu_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aad8]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		mov byte ptr ds:[0x510e16],0xff

		push dword ptr 0x40d81e
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aad8],0xfeffffff

		push dword ptr 0x40d80d
		ret
	}
}



void __declspec(naked) dma_irq_spu2_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aad8]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x40f0cf
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aad8],0xfeffffff

		mov func_ptr, 0x42a070
		push 4
		call func_ptr
		add esp,4
		
		push dword ptr 0x40f0cf
		ret
	}
}



void __declspec(naked) dma_irq_cdrom_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aacc]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		mov byte ptr ds:[0x5109e4],1

		push dword ptr 0x42b827
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aacc],0xfeffffff

		mov func_ptr, 0x42a070
		push 3
		call func_ptr
		add esp,4
		
		push dword ptr 0x42b827
		ret
	}
}



void __declspec(naked) dma_irq_gpu_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aac0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x42e848
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aac0],0xfeffffff

		push dword ptr 0x42e83e
		ret
	}
}



void __declspec(naked) dma_irq_gpu2_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aac0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x42e8b5
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aac0],0xfeffffff

		push dword ptr 0x42e8a1
		ret
	}
}



void __declspec(naked) dma_irq_gpu3_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aac0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x42e9a9
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aac0],0xfeffffff

		push dword ptr 0x42e99f
		ret
	}
}



void __declspec(naked) dma_irq_gpu4_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aac0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x42e9f9
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aac0],0xfeffffff

		push dword ptr 0x42e9ef
		ret
	}
}



void __declspec(naked) dma_irq_gpu5_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aac0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x42eac4
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aac0],0xfeffffff

		mov func_ptr, 0x42a070
		push 2
		call func_ptr
		add esp,4

	
		push dword ptr 0x42eac4
		ret
	}
}



void __declspec(naked) dma_irq_gpuotc_170()
{
	__asm {
		; check dma pin on
		mov eax, dword ptr ds:[0x81aaf0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		mov byte ptr ds:[0x510e15],0xff

		push dword ptr 0x42eb0c
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aaf0],0xfeffffff
	
		push dword ptr 0x42eafd
		ret
	}
}



void __declspec(naked) dma_irq_gpuotc2_170()
{
	__asm {
		; OLD code
		and ecx, 0xfeffffff
		and dword ptr ds:[0x81aaf0],0xfeffffff



		; check dma pin on
		mov eax, dword ptr ds:[0x81aaf0]
		test eax, 0x01000000
		jnz dma_on



		; skip interrupt
		push dword ptr 0x42ebe0
		ret



dma_on:
		; OLD code
		and dword ptr ds:[0x81aaf0],0xfeffffff
	
		push dword ptr 0x42ebd0
		ret
	}
}



void __declspec(naked) MMIO_read_170()
{
	_asm {
		// addr
		mov temp_var1, eax

		
		// OLD code
		lea ecx,[eax-0x1f801c00]


		// get rt
		mov func_ptr, 0x41ff2a
		call func_ptr



		pushad
		pushfd

		push eax
		push temp_var1
		call MMIO_Read

		popfd
		popad



		// OLD code
		ret
	}
}



void __declspec(naked) Cdrom_getlocp_track_170()
{
	__asm {
		pushad
		pushfd

		movzx eax, byte ptr ds:[ebp+0]
		mov temp_var1, eax
	}


	// track # = BCD format
	temp_var1 = itob( temp_var1 );


	__asm {
		mov eax,temp_var1
		mov byte ptr ds:[ebp+0], al

		popfd
		popad



		// OLD code
		shl al,4
		add al,dl

		push dword ptr 0x438be3
		ret
	}
}



void __declspec(naked) Cdrom_pause_short_170()
{
	__asm {
		// setup irq timer - ack + complete
		mov ebx, 2
		mov word ptr ds:[0x45f114], bx

		mov ebx, 0x80
		mov word ptr ds:[0x45f118], bx


		// OLD code
		mov byte ptr ds:[0x45f112],al

		push dword ptr 0x42c2a6
		ret
	}
}



void __declspec(naked) cdrom_repplay_flag_170()
{
	__asm {
		// check stop flag ($04)
		test byte ptr ds:[0x50f9cf],4
		jnz stop_flag


		// turn off DataReady IRQ (no repplay data)
		mov byte ptr ds:[0x50F9C5],0
		mov byte ptr ds:[0x50F940],0
		mov byte ptr ds:[0x50F941],0
		mov byte ptr ds:[0x50F942],0



stop_flag:
		// OLD code
		test byte ptr ds:[0x50f9cf],2

		push dword ptr 0x42b500
		ret
	}
}



void __declspec(naked) mouse_wheel_converter_170()
{
	__asm {
		// add mouse flags
		// - middle = $80
		// - wheel up = $40
		// - wheel down = $20

		test byte ptr ds:mouse_middle, 0x80
		jz mousewheel
		and byte ptr [eax+4],0x7f


mousewheel:
		cmp mousewheel_z,0
		je mouse_usb_done
		jl mousewheel_down


		; mouse_z [+]
		and byte ptr [eax+4],0xbf
		jmp mouse_usb_done


mousewheel_down:
		; mouse_z [-]
		and byte ptr [eax+4],0xdf




mouse_usb_done:
		// OLD code
		mov cl, byte ptr ds:[0x4C4C80]


		push dword ptr 0x42D7B4
		ret
	}
}



void __declspec(naked) pad_startpoll_170()
{
	__asm {
		pushad
		pushfd

		push dword ptr ds:[0x507be0]
		call PADStartPoll_func

		popfd
		popad



		// OLD code
		mov byte ptr ds:[0x507be1],1

		push dword ptr 0x439be5
		ret
	}
}



void __declspec(naked) pad_poll_pad1_170()
{
	__asm {
		// OLD code
		mov temp_var1,eax
		mov temp_var2,ecx

		mov func_ptr,0x43a2c0
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

		push dword ptr 0x43b401
		ret
	}
}



void __declspec(naked) pad_poll_pad2_170()
{
	__asm {
		// OLD code
		mov temp_var1,eax
		mov temp_var2,ecx

		mov func_ptr,0x43a2c0
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

		push dword ptr 0x43b421
		ret
	}
}



void _declspec(naked) GPUinit_Wrapper_170()
{
	_asm {
		mov GPUinit_func, eax
		mov eax, offset GPUinit



		; OLD code
		mov ds:[0x50f0bc], eax

		; JMP $0042de6b
		push 0x42de6b
		ret
	}
}



void _declspec(naked) GPUclose_Wrapper_170()
{
	_asm {
		mov GPUclose_func, eax
		mov eax, offset GPUclose



		; OLD code
		mov ds:[0x50f080], eax

		; JMP $0042dee5
		push 0x42dee5
		ret
	}
}



void __declspec(naked) GPUvisualVibration_Wrapper_170()
{
	_asm {
		mov GPUvisualVibration_func, eax



		; OLD code
		mov ds:[0x50ee88], eax

		; JMP $0042e192
		push 0x42e192
		ret
	}
}



void __declspec(naked) GPUcursor_Wrapper_170()
{
	_asm {
		mov GPUcursor_func, eax



		; OLD code
		mov ds:[0x50f060], eax

		; JMP $0042e15a
		push 0x42e15a
		ret
	}
}



void __declspec(naked) CDDA_tomb_raider1_cdda_resume_hack_170()
{
	__asm {
		pushad
		pushfd

		call Tomb_Raider_1_cdda_resume_hack

		popfd
		popad


		cmp temp_var1,0
		jnz no_stop



		mov byte ptr ds:[0x50f9c5],4

		push 0x42b532
		ret


no_stop:
		; HACK code - don't stop cdda for autopause
		push 0x42b578
		ret
	}
}



void __declspec(naked) cdrom_plugin_cdda_stop_170()
{
	__asm {
		pushad
		pushfd
		
		call cdrom_plugin_cdda_stop

		popfd
		popad


		// eat stack bytes
		pop ecx
		pop ecx
		pop ecx
		pop ecx


		// OLD code
		push 0x431581
		ret
	}
}



void __declspec(naked) cdrom_cdda_load_170()
{
	__asm {
		// check if cdrReadCDDA exists
		mov esi, dword ptr ds:[0x50E484]
		test esi,esi
		jz normal_cdrplay


		// then skip CDRPlay (don't mix)
		mov esi,0
		jmp done



normal_cdrplay:
		mov esi, dword ptr ds:[0x50e470]


done:
		// OLD code
		push 0x431158
		ret
	}
}



void __declspec(naked) cdrom_cdda_repplay_stop_over_170()
{
	__asm {
		pushad
		pushfd

		call cdda_repplay_overflow

		popfd
		popad


		cmp temp_var1,0
		jz done


		// overflow - stop cdda now
		push 0x42b52b
		ret


done:
		// OLD code
		mov dl,0x5109fa

		push 0x42b508
		ret
	}
}



void __declspec(naked) repplay_track_cdrom_plugin_170()
{
	__asm {
		// if( *( (int *) 0x50E488 ) == 0 ) {
		cmp dword ptr ds:[0x50e488],0
		jz DONE


		// fix cd plugin track # = (1 -> 2 audio)
		add edi,1


DONE:
		// OLD code
		mov eax,edi
		cdq
		mov ecx,0x0a


		push 0x42b38a
		ret
	}
}



void __declspec(naked) setloc_repplay_fix1_170()
{
	__asm {
		pushad
		pushfd
	}

	setloc_cdda_frames = *((unsigned char *) 0x50f9d2);
	setloc_cdda_noseek = 1;

	__asm {
		popfd
		popad



		// OLD code
		mov word ptr ds:[0x45f114],0x0f

		push 0x42be4c
		ret
	}
}



void __declspec(naked) setloc_repplay_fix2_170()
{
	__asm {
		pushad
		pushfd
	}

	if( setloc_cdda_noseek == 1 ) {
		*((unsigned char *) 0x50f9d2) = setloc_cdda_frames;
		setloc_cdda_noseek = 0;
	}

	__asm {
		popfd
		popad



		// OLD code
		movzx edx, byte ptr ds:[0x50f9d2]

		push 0x42b362
		ret
	}
}
