#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char

#define s32 signed int
#define s16 signed short
#define s8 signed char


#define itob(i)      ((i)/10*16 + (i)%10)
#define btoi(b)      ((b)/16*10 + (b)%16)

// ####################################
// ####################################
// ####################################
// ####################################

// main.h
int ePSXe_version;
HWND ePSXe_hwnd;

HMODULE hHost; //exe that load this module
HMODULE hThis; //this module



FILE *fp_log;



// DEBUG
int opcode_tracer;
unsigned int opcode_addr;
int start_tracer;
FILE *fp_log_trace;


// CDR
int dvd5_state;
int LBA_local;

unsigned char *CDR_setDVD5;
const char CDR_setDVD5_str[] = "CDR_setDVD5";




// SPU
char spu_lib_setframelimit[] = "SPUsetframelimit";
unsigned char *SPUsetframelimit_func;
unsigned char *SPUopen_func;



// GPU
unsigned char *GPUinit_func;
unsigned char *GPUshutdown_func;
unsigned char *GPUopen_func;
unsigned char *GPUclose_func;

unsigned char *GPUupdateLace_func;
unsigned char *GPUshowScreenPic_func;
unsigned char *GPUclearDynarec_func;
unsigned char *GPUsetframelimit_func;
unsigned char *GPUwriteStatus_func;
unsigned char *clear_dynarec;


typedef void (CALLBACK* GPUvisualVibration_type)(unsigned long, unsigned long);
typedef void (CALLBACK* GPUcursor_type)(int, int, int);

GPUvisualVibration_type GPUvisualVibration_func;
GPUcursor_type GPUcursor_func;


int framelimiter_mutex;



LARGE_INTEGER liCPUFrequency;
bool bIsPerformanceCounter;
float fFrameRateHz = 180+40;
int sleep_throttle = 1;

int *framelimit;


int DVD5_mode;
int start_gpu;
int timer_precise;
int show_cursor;
int apu_cycles, apu_rate;
int shark_cheater_boot;


HWND hWWindow;
WNDPROC	pOldWinProc=0;
char szGPUKeys[256];

unsigned char throttle_max_key = VK_BACK;
int max_fps_toggle;


// 0 = normal
// 1 = run cdrom (confirm)
// 2 = run cdrom (auto)
// 3 = run iso (start)
// 4 = run cdrom (confirm)
// 5 = run iso (auto)
int run_iso_mode;

int disable_memcards;
float dmachain_adjust;


int autostart_controllers;
int konami_justifier1;
int konami_justifier2;
int mousewheel_converter;
int move_window;
int slow_boot;
int spu_framelimit;
int opengl2_vramauto;

int cdrom_tracer;
int cdrom_tracer_active;


int gpu_loadstate_hack;

// ####################################
// ####################################
// ####################################
// ####################################

// pad dll plugins
char pad_name[2][1024] = { "ePSXe internal PAD", "ePSXe internal PAD" };
HINSTANCE pad_plugin[2];
int pad_plugin_active[2];

// ####################################
// ####################################
// ####################################
// ####################################

// global vars
int reset_spu=0;

char last_file[1024];
char temp_str[1024];

char *name_ptr;
u32 *func_ptr;

int temp_var1, temp_var2, temp_var3;

// ####################################
// ####################################
// ####################################
// ####################################

// cdr.h
const int STATE_TYPE = 6;


int cdxa_state;
int AttenuatorLeft[2] = { 0x80, 0x00 };
int AttenuatorRight[2] = { 0x80, 0x00 };


#define CDRINT_NONE					0x00
#define CDRINT_DATAREADY		0x01
#define CDRINT_COMPLETE			0x02
#define CDRINT_ACKNOWLEDGE	0x03
#define CDRINT_DISKERROR		0x05

#define CDRSTATUS_SEEK				0x40
#define CDRSTATUS_READ				0x20
#define CDRSTATUS_SHELLOPEN		0x10
#define CDRSTATUS_ROTATING		0x02
#define CDRSTATUS_ERROR				0x01


/* CD-ROM magic numbers */
#define CdlSync        0
#define CdlNop         1
#define CdlSetloc      2
#define CdlPlay        3
#define CdlForward     4
#define CdlBackward    5
#define CdlReadN       6
#define CdlStandby     7
#define CdlStop        8
#define CdlPause       9
#define CdlInit        10
#define CdlMute        11
#define CdlDemute      12
#define CdlSetfilter   13
#define CdlSetmode     14
#define CdlGetmode     15
#define CdlGetlocL     16
#define CdlGetlocP     17
#define CdlReadT       18
#define CdlGetTN       19
#define CdlGetTD       20
#define CdlSeekL       21
#define CdlSeekP       22
#define CdlSetclock    23
#define CdlGetclock    24
#define CdlTest        25
#define CdlID          26
#define CdlReadS       27
#define CdlReset       28
#define CdlReadToc     30


int lid_state;
int lid_timer;
int lid_flag;



// redump.org SBI files
u8 sbitime[256][3];
u8 sbicount;


char cdrom_cdda_plugin_table[99][3] = { {-1,-1,-1} };



int tomb1_cdda_hack = 0;
int tomb1_cdda_table[90] = { -1 };
int tomb1_last_play[9];
int tomb1_play_count;


int tomb2_cdda_hack = 0;
int rebel2_gpudma_hack = 0;


char setloc_cdda_frames;
int setloc_cdda_noseek;

// ####################################
// ####################################
// ####################################
// ####################################

// cpu.h
unsigned char mem_trace[0x200000];

unsigned int cpu_track[6];


// ####################################
// ####################################
// ####################################
// ####################################

// spu.h
typedef struct
{
	long	y0, y1;
} ADPCM_Decode_t;

typedef struct
{
	int				freq;
	int				nbits;
	int				stereo;
	int				nsamples;
	ADPCM_Decode_t	left, right;
	short			pcm[16384];
} xa_decode_t;


xa_decode_t local_xa;


#define XA_ATTENUATE 1
#define CDDA_ATTENUATE 2


// ####################################
// ####################################
// ####################################
// ####################################

// sio.h

int memcard_reset;




int guncon_command;

float guncon_sensitive_x;
float guncon_sensitive_y;



int analog_deadzone_lx_1;
int analog_deadzone_lx_2;
int analog_deadzone_ly_1;
int analog_deadzone_ly_2;

int analog_deadzone_rx_1;
int analog_deadzone_rx_2;
int analog_deadzone_ry_1;
int analog_deadzone_ry_2;



float mouse_sensitive_x;
float mouse_sensitive_y;

int mousewheel_z;
int mouse_left;
int mouse_right;
int mouse_middle;





int dualshock_mouse_active;

int dualshock_dpad_stick = 1;

s32 dualshock_mouse_x, dualshock_mouse_y;
int dualshock_mouse_stick = 2;


// map mouse to joypad
int dualshock_button_left;
int dualshock_button_right;
int dualshock_button_middle;


// (d-pad) pseudo-analog motion states
int dualshock_wheel_max;
int dualshock_wheel;
int dualshock_wheel_debounce;



// motion states - mouse wheel
int dualshock_mouse_up[16], dualshock_mouse_down[16];
int dualshock_mouse_left[16], dualshock_mouse_right[16];
int dualshock_mouse_up_left[16], dualshock_mouse_up_right[16];
int dualshock_mouse_down_left[16], dualshock_mouse_down_right[16];


float dualshock_mouse_sensitive;
float dualshock_mouse_deadzone;

float dualshock_mouse_snap_sensitive;
float dualshock_mouse_snap_deadzone;



// swap states
int toggle_dualshock_mouse[3][2];
int toggle_dualshock_dpad[3][2];
int toggle_dualshock_mouse_state;
int toggle_dualshock_dpad_state;

int toggle_dualshock_allow[3];



// snap state (on / off) - return to 0 -or- hold tilt
int dualshock_mouse_snap;
u32 dualshock_mouse_snap_key;
int dualshock_mouse_snap_speed;

int dualshock_mouse_snap_x;
int dualshock_mouse_snap_y;





int negcon_twist_deadzone_1;
int negcon_twist_deadzone_2;
int negcon_twist_option_center;
int negcon_twist_option_flip;
int negcon_twist_threshold_1;
int negcon_twist_threshold_2;
int negcon_twist_sensitivity;

int negcon_I_deadzone;
int negcon_I_option_center;
int negcon_I_option_flip;
int negcon_I_threshold;
int negcon_I_sensitivity;

int negcon_II_deadzone;
int negcon_II_option_center;
int negcon_II_option_flip;
int negcon_II_threshold;
int negcon_II_sensitivity;

int negcon_L_deadzone;
int negcon_L_option_center;
int negcon_L_option_flip;
int negcon_L_threshold;
int negcon_L_sensitivity;
