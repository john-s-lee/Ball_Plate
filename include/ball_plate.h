#include <math.h>  //Required for Accelerometer Calculations
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //usleep() read() write() etc.
#include <bluetooth/bluetooth.h>   //Used by cwiid library
#include <cwiid.h>   //Cwiid Library

//  Redefine CWIID Buttons as we have changed the orientation of the wiimote
#define DOWN_BUTTON CWIID_BTN_LEFT
#define UP_BUTTON CWIID_BTN_RIGHT
#define LEFT_BUTTON CWIID_BTN_UP
#define RIGHT_BUTTON CWIID_BTN_DOWN


#define STABLE_MODE 0
#define MANUAL_MODE 1
#define CIRCLE_MODE 2
#define SQUARE_MODE 3
#define PI 3.14159
#define X_SERVO_CENTRE 1512.5
#define Y_SERVO_CENTRE 1458

// PID Parameters
#define KC -0.5219
#define TAU_I 1.5605
#define TAU_D 0.6270
#define TAU_F 0.1465
#define DELTA_T 50  //DeltaT in ms

extern int next_mode, mode, two_button_pressed, touchscreen_touched;
extern unsigned char rpt_mode;
extern double x_cord, y_cord;

int set_bit(int number, int bit);
int toggle_bit(int number, int bit);
int check_bit(int number, int bit_to_check);
int clear_bit(int number, int bit);