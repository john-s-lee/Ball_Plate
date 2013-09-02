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
#define MAZE_MODE 3
#define PI 3.14159
#define X_SERVO_CENTRE 1505
#define Y_SERVO_CENTRE 1530

// PID Parameters
#define KC -1.174205
#define TAU_I 1.040363
#define TAU_D 0.417995
#define TAU_F 0.097637
#define DELTA_T 10  //DeltaT in ms

//PID Fast Parameters
#define KC_FAST -1.598224
#define TAUI_FAST 0.891740
#define TAUD_FAST 0.358281
#define TAUF_FAST 0.083689


//Circle PID Parameters
#define KC_CIRCLE -0.8154
#define TAU_I_CIRCLE 1.2484
#define TAU_D_CIRCLE 0.5016
#define TAU_F_CIRCLE 0.1172

#define UMAX 0.261
#define UMIN -0.261 
#define DUMAX 5
#define DUMIN -5
#define DDUMAX 10
#define DDUMIN -10

extern int mute, next_mode, mode, two_button_pressed, touchscreen_touched, one_button_pressed;
extern unsigned char rpt_mode, up_button_pressed, down_button_pressed, left_button_pressed, right_button_pressed;
extern double x_cord, y_cord, t_measuredx, t_measuredy, measuredx_dot, measuredy_dot;
extern double measuredx_dot_dot, measuredy_dot_dot;

int set_bit(int number, int bit);
int toggle_bit(int number, int bit);
int check_bit(int number, int bit_to_check);
int clear_bit(int number, int bit);
