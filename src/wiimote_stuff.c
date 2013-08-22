#include "../include/ball_plate.h"
#include "../include/wiimote_stuff.h"


//**************  Wiimote Callback Function   ************************/
void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                    union cwiid_mesg mesg[], struct timespec *timestamp)
{
	int i, j;

	for (i=0; i < mesg_count; i++)
	{
		if (mesg[i].type == CWIID_MESG_BTN)
		{

			if (mesg[i].btn_mesg.buttons & CWIID_BTN_PLUS) 
			{
					printf("Plus Button Pressed!!\n");
					mode = (mode+1)%3;
					next_mode++;
			}

			if (mesg[i].btn_mesg.buttons & CWIID_BTN_MINUS)
			{
					printf("Minus Button Pressed!!\n");
					if (mode == 0) mode = 2;
					else mode--;
					next_mode++;
			}

			if (mesg[i].btn_mesg.buttons & DOWN_BUTTON) 
			{
					printf("Down Button Pressed!!\n");
					down_button_pressed = 1;
			}

			if (mesg[i].btn_mesg.buttons & UP_BUTTON)
			{
					printf("Up Button Pressed!!\n");
					up_button_pressed = 1;
			}

			if (mesg[i].btn_mesg.buttons & LEFT_BUTTON) 
			{
					printf("Left Button Pressed!!\n");
					left_button_pressed = 1;
			}

			if (mesg[i].btn_mesg.buttons & RIGHT_BUTTON)
			{
					printf("Right Button Pressed!!\n");
					right_button_pressed = 1;
			}

			if (mesg[i].btn_mesg.buttons & CWIID_BTN_2)
			{
					printf("2 Button Pressed!!\n");
					two_button_pressed = 1;
			}

			if (mesg[i].btn_mesg.buttons & CWIID_BTN_1)
			{
					printf("1 Button Pressed!!\n");
					one_button_pressed = 1;
			}

		}

		
	}
}

//*******************  Set Wiimote LED State Function **************/
void set_led_state(cwiid_wiimote_t *wiimote, unsigned char led_state)
{
	if (cwiid_set_led(wiimote, led_state)) {
		fprintf(stderr, "Error setting LEDs \n");
	}
}
	
//*******************   Set Wiimote Report Mode Function ***********/
void set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode)
{
	if (cwiid_set_rpt_mode(wiimote, rpt_mode)) {
		fprintf(stderr, "Error setting report mode\n");
	}
}

//*************** Wiimote Error Handling Function *****************/
void err(cwiid_wiimote_t *wiimote, const char *s, va_list ap)
{
	if (wiimote) printf("%d:", cwiid_get_id(wiimote));
	else printf("-1:");
	vprintf(s, ap);
	printf("\n");
}
