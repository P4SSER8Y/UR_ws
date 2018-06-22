UR5 Experiment Workspace
========================

![ROS-Dist](https://img.shields.io/badge/ROS-Kinetic-green.svg)

Dependencies
------------
+ [Docker](https://www.docker.com/)

Usage Example
-------------
+ MCC DAQ
    - install dependencies
        * `rosrun misc install_mcc_daq_deps.sh`
    - launch usb1608G
        ~~~bash
        roslaunch mcc_daq usb1608g sample_width:=50 \
                                   channels:=4 \
                                   sample_frequency:=1000.0
        ~~~
+ EMG presentation
    - elbow_only
        * `roslaunch emg_presentation elbow_only robot_ip:=127.0.0.1`

Changelog
---------
+ 2018-06-22
    - add elbow_only presentation
    - use CLion as IDE
    - add auto install script for driver of MCC DAQ
    - add MCC USB1608G driver
    
+ 2018-06-15
    - add startup script for ursim installation & simulation

+ 2018-06-14
    - initialize
    - add [universal_robot](https://github.com/ros-industrial/universal_robot) and [ur_modern_driver](https://github.com/P4SSER8Y/ur_modern_driver)

