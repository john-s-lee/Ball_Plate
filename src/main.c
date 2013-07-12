#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <cwiid.h>
#include "../include/playsound.h"
#include "../include/manual_mode.h"
#include "../include/ball_plate.h"
#include "../include/wiimote_stuff.h

#define DOWN_BUTTON 0x0100
#define UP_BUTTON 0x0200
#define TWO_BUTTON 0x0001
#define STABLE_MODE 0
#define MANUAL_MODE 1
#define CIRCLE_MODE 2
#define SQUARE_MODE 3


//************   Function Prototypes ************/
//void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], struct timespec *timestamp);

//**************  Global Variables **************/
cwiid_mesg_callback_t cwiid_callback;
cwiid_err_t err;
int next_mode, mode;
int two_button_pressed;
unsigned char rpt_mode = 0;

//**************   Main Function   **************/
int main(int argc, char * argv[])
{
	mode = STABLE_MODE;
	next_mode = 0;
	cwiid_wiimote_t *wiimote;	/* wiimote handle */
	struct cwiid_state state;	/* wiimote state */
	bdaddr_t bdaddr;	/* bluetooth device address */
	unsigned char led_state = 0;
	pthread_t t1;

	cwiid_set_err(err);
	str2ba("00:1B:EA:00:E3:2F", &bdaddr);  //MAC address of wiimote if known

	//Connect to wiimote
	printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
	playsound("sounds/pair_remote.wav");
	while (!(wiimote = cwiid_open(&bdaddr, 0))) fprintf(stderr, "Unable to connect to wiimote\n");
	printf("Wiimote Connected.\n\n");

	//Set wiimote callback function
	if (cwiid_set_mesg_callback(wiimote, cwiid_callback))
	{
		fprintf(stderr, "Unable to set message callback\n");
		return 0;
	}

	toggle_bit(rpt_mode, CWIID_RPT_BTN);
	set_rpt_mode(wiimote, rpt_mode);  //enable button pressed messages for callback

	if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) 
	{
		fprintf(stderr, "Error enabling messages\n");
		return 0;
	}
	


	while (1)
	{
		if (mode == STABLE_MODE)
		{
			printf("Initialising Ball Stabilization Mode\n");
			led_state = 0;
			toggle_bit(led_state, CWIID_LED1_ON);
			set_led_state(wiimote, led_state);
			while (!next_mode)
			{
				printf("Executing Ball Stabilization Mode\n");
				sleep(1);
			}

			printf("Leaving Ball Stabilization Mode\n\n");
			next_mode = 0;
		}

		if (mode == MANUAL_MODE)
		{
			printf("Initialising Manual Control Mode\n");
			playsound("sounds/manual_start.wav");
			led_state = 0;
			toggle_bit(led_state, CWIID_LED2_ON);
			set_led_state(wiimote, led_state);
			two_button_pressed = 0;
			printf("Waiting for 2 Button on Wiimote to be pressed......\n");
			while (!next_mode)
			{
				if (two_button_pressed)  //Do this section once 2 button is pressed.
				{
					playsound("sounds/bodyjar.wav");
					manual_mode(wiimote);
				}
			}

			printf("Leaving Manual Control Mode\n\n");
			stopsound();
			next_mode = 0;
		}

		if (mode == CIRCLE_MODE)
		{
			printf("Initialising Circle Mode\n");
			led_state = 0;
			toggle_bit(led_state, CWIID_LED3_ON);
			set_led_state(wiimote, led_state);
			while (!next_mode)
			{
				printf("Executing Circle Mode\n");
				sleep(1);
			}

			printf("Leaving Circle Mode\n\n");
			next_mode = 0;
		}

		if (mode == SQUARE_MODE)
		{
			printf("Initialising Sqaure Mode\n");
			led_state =0;
			toggle_bit(led_state, CWIID_LED4_ON);
			set_led_state(wiimote, led_state);
			while (!next_mode)
			{
				printf("Executing Square Mode\n");
				sleep(1);
			}

			printf("Leaving Square Mode\n\n");
			next_mode =0;
		}
		
		
	}
	
	return 0;
}


