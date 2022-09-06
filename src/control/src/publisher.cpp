#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Publisher:public rclcpp::Node{
public:
  Publisher(std::string name):Node(name) {
    RCLCPP_INFO(this->get_logger(),"I am %s",name.c_str());
    controller_publisher_=this->create_publisher<std_msgs::msg::String>("command",10);
    timer_=this->create_wall_timer(std::chrono::milliseconds(500),std::bind(&Publisher::timer_callback,this)); 

  }
private:
  void timer_callback() {
    std_msgs::msg::String message;
    message.data = "forward";
    RCLCPP_INFO(this->get_logger(),"Publishing:%s",message.data.c_str());
    controller_publisher_->publish(message);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr controller_publisher_;
};

int main(int argc,char** argv) {
  rclcpp::init(argc,argv);
  auto node = std::make_shared<Publisher>("topic_publisher");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;

}
