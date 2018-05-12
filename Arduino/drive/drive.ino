/*#define USE_USBCON*/
#include <ros.h>
#include <rover_msgs/WheelVelocity.h>
#include <rover_msgs/Diag_wheel.h>
#include "drive_variables.h"
#include <Servo.h>

Servo pan;
Servo tilt;

ros::NodeHandle nh;

rover_msgs::Diag_wheel Diagnosis;
ros::Publisher diag_pub("diag/wheel_vel", &Diagnosis);




void roverMotionCallback(const rover_msgs::WheelVelocity& RoverVelocity){
  
  lf = int(constrain(RoverVelocity.left_front_vel,-255,255));
  rf = int(constrain(RoverVelocity.right_front_vel,-255,255));
  lb = int(constrain(RoverVelocity.left_back_vel,-255,255));
  rb = int(constrain(RoverVelocity.right_back_vel,-255,255));
  
  right_steer = int(constrain(RoverVelocity.right_steer,200,600));
  left_steer  = int(constrain(RoverVelocity.left_steer,200,600));
   
  pan_joy=RoverVelocity.pan_joy;
  tilt_joy=RoverVelocity.tilt_joy;
  
  right_steer_vel = RoverVelocity.right_steer_vel;
  left_steer_vel  = RoverVelocity.left_steer_vel;
  
  mode = RoverVelocity.mode;
  
 }
 
ros::Subscriber<rover_msgs::WheelVelocity> locomotion_sub("loco/wheel_vel", &roverMotionCallback); 
 
void loop(){
  nh.spinOnce();
  
        if(pan_joy==1){
          if(abs(millis()-servo_then)>100){
            pan.write(pan_pos);
            if(pan_pos<179) pan_pos++;
            servo_then = millis();
          }
        }
        else if(pan_joy==-1){
          if(abs(millis()-servo_then)>100){
            pan.write(pan_pos);
            if(pan_pos>1) pan_pos--;
            servo_then = millis();
          }
        }
        if(tilt_joy==1){
          if(abs(millis()-servo_then)>100){
            tilt.write(tilt_pos);
             if(tilt_pos<179) tilt_pos++;
            servo_then = millis();
          }
        }
        else if(tilt_joy==-1){
          if(abs(millis()-servo_then)>100){
            tilt.write(tilt_pos);
            if(tilt_pos>1) tilt_pos--;
            servo_then = millis();
          }
        }
        
        if(mode == 0)
        {   flag =1;                         // emergency stop
            flag2 = 1;
            loco(0,dirlf,pwmlf,slplf);
            loco(0,dirlb,pwmlb,slplb);
            loco(0,dirrf,pwmrf,slprf);
            loco(0,dirrb,pwmrb,slprb);
        }
         
        if(mode == 1)
        {                  // normal mode
            flag2 = 1;
            rpot = analogRead(rpotPin);
            lpot = analogRead(lpotPin);
            
            if(abs(lpot-set_l_zero)>angle_tolerance && abs(rpot-set_r_zero)>angle_tolerance && flag ==1)
            {
              rotate(set_r_zero,set_l_zero);
              loco(0,dirlf,pwmlf,slplf);
              loco(0,dirlb,pwmlb,slplb);
              loco(0,dirrf,pwmrf,slprf);
              loco(0,dirrb,pwmrb,slprb);   
              
            }
            else{
              Diagnosis.left_front_vel  = lf ;
              Diagnosis.right_front_vel = rf ;
              Diagnosis.left_back_vel   = lb ;
              Diagnosis.right_back_vel = rb ; 
              flag =0;
              rotate(right_steer,left_steer);
              loco(lf,dirlf,pwmlf,slplf);
              loco(lb,dirlb,pwmlb,slplb);
              loco(rf,dirrf,pwmrf,slprf);
              loco(rb,dirrb,pwmrb,slprb);    
            }
            
         }
        
         
        if(mode == 2)
        {                              // zero turn mode
              flag =1;
              rpot = analogRead(rpotPin);
              lpot = analogRead(lpotPin);
              
              if(abs(lpot-set_l_angle)>angle_tolerance && abs(rpot-set_r_angle)>angle_tolerance && flag2 == 1)
              { 
                rotate(set_r_angle,set_l_angle);
                loco(0,dirlf,pwmlf,slplf);
                loco(0,dirlb,pwmlb,slplb);
                loco(0,dirrf,pwmrf,slprf);
                loco(0,dirrb,pwmrb,slprb); 
                
               }
                
              else
              
              { 
                flag2 =0 ;
                analogWrite(lmpwm,0);
                analogWrite(rmpwm,0);
                Diagnosis.left_front_vel  = pan_pos ;
                Diagnosis.right_front_vel = tilt_pos ;
                Diagnosis.left_back_vel   = lb ;
                Diagnosis.right_back_vel = rb ; 
                loco(lf,dirlf,pwmlf,slplf);
                loco(lb,dirlb,pwmlb,slplb);
                loco(rf,dirrf,pwmrf,slprf);
                loco(rb,dirrb,pwmrb,slprb);
               }
                  
         }
         
        if(mode == 3)
        {                            // calibration mode
              flag =1;
              flag2 = 1;
              rotate_steer_left(left_steer_vel);
              rotate_steer_right(right_steer_vel); 
              set_r_zero = analogRead(rpotPin);
              set_l_zero = analogRead(lpotPin);
            
         }

        
              Diagnosis.left_pot        = analogRead(lpotPin); 
              Diagnosis.right_pot       = analogRead(rpotPin);
              Diagnosis.left_pot_zero   = set_l_zero ; 
              Diagnosis.right_pot_zero  = set_r_zero ; 
          Diagnosis.left_front_vel  = pan_pos ;
          Diagnosis.right_front_vel = rf ;    
          set_r_angle=set_r_zero+140;
          set_l_angle=set_l_zero+140;
          diag_pub.publish(&Diagnosis);
          nh.spinOnce();
          delay(1);
}
