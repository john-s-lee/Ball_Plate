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
	int fd;
	fd=open("/dev/input/event5",O_RDONLY);

	while(1)
	{
			read(fd, &ev, sizeof(struct input_event));
			if (ev.type == EV_ABS && ev.code==ABS_X) x_cord = ev.value;
 			if (ev.type == EV_ABS && ev.code==ABS_Y) y_cord = ev.value;
			if (ev.type == EV_KEY && ev.code==BTN_TOUCH) touchscreen_touched = ev.value;
	}

}