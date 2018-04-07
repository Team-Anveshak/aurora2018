#define dir1 31
#define pwm1 2
#define slp1 33
#define dir2 35
#define pwm2 3
#define slp2 37
#define dir3 39
#define pwm3 10
#define slp3 41
#define dir4 43
#define pwm4 5
#define slp4 45

#define ldir 29
#define lpwm 6
#define rdir 27
#define rpwm 7

#define lpotPin A8
#define rpotPin A9

int lf = 0,rf = 0,lb = 0,rb = 0;
float lpot = 0,rpot = 0; 

int rot_mode=0,right_steer = 300,left_steer = 300;
bool state=1;
int mode=0;

int set_r_zero = 300,set_l_zero = 300 ,set_r_angle =552,set_l_angle =528;
int left_steer_vel = 0,right_steer_vel = 0 ;

int flag =0;
int flag2 =0;
int angle_tolerance = 10;

