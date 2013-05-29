extern "C" int __cdecl _fseeki64(FILE *, __int64, int);
extern "C" __int64 __cdecl _ftelli64(FILE *);


FILE *fp_cdrom = 0;


unsigned int msf2sec(char *msf) {
	return ((msf[0] * 60 + msf[1]) * 75) + msf[2];
}


void sec2msf(int s, char *msf) {
	msf[0] = (s / 75) / 60;
	s = s - msf[0] * 75 * 60;
	msf[1] = s / 75;
	s = s - msf[1] * 75;
	msf[2] = s;
}


void Create_cdda_plugin_table()
{
	char start[3];


	// init track table
	if( cdrom_cdda_plugin_table[0][0] == -1 &&
			cdrom_cdda_plugin_table[0][1] == -1 &&
			cdrom_cdda_plugin_table[0][2] == -1 )
	{
		for( unsigned char track = 0; track < 99; track++ ) {
			__asm {
				pushad
				pushfd

				lea eax,start+2
				push eax
				
				lea eax,start+1
				push eax
				
				lea eax,start+0
				push eax

				movzx eax,char ptr ds:[track]
				push eax

				// cdrGetTE
				call dword ptr ds:[0x50E488]

				popfd
				popad
			}

			cdrom_cdda_plugin_table[ track ][0] = start[0];
			cdrom_cdda_plugin_table[ track ][1] = start[1];
			cdrom_cdda_plugin_table[ track ][2] = start[2];


			// check 'no track' condition - set as track size (for internal emu table -only-)
			if( track > 0 &&
					cdrom_cdda_plugin_table[ track ][0] == 0 &&
					cdrom_cdda_plugin_table[ track ][1] == -2 &&
					cdrom_cdda_plugin_table[ track ][2] == 0 )
			{
				cdrom_cdda_plugin_table[ track ][0] = cdrom_cdda_plugin_table[0][0];
				cdrom_cdda_plugin_table[ track ][1] = cdrom_cdda_plugin_table[0][1];
				cdrom_cdda_plugin_table[ track ][2] = cdrom_cdda_plugin_table[0][2];
			}
		}
	}
}


void __stdcall cdrom_plugin_cdda_stop()
{
	unsigned char time[3];
	int play, addr_temp;

	time[0] = *((unsigned char *) 0x50f9d0);
	time[1] = *((unsigned char *) 0x50f9d1);
	time[2] = *((unsigned char *) 0x50f9d2);
	play = msf2sec( (char *) time );


	Create_cdda_plugin_table();


	for( int track = 1; track < 99; track++ ) {
		// find 1st track above play time
		addr_temp = msf2sec( (char *) cdrom_cdda_plugin_table[ track ] );
		if( play < addr_temp ) {
			track--;
			break;
		}
	}


	// now fix stop time
	sec2msf( addr_temp, (char *) time );
	*((unsigned char *) 0x5109fa) = time[0];
	*((unsigned char *) 0x5109fb) = time[1];
	*((unsigned char *) 0x5109fc) = time[2];
}



void __stdcall cdda_repplay_overflow()
{
	char time[3];
	int sec1,sec2;


	// NOTE: This fixes cd plugin overrun problems

	// play
	time[0] = *((unsigned char *) 0x50f9d0 );
	time[1] = *((unsigned char *) 0x50f9d1 );
	time[2] = *((unsigned char *) 0x50f9d2 );
	sec1 = msf2sec( (char *) time );


	// stop
	time[0] = *((unsigned char *) 0x5109fa );
	time[1] = *((unsigned char *) 0x5109fb );
	time[2] = *((unsigned char *) 0x5109fc );
	sec2 = msf2sec( (char *) time );


	temp_var1 = 0;
	if( sec1 >= sec2 ) temp_var1 = 1;


	// safety check - repplay = cdda playing
	if( tomb1_play_count <= 0 ) tomb1_play_count = 1;
}



void __stdcall Tomb_Raider_1_cdda_resume_hack()
{
	static int cdda_track;
	static char time[3];
	static int sec;
	static int *addr;
	static FILE *fp;


	// stop track = normal
	temp_var1 = 0;


	if( ePSXe_version >= 170 && (tomb1_cdda_hack || tomb2_cdda_hack) ) {
		if( tomb1_cdda_table[0] == -1 ) {
			tomb1_cdda_table[0] = 0;


			if( tomb1_cdda_hack )
			{
				fopen_s(&fp, "inis\\ePSXe_shark_tomb1_tracks.ini", "r" );
				if( fp ) {
					char str[1024];
					int track_no, track_mode;

					while( !feof(fp) ) {
						fgets( str, 1024, fp );

						if( 2 == sscanf_s( str, "%d = %d", &track_no, &track_mode ) )
							tomb1_cdda_table[ track_no ] = track_mode;
					}

					fclose(fp);
				}
				else {
					// 4 - Ambient Noise
					tomb1_cdda_table[4] = 1;
				}
			}

			else if( tomb2_cdda_hack ) {
				fopen_s(&fp, "inis\\ePSXe_shark_tomb2_tracks.ini", "r" );
				if( fp ) {
					char str[1024];
					int track_no, track_mode;

					while( !feof(fp) ) {
						fgets( str, 1024, fp );

						if( 2 == sscanf_s( str, "%d = %d", &track_no, &track_mode ) )
							tomb1_cdda_table[ track_no ] = track_mode;
					}

					fclose(fp);
				}
				else {
					// music auto-loops by default
					memset( tomb1_cdda_table, 0, sizeof(tomb1_cdda_table) );


					// ambient tracks (loop)
					tomb1_cdda_table[26] = 1;
					tomb1_cdda_table[27] = 1;
					tomb1_cdda_table[28] = 1;
					tomb1_cdda_table[29] = 1;
				}
			}
		}


		time[0] = *((unsigned char *) 0x5109fa );
		time[1] = *((unsigned char *) 0x5109fb );
		time[2] = *((unsigned char *) 0x5109fc );
		sec = msf2sec( (char *) time );



		// cdda iso
		if( *( (int *) 0x50E488 ) == 0 ) {
			// find last track played (1 = cdda track #1)
			cdda_track = 0;

			// track 2 start
			addr = (int *) 0x5084b8;


			// note: we should be -at- the the track stop
			while(1) {
				// track stop (check next track start)
				if( sec == (*addr) ) {
					addr--;
					break;
				}
				else if( sec < (*addr) ) {
					addr -= 2;
					addr--;
					cdda_track--;
					break;
				}

				// invalid
				if( *addr == 0 ) break;


				// skip 2 dwords (!)
				cdda_track++;
				addr += 2;
			}



			// resume last play track
			if( tomb1_play_count >= 2 ) {
				tomb1_play_count--;


				temp_var1 = 1;


				// reset m:s:f of track
				sec2msf( tomb1_last_play[ tomb1_play_count ], (char *) time );
				*((unsigned char *) 0x50f9d0) = time[0];
				*((unsigned char *) 0x50f9d1) = time[1];
				*((unsigned char *) 0x50f9d2) = time[2];



				// find track play end
				cdda_track = 0;
				addr = (int *) 0x5084b8;


				// didn't hit track stop yet
				while(1) {
					// track stop
					if( tomb1_last_play[ tomb1_play_count ] < (*addr) ) {
						addr--;
						break;
					}

					// invalid
					if( *addr == 0 ) break;


					// skip 2 dwords (!)
					cdda_track++;
					addr += 2;
				}


				if( cdda_track <= 0 ) temp_var1 = 0;



				// reset end time
				sec2msf( (*(addr+1)), (char *) time );
				*((unsigned char *) 0x5109fa) = time[0];
				*((unsigned char *) 0x5109fb) = time[1];
				*((unsigned char *) 0x5109fc) = time[2];


				tomb1_last_play[ tomb1_play_count ] = 0;
			}
			else
			{
				switch( tomb1_cdda_table[ cdda_track ] ) {

				// stop track
				case 0:
					temp_var1 = 0;


					tomb1_last_play[ tomb1_play_count ] = 0;	
					tomb1_play_count = 0;
					break;


				// loop last track
				case 1:
					temp_var1 = 1;

					tomb1_play_count = 1;


					// reset start m:s:f of track
					sec2msf( *addr, (char *) time );
					*((unsigned char *) 0x50f9d0) = time[0];
					*((unsigned char *) 0x50f9d1) = time[1];
					*((unsigned char *) 0x50f9d2) = time[2];


					
					// reset end time
					sec2msf( (*(addr+1)), (char *) time );
					*((unsigned char *) 0x5109fa) = time[0];
					*((unsigned char *) 0x5109fb) = time[1];
					*((unsigned char *) 0x5109fc) = time[2];

					break;
				}
			}
		}
		else
		{
			int stop_addr;


			Create_cdda_plugin_table();


			// find last track played
			cdda_track = 1;

			while(1) {
				// track stop (check next track start)
				stop_addr = msf2sec( (char *) (cdrom_cdda_plugin_table[ cdda_track ]) );
				if( sec == stop_addr ) {
					cdda_track--;
					break;
				}

				else if( sec < stop_addr ) {
					cdda_track--;
					cdda_track--;
					break;
				}


				cdda_track++;
			}


			// resume last play track (max chain = 3?)
			if( tomb1_play_count >= 2 ) {
				tomb1_play_count--;


				temp_var1 = 1;


				// reset m:s:f of track
				sec2msf( tomb1_last_play[ tomb1_play_count ], (char *) time );
				*((unsigned char *) 0x50f9d0) = time[0];
				*((unsigned char *) 0x50f9d1) = time[1];
				*((unsigned char *) 0x50f9d2) = time[2];



				// find track play end
				cdda_track = 1;

				while(1) {
					// track stop (check next track start)
					stop_addr = msf2sec( (char *) (cdrom_cdda_plugin_table[ cdda_track ]) );

					// sitting at track end
					if( tomb1_last_play[ tomb1_play_count ] < stop_addr ) {
						cdda_track--;
						break;
					}

					cdda_track++;
				}


				if( cdda_track <= 0 ) temp_var1 = 0;


				// reset end time
				memcpy( time, cdrom_cdda_plugin_table[ cdda_track+1 ], 3 );
				*((unsigned char *) 0x5109fa) = time[0];
				*((unsigned char *) 0x5109fb) = time[1];
				*((unsigned char *) 0x5109fc) = time[2];
			}
			else {
				switch( tomb1_cdda_table[ cdda_track ] ) {

				// stop track
				case 0:
					temp_var1 = 0;

					tomb1_last_play[ tomb1_play_count ] = 0;	
					tomb1_play_count = 0;
					break;


				// loop last track
				case 1:
					temp_var1 = 1;

					tomb1_play_count = 1;


					// reset m:s:f of track
					memcpy( time, cdrom_cdda_plugin_table[ cdda_track ], 3 );
					*((unsigned char *) 0x50f9d0) = time[0];
					*((unsigned char *) 0x50f9d1) = time[1];
					*((unsigned char *) 0x50f9d2) = time[2];


					// reset end time (account for pregap)
					memcpy( time, cdrom_cdda_plugin_table[ cdda_track+1 ], 3 );
					*((unsigned char *) 0x5109fa) = time[0];
					*((unsigned char *) 0x5109fb) = time[1];
					*((unsigned char *) 0x5109fc) = time[2];
					break;
				}
			}
		} // end cdda iso



		// DataEnd --> DataReady (audio not done)
		if( temp_var1 == 1 )
		{
			// out[0]
			*((unsigned char *) 0x5109EC) &= ~0x10;


			/*
			NOTE:
			- ePSXe fires a repplay DataReady irq at track end
			- ePSXe then fires a DataEnd irq at track end

			#1 throws a cdrom irq pin
			#2 throws a 'fake' cdrom irq pin

			Game handles 2 'irqs' within same 'irq' period ==> abort
			- Avoid this condition to keep streaming music (hack only)
			*/

			
			// IRQ + output
			*((unsigned char *) 0x50f9c5) = 0;
			//*((unsigned char *) 0x50f9c5) = 1;

			*((unsigned char *) 0x50f940) = 8;
			*((unsigned char *) 0x50f941) = 0;
			*((unsigned char *) 0x50f942) = 1;
		}
	} // 170 hack
}



void __stdcall checkSBI( u8 *time ) {
	int lcv;

	// redump SBI files

	// both BCD format - absolute time
	for( lcv=0; lcv<sbicount; lcv++ )
	{
		if( time[5] == sbitime[lcv][0] && 
				time[6] == sbitime[lcv][1] && 
				time[7] == sbitime[lcv][2] ) {
			// LibCrypt - zap time data
			memset( time, 0, 8 );
		}
	}
}



int __stdcall opensbifile( const char *isoname ) {
	FILE *sbihandle;
	char sbiname[4096];


	// init
	sbicount = 0;


	// copy name of the iso and change extension from .img to .sbi
	strncpy_s(sbiname, isoname, sizeof(sbiname));
	sbiname[4096 - 1] = '\0';
	if (strlen(sbiname) >= 4) {
		strcpy_s((sbiname + strlen(sbiname) - 4),sizeof(sbiname)-strlen(sbiname)-4, ".sbi");
	}

	fopen_s(&sbihandle, sbiname, "rb");
	if (sbihandle == NULL)
		return 0;


	// 4-byte SBI header
	fread( sbiname, 1, 4, sbihandle );
	while( !feof(sbihandle) ) {
		u8 subq[11];
		fread( sbitime[ sbicount++ ], 1, 3, sbihandle );
		fread( subq, 1, 11, sbihandle );
	}

	return 1;
}


void __stdcall opensbifile_wrapper() {
	if( ePSXe_version == 170 ) {
		// run cdrom
		if( *((char *)0x510b5c) == 1 ) {
			opensbifile( "redump.sbi" );
		}

		// run iso
		else if( *((char *)0x510b5c) == 3 ) {
			// get last iso used
			static HKEY myKey;
			static unsigned long size, type;


			temp_str[0] = 0;
			if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\epsxe\\config",0,KEY_ALL_ACCESS,&myKey)==ERROR_SUCCESS)
			{
				size = 1024;
				RegQueryValueEx(myKey,"IsoDirectory",0,&type,(LPBYTE)&temp_str,&size);

				RegCloseKey(myKey);
			}


			if( !opensbifile( temp_str ) ) {
				opensbifile( "redump.sbi" );
			}
		}
	}
}


void __stdcall Lid_Interrupt()
{
	if( lid_timer > 0 ) {
		lid_timer--;
	}
	else if( lid_state > 0 ) {
		/*
		0 = normal
		1 = request lid open
		2 = open lid - empty cd
		3 = open lid - rotating
		4 = close lid - wait
		5 = close lid - seek
		6 = close lid - rotate
		7 = normal - return to state 0
		*/

		lid_state++;
		lid_flag = 1;


		// Metal Gear Solid - use larger time for detection
		lid_timer = 0x1000;


		if( lid_state == 7 ) lid_state = 0;
	}
}


void __cdecl Attenuation_Ctrl( int addr, int rt )
{
	// Tomb Raider 1/2 - cdda interrupt + resume behavior
	if( tomb1_cdda_hack || tomb2_cdda_hack )
	{
		if( addr == 0x1f801801 && cdxa_state == 0 )
		{
			// reset on ReadN
			if( rt == 6 )
				tomb1_play_count = 0;


			// first play
			else if( tomb1_play_count == 0 && rt == 3 )
				tomb1_play_count = 1;


			// normal stop
			else if( rt == 9 ) {
				tomb1_play_count--;
				if( tomb1_play_count < 0 ) tomb1_play_count = 0;
			}


			// cdda interrupt - allow play stack (play - setloc - seekp - play)
			else if( tomb1_play_count >= 1 && rt == 2 ) {
				unsigned char time[3];
	
				if( ePSXe_version == 170 ) {
					time[0] = *((unsigned char *) 0x50f9d0 );
					time[1] = *((unsigned char *) 0x50f9d1 );
					time[2] = *((unsigned char *) 0x50f9d2 );
				}

				if( ePSXe_version == 180 ) {
					time[0] = *((unsigned char *) 0x4F1790 );
					time[1] = *((unsigned char *) 0x4F1791 );
					time[2] = *((unsigned char *) 0x4F1792 );
				}

				tomb1_last_play[ tomb1_play_count ] = msf2sec( (char *) time );

				tomb1_play_count++;
			}
		}
	}


	if( cdrom_tracer_active ) {
		if( fp_cdrom == 0 )
			fopen_s(&fp_cdrom, "log-cdrom.txt", "w" );


		if( addr >= 0x1f801800 && addr <= 0x1f801803 )
		{
			if( addr == 0x1f801801 && cdxa_state == 0 )
			{
				switch( rt ) {
				case 0: fprintf( fp_cdrom, "[Sync]" ); break;
				case 1: fprintf( fp_cdrom, "[Nop]" ); break;
				case 2: fprintf( fp_cdrom, "[Setloc]" ); break;
				case 3: fprintf( fp_cdrom, "[Play]" ); break;
				case 4: fprintf( fp_cdrom, "[Forward]" ); break;
				case 5: fprintf( fp_cdrom, "[Backward]" ); break;
				case 6: fprintf( fp_cdrom, "[ReadN]" ); break;
				case 7: fprintf( fp_cdrom, "[Standby]" ); break;
				case 8: fprintf( fp_cdrom, "[Stop]" ); break;
				case 9: fprintf( fp_cdrom, "[Pause]" ); break;
				case 10: fprintf( fp_cdrom, "[Init]" ); break;
				case 11: fprintf( fp_cdrom, "[Mute]" ); break;
				case 12: fprintf( fp_cdrom, "[Demute]" ); break;
				case 13: fprintf( fp_cdrom, "[Setfilter]" ); break;
				case 14: fprintf( fp_cdrom, "[Setmode]" ); break;
				case 15: fprintf( fp_cdrom, "[Getmode]" ); break;
				case 16: fprintf( fp_cdrom, "[GetlocL]" ); break;
				case 17: fprintf( fp_cdrom, "[GetlocP]" ); break;
				case 18: fprintf( fp_cdrom, "[ReadT]" ); break;
				case 19: fprintf( fp_cdrom, "[GetTN]" ); break;
				case 20: fprintf( fp_cdrom, "[GetTD]" ); break;
				case 21: fprintf( fp_cdrom, "[SeekL]" ); break;
				case 22: fprintf( fp_cdrom, "[SeekP]" ); break;
				case 23: fprintf( fp_cdrom, "[Setclock]" ); break;
				case 24: fprintf( fp_cdrom, "[Getclock]" ); break;
				case 25: fprintf( fp_cdrom, "[Test]" ); break;
				case 26: fprintf( fp_cdrom, "[ID]" ); break;
				case 27: fprintf( fp_cdrom, "[ReadS]" ); break;
				case 28: fprintf( fp_cdrom, "[Reset]" ); break;
				case 29: fprintf( fp_cdrom, "[ReadTOC]" ); break;
				default: fprintf( fp_cdrom, "[Unknown]" ); break;
				}

				fprintf( fp_cdrom, "[CMD] %08X = %02X\n", addr, rt );
			}
			else
				fprintf( fp_cdrom, "[W] %08X = %02X\n", addr, rt );


			fflush( fp_cdrom );
		}
	}



	switch( addr ) {
		case 0x1f801800:
			if( rt == 2 ) cdxa_state = 2;
			else if( rt == 3 ) cdxa_state = 3;
			else {
				cdxa_state = 0;


				// Resident Evil 2
				// - (random) cdlpause irq in --middle-- of attenuation writes

				if( ePSXe_version == 170 ) {
					*((char *) 0x5109E6) = 0;
				}
				if( ePSXe_version == 180 ) {
					*((char *) 0x04F27A6) = 0;
				}

			}
			break;


		case 0x1f801801:
			if( cdxa_state == 3 ) {
				AttenuatorRight[0] = rt;
			}
			break;


		case 0x1f801802:
			if( cdxa_state == 2 ) {
				AttenuatorLeft[0] = rt;
			}
			else if( cdxa_state == 3 ) {
				// Chronicles of the Sword - XA pseudo-fix
				if( AttenuatorRight[0] != 0 ) {
					AttenuatorRight[1] = rt;
				} else {
					// reset?
					AttenuatorRight[0] = rt;
					AttenuatorRight[1] = 0;
				}
			}
			break;


		case 0x1f801803:
			if( cdxa_state == 2 ) {
				// Chronicles of the Sword - XA pseudo-fix
				if( AttenuatorLeft[0] != 0 ) {
					AttenuatorLeft[1] = rt;
				} else {
					// reset?
					AttenuatorLeft[0] = rt;
					AttenuatorLeft[1] = 0;
				}
			}
			else if( cdxa_state == 3 && rt == 0x20 ) {
				cdxa_state = 0;


				if( cdrom_tracer_active ) {
					if( fp_cdrom == 0 )
						fopen_s(&fp_cdrom, "log-cdrom.txt", "w" );


					fprintf( fp_cdrom, "CD-XA Volume: %X %X | %X %X\n",
						AttenuatorLeft[0], AttenuatorLeft[1],
						AttenuatorRight[0], AttenuatorRight[1] );

					fflush( fp_cdrom );
				}
			}
			break;


		default:
			break;
	}
}


void __stdcall MMIO_Write( int addr, int param )
{
	if( addr >= 0x1f801800 && addr <= 0x1f801803 )
		Attenuation_Ctrl( addr, param );
}


void __stdcall CdlReadTrack_ISO( FILE *fp_iso, unsigned int pos, int origin )
{
#if 0
	static FILE *fp = fopen("log.txt","a");
	fprintf( fp, "%X %X %X\n", fp, pos, origin );
	fclose(fp);
#endif

	// NOTE: These don't work with ePSXe libraries
	//_fseeki64( fp_iso, pos, origin );
	//fseek( fp_iso, pos, origin );



	static bool file_name_hack = 0;
	static unsigned int file_size = 0;
	static int license_check = 0;



	// license data $0-f
	if( license_check == 0 && pos < 0x930 * 0x10 ) {
		int license_pos;
		u8 name[4];
		u8 *name_ptr;


		license_check = 1;
		license_pos = 0x2514;

		if( ePSXe_version > 170 )
		{
		}
		else if( ePSXe_version == 170 ) {
			u8 *scea_logo;


			scea_logo = (u8 *) 0x45f108;
			name_ptr = name;

			_asm
			{
				push SEEK_SET
				push license_pos
				push fp_iso

				; fseek
				mov eax, dword ptr 0x43e2e2
				call eax

				add esp, 0x0c
			}


			__asm {
				push fp_iso
				push 4
				push 1
				push dword ptr name_ptr

				; fread
				mov eax, dword ptr 0x43daeb
				call eax

				add esp, 0x10
			}


			// soft-patch region
			if( name[0] == 'A' && name[1] == 'm' && name[2] == 'e' ) {
				scea_logo[0] = 'S';
				scea_logo[1] = 'C';
				scea_logo[2] = 'E';
				scea_logo[3] = 'A';
			}

			if( name[0] == 'E' && name[1] == 'u' && name[2] == 'r' ) {
				scea_logo[0] = 'S';
				scea_logo[1] = 'C';
				scea_logo[2] = 'E';
				scea_logo[3] = 'E';
			}

			if( name[0] == 'I' && name[1] == 'n' && name[2] == 'c' ) {
				scea_logo[0] = 'S';
				scea_logo[1] = 'C';
				scea_logo[2] = 'E';
				scea_logo[3] = 'I';
			}
		}
	}


	if( file_name_hack == 0 )
	{
		// check if file exists
		static HKEY myKey;
		static unsigned long size, type;

		if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\epsxe\\config",0,KEY_ALL_ACCESS,&myKey)==ERROR_SUCCESS)
		{
			size = 1024;
			RegQueryValueEx(myKey,"IsoDirectory",0,&type,(LPBYTE)&last_file,&size);

			RegCloseKey(myKey);
		}

		int len = strlen(last_file);
		if( len>4 )
		{
			last_file[ len-3 ] = 'b';
			last_file[ len-2 ] = 'i';
			last_file[ len-1 ] = 'n';
		}


		static FILE *fp;
		fopen_s(&fp, last_file, "rb" );
		if( fp )
		{
			_fseeki64(fp,0,SEEK_END);
			file_size = _ftelli64(fp);

			fclose( fp );
		}
		else last_file[0] = 0;


		file_name_hack = 1;
	}



	if( ePSXe_version > 170 )
	{
	}	
	else if( ePSXe_version == 170 )
	{
		// 2GB+ workaround
		if( pos >= 0x80000000 )
		{
			pos -= file_size;

			_asm
			{
				push SEEK_END
				push pos
				push fp_iso

				mov eax, dword ptr 0x43e2e2
				call eax

				add esp, 0x0c
			}
		}

		else
		{
			// normal range
			_asm
			{
				push SEEK_SET
				push pos
				push fp_iso

				mov eax, dword ptr 0x43e2e2
				call eax

				add esp, 0x0c
			}
		}
	}
}



void __stdcall CdlSetLoc( char *time )
{
#if 0
	FILE *fp = fopen("log.txt","a");
	fprintf( fp, "%02X %02X %02X\n", time[0], time[1], time[2] );
	fclose(fp);
#endif


	// if( ISO_image >= 0xAFC80 * 2352 )
	if( DVD5_mode == 0 ) {
		dvd5_state = 0;
	} else {
		// 320-480 minutes
		if( time[1] & 0x80 )
		{
			dvd5_state = 2;
		}

		// 160-320 minutes
		else if( time[2] & 0x80 )
		{
			dvd5_state = 1;
		}

		// 0-160 minutes
		else
		{
			dvd5_state = 0;
		}

		time[1] &= 0x7f;
		time[2] &= 0x7f;
	}



	// external CDR plugin
	if( CDR_setDVD5 )
	{
		__asm
		{
			push dvd5_state
			call CDR_setDVD5
		}
	}
}



void __stdcall Time2Addr( int LBA )
{
#if 0
	FILE *fp = fopen("log.txt","a");
	fprintf( fp, "%08X %01d\n", LBA, dvd5_state );
	fclose(fp);
#endif


	// if( ISO_image >= 0xAFC80 * 2352 )
	if( DVD5_mode == 0 ) {
		dvd5_state = 0;
	} else {
		// 320-480 minutes
		if( dvd5_state == 2 )
		{
			LBA += 0x15F900;
		}

		// 160-320 minutes
		else if( dvd5_state == 1 )
		{
			LBA += 0xAFC80;
		}

		// 0-160 minutes (NORMAL)
		else
		{}
	}


	// save for future reference
	LBA_local = LBA;
}



char file[512];
void __stdcall save_dvd5_state( char *state, char *str )
{
#if 0
	FILE *fp = fopen("log.txt","a");
	fprintf( fp, "%s %d\n", str, *state );
	fclose(fp);
#endif

	sprintf_s( file, "%s%s.%03d.dvd5", "sstates\\", str, *state );
	FILE *fp;
	fopen_s(&fp, file, "wb" );
	if( fp )
	{
		/*
		1 - dvd5_state (4 bytes)
		2 - xa state (8*4 bytes)
		3 - attenuators (4 + 4*4 bytes)
		*/

		fwrite( &STATE_TYPE, 1, 4, fp );

		fwrite( &dvd5_state, 1, 4, fp );
		fwrite( &local_xa, 1, 32, fp );

		fwrite( &cdxa_state, 1, 4, fp );
		fwrite( &AttenuatorLeft, 1, 8, fp );
		fwrite( &AttenuatorRight, 1, 8, fp );

		fwrite( &tomb1_last_play, 1, 4*9, fp );
		fwrite( &tomb1_play_count, 1, 4, fp );

		fclose( fp );
	}
}


void __stdcall load_dvd5_state( char *state, char *str )
{
	FILE *fp;

#if 0
	fp = fopen("log.txt","a");
	fprintf( fp, "%s %d %d\n", str, *state, CDR_setDVD5 );
	fclose(fp);
#endif



	sprintf_s( file, "%s%s.%03d.dvd5", "sstates\\", str, *state );
	fopen_s(&fp, file, "rb" );
	if( fp )
	{
		static int state_type;


		// check file size
		fseek( fp, 0, SEEK_END );
		state_type = ftell(fp);
		fseek( fp, 0, SEEK_SET );
		

		if( state_type == 1 )
		{
			fread( &dvd5_state, 1, 1, fp );
		} else { 
			/*
			1 - dvd5_state (4 bytes)
			2 - xa state (8*4 bytes)
			3 - attenuators (4 + 4*4 bytes)
			*/

			fread( &state_type, 1, 4, fp );

			
			if( state_type >= 1 ) {
				fread( &dvd5_state, 1, 4, fp );
			}

			if( state_type >= 2 ) {
				fread( &local_xa, 1, 32, fp );
			}

			if( state_type >= 3 ) {
				fread( &cdxa_state, 1, 4, fp );
				fread( &AttenuatorLeft, 1, 8, fp );
				fread( &AttenuatorRight, 1, 8, fp );
			}

			if( state_type < 6 ) {
				if( state_type >= 4 ) {
					fread( &tomb1_last_play, 1, 4, fp );
				}

				if( state_type >= 5 ) {
					fread( &tomb1_play_count, 1, 4, fp );
				}
			}
			else if( state_type >= 6 ) {
				fread( &tomb1_last_play, 1, 4*9, fp );
				fread( &tomb1_play_count, 1, 4, fp );
			}
		}

		fclose( fp );
	}


	// external CDR plugin
	if( CDR_setDVD5 )
	{
		__asm
		{
			push dvd5_state
			call CDR_setDVD5
		}
	}
}


void Cdrom_Read( int addr, int rt )
{
	if( cdrom_tracer_active )
	{
		if( addr >= 0x1f801800 && addr <= 0x1f801803 )
		{
			// avoid crash bug - check fp_cdrom on
			if( ePSXe_version == 170 && fp_cdrom )
			{
				if( fp_cdrom == 0 )
					fopen_s(&fp_cdrom, "log-cdrom.txt", "w" );


				fprintf( fp_cdrom, "[R] %08X = %02X [%d / %d]\n",
					addr, rt & 0xff,
					*((char *) 0x50f941), *((char *) 0x50f940) );

				fflush( fp_cdrom );
			}
		}
	}
}


void __stdcall MMIO_Read( int addr, int param )
{
	if( addr >= 0x1f801800 && addr <= 0x1f801803 )
		Cdrom_Read( addr, param );
}
