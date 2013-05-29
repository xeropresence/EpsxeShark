#include "epsxe_180_asm.h"

void Attach_180()
{

	// window caption
	static char name[] = " ePSXe - Shark enhanced";
	WriteCode(
		(unsigned char*) 0x0045B810,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x0045B810 );

	WriteCode(
		(unsigned char*) 0x0045CECC,
		(unsigned char*) name,
		sizeof(name) );

	DirtyPatch( 0x0045CECC );

	// slow boot
	ApplyJmpHook(
		(unsigned char*) 0x00403CA2,
		(unsigned char*) slow_boot_180,
		(0x00403CA7 - 0x00403CA2) - 5 );

	DirtyPatch( 0x00403CA2 );

	// gpuinit
	ApplyJmpHook(
		(unsigned char*) 0x004323E6,
		(unsigned char*) &GPUinit_Wrapper_180,
		(0x004323EB - 0x004323E6) - 5 );

	DirtyPatch( 0x004323E6 );

	// gpuopen
	ApplyJmpHook(
		(unsigned char*) 0x00432438,
		(unsigned char*) &GPUopen_Wrapper_180,
		(0x0043243D - 0x00432438) - 5 );

	DirtyPatch( 0x00432438 );

	// gpuclose
	ApplyJmpHook(
		(unsigned char*) 0x00432460,
		(unsigned char*) &GPUclose_Wrapper_180,
		(0x00432465 - 0x00432460) - 5 );

	DirtyPatch( 0x00432460 );

	// gpushutdown
	ApplyJmpHook(
		(unsigned char*) 0x0043240F,
		(unsigned char*) &GPUshutdown_Wrapper_180,
		(0x00432414 - 0x0043240F) - 5 );

	DirtyPatch( 0x0043240F );

	// GPUwriteStatus
	ApplyJmpHook(
		(unsigned char*) 0x0043252C,
		(unsigned char*) &GPUwriteStatus_Wrapper_180,
		(0x00432531 - 0x0043252C) - 5 );

	DirtyPatch( 0x0043252C );

	// GPUsetframelimit
	ApplyJmpHook(
		(unsigned char*) 0x004326DC,
		(unsigned char*) &GPUsetframelimit_Wrapper_180,
		(0x004326E1 - 0x004326DC) - 5 );

	DirtyPatch( 0x004326DC );

	// GPUclearDynarec
	ApplyJmpHook(
		(unsigned char*) 0x00432717,
		(unsigned char*) &GPUclearDynarec_Wrapper_180,
		(0x0043271C - 0x00432717) - 5 );

	DirtyPatch( 0x00432717 );

	// GPUupdateLace
	ApplyJmpHook(
		(unsigned char*) 0x00432620,
		(unsigned char*) &GPUupdateLace_Wrapper_180,
		(0x00432625 - 0x00432620) - 5 );

	DirtyPatch( 0x00432620 );

	// GPUshowScreenPic
	ApplyJmpHook(
		(unsigned char*) 0x004326B6,
		(unsigned char*) &GPUshowScreenPic_Wrapper_180,
		(0x004326BC - 0x004326B6) - 5 );

	DirtyPatch( 0x004326B6 );

	// reset emu - spu crash
	ApplyJmpHook(
		(unsigned char*) 0x0040BF5E,
		(unsigned char*) &reset_emu_180,
		(0x0040BF65 - 0x0040BF5E) - 5 );

	DirtyPatch( 0x0040BF5E );

	// system reset - do extra init
	ApplyJmpHook(
		(unsigned char*) 0x0042DAA0,
		(unsigned char*) &cpu_reset_180,
		(0x0042DAA5 - 0x0042DAA0) - 5 );

	DirtyPatch( 0x0042DAA0 );

	// spu async rate
	static char async_cycles[1] = { 1 };
	static char async_rate[1] = { 1 };

	async_cycles[0] = apu_cycles-1;
	async_rate[0] = apu_rate;

	WriteCode(
		(unsigned char*) 0x00423B24+1,
		(unsigned char*) async_cycles,
		1 );

	DirtyPatch( 0x00423B24 );
	
	WriteCode(
		(unsigned char*) 0x00423B32+2,
		(unsigned char*) async_rate,
		1 );

	DirtyPatch( 0x00423B32 );

	// spu cdda play
	static char spu_cddaplay = 1;

	WriteCode(
		(unsigned char*) 0x0041F84E+1,
		(unsigned char*) &spu_cddaplay,
		1 );

	DirtyPatch( 0x0041F84E );

	ApplyJmpHook(
		(unsigned char*) 0x0040DBE5,
		(unsigned char*) &SPU_PlayCDDA_180,
		(0x0040DBF6 - 0x0040DBE5) - 5 );

	DirtyPatch( 0x0040DBE5 );

	ApplyJmpHook(
		(unsigned char*) 0x0041FBC4,
		(unsigned char*) &MMIO_Write_180,
		(0x0041FBC9 - 0x0041FBC4) - 5 );

	DirtyPatch( 0x0041FBC4 );

	ApplyJmpHook(
		(unsigned char*) 0x0040DB9E,
		(unsigned char*) &SPU_PlayXA_180,
		(0x0040DBA3 - 0x0040DB9E) - 5 );

	DirtyPatch( 0x0040DB9E );

	// spu based frame limiter

	ApplyJmpHook(
		(unsigned char*) 0x0040D0B1,
		(unsigned char*) &SPUopen_Wrapper_180,
		(0x0040D0B6 - 0x0040D0B1) - 5 );

	DirtyPatch( 0x0040D0B1 );

	ApplyJmpHook(
		(unsigned char*) 0x0040D3FA,
		(unsigned char*) &SPUsetframelimit_Wrapper_180,
		(0x0040D400 - 0x0040D3FA) - 5 );

	DirtyPatch( 0x0040D3FA );

	char nop[5] = { (char) 0x90, (char) 0x90, (char) 0x90, (char) 0x90, (char) 0x90 };
	WriteCode(
		(unsigned char*) 0x0040D406,
		(unsigned char*) nop,
		5 );

	DirtyPatch( 0x0040D406 );

	// pad plugins - startpoll
	ApplyJmpHook(
		(unsigned char*) 0x00421541,
		(unsigned char*) &pad_startpoll_180,
		(0x00421548 - 0x00421541) - 5 );

	DirtyPatch( 0x00421541 );


	// pad plugins - poll pad1
	ApplyJmpHook(
		(unsigned char*) 0x0042309C,
		(unsigned char*) &pad_poll_pad1_180,
		(0x004230A1 - 0x0042309C) - 5 );

	DirtyPatch( 0x0042309C );


	// pad plugins - poll pad2
	ApplyJmpHook(
		(unsigned char*) 0x004230BC,
		(unsigned char*) &pad_poll_pad2_180,
		(0x004230C1 - 0x004230BC) - 5 );

	DirtyPatch( 0x004230BC );

}