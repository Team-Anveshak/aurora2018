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
  :slots (_left_front_vel _right_front_vel _left_back_vel _right_back_vel _rot _rots ))

(defmethod rover_msgs::WheelVelocity
  (:init
   (&key
    ((:left_front_vel __left_front_vel) 0.0)
    ((:right_front_vel __right_front_vel) 0.0)
    ((:left_back_vel __left_back_vel) 0.0)
    ((:right_back_vel __right_back_vel) 0.0)
    ((:rot __rot) 0)
    ((:rots __rots) 0)
    )
   (send-super :init)
   (setq _left_front_vel (float __left_front_vel))
   (setq _right_front_vel (float __right_front_vel))
   (setq _left_back_vel (float __left_back_vel))
   (setq _right_back_vel (float __right_back_vel))
   (setq _rot (round __rot))
   (setq _rots (round __rots))
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
  (:rots
   (&optional __rots)
   (if __rots (setq _rots __rots)) _rots)
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
    ;; int16 _rot
    2
    ;; int16 _rots
    2
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
     ;; int16 _rot
       (write-word _rot s)
     ;; int16 _rots
       (write-word _rots s)
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
   ;; int16 _rot
     (setq _rot (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _rots
     (setq _rots (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get rover_msgs::WheelVelocity :md5sum-) "1a7e790a86a891aac209cbcfa527fcf8")
(setf (get rover_msgs::WheelVelocity :datatype-) "rover_msgs/WheelVelocity")
(setf (get rover_msgs::WheelVelocity :definition-)
      "float64 left_front_vel
float64 right_front_vel
float64 left_back_vel
float64 right_back_vel
int16 rot
int16 rots


")



(provide :rover_msgs/WheelVelocity "1a7e790a86a891aac209cbcfa527fcf8")


