#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <rover_msgs/WheelVelocity.h>


float x_axis_val;
float y_axis_val;
int c2,rot,rot2;



void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    
    
    x_axis_val = joy->axes[0];
    y_axis_val = joy->axes[1];
  if(joy->buttons[3]==1) rot = 1;
 else if(joy->buttons[0]==1) rot =-1;
 else rot=0;
 if(joy->buttons[2]==1) rot2 = 1;
 else if(joy->buttons[2]==1) rot2 =-1;
 else rot2 =0;
 
   
   
   
    
}



int main(int argc, char** argv) {

    ros::init(argc,argv,"drive");
    ros::NodeHandle nh;
    ros::Publisher vel_pub = nh.advertise<rover_msgs::WheelVelocity>("/loco/wheel_vel",10);
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel",10);
    ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("/joy",10,joyCallback);
    ros::Rate loop_rate(10);	
    


while(ros::ok())
	{

	ros::spinOnce();
    rover_msgs::WheelVelocity vel;
  geometry_msgs::Twist twist;



    c2 = 1;

    if ((y_axis_val>0.25) && (fabs(y_axis_val)>fabs(x_axis_val)))
	{
	vel.left_front_vel  = -c2 * y_axis_val*70;
        vel.right_front_vel = -c2 * y_axis_val*70; 
        vel.left_back_vel   = c2 * y_axis_val*70;
        vel.right_back_vel  = c2 * y_axis_val*70;
        twist.linear.x      =  y_axis_val;
	twist.angular.z     =   0;
     


        
	}

	
	else if ((y_axis_val< (-0.25)) && (fabs(y_axis_val)>fabs(x_axis_val)))
	{
		
	vel.left_front_vel = -c2 * y_axis_val*70;
        vel.right_front_vel= -c2 * y_axis_val*70;
        vel.left_back_vel  = c2 * y_axis_val*70;
        vel.right_back_vel = c2 * y_axis_val*70;
       twist.linear.x      =  y_axis_val;
	twist.angular.z     =   0;

	}
	
	
	else if ((x_axis_val>0.25) && (fabs(x_axis_val)>fabs(y_axis_val)))
	{
		vel.left_front_vel = -c2 * x_axis_val*70;
        vel.right_front_vel= c2 * x_axis_val*70;
        vel.left_back_vel  = -c2 * x_axis_val*70;
        vel.right_back_vel = c2 * x_axis_val*70;

        twist.linear.x      =  0;
	twist.angular.z     =   x_axis_val;
	}

	else if ((x_axis_val < (-0.25)) && (fabs(x_axis_val)>fabs(y_axis_val)))
	{
	   vel.left_front_vel = -c2 * x_axis_val*70;
       vel.right_front_vel= c2 * x_axis_val*70;
       vel.left_back_vel  = c2 * x_axis_val*70;
       vel.right_back_vel = -c2 * x_axis_val*70;

       twist.linear.x      =  0;
	twist.angular.z     =   x_axis_val;

        
	}

	else 
    {
        vel.left_front_vel = 0;
        vel.right_front_vel =  0;
        vel.left_back_vel =  0;
        vel.right_back_vel =  0;
    }
	
	vel.rot = rot;
	vel.rots =rot2;

    vel_pub.publish(vel);
    cmd_pub.publish(twist);
	loop_rate.sleep();
}
    ros::spin();
    return 0;
}
