#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class Client:public rclcpp::Node {
  public:
    Client(std::string name):Node(name) {
      RCLCPP_INFO(this->get_logger(),"node %s start...",name.c_str());
      client_ = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints_srv");
    }
    void send_request(int a, int b){
      RCLCPP_INFO(this->get_logger(),"calculate %d+%d",a,b);
      // 等待服务端上线
      while(!client_->wait_for_service()) {
        if(!rclcpp::ok()){
          RCLCPP_ERROR(this->get_logger(),"等待过程被打断");
          
        }
        RCLCPP_INFO(this->get_logger(),"linking to server...");
      }
      // 构造请求数据
      auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
      request->a = a;
      request->b = b;

      // 发送异步请求，触发回调
      client_->async_send_request(request,std::bind(&Client::result_callback_,this,std::placeholders::_1));
    
    }
  private:
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
    void result_callback_(rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture result_future) {
      auto response = result_future.get();
      RCLCPP_INFO(this->get_logger(),"result: %d",response->sum);
    }

};

int main(int argc,char** argv) {
  rclcpp::init(argc,argv);
  auto node = std::make_shared<Client>("client");
  node->send_request(3,4);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;

}
