#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>
#include "std_msgs/String.h"

using namespace std;

static const char WINDOW[] = "Image window";
int main(int argc, char** argv)
{
  
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("OutImage", 1);
  image_transport::Publisher pub1 = it.advertise("OutImage1", 1);
  sensor_msgs::ImagePtr msg; //= cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg(); 注释部分为另一种写法
  std::string pattern_jpg;
  std::vector<cv::String> image_files;
  
  pattern_jpg = "/home/zrj/Wattman_Code/imgs_dir/*.jpg";

  bool flag = 1;
  int ii = 0;
  ros::Rate loop_rate(5);
  
  ROS_INFO("loop will start");
  cv::glob(pattern_jpg, image_files,flag);

    while (nh.ok()) 
    {
      cout << image_files[ii] << endl;
      cv::namedWindow("dd");
      cv::Mat dd=imread(image_files[ii]);
      cv::imshow("dd",dd);
      cv::waitKey(50);
      ii++;
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", dd).toImageMsg();
      pub.publish(msg);
      pub1.publish(msg);
      printf("msg has been pub\n");
      ros::spinOnce();
      loop_rate.sleep();
      if (ii==5)
      {
        ii=0;
      }
    }
}

