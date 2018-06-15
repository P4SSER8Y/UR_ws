#!/usr/bin/env bash
sudo docker run -it --security-opt label=disable \
       --device /dev/dri --env="DISPLAY" \
       --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
       -p 30001-30004:30001-30004  \
       jacknlliu/ursim:3.1.18024 start-ursim.sh UR5 
