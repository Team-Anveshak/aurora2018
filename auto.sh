#!/bin/bash

roscore & roslaunch navigation ekf.launch & roslaunch navigation nav_stack.launch &roslaunch navigation rviz.launch

