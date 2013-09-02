#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <cwiid.h>
#include "../include/playsound.h"
#include "../include/manual_mode.h"
#include "../include/ball_plate.h"
#include "../include/wiimote_stuff.h"
#include "../include/touchscreen.h"
#include "../include/pid.h"
#include "../include/resonant.h"


//**************  Global Variables **************/
cwiid_mesg_callback_t cwiid_callback;
cwiid_err_t err;
int next_mode, mode;
int two_button_pressed = 0;
int one_button_pressed = 0;
unsigned char rpt_mode = 0;
unsigned char up_button_pressed = 0;
unsigned char down_button_pressed = 0;
unsigned char left_button_pressed = 0;
unsigned char right_button_pressed = 0;
double t_measuredy = 0;
double t_measuredx = 0;
double measuredx_dot = 0;
double measuredy_dot = 0;
double measuredx_dot_dot = 0;
double measuredy_dot_dot = 0;

//**************   Main Function   **************/
int main(int argc, char * argv[])
{
	mode = CIRCLE_MODE;
	next_mode = 0;
	cwiid_wiimote_t *wiimote;	/* wiimote handle */
	struct cwiid_state state;	/* wiimote state */
	bdaddr_t bdaddr;	/* bluetooth device address */
	unsigned char led_state = 0;

	init_touchscreen();  //initialise touchscreen information (see touchscreen.c)

	cwiid_set_err(err);
	//bdaddr = *BDADDR_ANY;  //MAC Address of wiimote if unknown
	str2ba("00:1B:EA:00:E3:2F", &bdaddr);  //MAC address of wiimote if known

	//Connect to wiimote
	printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
	playsound("/usr/share/sounds/ball_plate/greeting.wav");
	while (!(wiimote = cwiid_open(&bdaddr, 0))) fprintf(stderr, "Unable to connect to wiimote\n");
	printf("Wiimote Connected.\n\n");

	//Set wiimote callback function
	if (cwiid_set_mesg_callback(wiimote, cwiid_callback))
	{
		fprintf(stderr, "Unable to set message callback\n");
		return 0;
	}


	rpt_mode = toggle_bit(rpt_mode, CWIID_RPT_BTN);
	set_rpt_mode(wiimote, rpt_mode);  //enable button pressed messages for callback

	if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) 
	{
		fprintf(stderr, "Error enabling messages\n");
		return 0;
	}
	
	cwiid_enable(wiimote, CWIID_FLAG_CONTINUOUS);

	while (1)
	{
		if (mode == STABLE_MODE)
		{
			printf("Initialising Ball Stabilization Mode\n");
			playsound("/usr/share/sounds/ball_plate/disturbance.wav");
			led_state = 0;
			led_state = toggle_bit(led_state, CWIID_LED1_ON);
			set_led_state(wiimote, led_state);
			printf("Executing Ball Stabilization Mode\n");
			stable_mode();
			printf("Leaving Ball Stabilization Mode\n\n");
			next_mode = 0;
		}

		if (mode == MANUAL_MODE)
		{
			printf("Initialising Manual Control Mode\n");
			playsound("/usr/share/sounds/ball_plate/manual.wav");
			led_state = 0;
			led_state=toggle_bit(led_state, CWIID_LED2_ON);
			set_led_state(wiimote, led_state);
			manual_mode(wiimote);
			printf("Leaving Manual Control Mode\n\n");
			next_mode = 0;
		}

		if (mode == CIRCLE_MODE)
		{
			printf("Initialising Circle Mode\n");
			playsound("/usr/share/sounds/ball_plate/resonant.wav");
			led_state = 0;
			led_state=toggle_bit(led_state, CWIID_LED3_ON);
			set_led_state(wiimote, led_state);
			
			circle_mode();		
			return 0;
			printf("Leaving Circle Mode\n\n");
			next_mode = 0;
		}
		
		if (mode == MAZE_MODE)
		{
			printf("Initialising Sqaure Mode\n");
			led_state =0;
			led_state=toggle_bit(led_state, CWIID_LED4_ON);
			set_led_state(wiimote, led_state);
			printf("Executing Square Mode\n");
			maze_mode();
			printf("Leaving Square Mode\n\n");
			next_mode =0;
			return 0;
		}
		
		
	}
	
	return 0;
}


int set_bit(int number, int bit)
{
	number |= bit;
	return number;
}

int toggle_bit(int number, int bit)
{
	number ^= bit;
	return number;
}

int check_bit(int number, int bit_to_check)
{
	int bit = number & bit_to_check;
		return bit;
}

int clear_bit(int number, int bit)
{
	number &= ~(bit);
}
