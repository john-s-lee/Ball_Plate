#include "../include/ball_plate.h"
#include "../include/micro_maestro.h"
#include "../include/pid.h"
#include "../include/playsound.h"

void manual_mode(cwiid_wiimote_t *wiimote)
{
	struct cwiid_state state;	/* wiimote state */
	int fd = init_maestro();
	int target;

	/*Set wiimote to report accelerometer readings*/
	rpt_mode=toggle_bit(rpt_mode, CWIID_RPT_ACC);
	if (cwiid_set_rpt_mode(wiimote, rpt_mode)) fprintf(stderr, "Error setting report mode\n");

	int i = 0;
	struct acc_cal wm_cal;
	cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &wm_cal);
	double a, a_x, a_y, a_z, pitch, roll;
	double deltaT_x, deltaT_y;
	double t_x_curr, t_x_past, t_y_curr, t_y_past;
	struct timeval tim;

	char man_mode_select = 0;

	//Initialise PID parameters for the x-axis and the wait or DELTA_T/2
	pid_params x = {KC, TAU_I, TAU_D, TAU_F, 0, 0, x_cord/1000, 0, 0, 0, 0, 0};  //initialise PID parameters for x axis
	gettimeofday(&tim, NULL);
	t_x_past=tim.tv_sec+(tim.tv_usec/1000000.0); //initialise t_x_past with current time (in seconds)


	wait_for_deltat(&tim, &t_x_curr, &t_x_past, &deltaT_x, DELTA_T/2); //Wait until Delta_T/2

	// Initialise  PID parameters for the y-axis
	pid_params y = {KC, TAU_I, TAU_D, TAU_F, 0, 0, y_cord/1000, 0, 0, 0, 0, 0}; //initialise PID paramaters for y axis
	gettimeofday(&tim, NULL);
	t_y_past=tim.tv_sec+(tim.tv_usec/1000000.0); //initialise t_y_past with current time (in seconds)


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
		pitch = atan(a_y/a_z*cos(roll));
		roll *= (-180/PI);
		pitch *= (180/PI);
		roll *= 0.5;
		pitch *= 0.5;

		if (two_button_pressed)
		{
			if (man_mode_select ==0){
				man_mode_select = 1;  //mode 0 is accelerometer mode 1 is keypad
					playsound("/usr/share/sounds/ball_plate/key_input.wav");}
			else{
			man_mode_select = 0;
				playsound("/usr/share/sounds/ball_plate/acc_input.wav");}
			two_button_pressed = 0;
		}

		if (left_button_pressed && man_mode_select == 1)
		{
			x.set_pt -= 0.005;
			printf("x= %f\n", x.set_pt);
			if (x.set_pt > 0.15) x.set_pt = 0.15;
			if (x.set_pt < -0.15) x.set_pt = -0.15;
			left_button_pressed = 0;
		}

		if (right_button_pressed && man_mode_select == 1)
		{
			x.set_pt += 0.005;
			printf("x= %f\n", x.set_pt);
			if (x.set_pt > 0.15) x.set_pt = 0.15;
			if (x.set_pt < -0.15) x.set_pt = -0.15;
			right_button_pressed = 0;
		}

		if (up_button_pressed && man_mode_select == 1)
		{
			y.set_pt += 0.005;
			printf("y= %f\n", y.set_pt);
			if (y.set_pt > 0.12) x.set_pt = 0.12;
			if (y.set_pt < -0.12) x.set_pt = -0.12;
			up_button_pressed = 0;
		}

		if (down_button_pressed && man_mode_select == 1)
		{
			y.set_pt -= 0.005;
			printf("y= %f\n", y.set_pt);
			if (y.set_pt > 0.12) x.set_pt = 0.12;
			if (y.set_pt < -0.12) x.set_pt = -0.12;
			down_button_pressed = 0;
		}

		if (man_mode_select == 0)
		{
			if ((int)pitch > 4 || (int)pitch < -4)
			{
				x.set_pt += -0.00005*pitch/10;
				if (x.set_pt > 0.15) x.set_pt = 0.15;
				if (x.set_pt < -0.15) x.set_pt = -0.15;
			}

			if ((int)roll > 4 || (int)roll < -4)
			{
				y.set_pt += -0.00005*roll/10;
				if (y.set_pt > 0.12) y.set_pt = 0.12;
				if (y.set_pt < -0.12) y.set_pt = -0.12;
			}
		}

		wait_for_deltat(&tim, &t_x_curr, &t_x_past, &deltaT_x, DELTA_T); //Wait until DELTA_T for x-axis
	
		x.pos_past = x.pos_curr;  //store past ball position
		x.pos_curr = x_cord/1000;  //current ball position is equal to the coordinates read from the touchscreen
		x.u_D_past = x.u_D;  //store past derivative control signal
		x.u_act_past = x.u_act;  //store past control signal
		x.error = (x.set_pt - x.pos_curr); //calculate error r(t) - y(t)
		t_x_past = t_x_curr;  //Save new time

		//Calculate new derivative term
		x.u_D = (x.tauF/(x.tauF+deltaT_x/1000))*x.u_D_past + ((x.kc*x.tauD)/(x.tauF+deltaT_x/1000))*(x.pos_curr - x.pos_past);
		//Caluclate new control signal
		x.u_act = x.u_act_past + x.kc*(-x.pos_curr + x.pos_past) + ((x.kc * deltaT_x/1000)/x.tauI)*(x.error) - x.u_D + x.u_D_past;

		if (x.u_act > UMAX) x.u_act = UMAX;
		if (x.u_act < UMIN) x.u_act = UMIN;

		//Output Control Signal
		target=(int)((x.u_act*(2.4*180/PI))*40+(4*X_SERVO_CENTRE));
		maestroSetTarget(fd, 0, target);
		//printf("Test Control Signal u_act_x = %f degrees\n", x.u_act*(180/PI));

		wait_for_deltat(&tim, &t_y_curr, &t_y_past, &deltaT_y, DELTA_T); //Get Accurate timings

		y.pos_past = y.pos_curr;  //store past ball position
		y.pos_curr = y_cord/1000;  //current ball position is equal to the coordinates read from the touchscreen
		y.u_D_past = y.u_D;  //store past derivative control signal
		y.u_act_past = y.u_act;
		y.error = (y.set_pt - y.pos_curr);
		t_y_past = t_y_curr;  //Save new time

		//Calculate new derivative term
		y.u_D = (y.tauF/(y.tauF+deltaT_y/1000))*y.u_D_past + ((y.kc*y.tauD)/(y.tauF+deltaT_y/1000))*(y.pos_curr - y.pos_past);
		//Caluclate new control signal
		y.u_act = y.u_act_past + y.kc*(-y.pos_curr + y.pos_past) + ((y.kc * deltaT_y/1000)/y.tauI)*(y.error) - y.u_D + y.u_D_past;

		if (x.u_act > UMAX) x.u_act = UMAX;
		if (x.u_act < UMIN) x.u_act = UMIN;

		//Output control signal
		target=(int)(y.u_act*(2.4*180/PI)*40+(4*Y_SERVO_CENTRE));
		maestroSetTarget(fd, 1, target);
		//printf("Test Control Signal u_act_y = %f degrees\n", y.u_act*(180/PI));



	}
	
	printf("\n\n");

	close_maestro(fd);

	/*Cancel accelerometer readings*/
	rpt_mode=toggle_bit(rpt_mode, CWIID_RPT_ACC);
	if (cwiid_set_rpt_mode(wiimote, rpt_mode)) fprintf(stderr, "Error setting report mode\n");

}
