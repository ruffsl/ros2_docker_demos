FROM osrf/ros2:bouncy-basic

# install more rwm implementations
ENV RTI_NC_LICENSE_ACCEPTED yes
RUN apt-get update && apt-get install -y \
      ros-$ROS2_DISTRO-rmw-connext-cpp \
      ros-$ROS2_DISTRO-rmw-opensplice-cpp && \
    rm -rf /var/lib/apt/lists/*
