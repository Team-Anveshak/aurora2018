Enter the initial and destination latitude and longitude in GPS_goal.py(src/navigation/scripts/GPS_goal.py)


Run 
---------

1. roslaunch navigation ekf.launch
2. roslaunch navigation nav_stack.launch
3. roslaunch navigation rviz.launch
4. rosrun navigation GPS_goal.py

initially run diff_tf.py node separately and check the value of theta (src/navigation/scripts/diff_tf.py)
it is commented out in nav_stack.launch


If imu and lidar are connected then run 
-------------------------------

5. roslaunch razor_imu_9dof razor-pub.launch
6. roslaunch rplidar_ros rplidar.launch
