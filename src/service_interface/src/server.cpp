#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"


class Server:public rclcpp::Node {
  public:
    Server(std::string name):Node(name) {
      RCLCPP_INFO(this->get_logger(),"node %s start...",name.c_str());
      server_=this->create_service<example_interfaces::srv::AddTwoInts>("add_two_ints_srv",std::bind(&Server::handle_two_ints,this,std::placeholders::_1,std::placeholders::_2));
  }
  private:
    rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr server_;
    void handle_two_ints(const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request>request,
          const std::shared_ptr<example_interfaces::srv::AddTwoInts::Response>response) {

        RCLCPP_INFO(this->get_logger(),"receive requset a=%d,b=%d",request->a,request->b);
        response->sum=request->a+request->b;
    }

};

int main(int argc,char** argv) {
  rclcpp::init(argc,argv);
  auto node = std::make_shared<Server>("server");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;

}
