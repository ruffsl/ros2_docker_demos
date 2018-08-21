#!/usr/bin/env bash
rm -rfv demo_keys
ros2 security create_keystore demo_keys
ros2 security create_key demo_keys talker
ros2 security create_key demo_keys listener
