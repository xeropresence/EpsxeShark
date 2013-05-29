/*
L2				0x0001
R2				0x0002
L1				0x0004
R1				0x0008
Triangle 	0x0010
O					0x0020
X					0x0040
Square 		0x0080
Select 		0x0100
L3 				0x0200 (dualshock)
R3				0x0400 (dualshock)
Start 		0x0800
Up 				0x1000
Right 		0x2000
Down 			0x4000
Left 			0x8000
*/


FILE *fp_negcon;


void __stdcall Mouse_Cmd_Response( int *output_size, int cmd )
{
	cmd &= 0xff;


	// invalid command - no response after ID + cmd
	if( cmd != 0x42 )
		(*output_size) = 1+2;
}


void __stdcall Guncon_Cmd_Response( int *output_size )
{
	// invalid command - no response after ID + cmd
	// - not needed maybe
	//if( guncon_command != 0x42 )
		//(*output_size) = 1+2;
}


void __stdcall sio_negcon_clamp_twist( int value )
{
	// clamp down now
	if( value < -0x100 ) value = -0x100;
	if( value > 0x100 ) value = 0x100;
	
	

	// re-center adjustment ($7f -> $ff) (idle)
	if( negcon_twist_option_center ) {
		value += 0x80;
	}


	// sensitivity adjusting
	value *= negcon_twist_sensitivity;



	// clamp down now (-128 to 127)
	if( value < -0x80 ) value = -0x80;
	if( value > 0x7f ) value = 0x7f;



	// direction flip ($7f -> -$7f) (idle)
	if( negcon_twist_option_flip ) {
		value = ( 0xff - value );
	}


	// threshold clip
	if( value < negcon_twist_threshold_1 ) {
		value = negcon_twist_threshold_1;
	}
	if( value > negcon_twist_threshold_2 ) {
		value = negcon_twist_threshold_2;
	}


	// deadzone clip
	if( value >= negcon_twist_deadzone_1 &&
			value <= negcon_twist_deadzone_2 ) {
		value = 0;
	}


	temp_var2 = value;
}



void __stdcall sio_negcon_clamp_I( int value )
{
	// clamp down now
	if( value < -0x100 ) value = -0x100;
	if( value > 0x100 ) value = 0x100;
	
	

	// re-center adjustment ($7f -> $ff) (idle)
	if( negcon_I_option_center ) {
		value += 0x80;
	}


	// sensitivity adjusting
	value *= negcon_I_sensitivity;



	// clamp down now (0-255)
	if( value < 0x00 ) value = 0;
	if( value > 0xff ) value = 0xff;



	// direction flip ($ff -> $00) (idle)
	if( negcon_I_option_flip ) {
		value = ( 0xff - value );
	}


	// threshold clip
	if( value > negcon_I_threshold ) {
		value = negcon_I_threshold;
	}


	// deadzone clip
	if( value <= negcon_I_deadzone ) {
		value = 0;
	}


	temp_var2 = value;
}



void __stdcall sio_negcon_clamp_II( int value )
{
	// clamp down now
	if( value < -0x100 ) value = -0x100;
	if( value > 0x100 ) value = 0x100;
	
	

	// re-center adjustment ($7f -> $ff) (idle)
	if( negcon_II_option_center ) {
		value += 0x80;
	}


	// sensitivity adjusting
	value *= negcon_II_sensitivity;



	// clamp down now (0-255)
	if( value < 0x00 ) value = 0;
	if( value > 0xff ) value = 0xff;



	// direction flip ($ff -> $00) (idle)
	if( negcon_II_option_flip ) {
		value = ( 0xff - value );
	}


	// threshold clip
	if( value > negcon_II_threshold ) {
		value = negcon_II_threshold;
	}


	// deadzone clip
	if( value <= negcon_II_deadzone ) {
		value = 0;
	}


	temp_var2 = value;
}



void __stdcall sio_negcon_clamp_L( int value )
{
	// clamp down now
	if( value < -0x100 ) value = -0x100;
	if( value > 0x100 ) value = 0x100;
	
	

	// re-center adjustment ($7f -> $ff) (idle)
	if( negcon_L_option_center ) {
		value += 0x80;
	}


	// sensitivity adjusting
	value *= negcon_L_sensitivity;



	// clamp down now (0-255)
	if( value < 0x00 ) value = 0;
	if( value > 0xff ) value = 0xff;



	// direction flip ($ff -> $00) (idle)
	if( negcon_L_option_flip ) {
		value = ( 0xff - value );
	}


	// threshold clip
	if( value > negcon_L_threshold ) {
		value = negcon_L_threshold;
	}


	// deadzone clip
	if( value <= negcon_L_deadzone ) {
		value = 0;
	}


	temp_var2 = value;
}



void __stdcall dualshock_mouse_poll()
{
	static int poll_again = 0;


	// note: pad1 only!!!
	if( dualshock_mouse_active == 0 ) return;



	/*
	real device apparently polls slower than say... 60 fps
	- dualshock = port1, mouse = port2
	- poll mouse first, then set dualshock
	*/

	poll_again--;
	if( poll_again <= 0 ) {
		dualshock_mouse_x = *((s32 *) 0x4c0930 );
		dualshock_mouse_y = *((s32 *) 0x4c0934 );


		// time to snap back to center (or next motion)
		// - min 3 recommended
		poll_again = dualshock_mouse_snap_speed;


		if( dualshock_mouse_snap == 0 ) {
			// thumbstick - hold tilt (no snap)
			

			// deadzone sensitivity
			dualshock_mouse_x = (int)( (float) (dualshock_mouse_x) * dualshock_mouse_deadzone / 100.0f );
			dualshock_mouse_y = (int)( (float) (dualshock_mouse_y) * dualshock_mouse_deadzone / 100.0f );


			// scale before clamping
			dualshock_mouse_x = (int)( (float) (dualshock_mouse_x) * dualshock_mouse_sensitive / 100.0f );
			dualshock_mouse_y = (int)( (float) (dualshock_mouse_y) * dualshock_mouse_sensitive / 100.0f );

			
			dualshock_mouse_snap_x += dualshock_mouse_x;
			dualshock_mouse_snap_y += dualshock_mouse_y;


			// clamp + $08
			if( dualshock_mouse_snap_x < -0x88 ) dualshock_mouse_snap_x = -0x88;
			if( dualshock_mouse_snap_y < -0x88 ) dualshock_mouse_snap_y = -0x88;
			if( dualshock_mouse_snap_x > 0x88 ) dualshock_mouse_snap_x = 0x88;
			if( dualshock_mouse_snap_y > 0x88 ) dualshock_mouse_snap_y = 0x88;


			dualshock_mouse_x = dualshock_mouse_snap_x;
			dualshock_mouse_y = dualshock_mouse_snap_y;
		} else {
			// thumbstick - snap back

			// deadzone sensitivity
			dualshock_mouse_x = (int)( (float) (dualshock_mouse_x) * dualshock_mouse_deadzone / 100.0f );
			dualshock_mouse_y = (int)( (float) (dualshock_mouse_y) * dualshock_mouse_deadzone / 100.0f );


			// scale before clamping
			dualshock_mouse_x = (int)( (float) (dualshock_mouse_x) * dualshock_mouse_sensitive / 100.0f );
			dualshock_mouse_y = (int)( (float) (dualshock_mouse_y) * dualshock_mouse_sensitive / 100.0f );
		}



		// clamp now
		if( dualshock_mouse_x < -0x80 ) dualshock_mouse_x = -0x80;
		if( dualshock_mouse_y < -0x80 ) dualshock_mouse_y = -0x80;
		if( dualshock_mouse_x > 0x7f ) dualshock_mouse_x = 0x7f;
		if( dualshock_mouse_y > 0x7f ) dualshock_mouse_y = 0x7f;
	}
}


void __stdcall dualshock_analog_mouse()
{
	s32 *stick_mouse_x, *stick_mouse_y;
	s32 *stick_dpad_x, *stick_dpad_y;
	s32 *dualshock_lx, *dualshock_ly;
	s32 *dualshock_rx, *dualshock_ry;
	u16 *dualshock_pad;
	u16 dpad_down, dpad_up, dpad_right, dpad_left;



	if( dualshock_mouse_active == 0 ) return;


	stick_mouse_x = 0;
	stick_mouse_y = 0;
	stick_dpad_x = 0;
	stick_dpad_y = 0;


	dualshock_lx = (s32 *) 0x4c4c90;
	dualshock_ly = (s32 *) 0x4c4cb0;
	dualshock_rx = (s32 *) 0x4c4cd0;
	dualshock_ry = (s32 *) 0x4c4cf0;
	dualshock_pad = (u16 *) 0x4bdb4c;


	dpad_up = (*dualshock_pad) & 0x1000;
	dpad_right = (*dualshock_pad) & 0x2000;
	dpad_down = (*dualshock_pad) & 0x4000;
	dpad_left = (*dualshock_pad) & 0x8000;

	// ###################################
	// ###################################
	// ###################################

	static int debounce_left = 0;
	static int debounce_right = 0;
	static int debounce_middle = 0;


	// swap devices
	if( toggle_dualshock_allow[0] ) {
		if( (mouse_left & 0x80) && debounce_left == 0 ) {
			debounce_left = 1;

			toggle_dualshock_mouse_state++;
			toggle_dualshock_mouse_state &= 1;

			toggle_dualshock_dpad_state++;
			toggle_dualshock_dpad_state &= 1;

			dualshock_mouse_stick = toggle_dualshock_mouse[0][toggle_dualshock_mouse_state];
			dualshock_dpad_stick = toggle_dualshock_dpad[0][toggle_dualshock_dpad_state];
		}
		else if( (mouse_left & 0x80) == 0 && debounce_left == 1 )
			debounce_left = 0;
	}


	if( toggle_dualshock_allow[1] ) {
		if( (mouse_right & 0x80) && debounce_right == 0 ) {
			debounce_right = 1;

			toggle_dualshock_mouse_state++;
			toggle_dualshock_mouse_state &= 1;

			toggle_dualshock_dpad_state++;
			toggle_dualshock_dpad_state &= 1;

			dualshock_mouse_stick = toggle_dualshock_mouse[1][toggle_dualshock_mouse_state];
			dualshock_dpad_stick = toggle_dualshock_dpad[1][toggle_dualshock_dpad_state];
		}
		else if( (mouse_right & 0x80) == 0 && debounce_right == 1 )
			debounce_right = 0;
	}



	if( toggle_dualshock_allow[2] ) {
		if( (mouse_middle & 0x80) && debounce_middle == 0 ) {
			debounce_middle = 1;

			toggle_dualshock_mouse_state++;
			toggle_dualshock_mouse_state &= 1;

			toggle_dualshock_dpad_state++;
			toggle_dualshock_dpad_state &= 1;

			dualshock_mouse_stick = toggle_dualshock_mouse[2][toggle_dualshock_mouse_state];
			dualshock_dpad_stick = toggle_dualshock_dpad[2][toggle_dualshock_dpad_state];
		}
		else if( (mouse_middle & 0x80) == 0 && debounce_middle == 1 )
			debounce_middle = 0;
	}

	// ###################################
	// ###################################
	// ###################################

	// wheel state transitions - once per teeth click
	if( dualshock_wheel_debounce == 0 && mousewheel_z ) {
		// wheel up - wheel down
		if( mousewheel_z > 0 )
			dualshock_wheel++;
		else if( mousewheel_z < 0 )
			dualshock_wheel--;


		// wheel = [0..max-1]
		if( dualshock_wheel < 0 )
			dualshock_wheel = 0;
		else if( dualshock_wheel >= dualshock_wheel_max )
			dualshock_wheel = dualshock_wheel_max - 1;
	}
	else if( dualshock_wheel_debounce && mousewheel_z == 0 ) {
		dualshock_wheel_debounce = 0;
	}

	// ###################################
	// ###################################
	// ###################################

	if( dualshock_mouse_stick == 1 ) {
		// left stick
		stick_mouse_x = dualshock_lx;
		stick_mouse_y = dualshock_ly;
	}
	else if( dualshock_mouse_stick == 2 ) {
		// right stick
		stick_mouse_x = dualshock_rx;
		stick_mouse_y = dualshock_ry;
	}
	else if( dualshock_mouse_stick == 3 ) {
		// clear d-pad
		*dualshock_pad |= 0xf000;


		// left - right
		if( dualshock_mouse_x < 0 ) *dualshock_pad &= ~0x8000;
		else if( dualshock_mouse_x > 0 ) *dualshock_pad &= ~0x2000;


		// up - down
		if( dualshock_mouse_y < 0 ) *dualshock_pad &= ~0x1000;
		else if( dualshock_mouse_y > 0 ) *dualshock_pad &= ~0x4000;
	}


	// analog mouse motion
	if( stick_mouse_x && stick_mouse_y ) {
		*stick_mouse_x = dualshock_mouse_x;
		*stick_mouse_y = dualshock_mouse_y;
	}

	// ###################################
	// ###################################
	// ###################################

	if( dualshock_dpad_stick == 1 ) {
		// left stick
		stick_dpad_x = dualshock_lx;
		stick_dpad_y = dualshock_ly;


		// clear d-pad
		if( dualshock_mouse_stick != 3 )
			*dualshock_pad |= 0xf000;
	}
	else if( dualshock_dpad_stick == 2 ) {
		// right stick
		stick_dpad_x = dualshock_rx;
		stick_dpad_y = dualshock_ry;


		// clear d-pad
		if( dualshock_mouse_stick != 3 )
			*dualshock_pad |= 0xf000;
	}
	else if( dualshock_dpad_stick == 3 ) {
		// leave alone
	}


	
	// remap d-pad to -other- stick
	if( stick_dpad_x && stick_dpad_y ) {
		if( dpad_up == 0 && dpad_left == 0 ) {
			*stick_dpad_x = -dualshock_mouse_up_left[ dualshock_wheel ];
			*stick_dpad_y = -dualshock_mouse_up_left[ dualshock_wheel ];
		}
		else if( dpad_up == 0 && dpad_right == 0 ) {
			*stick_dpad_x = +dualshock_mouse_up_right[ dualshock_wheel ];
			*stick_dpad_y = -dualshock_mouse_up_right[ dualshock_wheel ];
		}
		else if( dpad_down == 0 && dpad_left == 0 ) {
			*stick_dpad_x = -dualshock_mouse_down_left[ dualshock_wheel ];
			*stick_dpad_y = +dualshock_mouse_down_left[ dualshock_wheel ];
		}
		else if( dpad_down == 0 && dpad_right == 0 ) {
			*stick_dpad_x = +dualshock_mouse_down_right[ dualshock_wheel ];
			*stick_dpad_y = +dualshock_mouse_down_right[ dualshock_wheel ];
		}
		else {
			if( dpad_up == 0 ) {
				*stick_dpad_x = 0;
				*stick_dpad_y = -dualshock_mouse_up[ dualshock_wheel ];
			}
			else if( dpad_down == 0 ) {
				*stick_dpad_x = 0;
				*stick_dpad_y = +dualshock_mouse_down[ dualshock_wheel ];
			}


			if( dpad_left == 0 ) {
				*stick_dpad_x = -dualshock_mouse_left[ dualshock_wheel ];
				*stick_dpad_y = 0;
			}
			else if( dpad_right == 0 ) {
				*stick_dpad_x = +dualshock_mouse_right[ dualshock_wheel ];
				*stick_dpad_y = 0;
			}
		}



		// clamp now
		if( *stick_dpad_x < -0x80 ) *stick_dpad_x = -0x80;
		if( *stick_dpad_y < -0x80 ) *stick_dpad_y = -0x80;
		if( *stick_dpad_x > 0x7f ) *stick_dpad_x = 0x7f;
		if( *stick_dpad_y > 0x7f ) *stick_dpad_y = 0x7f;
	}

	// ###################################
	// ###################################
	// ###################################

	// map mouse to joypad (active low)
	if( mouse_left & 0x80 )
		*dualshock_pad &= ~dualshock_button_left;

	if( mouse_right & 0x80 )
		*dualshock_pad &= ~dualshock_button_right;

	if( mouse_middle & 0x80 )
		*dualshock_pad &= ~dualshock_button_middle;
}


void __stdcall Guncon_Commands( int cmd, u8 *ptr )
{
	guncon_command = cmd & 0xff;


	switch( cmd & 0xff ) {
		case 0x42:
			break;


		default:
			// invalid command - bad data?
			// - Point Blank 3
			// - Gunfighter - Jesse James

			// return ID + $ff
			*(ptr+2) = 0xff;
			break;
	}
}


void __stdcall Negcon_Commands( int cmd, u8 *ptr )
{
	switch( cmd & 0xff ) {
		case 0x43:
			// Wipeout 3 - ID (analog)
			*(ptr+1) = 0x79;
			break;

		case 0x45:
			// Wipeout 3 - ID (config)
			*(ptr+1) = 0xf3;
			break;
	}
}



void __stdcall mouse_sensitivity()
{
	int *raw_x, *raw_y;


	// raw movement
	raw_x = (int *) 0x4c0930;
	raw_y = (int *) 0x4c0934;


	// scale before clamping (0.0001-1000.0)
	if( *((char *)0x4bdcb0) == 2 || *((char *)0x4bdcf0) == 2 ) {
		*raw_x = (int)( (float) (*raw_x) * mouse_sensitive_x / 100.0f );
		*raw_y = (int)( (float) (*raw_y) * mouse_sensitive_y / 100.0f );
	}
	else if( *((char *)0x4bdcb0) == 3 || *((char *)0x4bdcf0) == 3 ) {
		*raw_x = (int)( (float) (*raw_x) * guncon_sensitive_x / 100.0f );
		*raw_y = (int)( (float) (*raw_y) * guncon_sensitive_y / 100.0f );
	}
}


void __stdcall analog_deadzone( u8 *output_args )
{
	u8 *analog_lx;
	u8 *analog_ly;
	u8 *analog_rx;
	u8 *analog_ry;

	analog_rx = output_args + 5;
	analog_ry = output_args + 6;
	analog_lx = output_args + 7;
	analog_ly = output_args + 8;


	// Ape Escape - doesn't like this for +x

	// $80 -> $7f center
	if( (*analog_lx) > 0 && (*analog_lx) <= 0x80 ) (*analog_lx)--;
	if( (*analog_ly) > 0 && (*analog_ly) <= 0x80 ) (*analog_ly)--;
	if( (*analog_rx) > 0 && (*analog_rx) <= 0x80 ) (*analog_rx)--;
	if( (*analog_ry) > 0 && (*analog_ry) <= 0x80 ) (*analog_ry)--;


	if( *analog_lx < 127 ) {
		// lx < 0
		if( (*analog_lx) - analog_deadzone_lx_1 >= 127 ) *analog_lx = 127;
	} else if( *analog_lx > 127 ) {
		// lx > 0
		if( (*analog_lx) - analog_deadzone_lx_2 <= 127 ) *analog_lx = 127;
	}


	if( *analog_ly < 127 ) {
		// ly < 0
		if( (*analog_ly) - analog_deadzone_ly_1 >= 127 ) *analog_ly = 127;
	} else {
		// ly > 0
		if( (*analog_ly) - analog_deadzone_ly_2 <= 127 ) *analog_ly = 127;
	}


	if( *analog_rx < 127 ) {
		// rx < 0
		if( (*analog_rx) - analog_deadzone_rx_1 >= 127 ) *analog_rx = 127;
	} else if( *analog_rx > 127 ) {
		// rx > 0
		if( (*analog_rx) - analog_deadzone_rx_2 <= 127 ) *analog_rx = 127;
	}


	if( *analog_ry < 127 ) {
		// ry < 0
		if( (*analog_ry) - analog_deadzone_ry_1 >= 127 ) *analog_ry = 127;
	} else {
		// ry > 0
		if( (*analog_ry) - analog_deadzone_ry_2 <= 127 ) *analog_ry = 127;
	}
}


void __stdcall Restart_Memcards()
{
	memcard_reset = 1;
}


void __stdcall Disable_Memcard()
{
	// permanently disable memcards
	if( disable_memcards == 1 ) {

		// Lifeforce Tenka - no hardware found (no output)
		if( ePSXe_version == 170 ) {
			*((char *)(0x507BE1)) = 0;
		}
		if( ePSXe_version == 180 ) {
			*((char *)(0x911481)) = 0;
		}

		memcard_reset = 1;
		return;
	}


	if( ePSXe_version == 170 )
	{
		// disable memcard1
		if( strcmp( (char *)0xC0FB40, "none" ) == 0 &&
			((*((short *)0x5079C8) & 0x2000) == 0) )
		{

				// Lifeforce Tenka - no hardware found (no output)
				*((char *)(0x507BE1)) = 0;
				return;
		}

		// disable memcard2
		if( strcmp( (char *)0xC0FF40, "none" ) == 0 &&
			((*((short *)0x5079C8) & 0x2000) != 0) ) 
		{

			// Lifeforce Tenka - no hardware found (no output)
			*((char *)(0x507BE1)) = 0;


			return;
		}

	}

	if( ePSXe_version == 180 )
	{
		// disable memcard1
		if( strcmp( (char *)0xD26820, "none" ) == 0 &&
			((*((short *)0x911268) & 0x2000) == 0) )
		{

			// Lifeforce Tenka - no hardware found (no output)
			*((char *)(0x911481)) = 0;
			return;
		}

		// disable memcard2
		if( strcmp( (char *)0xD26C20, "none" ) == 0 &&
			((*((short *)0x911268) & 0x2000) != 0) ) 
		{

			// Lifeforce Tenka - no hardware found (no output)
			*((char *)(0x911481)) = 0;

			return;
		}
	}


	// not plugged in
	if( memcard_reset == 1 ) 
	{
		static int count = 0;


		// Lifeforce Tenka - no hardware found (no output)
		if( ePSXe_version == 170 ) {
			*((char *)(0x507BE1)) = 0;
		}

		if( ePSXe_version == 180 ) {
			*((char *)(0x911481)) = 0;
		}


		// disable memcards for few calls
		count++;
		if( count > 3 ) {
			memcard_reset = 2;

			count = 0;
		}

		
		return;
	}
}


void __stdcall Reset_Memcards( u8 *card_flag )
{
	switch( memcard_reset ) {

	// plugged in - starting..?
	case 2:
		(*card_flag) |= 0x04;

		memcard_reset = 3;
		break;


	// plugged in - checking..?
	case 3:
		(*card_flag) |= 0x08;

		memcard_reset = 0;
		break;


	// plugged in - ready
	default:
		break;
	}
}
