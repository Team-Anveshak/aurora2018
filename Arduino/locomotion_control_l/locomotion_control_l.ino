/* rosserial Subscriber For Locomotion Control*/
/*#define USE_USBCON*/
#include <ros.h>
#include <rover_msgs/WheelVelocity.h>

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

int rot = 0,orot = 0;
int tl = 0,tr = 0, bl = 0, br = 0;
float lt = 0,rt = 0,lb = 0,rb = 0;
float lpot = 0,rpot = 0,set = 0; 
int set_r, set_l;
int state=0;
int vel_state;
   



ros::NodeHandle nh;

rover_msgs::WheelVelocity RoverVel;
ros::Publisher vel_pub("motor/wheel_vel", &RoverVel);

void loco(int vel,int dir_pin,int pwm_pin,int slp_pin)
{
  if(vel<=0)
    {
     //digitalWrite(slp_pin,HIGH);
        digitalWrite(dir_pin,LOW);
        analogWrite(pwm_pin,abs(vel));
    }
      
     
  else
    { 
      //digitalWrite(slp_pin,HIGH);
      digitalWrite(dir_pin,HIGH); 
      analogWrite(pwm_pin,abs(vel));
    }
}

void rotate(int set_r,int set_l)
{
  
    if(abs(lpot-set_l)>20){
       state=0;
      if((lpot-set_l)<0){
        digitalWrite(ldir,HIGH);
        analogWrite(lpwm,100);
      }
      else if((lpot-set_l)>0){
        digitalWrite(ldir,LOW);
        analogWrite(lpwm,100);
      }
    }
    else{
      digitalWrite(ldir,LOW);
      analogWrite(lpwm,0);
    }

   
    if(abs(rpot-set_r)>20){
      state=0;
      
      if((rpot-set_r)<0){
        digitalWrite(rdir,HIGH);
        analogWrite(rpwm,100);
      }
      else if((rpot-set_r)>0){
        digitalWrite(rdir,LOW);
        analogWrite(rpwm,100);
      }
    }
    else{
      digitalWrite(rdir,LOW);
      analogWrite(rpwm,0);
    }
    if(abs(lpot-set_l)<20 && abs(lpot-set_l)<20)
    {
      state=1;
    }
    rpot = analogRead(rpotPin);
    lpot = analogRead(lpotPin);
}



void roverMotionCallback(const rover_msgs::WheelVelocity& RoverVelocity){
  lt = map(RoverVelocity.left_front_vel,-70,70,-150,150);
  rt = map(RoverVelocity.right_front_vel,-70,70,-150,150);
  lb = map(RoverVelocity.left_back_vel,-70,70,-150,150);
  rb = map(RoverVelocity.right_back_vel,-70,70,-150,150);
  rot = RoverVelocity.rot ;
 }
 
 ros::Subscriber<rover_msgs::WheelVelocity> locomotion_sub("loco/wheel_vel", &roverMotionCallback);
 
void setup(){
  nh.initNode();
  nh.subscribe(locomotion_sub);

  TCCR3B = TCCR3B & B11111000 | B00000001;    // set timer 3 divisor to     1 for PWM frequency of 31372.55 Hz  
  TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
  
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT); 
  
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(pwm4,OUTPUT);
  
  pinMode(lpotPin, INPUT);
  pinMode(rpotPin, INPUT);

  digitalWrite(slp1,HIGH);
  digitalWrite(slp2,HIGH);
  digitalWrite(slp3,HIGH);
  digitalWrite(slp4,HIGH);
  
  nh.advertise(vel_pub);
 
}
 
void loop(){
  nh.spinOnce();
  
  tl = (int)lt;
  tr = (int)rt;
  bl = (int)lb;
  br = (int)rb;
  
//  if(/*state==1*/1){  umcomment this when the linac pot are connected to the pcb

rpot = analogRead(rpotPin);
lpot = analogRead(lpotPin);

  loco(tl,dir1,pwm1,slp1);
  loco(tr,dir2,pwm2,slp2);
  loco(bl,dir3,pwm3,slp3);
  loco(br,dir4,pwm4,slp4);
  RoverVel.left_front_vel=rpot;
  RoverVel.right_front_vel=lpot;
  RoverVel.left_back_vel=bl;
  RoverVel.right_back_vel=br;
//  }
//  else{
//  loco(0,dir1,pwm1,slp1);    umcomment these also when the linac pot are connected to the pcb
//  loco(0,dir2,pwm2,slp2);
//  loco(0,dir3,pwm3,slp3);
//  loco(0,dir4,pwm4,slp4);
//    RoverVel.right_back_vel=0;
//  }
//  
  if(rot == 0){
    set_r = 500 ;
    set_l = 500 ;
    loco(tl,dir1,0,slp1);
    loco(tr,dir2,0,slp2);
    loco(bl,dir3,0,slp3);
    loco(br,dir4,0,slp4);
    rotate(set_r,set_l);
    
  }
  else if(rot == -1){
  
    set_r = 200 ;
    set_l = 200 ;
    
    rotate(set_r,set_l);
  }
  else if(rot == 1){
    
    set_r = 800 ;
    set_l = 800 ;
    rotate(set_r,set_l);
  }
  
  

  RoverVel.rot = rot;

  
  vel_pub.publish(&RoverVel);
  
  delay(1);
}
