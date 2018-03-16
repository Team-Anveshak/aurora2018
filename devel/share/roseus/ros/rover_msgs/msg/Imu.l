;; Auto-generated. Do not edit!


(when (boundp 'rover_msgs::Imu)
  (if (not (find-package "ROVER_MSGS"))
    (make-package "ROVER_MSGS"))
  (shadow 'Imu (find-package "ROVER_MSGS")))
(unless (find-package "ROVER_MSGS::IMU")
  (make-package "ROVER_MSGS::IMU"))

(in-package "ROS")
;;//! \htmlinclude Imu.msg.html


(defclass rover_msgs::Imu
  :super ros::object
  :slots (_yaw ))

(defmethod rover_msgs::Imu
  (:init
   (&key
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _yaw (float __yaw))
   self)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:serialization-length
   ()
   (+
    ;; float64 _yaw
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _yaw
     (setq _yaw (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get rover_msgs::Imu :md5sum-) "08cb8274b6ddb17af5a842bca0b17db1")
(setf (get rover_msgs::Imu :datatype-) "rover_msgs/Imu")
(setf (get rover_msgs::Imu :definition-)
      "float64 yaw

")



(provide :rover_msgs/Imu "08cb8274b6ddb17af5a842bca0b17db1")


