// license removed for brevity

#include <urdf/model.h>

#include <chrono>
#include <memory>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher() : Node("minimal_publisher")
  {
    parameters_client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "/robot_state_publisher");
    while (!parameters_client_->wait_for_service(1s))
    {
      if (!rclcpp::ok())
      {
        RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
        rclcpp::shutdown();
      }
      RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
    }

    std::string urdf_string;
    auto parameters = parameters_client_->get_parameters({ "robot_description" });
    for (auto& parameter : parameters)
    {
      if (parameter.get_name() == "robot_description")
      {
        urdf_string = parameter.value_to_string();
        break;
      }
    }

    urdf::Model urdfModel;
    urdfModel.initString(urdf_string);
    RCLCPP_INFO_STREAM(this->get_logger(), "Name:" << urdfModel.getName());
  }

private:
  std::shared_ptr<rclcpp::SyncParametersClient> parameters_client_;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
