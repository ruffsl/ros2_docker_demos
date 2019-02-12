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

#include <inttypes.h>
#include <memory>

#include "my_bridge/bridge_node.hpp"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace my_bridge
{

Bridge::Bridge(const std::string & node_name)
: rclcpp::Node(node_name)
{
  subscription_ = this->create_subscription<String>(
    "chatter", std::bind(&Bridge::message_callback, this, _1));
  server_ = this->create_service<AddTwoInts>(
    "add_two_ints", std::bind(&Bridge::service_callback, this, _1, _2, _3));
}

void Bridge::service_callback(
  const std::shared_ptr<rmw_request_id_t> request_header,
  const std::shared_ptr<AddTwoInts::Request> request,
  const std::shared_ptr<AddTwoInts::Response> response)
{
  (void)request_header;
  RCLCPP_INFO(
    this->get_logger(),
    "request: %" PRId64 " + %" PRId64, request->a, request->b);
  response->sum = request->a + request->b;
}

void Bridge::message_callback(
    const String::SharedPtr msg)
{
  RCLCPP_INFO(
    this->get_logger(),
    "I heard: '%s'", msg->data.c_str());
}

}  // namespace my_bridge
