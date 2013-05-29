void __stdcall Opcode_Tracer_Run_170()
{
	unsigned int addr;
	//unsigned int temp1, temp2;


	if( !fp_log_trace )
	{
		fopen_s(&fp_log_trace, "trace_opcode.txt", "w" );
	}



	// PC counter
	__asm
	{
		push eax

		mov eax, ds:[0x510a40]
		mov addr, eax



		;mov eax, ds:[0x94C020 + 0xbe4b0]
		;mov temp1, eax

		;mov eax, ds:[0x94C020 + 0xbeb98]
		;mov temp2, eax

		pop eax
	}



	//if( addr == 0x8001d738 )
	if( opcode_tracer == 2 && addr == opcode_addr )
	{
		start_tracer = 1;
	}


	// freeze program
	if( opcode_tracer == 3 && addr == opcode_addr )
	{
		__asm
		{
			push eax

			mov eax, ds:[0x510a40]
			sub eax, 4
			mov dword ptr ds:[0x510a40], eax

			pop eax
		}
	}


/*
	if( (temp1 & 0xffff) == 0x00ff )
	{
		start_tracer = 1;

		if( cpu_track[0] == 0 )
		{
			fprintf( fp_log_trace, "TRACK!\n" );

			cpu_track[0] = 1;
			memset( mem_trace, 0, sizeof(mem_trace) );
		}
	}

	if( (temp1 & 0xffff) != 0x00ff )
	{
		if( cpu_track[0] == 1 )
		{
			cpu_track[0] = 0;

			fprintf( fp_log_trace, "TRACK OFF!\n" );
		}
	}



	if( (temp2 & 0xffffff) == (unsigned int) 0xbe498 )
	{
		start_tracer = 1;

		if( cpu_track[1] == 0 )
		{
			fprintf( fp_log_trace, "ADDR TRACK!\n" );

			cpu_track[1] = 1;
			//memset( mem_trace, 0, sizeof(mem_trace) );
		}
	}

	if( (temp2 & 0xffffff) != (unsigned int) 0xbe498 )
	{
		if( cpu_track[1] == 1 )
		{
			cpu_track[1] = 0;

			fprintf( fp_log_trace, "ADDR TRACK OFF!\n" );
		}
	}



	if( (temp2 & 0xffffff) == (unsigned int) 0xbe4b4 )
	{
		start_tracer = 1;

		if( cpu_track[2] == 0 )
		{
			fprintf( fp_log_trace, "ADDR2 TRACK!\n" );

			cpu_track[2] = 1;
			//memset( mem_trace, 0, sizeof(mem_trace) );
		}
	}

	if( (temp2 & 0xffffff) != (unsigned int) 0xbe4b4 )
	{
		if( cpu_track[2] == 1 )
		{
			cpu_track[2] = 0;

			fprintf( fp_log_trace, "ADDR2 TRACK OFF!\n" );
		}
	}
	*/



	if( start_tracer )
	{
		if( mem_trace[ addr & 0xffffff ] == 0 )
		{
			fprintf( fp_log_trace, "%08X\n", addr );
			mem_trace[ addr & 0xffffff ] = 1;
		}
	}
}



void __stdcall Opcode_Tracer_Run_160()
{
	unsigned int addr;
	//unsigned int temp1, temp2;


	if( !fp_log_trace )
	{
		fopen_s(&fp_log_trace, "trace_opcode.txt", "w" );
	}



	// PC counter
	__asm
	{
		push eax

		mov eax, ds:[0x50C260]
		mov addr, eax



		pop eax
	}



	if( opcode_tracer == 2 && addr == opcode_addr )
	{
		start_tracer = 1;
	}


	// freeze program
	if( opcode_tracer == 3 && addr == opcode_addr )
	{
		__asm
		{
			push eax

			mov eax, ds:[0x50C260]
			sub eax, 4
			mov dword ptr ds:[0x50C260], eax

			pop eax
		}
	}




	if( start_tracer )
	{
		if( mem_trace[ addr & 0xffffff ] == 0 )
		{
			fprintf( fp_log_trace, "%08X\n", addr );
			mem_trace[ addr & 0xffffff ] = 1;
		}
	}
}