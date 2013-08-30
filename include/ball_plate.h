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
#define X_SERVO_CENTRE 1569
#define Y_SERVO_CENTRE 1492

// PID Parameters
#define KC -1.5982
#define TAU_I 0.8917
#define TAU_D 0.3583
#define TAU_F 0.0837
#define DELTA_T 10  //DeltaT in ms


//Circle PID Parameters
#define KC_CIRCLE -0.8154
#define TAU_I_CIRCLE 1.2484
#define TAU_D_CIRCLE 0.5016
#define TAU_F_CIRCLE 0.1172

#define UMAX 0.261
#define UMIN -0.261

extern int next_mode, mode, two_button_pressed, touchscreen_touched, one_button_pressed;
extern unsigned char rpt_mode, up_button_pressed, down_button_pressed, left_button_pressed, right_button_pressed;
extern double x_cord, y_cord, t_measuredx, t_measuredy, measuredx_dot, measuredy_dot;
extern double measuredx_dot_dot, measuredy_dot_dot;

int set_bit(int number, int bit);
int toggle_bit(int number, int bit);
int check_bit(int number, int bit_to_check);
int clear_bit(int number, int bit);
