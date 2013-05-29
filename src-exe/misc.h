FILE *in, *out;
unsigned char buffer[ 0x10 * 0x100000 ];



void SaveApp()
{
	FILE *fp, *fp2;
	int size;


	fopen_s(&fp, "ePSXe.exe", "rb+" );

	if( !fp ) return;


	if( ePSXe_version == 170 ) {
		fopen_s(&fp2, "ePSXe-170.exe", "rb" );
		if( !fp2 ) {
			fopen_s(&fp2, "ePSXe-170.exe", "wb" );

			size = fread( buffer, 1, 0x100000 * 0x10, fp );
			fwrite( buffer, 1, size, fp2 );

			fclose( fp2 );
		}
	}


	fclose( fp );
}



void StallApp()
{
	FILE *fp;

	fopen_s(&fp, "ePSXe.exe", "rb+" );
	if( !fp ) return;



	// Wait for Win32 to finish setting up
	// - use a hard-coded jmp point at WinMain
	if( ePSXe_version >= 170 ) {
/*
WinMain
		
.text:00403970                 sub     esp, 408h
81 ec 08 04 00 00

.text:00403976                 mov     ecx, [esp+408h+lpCmdLine]
.text:0040397D                 push    ebx
*/

		// insert burn cycle loop - wait until dll installed
		// (not professional, don't care)

		fseek( fp, 0x3970, SEEK_SET );

		// eb fe = endless loop
		fputc( 0xeb, fp );
		fputc( 0xfe, fp );
	}


	fclose( fp );
}





void unpack_exe()
{
    // Structures for creating the process
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
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
		
		GetCurrentDirectory(MAX_PATH,workingDir);
		strcat_s( exeString, "\"" );
		strcat_s( exeString, workingDir );
		strcat_s( exeString, "\\" );
		strcat_s( exeString, "upx.exe\" -d " );
		strcat_s( exeString, PROC_NAME );

    // Need to set this for the structure
    si.cb = sizeof(STARTUPINFO);

    // Try to load our process
    result = CreateProcessA(NULL, exeString, NULL, NULL, FALSE,
                            NULL, NULL, workingDir, &si, &pi);
    if(!result)
    {
        MessageBox(0, "Could not find upx.exe!", "Error", MB_ICONERROR);
    }

		WaitForSingleObject( pi.hProcess,INFINITE );
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
}


int detect_version()
{
	FILE *fp;

	fopen_s(&fp, PROC_NAME, "rb" );
	if( fp )
	{
		unsigned int value;

		fseek( fp, 0x10000, SEEK_SET );
		fread( &value, 1, 4, fp );
		fclose(fp);

		if( value == (unsigned int) 0x870f0eff )
			return 170;
		if( value == (unsigned int) 0xf47f0ffc )
			return 160;
		if( value == (unsigned int) 0x0d81127d )
			return 152;
	}

	return 0;
}
