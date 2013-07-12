#include "../include/ball_plate.h"
#include "../include/micro_maestro.h"

void manual_mode()
{
	struct cwiid_state state;	/* wiimote state */
	int fd = init_maestro();
	int target;

	/*Set wiimote to report accelerometer readings*/
	toggle_bit(rpt_mode, CWIID_RPT_ACC);
	if (cwiid_set_rpt_mode(wiimote, rpt_mode)) fprintf(stderr, "Error setting report mode\n");

	int i = 0;
	struct acc_cal wm_cal;
	cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &wm_cal);
	double a, a_x, a_y, a_z, pitch, roll;
	int mode = 0;

	while(!next_mode)
	{
		if (cwiid_get_state(wiimote, &state)) {
				fprintf(stderr, "Error getting state\n");
			}
	

		a_x = ((double)state.acc[CWIID_X] - wm_cal.zero[CWIID_X]) / (wm_cal.one[CWIID_X] - wm_cal.zero[CWIID_X]);
		a_y = ((double)state.acc[CWIID_Y] - wm_cal.zero[CWIID_Y]) / (wm_cal.one[CWIID_Y] - wm_cal.zero[CWIID_Y]);
		a_z = ((double)state.acc[CWIID_Z] - wm_cal.zero[CWIID_Z]) / (wm_cal.one[CWIID_Z] - wm_cal.zero[CWIID_Z]);
		a = sqrt(pow(a_x,2)+pow(a_y,2)+pow(a_z,2));
		roll = atan(a_x/a_z);
		if (a_z <= 0.0) {
		roll += PI * ((a_x > 0.0) ? 1 : -1);
		}
		roll *= -1;
		pitch = atan(a_y/a_z*cos(roll));
		roll = roll * (180/PI);
		pitch = pitch * (180/PI);

		if ((i/50) > 1) printf("\rTime in Manual Mode: %i Seconds,   Pitch: %0.2f,  Roll:  %0.2f                           ", i/50 , pitch, roll);
		fflush(stdout);
		if (mode ==0)
		{
			target=(int)(pitch*40+(4*1512.5));
			maestroSetTarget(fd, 0, target);
			mode = 1;
		}
		else
		{
			target=(int)(roll*40+(4*1458));
			maestroSetTarget(fd, 1, target);
			mode =0;
		}
		usleep(20000);
		i++;
	}
	
	printf("\n\n");

	close_maestro(fd);

	/*Cancel accelerometer readings*/
	toggle_bit(rpt_mode, CWIID_RPT_ACC);
	if (cwiid_set_rpt_mode(wiimote, rpt_mode)) fprintf(stderr, "Error setting report mode\n");

}