;; Auto-generated. Do not edit!


(when (boundp 'rover_msgs::Goal)
  (if (not (find-package "ROVER_MSGS"))
    (make-package "ROVER_MSGS"))
  (shadow 'Goal (find-package "ROVER_MSGS")))
(unless (find-package "ROVER_MSGS::GOAL")
  (make-package "ROVER_MSGS::GOAL"))

(in-package "ROS")
;;//! \htmlinclude Goal.msg.html


(defclass rover_msgs::Goal
  :super ros::object
  :slots ())

(defmethod rover_msgs::Goal
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(setf (get rover_msgs::Goal :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get rover_msgs::Goal :datatype-) "rover_msgs/Goal")
(setf (get rover_msgs::Goal :definition-)
      "
")



(provide :rover_msgs/Goal "d41d8cd98f00b204e9800998ecf8427e")


