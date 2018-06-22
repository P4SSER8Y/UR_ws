#include "usb1608g.h"

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>

#include <unistd.h>
#include <string>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mcc_usb1608g");

  ros::NodeHandle node;

  int nread;
  int nchannel;
  double frequency;
  bool err = true;

  if (!node.getParam("sample_width", nread))
  {
    ROS_ERROR("Parameter \"sample_width\" is REQUIRED");
    err = false;
  }
  if (!node.getParam("channels", nchannel))
  {
    ROS_ERROR("Parameter \"channels\" is REQUIRED");
    err = false;
  }
  if (!node.getParam("sample_frequency", frequency))
  {
    ROS_ERROR("Parameter \"sample_frequency\" is REQUIRED");
    err = false;
  }
  if (!err)
    return 1;

  ROS_INFO("sample_width = %d", nread);
  ROS_INFO("channels = %d", nchannel);
  ROS_INFO("sample_frequency = %0.2lf", frequency);

  bool isContinuouslyScan = false;

  ros::Publisher pub = node.advertise<std_msgs::Float64MultiArray>("daq", 1000);

  ros::Rate collect_rate(frequency / nread);

  auto daq = MCC_USB1608G::getInstance();
  daq->setTimeout(1000);
  daq->setNRead(nread);
  daq->setNChannel(nchannel);
  daq->setContinuousScanMode(isContinuouslyScan);
  daq->setMode(SINGLE_ENDED);
  daq->setGain(BP_10V);
  daq->setFrequency(frequency);
  if (daq->config() != MCC1608G_SUCCESS)
  {
    ROS_ERROR("configure MCC USB1608G failed");
    return 1;
  }
  daq->start();

  std_msgs::Float64MultiArray msg;

  msg.layout.data_offset = 0;
  msg.layout.dim.resize(2);
  msg.layout.dim[0].size = static_cast<unsigned int>(nread);
  msg.layout.dim[0].label = "Time";
  msg.layout.dim[0].stride = static_cast<unsigned int>(nchannel);
  msg.layout.dim[1].size = static_cast<unsigned int>(nchannel);
  msg.layout.dim[1].label = "Channel";
  msg.layout.dim[1].stride = 1;

  msg.data.resize(static_cast<unsigned long>(nread * nchannel));

  int cnt = 0;
  while (ros::ok())
  {
    auto data = new double[nread * nchannel];
    auto ret = daq->read(data);
    cnt++;

    for (auto i = 0; i < nread * nchannel; i++)
    {
      msg.data[i] = data[i];
    }
    pub.publish(msg);
  }
  daq->stop();
}
