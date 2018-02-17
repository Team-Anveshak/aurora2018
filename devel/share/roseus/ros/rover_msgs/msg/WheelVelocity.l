;; Auto-generated. Do not edit!


(when (boundp 'rover_msgs::WheelVelocity)
  (if (not (find-package "ROVER_MSGS"))
    (make-package "ROVER_MSGS"))
  (shadow 'WheelVelocity (find-package "ROVER_MSGS")))
(unless (find-package "ROVER_MSGS::WHEELVELOCITY")
  (make-package "ROVER_MSGS::WHEELVELOCITY"))

(in-package "ROS")
;;//! \htmlinclude WheelVelocity.msg.html


(defclass rover_msgs::WheelVelocity
  :super ros::object
  :slots (_left_front_vel _right_front_vel _left_back_vel _right_back_vel _rot ))

(defmethod rover_msgs::WheelVelocity
  (:init
   (&key
    ((:left_front_vel __left_front_vel) 0.0)
    ((:right_front_vel __right_front_vel) 0.0)
    ((:left_back_vel __left_back_vel) 0.0)
    ((:right_back_vel __right_back_vel) 0.0)
    ((:rot __rot) 0.0)
    )
   (send-super :init)
   (setq _left_front_vel (float __left_front_vel))
   (setq _right_front_vel (float __right_front_vel))
   (setq _left_back_vel (float __left_back_vel))
   (setq _right_back_vel (float __right_back_vel))
   (setq _rot (float __rot))
   self)
  (:left_front_vel
   (&optional __left_front_vel)
   (if __left_front_vel (setq _left_front_vel __left_front_vel)) _left_front_vel)
  (:right_front_vel
   (&optional __right_front_vel)
   (if __right_front_vel (setq _right_front_vel __right_front_vel)) _right_front_vel)
  (:left_back_vel
   (&optional __left_back_vel)
   (if __left_back_vel (setq _left_back_vel __left_back_vel)) _left_back_vel)
  (:right_back_vel
   (&optional __right_back_vel)
   (if __right_back_vel (setq _right_back_vel __right_back_vel)) _right_back_vel)
  (:rot
   (&optional __rot)
   (if __rot (setq _rot __rot)) _rot)
  (:serialization-length
   ()
   (+
    ;; float64 _left_front_vel
    8
    ;; float64 _right_front_vel
    8
    ;; float64 _left_back_vel
    8
    ;; float64 _right_back_vel
    8
    ;; float64 _rot
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _left_front_vel
       (sys::poke _left_front_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _right_front_vel
       (sys::poke _right_front_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _left_back_vel
       (sys::poke _left_back_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _right_back_vel
       (sys::poke _right_back_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _rot
       (sys::poke _rot (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _left_front_vel
     (setq _left_front_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _right_front_vel
     (setq _right_front_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _left_back_vel
     (setq _left_back_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _right_back_vel
     (setq _right_back_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _rot
     (setq _rot (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get rover_msgs::WheelVelocity :md5sum-) "6d6193a2bd63a31fd8382ad51738fe63")
(setf (get rover_msgs::WheelVelocity :datatype-) "rover_msgs/WheelVelocity")
(setf (get rover_msgs::WheelVelocity :definition-)
      "float64 left_front_vel
float64 right_front_vel
float64 left_back_vel
float64 right_back_vel
float64 rot


")



(provide :rover_msgs/WheelVelocity "6d6193a2bd63a31fd8382ad51738fe63")


