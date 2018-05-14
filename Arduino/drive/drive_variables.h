#define dirlf 31
#define pwmlf 2
#define slplf 33
#define dirlb 35
#define pwmlb 3
#define slplb 37
#define dirrf 39
#define pwmrf 5
#define slprf 41
#define dirrb 43
#define pwmrb 6
#define slprb 45

#define rmen 24 
#define rmINA 26 
#define rmINB 28
#define rmpwm 9
#define lmen 44 
#define lmINA 46 
#define lmINB 48
#define lmpwm 10
#define pan_pin 11
#define tilt_pin 12


#define lpotPin A9
#define rpotPin A8

int lf = 0,rf = 0,lb = 0,rb = 0;
float lpot = 0,rpot = 0; 

int rot_mode=0,right_steer = 300,left_steer = 300;
bool state=1;
int mode=0;

int set_r_zero = 380,set_l_zero = 395 ,set_r_angle =210,set_l_angle=200;
int left_steer_vel = 0,right_steer_vel = 0 ;

int flag =0;
int flag2 =0;
int angle_tolerance = 7;
int pan_pos=90,tilt_pos=90;
int pan_joy=0,tilt_joy=0;
float servo_now=0,servo_then=0;

