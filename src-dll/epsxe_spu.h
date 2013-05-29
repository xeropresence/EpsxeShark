void __stdcall LoadXA_Data( int xa_addr )
{
	memcpy( (void *) xa_addr, &local_xa, 4*8 );
}


void __stdcall SaveXA_Data( int xa_addr )
{
	memcpy( &local_xa, (void *) xa_addr, 4*8 );
}

#define ssat32_to_16(v) do { \
	if (v < -32768) v = -32768; \
	else if (v > 32767) v = 32767; \
} while (0)

// PCSX-reloaded
void __cdecl CDXA_Attenuation( short *buf, int size, int stereo, int attenuate_type )
{
	short *spsound;
	int lc,rc;
	int i;

#ifdef CDR_LOG___0
	CDR_LOG( "CDXA Attenuate: %X\n", size );
#endif

#ifdef CDR_LOG
	CDR_LOG( "CD-XA Volume: %X %X | %X %X\n",
		cdr.AttenuatorLeft[0], cdr.AttenuatorLeft[1],
		cdr.AttenuatorRight[0], cdr.AttenuatorRight[1] );
#endif

	spsound = buf;

	// mono xa attenuation
	// - Tales of Phantasia (voice meter)
	if( stereo == 0 ) {
		short temp[32768];
		int dst;

		dst = 0;
		for( i = 0; i < size; i++, dst+=2 ) {
			temp[dst+0] = spsound[i];
			temp[dst+1] = spsound[i];
		}

		size *= 2;
		memcpy( spsound, temp, size );
	}


	for( i = 0; i < size / 2; i += 2 )
	{
		// Chronicles of the Sword - speech (cdda)
		// Gamera 2000 - movies (xa)
		lc = (spsound[i+0] * AttenuatorLeft[0]  + spsound[i+1] * AttenuatorRight[1]) / 128;
		rc = (spsound[i+1] * AttenuatorRight[0] + spsound[i+0] * AttenuatorLeft[1]) / 128;

		ssat32_to_16(lc);
		ssat32_to_16(rc);

		spsound[i + 0] = lc;
		spsound[i + 1] = rc;
	}
}


void __stdcall xa_attenuation( xa_decode_t *xa_temp )
{
	if( xa_temp->stereo == 0 )
		CDXA_Attenuation( xa_temp->pcm, xa_temp->nsamples * 2, xa_temp->stereo, CDDA_ATTENUATE );
	else
		CDXA_Attenuation( xa_temp->pcm, xa_temp->nsamples * 4, xa_temp->stereo, CDDA_ATTENUATE );


	// force mono -> stereo
	xa_temp->stereo = 1;
}


void __stdcall cdda_attenuation( short *buf )
{
	CDXA_Attenuation( buf, 2352, 1, CDDA_ATTENUATE );
}


void SPUsetframelimit( int option )
{
	__asm 
	{
		push option
		call SPUsetframelimit_func
	}
}

extern void __stdcall GPUsetframelimit( int option );

long __stdcall SPUopen(HWND hSpuWnd)
{

	if(SPUsetframelimit_func) 
	{
		if(spu_framelimit)
		{
			SPUsetframelimit( 1 );
			GPUsetframelimit( 0 );
		}
		// make P.E.Op.S SPU 1.10b disable frame limiter, it should be disabled by default
		else SPUsetframelimit( 0 );
	}

	long result;
	__asm
	{
		// call REAL routine
		push hSpuWnd
		call SPUopen_func
		mov result,eax
	}

	return result;
}