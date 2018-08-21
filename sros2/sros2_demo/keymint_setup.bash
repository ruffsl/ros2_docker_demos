#!/usr/bin/env bash
rm -rfv demo_keys
keymint keystore init --bootstrap keymint_ros
keymint keystore create_pkg talker
keymint keystore create_pkg listener
keymint keystore build_pkg src/talker
keymint keystore build_pkg src/listener

find install/ -type d -exec cp -v public/* {} \;
mv -Tv install demo_keys
