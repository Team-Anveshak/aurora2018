/* rosserial Subscriber For Locomotion Control*/
#define USE_USBCON
#include <ros.h>
#include <rover_msgs/WheelVelocity.h>

#define dir1 31
#define pwm1 2
#define slp1 33
#define dir2 35
#define pwm2 3
#define slp2 37
#define dir3 39
#define pwm3 4
#define slp3 41
#define dir4 43
#define pwm4 5
#define slp4 45

#define ldir 29
#define lpwm 6
#define rdir 27
#define rpwm 7

#define lpotPin A6
#define rpotPin A7

int rot = 0,orot = 0;
int tl = 0,tr = 0, bl = 0, br = 0;
float lt = 0,rt = 0,lb = 0,rb = 0;
float lpot = 0,rpot = 0,set = 0; 

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

void rotate(int set)
{
  while(abs(lpot-set)>5 || abs(rpot-set)>5){
    if(abs(lpot-set)>5){
      if((lpot-set)<5){
        digitalWrite(ldir,HIGH);
        analogWrite(lpwm,100);
      }
      else if((lpot-set)>5){
        digitalWrite(ldir,LOW);
        analogWrite(lpwm,100);
      }
    }
    else{
      digitalWrite(ldir,LOW);
      analogWrite(lpwm,0);
    }
    if(abs(rpot-set)>5){
      if((rpot-set)<5){
        digitalWrite(rdir,HIGH);
        analogWrite(rpwm,100);
      }
      else if((rpot-set)>5){
        digitalWrite(rdir,LOW);
        analogWrite(rpwm,100);
      }
    }
    else{
      digitalWrite(rdir,LOW);
      analogWrite(rpwm,0);
    }
    lpot = analogRead(lpotPin);
    rpot = analogRead(rpotPin);
  }
}

void roverMotionCallback(const rover_msgs::WheelVelocity& RoverVelocity){
  lt = map(RoverVelocity.left_front_vel,-70,70,-175,175);
  rt = map(RoverVelocity.right_front_vel,-70,70,-175,175);
  lb = map(RoverVelocity.left_back_vel,-70,70,-175,175);
  rb = map(RoverVelocity.right_back_vel,-70,70,-175,175);
  rot = RoverVelocity.rot ;
 }
 
 ros::Subscriber<rover_msgs::WheelVelocity> locomotion_sub("loco/wheel_vel", &roverMotionCallback);
 
void setup(){
  nh.initNode();
  nh.subscribe(locomotion_sub);
  
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
  
  nh.advertise(vel_pub);
 
}
 
void loop(){
  nh.spinOnce();
  
  tl = (int)lt;///k1*mink;
  tr = (int)rt;///k1*mink;
  bl = (int)lb;///k3*mink;
  br = (int)rb;///k3*mink;
  
  if(rot == orot){
  digitalWrite(slp1,HIGH);
  digitalWrite(slp2,HIGH);
  digitalWrite(slp3,HIGH);
  digitalWrite(slp4,HIGH);
  loco(tl,dir1,pwm1,slp1);
  loco(tr,dir2,pwm2,slp2);
  loco(bl,dir3,pwm3,slp3);
  loco(br,dir4,pwm4,slp4);
  }
  else if(rot == 0){
    set = 500 ;
    rotate(set);
  }
  else if(rot == -1){
    set = 200 ;
    rotate(set);    
  }
  else if(rot == 1){
    set = 800 ;
    rotate(set);
  }
  
  RoverVel.left_front_vel=tl;
  RoverVel.right_front_vel=tr;
  RoverVel.left_back_vel=bl;
  RoverVel.right_back_vel=br;
  //RoverVel.rot = rot;
  RoverVel.rot = lpot;
  orot = rot;
  
  vel_pub.publish(&RoverVel);
  
  delay(1);
}
