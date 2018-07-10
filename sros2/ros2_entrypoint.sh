#!/bin/bash
set -e

# setup ros2 environment
source "/opt/ros/$ROS2_DISTRO/setup.bash"
source "/opt/rti/rti_connext_dds-$RTI_CONNEXT_VERSION/resource/scripts/rtisetenv_x64Linux3gcc$RTI_GCC_VERSION.bash"
exec "$@"
