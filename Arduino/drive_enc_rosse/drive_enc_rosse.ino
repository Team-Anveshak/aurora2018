#include <ros.h>
#include <rover_msgs/WheelVelocity.h>
#include <rover_msgs/enc.h>

#define BAUD_RATE 57600
#define M_DIR1  37
#define M_DIR2  33
#define M_PWM1  8
#define M_PWM2  2
#define ENC1  9
#define ENC2  3



volatile long enc_1_tick=0;
volatile long enc_2_tick=0;
int pwm_value_1;
int pwm_value_2;

float left=0,right=0;



ros::NodeHandle nh;

rover_msgs::enc EncValue;
ros::Publisher enc_pub("Encoder", &EncValue);

void loco(int vel,int dir_pin,int pwm_pin)
{
if(vel<=0)
  {
   digitalWrite(dir_pin,LOW);
   analogWrite(pwm_pin,abs(vel));
  }
else
  { 
    digitalWrite(dir_pin,HIGH);
    analogWrite(pwm_pin,abs(vel));
  }
   
  
}


void roverMotionCallback(const rover_msgs::WheelVelocity& RoverVelocity){

  

  
  left = map(RoverVelocity.left,-70,70,-175,175);
  right = map(RoverVelocity.right,-70,70,-175,175);
  

   
 

  loco(left,M_DIR1,M_PWM1);
  loco(right,M_DIR2,M_PWM2);
  
  
 }

 ros::Subscriber<rover_msgs::WheelVelocity> locomotion_sub("rover/wheel_vel", &roverMotionCallback);


 void Enc_1_count()
{
  enc_1_tick++;
}

void Enc_2_count()
{
  enc_2_tick++;
}

void setup() {


   nh.initNode();
 
   nh.subscribe(locomotion_sub);
   nh.advertise(enc_pub);
 //Serial.begin(BAUD_RATE);
 


   pinMode(M_DIR1,OUTPUT);
   pinMode(M_DIR2,OUTPUT);
   pinMode(M_DIR1,OUTPUT);
   pinMode(M_DIR2,OUTPUT);
  
   pinMode(ENC1,INPUT);
   pinMode(ENC2,INPUT);

   
 
 attachInterrupt(digitalPinToInterrupt(ENC1),Enc_1_count,RISING);
 attachInterrupt(digitalPinToInterrupt(ENC2),Enc_2_count,RISING);
 

}

void loop() 
{
  

  
  
  EncValue.left=enc_1_tick;
  EncValue.right=enc_2_tick;
  enc_pub.publish(&EncValue);

  nh.spinOnce();
delay(100);



}

