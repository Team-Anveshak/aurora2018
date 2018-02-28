#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <rover_msgs/WheelVelocity.h>


float x_axis_val;
float y_axis_val;
float y=50,x=20;



void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    
    
    x_axis_val = joy->axes[2];
    y_axis_val = joy->axes[1];   
}



int main(int argc, char** argv) {

    ros::init(argc,argv,"drive_diff");
    ros::NodeHandle nh;
    ros::Publisher vel_pub = nh.advertise<rover_msgs::WheelVelocity>("/loco/wheel_vel",10);
    ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("/joy",10,joyCallback);
    ros::Rate loop_rate(10);    
    


while(ros::ok())
    {

    ros::spinOnce();
    rover_msgs::WheelVelocity vel;




    if (y_axis_val>0.1)
    {
        if (abs(y_axis_val*y)<abs(x_axis_val*x)){
            x_axis_val = (x_axis_val/abs(x_axis_val))*(y_axis_val*y)/x ;
        }
  

        vel.left_front_vel  = y_axis_val*y-x_axis_val*x;
        vel.right_front_vel = y_axis_val*y+x_axis_val*x;
        vel.left_back_vel   = y_axis_val*y-x_axis_val*x;
        vel.right_back_vel  = y_axis_val*y+x_axis_val*x;
    



        
    }

    
    
    else if (y_axis_val<-0.1)
    {
       if (abs(y_axis_val*y)<abs(x_axis_val*x)){
            x_axis_val = (x_axis_val/abs(x_axis_val))*(y_axis_val*y)/x ;
        }
        vel.left_front_vel  = y_axis_val*y+x_axis_val*x;
        vel.right_front_vel = y_axis_val*y-x_axis_val*x;
        vel.left_back_vel   = y_axis_val*y+x_axis_val*x;
        vel.right_back_vel  = y_axis_val*y-x_axis_val*x;

        
    }

    else 
    {
        vel.left_front_vel = 0;
        vel.right_front_vel =  0;
        vel.left_back_vel =  0;
        vel.right_back_vel =  0;
    }
    

    vel_pub.publish(vel);
    loop_rate.sleep();
}
    ros::spin();
    return 0;
}
