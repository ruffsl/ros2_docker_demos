from example_interfaces.srv import AddTwoInts

import rclpy


from rclpy.node import Node

from std_msgs.msg import String


object_permission_mapping = {
    'ros_topic': {'ros_publish': 'create_publisher_',
                  'ros_subscribe':  'create_subscription_'},
    'ros_service': {'ros_call':   'create_client_',
                    'ros_execute':   'create_service_'}
}


class MinimalNode(Node):

    def __init__(self, subject_name, object_kind, permission, object_name):
        super().__init__(subject_name[1:])
        getattr(self, object_permission_mapping[object_kind][permission])(object_name)

    def create_publisher_(self, object_name):
        self.publisher = self.create_publisher(String, object_name)

    def create_subscription_(self, object_name):
        self.subscription = self.create_subscription(String, object_name, self.listener_callback)

    def create_client_(self, object_name):
        self.cli = self.create_client(AddTwoInts, object_name)

    def create_service_(self, object_name):
        self.srv = self.create_service(AddTwoInts, object_name, self.add_two_ints_callback)

    def listener_callback(self, msg):
        # self.get_logger().info('I heard: "%s"' % msg.data)
        pass

    def add_two_ints_callback(self, request, response):
        # response.sum = request.a + request.b
        # self.get_logger().info('Incoming request\na: %d b: %d' % (request.a, request.b))
        pass


def check_node(subject_name, object_kind, permission, object_name):
    rclpy.init(args=None)

    minimal_node = MinimalNode(subject_name, object_kind, permission, object_name)

#     rclpy.spin(minimal_node)

    minimal_node.destroy_node()
    rclpy.shutdown()


expected_errors = {
    'ros_topic': {'ros_publish': "Failed to create publisher:",
                  'ros_subscribe':  "Failed to create subscription:"},
    'ros_service': {'ros_call':   "Failed to create client:",
                    'ros_execute':   "Failed to create service:"}
}


def test_node(subject_name, object_kind, permission, object_name):
    qualifier = None
    try:
        rclpy.try_shutdown()
        check_node(subject_name, object_kind, permission, object_name)
    except RuntimeError as e:
        message = str(e)
        expected_error = expected_errors[object_kind][permission]
        if message.startswith(expected_error):
            rclpy.try_shutdown()
            qualifier = 'deny'
        else:
            print("subject_name: {}\n object_kind: {}\n permission: {}\n object_name: {}\n".format(
                    subject_name, object_kind, permission, object_name))
            raise e
    else:
        qualifier = 'allow'
    return qualifier
