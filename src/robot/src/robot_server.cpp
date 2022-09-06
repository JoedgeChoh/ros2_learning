#include "rclcpp/rclcpp.hpp"
#include "interface/srv/move_robot.hpp"
#include "interface/msg/robot_status.hpp"
class Robot {
public:	
    Robot()=default;
    ~Robot()=default; 
    float move_distance(float distance) {
	robot_status_ = interface::msg::RobotStatus::STATUS_MOVEING;
    	target_pose_ += distance;
	while( fabs(target_pose_ - current_pose_) > 0.01) {
            current_pose_ += fabs(distance)/distance * fabs(target_pose_ - current_pose_) * 0.1 ;
	    std::cout<<"current_pose_ = "<<current_pose_<<std::endl;
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    }
	return current_pose_;
	robot_status_ = interface::msg::RobotStatus::STATUS_STOP;
    }
    float get_current_pose() {
    	return this->current_pose_;
    }
    int get_status() {
    	return this->robot_status_;
    }

private:
    float current_pose_ = 0;
    float target_pose_ = 0;
    int robot_status_ = interface::msg::RobotStatus::STATUS_STOP ;

};
class RobotInterface:public rclcpp::Node {
public:
   RobotInterface(std::string name):Node(name) {
   	 RCLCPP_INFO(this->get_logger(),"%s node start ",name.c_str());
	   robot_server_=this->create_service<interface::srv::MoveRobot>("control_request",std::bind(&RobotInterface::handle_control,this,std::placeholders::_1,std::placeholders::_2));
   
   robot_status_publisher_ = this->create_publisher<interface::msg::RobotStatus>("robot_status",10);
   // 创建周期为５００ｍｓ的定时器
   timer_ = this->create_wall_timer(std::chrono::milliseconds(500),std::bind(&RobotInterface::timer_callback,this));
   }
private:
   Robot robot;
   rclcpp::Service<interface::srv::MoveRobot>::SharedPtr robot_server_;
   rclcpp::Publisher<interface::msg::RobotStatus>::SharedPtr robot_status_publisher_;
   rclcpp::TimerBase::SharedPtr timer_;
   void handle_control(const std::shared_ptr<interface::srv::MoveRobot::Request>request, std::shared_ptr<interface::srv::MoveRobot::Response>response) {
	   RCLCPP_INFO(this->get_logger(), "receive distance is %f, current_pose_ is %f ", request->distance, robot.get_current_pose());
	   robot.move_distance(request->distance);
	   response->pose = robot.get_current_pose();
   }
   void timer_callback() {
   	interface::msg::RobotStatus message;
	message.status = robot.get_status();
	message.pose = robot.get_current_pose();
	RCLCPP_INFO(this->get_logger(),"Publishing %f",robot.get_current_pose());
	robot_status_publisher_->publish(message);	
   }
};

int main(int argc, char** argv) {
    rclcpp::init(argc,argv);
    auto node = std::make_shared<RobotInterface>("robot_server");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;

}
