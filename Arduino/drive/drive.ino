/*#define USE_USBCON*/
#include <ros.h>
#include <rover_msgs/WheelVelocity.h>
#include <rover_msgs/Diag_wheel.h>
#include "drive_variables.h"

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

  right_steer_vel = RoverVelocity.right_steer_vel;
  left_steer_vel  = RoverVelocity.left_steer_vel;
  
  mode = RoverVelocity.mode;
  
 }
 
ros::Subscriber<rover_msgs::WheelVelocity> locomotion_sub("loco/wheel_vel", &roverMotionCallback); 
 
void loop(){
  nh.spinOnce();
  
        if(mode == 0)
        {   flag =1;                         // emergency stop
            flag2 = 1;
            loco(0,dir1,pwm1,slp1);    
            loco(0,dir2,pwm2,slp2);
            loco(0,dir3,pwm3,slp3);
            loco(0,dir4,pwm4,slp4);
        }
         
        if(mode == 1)
        {                  // normal mode
            flag2 = 1;
            rpot = analogRead(rpotPin);
            lpot = analogRead(lpotPin);
            
            if(abs(lpot-set_l_zero)>angle_tolerance && abs(rpot-set_r_zero)>angle_tolerance && flag ==1)
            {
              rotate(set_r_zero,set_l_zero);
              loco(0,dir1,pwm1,slp1);
              loco(0,dir2,pwm2,slp2);
              loco(0,dir3,pwm3,slp3);
              loco(0,dir4,pwm4,slp4);   
              
            }
            else{
              Diagnosis.left_front_vel  = lf ;
              Diagnosis.right_front_vel = rf ;
              Diagnosis.left_back_vel   = lb ;
              Diagnosis.right_back_vel = rb ; 
              flag =0;
              rotate(right_steer,left_steer);
              loco(lf,dir1,pwm1,slp1);
              loco(rf,dir2,pwm2,slp2);
              loco(lb,dir3,pwm3,slp3);
              loco(rb,dir4,pwm4,slp4);    
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
                loco(0,dir1,pwm1,slp1);
                loco(0,dir2,pwm2,slp2);
                loco(0,dir3,pwm3,slp3);
                loco(0,dir4,pwm4,slp4);  
                
               }
                
              else
              
              { 
                flag2 =0 ;
                analogWrite(lpwm,0);
                analogWrite(rpwm,0);
                Diagnosis.left_front_vel  = lf ;
                Diagnosis.right_front_vel = rf ;
                Diagnosis.left_back_vel   = lb ;
                Diagnosis.right_back_vel = rb ; 
                loco(lf,dir1,pwm1,slp1);
                loco(rf,dir2,pwm2,slp2);
                loco(lb,dir3,pwm3,slp3);
                loco(rb,dir4,pwm4,slp4);
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

          diag_pub.publish(&Diagnosis);
          nh.spinOnce();
          delay(1);
}
