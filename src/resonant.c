
#include <stdio.h>
#include <sys/time.h>
#include "pid.h"
#include "../include/ball_plate.h"
#include "../include/micro_maestro.h"
#include "../include/resonant.h"


void circle_mode()
{
	int fd = init_maestro();
	int target; //Micro Maestro target value
	double t_start, t_xpast, t_xcurr, t_ypast, t_ycurr;
	struct timeval tim;
	double r_x;
	double r_y;
	float w0 = 0.3491;
	int i = 0;

	float numd[] = {-1.6231,    6.4457,   -9.5994,    6.3540,   -1.5773};
	float dend[] = {1.0000,   -3.8735,    5.6205,   -3.6205,    0.8735};
	float e_x[] = {0, 0, 0, 0, 0, 0};
	float u_x[] = {0, 0, 0, 0, 0, 0};
	float e_y[] = {0, 0, 0, 0, 0, 0};
	float u_y[] = {0, 0, 0, 0, 0, 0};

	double cycles = 0;

	gettimeofday(&tim, NULL);
	t_start=tim.tv_sec+(tim.tv_usec/1000000.0); 
	t_ypast = 0;

	
	while(!next_mode)
	{
		r_x = 0.07*sin(w0*cycles*RES_DELTAT/2000);   //calculate setpoint signal;
		for (i = 5; i > 0; i--) e_x[i] = e_x[i-1]; 
		for (i = 5; i > 0; i--) u_x[i] = u_x[i-1];

		e_x[0] = r_x - x_cord/1000;
		u_x[0] =  u_x[1] + numd[0]*e_x[0]  +numd[1]*e_x[1]  +numd[2]*e_x[2]  +numd[3]*e_x[3] +numd[4]*e_x[4] - dend[1]*u_x[1] - dend[2]*u_x[2] - dend[3]*u_x[3] - dend[4]*u_x[4] - (numd[0]*e_x[1]  +numd[1]*e_x[2]  +numd[2]*e_x[3]  +numd[3]*e_x[4] +numd[4]*e_x[5] - dend[1]*u_x[2] - dend[2]*u_x[3] - dend[3]*u_x[4] - dend[4]*u_x[5]);

		//Output Control Signal
		target=(int)((u_x[0]*(2.4*-180/PI))*40+(4*X_SERVO_CENTRE));
		maestroSetTarget(fd, 0, target);
		//printf("Test Control Signal u_act_x = %f degrees\n", x.u_act*(180/PI));

		t_xpast = t_xcurr;

		gettimeofday(&tim, NULL);
		t_ycurr=tim.tv_sec+(tim.tv_usec/1000000.0);


		nanosleep((struct timespec[]){{0, ((RES_DELTA-(t_ycurr-t_ypast)) * 1000000L)}}, NULL);
		
	
		gettimeofday(&tim, NULL);
		t_ycurr=tim.tv_sec+(tim.tv_usec/1000000.0);

		cycles ++;

		r_y = 0.07*sin(w0*cycles*RES_DELTAT/2000);   //calculate setpoint signal;
		printf("r_y = %f \n", 0.07*sin(w0*cycles*RES_DELTAT/2000));
		for (i = 5; i > 0; i--) e_y[i] = e_y[i-1];
		for (i = 5; i > 0; i--) u_y[i] = u_y[i-1];

		e_y[0] = r_y - y_cord/1000;
		printf("ey 0 = %f\n", e_y[0]);
		u_y[0] =  u_y[1] + numd[0]*e_y[0]  +numd[1]*e_y[1]  +numd[2]*e_y[2]  +numd[3]*e_y[3] +numd[4]*e_y[4] - dend[1]*u_y[1] - dend[2]*u_y[2] - dend[3]*u_y[3] - dend[4]*u_y[4] - (numd[0]*e_y[1]  +numd[1]*e_y[2]  +numd[2]*e_y[3]  +numd[3]*e_y[4] +numd[4]*e_y[5] - dend[1]*u_y[2] - dend[2]*u_y[3] - dend[3]*u_y[4] - dend[4]*u_y[5]);		

		//Output control signal
		target=(int)(u_y[0]*(2.4*-180/PI)*40+(4*Y_SERVO_CENTRE));
		maestroSetTarget(fd, 1, target);
		//printf("Test Control Signal u_act_y = %f degrees\n", y.u_act*(180/PI));
		t_ypast = t_ycurr;

		gettimeofday(&tim, NULL);
		t_xcurr=tim.tv_sec+(tim.tv_usec/1000000.0);

		nanosleep((struct timespec[]){{0, ((RES_DELTA-(t_ycurr-t_ypast)) * 1000000L)}}, NULL);
	
		gettimeofday(&tim, NULL);
		t_xcurr=tim.tv_sec+(tim.tv_usec/1000000.0);

	}

	close_maestro(fd);
	return;
}


