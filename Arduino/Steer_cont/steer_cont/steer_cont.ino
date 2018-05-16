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

int rot = 0,rot2 = 0;
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

void rotate_steer_right(int vel)
{
  if(vel>0.8){
    
    digitalWrite(rdir,LOW);
    analogWrite(rpwm,100);
  }
  else if (vel< -0.8){

    digitalWrite(rdir,HIGH);
    analogWrite(rpwm,100);
  }
  
  else{
   
    digitalWrite(rdir,LOW);
    analogWrite(rpwm,0);
  }
}

void rotate_steer_left(int vel)
{
  if(vel>0.8){
    digitalWrite(ldir,HIGH);
    analogWrite(lpwm,100);
    
  }
  else if (vel<-0.8){
    digitalWrite(ldir,LOW);
    analogWrite(lpwm,100);
    
  }
  
  else{
     digitalWrite(ldir,LOW);
    analogWrite(lpwm,0);
    
  }
}

void roverMotionCallback(const rover_msgs::WheelVelocity& RoverVelocity){
  lt = map(RoverVelocity.left_front_vel,-70,70,-200,200);
  rt = map(RoverVelocity.right_front_vel,-70,70,-200,200);
  lb = map(RoverVelocity.left_back_vel,-70,70,-200,200);
  rb = map(RoverVelocity.right_back_vel,-70,70,-200,200);
  rot = RoverVelocity.rot ;
  rot2 = RoverVelocity.rots ;
  
  rotate_steer_left(int(rot2));
  rotate_steer_right(int(rot));

  
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

  pinMode(ldir,OUTPUT);
   pinMode(rdir,OUTPUT);
    pinMode(lpwm,OUTPUT);
     pinMode(rpwm,OUTPUT);
  
// int myEraser = 7;             // this is 111 in binary and is used as an eraser
//TCCR3B &= ~myEraser;  
//int myPrescaler = 1;         // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.   
//TCCR3B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
//     // this is 111 in binary and is used as an eraser

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
  
  loco(tl,dir1,pwm1,slp1);
  loco(tr,dir2,pwm2,slp2);
  loco(bl,dir3,pwm3,slp3);
  loco(br,dir4,pwm4,slp4);

  rpot = analogRead(rpotPin);
  lpot = analogRead(lpotPin);
  
  RoverVel.left_front_vel=rpot;
  RoverVel.right_front_vel=lpot;
  RoverVel.left_back_vel=bl;
  RoverVel.right_back_vel=br;
  RoverVel.rot = rot;
  RoverVel.rots = rot2;
  

  vel_pub.publish(&RoverVel);
  
  delay(1);
}
