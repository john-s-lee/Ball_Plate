#include <math.h>  //Required for Accelerometer Calculations
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //usleep() read() write() etc.
#include <bluetooth/bluetooth.h>   //Used by cwiid library
#include <cwiid.h>   //Cwiid Library

#define DOWN_BUTTON 0x0100
#define UP_BUTTON 0x0200
#define TWO_BUTTON 0x0001
#define STABLE_MODE 0
#define MANUAL_MODE 1
#define CIRCLE_MODE 2
#define SQUARE_MODE 3
#define PI 3.14159
#define toggle_bit(bf,b)	\
	(bf) = ((bf) & b)		\
	       ? ((bf) & ~(b))	\
	       : ((bf) | (b))

extern int next_mode, mode, two_button_pressed, touchscreen_touched;
extern unsigned char rpt_mode;
extern double x_cord, y_cord;

