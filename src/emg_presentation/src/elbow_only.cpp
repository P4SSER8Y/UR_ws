#include <ros/ros.h>

// ROS
#include <sensor_msgs/JointState.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>

// std
#include <cstdio>
#include <string>
#include <memory>
#include <vector>

const double MAX_POSITION = 150.0 / 180.0 * M_PI;
const std::vector<double> INIT_JOINT_STATE = { M_PI / 2, M_PI / 2, 0.0, 0.0, 0.0, 0.0 };
const std::string TARGET_TOPIC = "elbow_target";
const std::vector<std::string> JNT_NAMES = { "shoulder_pan_joint", "shoulder_lift_joint", "elbow_joint",
                                             "wrist_1_joint",      "wrist_2_joint",       "wrist_3_joint" };

ros::Subscriber g_sub_target;
ros::Publisher g_pub_cmd;

void updateTarget(const std_msgs::Float64MultiArray::ConstPtr& msg);
void init();

void updateTarget(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
  static char cmd_c[255];
  std_msgs::String cmd;
  sprintf(cmd_c, "servoj([%lf, %lf, %lf, %lf, %lf, %lf], 0, 0, %lf, 0.2, 300)", M_PI / 2, M_PI / 2,
          msg->data[0] * MAX_POSITION, 0.0, 0.0, 0.0, 0.02);
  cmd.data = cmd_c;
  g_pub_cmd.publish(cmd);
}

bool inRange(const std::vector<double>& a, const std::vector<double>& b, double delta)
{
  for (auto i = 0; i < a.size(); i++)
    if (std::abs(a[i] - b[i]) > delta)
      return false;
  return true;
}

void init(ros::NodeHandle& node)
{
  ROS_WARN("please move arm in \"Freedrive\" mode to joint state nearby [90, 90, 0, 0, 0, 0]");
  auto curr_state = ros::topic::waitForMessage<sensor_msgs::JointState>("joint_states");
  printf("[%0.2lf, %0.2lf, %0.2lf]\r", curr_state->position[0], curr_state->position[1], curr_state->position[2]);
  ros::Rate rate(1);
  while (!inRange(INIT_JOINT_STATE, curr_state->position, 0.5))
  {
    rate.sleep();
    curr_state = ros::topic::waitForMessage<sensor_msgs::JointState>("joint_states");
  }
  ROS_WARN("I'll move myself");

  char cmd_c[255];
  std_msgs::String cmd;
  sprintf(cmd_c, "movej([%lf,%lf,%lf,%lf,%lf,%lf])", INIT_JOINT_STATE[0], INIT_JOINT_STATE[1], INIT_JOINT_STATE[2],
          INIT_JOINT_STATE[3], INIT_JOINT_STATE[4], INIT_JOINT_STATE[5]);
  cmd.data = cmd_c;
  g_pub_cmd.publish(cmd);
  while (!inRange(INIT_JOINT_STATE, curr_state->position, 0.01))
  {
    rate.sleep();
    curr_state = ros::topic::waitForMessage<sensor_msgs::JointState>("joint_states");
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "elbow_only");
  ros::NodeHandle node;

  g_pub_cmd = node.advertise<std_msgs::String>("ur_driver/URScript", 50);
  init(node);
  g_sub_target = node.subscribe(TARGET_TOPIC, 50, updateTarget);

  ros::spin();
}
