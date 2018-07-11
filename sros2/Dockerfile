FROM osrf/ros2:bouncy-basic

# install more rwm implementations
ENV RTI_NC_LICENSE_ACCEPTED yes
RUN apt-get update && apt-get install -y --no-install-recommends \
      curl \
      default-jre-headless \
      python3-pexpect \
      ros-$ROS2_DISTRO-rmw-connext-cpp \
      ros-$ROS2_DISTRO-rmw-opensplice-cpp && \
    rm -rf /var/lib/apt/lists/*

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

# update entrypoint to source rti setup
COPY ./ros2_entrypoint.sh /
