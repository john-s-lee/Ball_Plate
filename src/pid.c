
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "pid.h"
#include "../include/ball_plate.h"
#include "../include/micro_maestro.h"

void wait_for_deltat(struct timeval *tim, double *t_curr, double *t_past, double *new_delta, double required_delta);  //no sleep (maintain cpu time)

void stable_mode()
{
	int fd = init_maestro();
	int target; //Micro Maestro target value
	double deltaT_x, deltaT_y;
	double t_x_curr, t_x_past, t_y_curr, t_y_past, t_curr;
	struct timeval tim;
	double r_act = 0;
	float x_pos[] = {-0.075, 0.075, 0.075, -0.075};
	float y_pos[] = {0.075, 0.075, -0.075, -0.075};
	int pos_current = 0;

	maestroSetSpeed(fd, 20);
	maestroSetTarget(fd, 1, 4*Y_SERVO_CENTRE);
	usleep(20000);
	maestroSetTarget(fd, 0, 4*X_SERVO_CENTRE);	
	usleep(400000);
	maestroSetSpeed(fd, 0);	
	
	
	one_button_pressed = 0;
	two_button_pressed = 0;

	gettimeofday(&tim, NULL);
	t_curr=tim.tv_sec+(tim.tv_usec/1000000.0);
	
	//Initialise PID parameters for the x-axis and the wait or DELTA_T/2
	//pid_params x = {KC, TAU_I, TAU_D, TAU_F, 0, 0, (x_cord+measuredx_dot*(t_curr-t_measuredx))/1000, 0, 0, 0, 0, 0};  //initialise PID parameters for x axis
	pid_params x = {KC, TAU_I, TAU_D, TAU_F, 0, 0, x_cord/1000, 0, 0, 0, 0, 0};
	gettimeofday(&tim, NULL);
	t_x_past=tim.tv_sec+(tim.tv_usec/1000000.0); //initialise t_x_past with current time (in seconds)


	wait_for_deltat(&tim, &t_x_curr, &t_x_past, &deltaT_x, DELTA_T/2); //Wait until Delta_T/2

	gettimeofday(&tim, NULL);
	t_curr=tim.tv_sec+(tim.tv_usec/1000000.0);
	// Initialise  PID parameters for the y-axis
	//pid_params y = {KC, TAU_I, TAU_D, TAU_F, 0, 0, (y_cord+measuredy_dot*(t_curr-t_measuredy))/1000, 0, 0, 0, 0, 0}; //initialise PID paramaters for y axis
	pid_params y = {KC, TAU_I, TAU_D, TAU_F, 0, 0, y_cord/1000, 0, 0, 0, 0, 0};
	gettimeofday(&tim, NULL);
	t_y_past=tim.tv_sec+(tim.tv_usec/1000000.0); //initialise t_y_past with current time (in seconds)


	while(!next_mode)
	{
		if (one_button_pressed)
		{
			x.set_pt = 0;
			y.set_pt = 0;
			one_button_pressed = 0;
		}

		if (two_button_pressed)
		{
			x.set_pt = x_pos[pos_current];
			y.set_pt = y_pos[pos_current];
			printf("x_pos = %f, y_pos = %f\n", x.set_pt, y.set_pt);
			pos_current++;
			pos_current = pos_current%4;
			two_button_pressed = 0;
		}



		wait_for_deltat(&tim, &t_x_curr, &t_x_past, &deltaT_x, DELTA_T); //Wait until DELTA_T for x-axis
	
		x.pos_past = x.pos_curr;  //store past ball position
		x.pos_curr=(x_cord+measuredx_dot*(t_x_curr-t_measuredx)+0.5*measuredx_dot_dot*pow((t_x_curr-t_measuredx),2.0))/1000;  //Get balls position from 2nd order estimator
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
		//y.pos_curr = (y_cord+measuredy_dot*(t_y_curr-t_measuredy))/1000;  //current ball position is equal to the coordinates read from the touchscreen
		y.pos_curr=(y_cord+measuredy_dot*(t_y_curr-t_measuredy)+0.5*measuredy_dot_dot*pow((t_y_curr-t_measuredy),2.0))/1000;
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

	close_maestro(fd);
	return;
}



void wait_for_deltat(struct timeval *tim, double *t_curr, double *t_past, double *new_delta, double required_delta)
{
		gettimeofday(tim, NULL);
		(*t_curr)=(tim->tv_sec)+(tim->tv_usec/1000000.0); 
		(*new_delta) = (*t_curr-*t_past)*1000;


		gettimeofday(tim, NULL);
		(*t_curr)=(tim->tv_sec)+(tim->tv_usec/1000000.0); 
		(*new_delta) = (*t_curr-*t_past)*1000;

		while(*new_delta < (required_delta-0.02)) //Wait till Delta_T/2 is reached
		{
			gettimeofday(tim, NULL);
			(*t_curr)=tim->tv_sec+(tim->tv_usec/1000000.0); 
			(*new_delta) = (*t_curr-*t_past)*1000;
		}
}
