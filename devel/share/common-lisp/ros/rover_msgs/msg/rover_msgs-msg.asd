
(cl:in-package :asdf)

(defsystem "rover_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Goal" :depends-on ("_package_Goal"))
    (:file "_package_Goal" :depends-on ("_package"))
    (:file "Imu" :depends-on ("_package_Imu"))
    (:file "_package_Imu" :depends-on ("_package"))
    (:file "WheelVelocity" :depends-on ("_package_WheelVelocity"))
    (:file "_package_WheelVelocity" :depends-on ("_package"))
    (:file "enc" :depends-on ("_package_enc"))
    (:file "_package_enc" :depends-on ("_package"))
  ))