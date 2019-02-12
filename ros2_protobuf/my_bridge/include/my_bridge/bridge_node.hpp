// Copyright 2019, Ruffin White.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MY_BRIDGE__BRIDGE_NODE_HPP_
#define MY_BRIDGE__BRIDGE_NODE_HPP_

#include "rclcpp/rclcpp.hpp"

#include "example_interfaces/srv/add_two_ints.hpp"
#include "std_msgs/msg/string.hpp"

using std_msgs::msg::String;
using example_interfaces::srv::AddTwoInts;

namespace my_bridge
{

class Bridge
    : public rclcpp::Node
{
 public:
  explicit Bridge(const std::string & node_name);
  ~Bridge() override = default;

  void service_callback(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<AddTwoInts::Request> request,
    const std::shared_ptr<AddTwoInts::Response> response);

  void message_callback(
      const String::SharedPtr msg);

 private:
  rclcpp::Subscription<String>::SharedPtr subscription_;
  rclcpp::Service<AddTwoInts>::SharedPtr server_;
};

}  // namespace my_bridge

#endif  // MY_BRIDGE__BRIDGE_NODE_HPP_
