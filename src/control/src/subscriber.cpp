#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Subscriber:public rclcpp::Node {
public:
  Subscriber(std::string name):Node(name) {
     RCLCPP_INFO(this->get_logger(),"I am %s ",name.c_str());
     subscriber_=this->create_subscription<std_msgs::msg::String>("command",10,std::bind(&Subscriber::command_callback,this,std::placeholders::_1));
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
  void command_callback(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(),"getinto callback");
    double speed = 0.0;
    if(msg->data == "forward") {
      speed = 0.2;
    }
    RCLCPP_INFO(this->get_logger(),"receive %s command,publish speed %f",msg->data.c_str(),speed);
  }


};

int main(int argc, char** argv) {
  rclcpp::init(argc,argv);
  auto node = std::make_shared<Subscriber>("subcriber");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
