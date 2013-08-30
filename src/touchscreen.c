#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "../include/ball_plate.h"
#include "../include/touchscreen.h"

double x_cord, y_cord;
int touchscreen_touched;

void init_touchscreen()
{
	x_cord = 0;
	y_cord = 0;
	touchscreen_touched = 0;
	pthread_t t1;
	pthread_create( &t1, NULL, touchscreen_process, NULL); // create a thread running touchscreen_process()
}

void * touchscreen_process()
{
	struct input_event ev;
	int fd, i;
	fd=open("/dev/input/eGalax",O_RDONLY);

	double t_measuredx_past = 0;
	double t_measuredy_past = 0;
	double x_cord_past = 0;
	double y_cord_past = 0;
	double x_dot_past = 0;
	double y_dot_past = 0;
	
	struct timeval tim;


	while(1)
	{
			read(fd, &ev, sizeof(struct input_event));
			if (ev.type == EV_ABS && ev.code==ABS_X)
			{
				x_dot_past = measuredx_dot;
				x_cord_past = x_cord;
				x_cord =((double)ev.value-2018)/11.427;
				t_measuredx_past = t_measuredx;
				gettimeofday(&tim, NULL);
				t_measuredx=ev.time.tv_sec+(ev.time.tv_usec/1000000.0);
				measuredx_dot = (x_cord/1000 - x_cord_past/1000)/(t_measuredx - t_measuredx_past);
				measuredx_dot_dot = (measuredx_dot - x_dot_past)/(t_measuredx - t_measuredx_past);
				
			}
 			if (ev.type == EV_ABS && ev.code==ABS_Y)
			{
				y_cord_past = y_cord;
				y_cord = ((double)-ev.value+2070)/14.694;  //2046
				t_measuredy_past = t_measuredy;
				gettimeofday(&tim, NULL);
				t_measuredy=ev.time.tv_sec+(ev.time.tv_usec/1000000.0);		
				measuredy_dot = (y_cord/1000 - y_cord_past/1000)/(t_measuredy - t_measuredy_past);
			}
			if (ev.type == EV_KEY && ev.code==BTN_TOUCH) touchscreen_touched = ev.value;
	}

}
