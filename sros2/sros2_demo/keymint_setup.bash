#!/usr/bin/env bash
rm -rfv demo_keys
keymint keystore init --bootstrap keymint_ros

keymint keystore create_pkg talker
keymint keystore create_pkg listener
keymint keystore create_pkg launch_ros
keymint keystore create_pkg add_two_ints_client
keymint keystore create_pkg add_two_ints_server

keymint keystore build_pkg src/talker
keymint keystore build_pkg src/listener
keymint keystore build_pkg src/launch_ros
keymint keystore build_pkg src/add_two_ints_client
keymint keystore build_pkg src/add_two_ints_server

# cp -LRTv install demo_keys
# mv -Tv install demo_keys
ln -sv install demo_keys
