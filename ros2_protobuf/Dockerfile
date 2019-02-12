FROM ros:crystal

# install build dependencies
RUN apt-get update && apt-get install -q -y \
      build-essential \
      cmake \
      git \
      python3-colcon-common-extensions \
      python3-vcstool \
      wget \
    && rm -rf /var/lib/apt/lists/*

# copy all package.xml
ENV ROS_WS /opt/ros_ws
RUN mkdir -p $ROS_WS/src
WORKDIR $ROS_WS/src
COPY ./my_bridge/package.xml my_bridge/
COPY ./my_common/package.xml my_common/
COPY ./my_protobuf/package.xml my_protobuf/

WORKDIR $ROS_WS
# install package dependencies
RUN apt-get update && apt-get install -y \
      ros-$ROS_DISTRO-ros2bag && \
    rosdep install -q -y \
      --from-paths \
        src \
      --ignore-src \
    && rm -rf /var/lib/apt/lists/*

# copy repo packages
COPY ./ src/

# build package source
ARG CMAKE_BUILD_TYPE=Release
RUN . /opt/ros/$ROS_DISTRO/setup.sh && \
    colcon build \
      --symlink-install \
      --cmake-args \
        --no-warn-unused-cli \
        -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
        -DCMAKE_CXX_FLAGS="-Wno-unused-parameter"

# source workspace from entrypoint
RUN sed --in-place --expression \
      '$isource "$ROS_WS/install/setup.bash"' \
      /ros_entrypoint.sh
