#include <math.h>  //Required for Accelerometer Calculations
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //usleep() read() write() etc.
#include <bluetooth/bluetooth.h>   //Used by cwiid library
#include <cwiid.h>   //Cwiid Library

#define PI 3.14159
#define toggle_bit(bf,b)	\
	(bf) = ((bf) & b)		\
	       ? ((bf) & ~(b))	\
	       : ((bf) | (b))

extern int next_mode;
extern unsigned char rpt_mode;
extern cwiid_wiimote_t *wiimote;	/* wiimote handle */
