#include "../include/ball_plate.h"
#include "../include/cwiid_callback.h"

//**************  Wiimote Callback Function   ************************/
void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                    union cwiid_mesg mesg[], struct timespec *timestamp)
{
	int i, j;

	for (i=0; i < mesg_count; i++)
	{
		if (mesg[i].type == CWIID_MESG_BTN)
		{

			if (mesg[i].btn_mesg.buttons & DOWN_BUTTON) 
			{
					printf("Down Button Pressed!!\n");
					mode = (mode+1)%4;
					next_mode++;
			}

			if (mesg[i].btn_mesg.buttons & UP_BUTTON)
			{
					printf("Up Button Pressed!!\n");
					if (mode == 0) mode = 3;
					else mode--;
					next_mode++;
			}

			if (mesg[i].btn_mesg.buttons & TWO_BUTTON)
			{
					printf("2 Button Pressed!!\n");
					two_button_pressed = 1;
			}

		}

		
	}
}