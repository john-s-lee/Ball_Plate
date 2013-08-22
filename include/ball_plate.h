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
#define X_SERVO_CENTRE 1512.75
#define Y_SERVO_CENTRE 1574.25

// PID Parameters
#define KC -1.4208
#define TAU_I 0.9458
#define TAU_D 0.3800
#define TAU_F 0.0888
#define DELTA_T 10  //DeltaT in ms
#define RES_DELTAT 10  //Resonant delta t in ms
#define RES_DELTA 10  //Resonant delta t in ms
#define UMAX 0.261
#define UMIN -0.261

extern int next_mode, mode, two_button_pressed, touchscreen_touched, one_button_pressed;
extern unsigned char rpt_mode, up_button_pressed, down_button_pressed, left_button_pressed, right_button_pressed;
extern double x_cord, y_cord;

int set_bit(int number, int bit);
int toggle_bit(int number, int bit);
int check_bit(int number, int bit_to_check);
int clear_bit(int number, int bit);
