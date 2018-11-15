FROM ubuntu:bionic

RUN echo 'Etc/UTC' > /etc/timezone && \
    ln -s /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    apt-get update && apt-get install -q -y tzdata && rm -rf /var/lib/apt/lists/*

# install packages
RUN apt-get update && apt-get install -q -y \
    bash-completion \
    dirmngr \
    git \
    gnupg2 \
    libasio-dev \
    libtinyxml2-dev \
    lsb-release \
    python3-pip \
    wget \
    curl \
    default-jre-headless \
    python3-pexpect \
  && rm -rf /var/lib/apt/lists/*

# extra env setup for rti install
ENV RTI_CONNEXT_VERSION 5.3.1
ENV RTI_GCC_VERSION 5.4.0
ENV RTI_OPENSSL_VERSION 1.0.2n
ENV RTI_INSTALL_PATH /opt/rti
RUN mkdir -p $RTI_INSTALL_PATH

# prep temp stace for rti install
ENV TEMP_DIR /tmp
WORKDIR $TEMP_DIR
COPY rti_web_binaries_install_script.py $TEMP_DIR

# download and install rti libraries
ENV RTI_CONNEXT_URL https://s3.amazonaws.com/RTI/Bundles/$RTI_CONNEXT_VERSION/Evaluation/rti_connext_dds_secure-$RTI_CONNEXT_VERSION-eval-x64Linux3gcc$RTI_GCC_VERSION.tar.gz
RUN curl $RTI_CONNEXT_URL | tar -xz && \
    tar -xvf /tmp/openssl-$RTI_OPENSSL_VERSION-target-x64Linux3gcc$RTI_GCC_VERSION.tar.gz && \
    mv openssl-$RTI_OPENSSL_VERSION $RTI_INSTALL_PATH/openssl-$RTI_OPENSSL_VERSION && \
    python3 -u rti_web_binaries_install_script.py \
      $TEMP_DIR/rti_connext_dds-$RTI_CONNEXT_VERSION-eval-x64Linux3gcc$RTI_GCC_VERSION.run \
      $RTI_INSTALL_PATH/rti_connext_dds-$RTI_CONNEXT_VERSION \
      --rtipkg_paths \
        $TEMP_DIR/rti_security_plugins-$RTI_CONNEXT_VERSION-eval-x64Linux3gcc$RTI_GCC_VERSION.rtipkg \
        $TEMP_DIR/openssl-$RTI_OPENSSL_VERSION-$RTI_CONNEXT_VERSION-host-x64Linux.rtipkg && \
    rm -rf ./*
WORKDIR /

# set env to point to rti libraries
ENV RTI_OPENSSL_BIN $RTI_INSTALL_PATH/openssl-$RTI_OPENSSL_VERSION/x64Linux3gcc$RTI_GCC_VERSION/release/bin
ENV PATH $RTI_OPENSSL_BIN:$PATH
ENV RTI_OPENSSL_LIBS $RTI_INSTALL_PATH/openssl-$RTI_OPENSSL_VERSION/x64Linux3gcc$RTI_GCC_VERSION/release/lib
ENV LD_LIBRARY_PATH $RTI_OPENSSL_LIBS:$LD_LIBRARY_PATH

# setup keys
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 421C365BD9FF1F717815A3895523BAEEB01FA116

# setup sources.list
RUN . /etc/os-release \
    && echo "deb http://repo.ros2.org/$ID/main `lsb_release -sc` main" > /etc/apt/sources.list.d/ros2-latest.list

# setup environment
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

# install packages from the ROS repositories
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-catkin-pkg-modules \
    python3-colcon-common-extensions \
    python3-rosdep \
    python3-vcstool \
    && rm -rf /var/lib/apt/lists/*

# install python packages
RUN pip3 install -U \
    argcomplete \
    flake8 \
    flake8-blind-except \
    flake8-builtins \
    flake8-class-newline \
    flake8-comprehensions \
    flake8-deprecated \
    flake8-docstrings \
    flake8-import-order \
    flake8-quotes \
    pytest-repeat \
    pytest-rerunfailures

# bootstrap rosdep
ENV ROSDISTRO_INDEX_URL https://raw.githubusercontent.com/ros2/rosdistro/ros2/index.yaml
RUN rosdep init \
    && rosdep update

# clone source
ENV ROS2_WS /root/ros2_ws
RUN mkdir -p $ROS2_WS/src
WORKDIR $ROS2_WS
RUN wget https://gist.githubusercontent.com/ruffsl/0181bfb52f169d0967d2fd602cbffcfb/raw/c80720f2e7dd84321a294f547a13640e05669e9f/ros2.repos \
    && vcs import src < ros2.repos

# install dependencies
RUN apt-get update && rosdep install -y \
    --from-paths src \
    --ignore-src \
    --rosdistro bouncy \
    --skip-keys "cmake_modules console_bridge fastcdr fastrtps libopensplice67 rti-connext-dds-5.3.1 urdfdom_headers" \
    && rm -rf /var/lib/apt/lists/*

SHELL ["/bin/bash", "-c"]

# build source
WORKDIR $ROS2_WS
RUN source "/opt/rti/rti_connext_dds-$RTI_CONNEXT_VERSION/resource/scripts/rtisetenv_x64Linux3gcc$RTI_GCC_VERSION.bash" \
    && colcon \
    build \
    --cmake-args \
      -DSECURITY=ON \
      -DCMAKE_BUILD_TYPE=Debug\
    --no-warn-unused-cli \
    --symlink-install \
    --packages-up-to \
      ros2run \
      demo_nodes_cpp \
      sros2

# setup bashrc
RUN cp /etc/skel/.bashrc ~/

# setup entrypoint
COPY ./ros2_source_entrypoint.sh /

ENTRYPOINT ["/ros2_source_entrypoint.sh"]
CMD ["bash"]
