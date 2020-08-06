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
#include <boost/thread.hpp>

void Thread_Function()
{
    while(true)
    {
        ROS_INFO("Threads Has Been Started!");
    }
 
}


class PictureDealer
{
    public: 
        PictureDealer()
        :it(n_)
        {
            pub_ = it.advertise/*<sensor_msgs::ImagePtr>*/("SpinOutImage", 1);    
            sub_ = it.subscribe("OutImage", 1, &PictureDealer::imageCallback, this);
        }
        void imageCallback(const sensor_msgs::ImageConstPtr& msg)
        {
            try
            {
                cv::Mat image=cv_bridge::toCvShare(msg, "bgr8")->image;
                //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
                //cv::imshow("view",image);
                cv::Mat dst;
	            cv::Point center(image.cols/2,image.rows/2); //旋转中心
	            double angle = -90.0;  //角度
	            double scale = 1.0;  //缩放系数
	            cv::Mat rotMat = getRotationMatrix2D(center,angle,scale);
	            warpAffine(image,dst,rotMat,image.size());
                sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", dst).toImageMsg();
                pub_.publish(msg);
	            cv::imshow("dst",dst);
                cv::waitKey(50);
                //ROS_INFO("CallBack Has Been Started");
            }
                catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
            }
        }
    private:  
       ros::NodeHandle n_; 
       image_transport::ImageTransport it; 
       image_transport::Publisher pub_;  
       image_transport::Subscriber sub_;  

};
int main(int argc, char **argv)  
{  
    //Initiate ROS  
    ros::init(argc, argv, "image_listener");   
    //Create an object of class SubscribeAndPublish that will take care of everything  
    PictureDealer picturedeal; 
    boost::thread server(Thread_Function);   
    ros::spin();  
      
    return 0;  
}  





 
// void imageCallback(const sensor_msgs::ImageConstPtr& msg)
// {
//   try
//   {
//     cv::Mat image=cv_bridge::toCvShare(msg, "bgr8")->image;
//     //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
//     //cv::imshow("view",image);
//     cv::Mat dst;
// 	cv::Point center(image.cols/2,image.rows/2); //旋转中心
// 	double angle = -90.0;  //角度
// 	double scale = 1.0;  //缩放系数
// 	cv::Mat rotMat = getRotationMatrix2D(center,angle,scale);
// 	warpAffine(image,dst,rotMat,image.size());
// 	cv::imshow("dst",dst);
//     cv::waitKey(50);
//   }
//   catch (cv_bridge::Exception& e)
//   {
//     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
//   }
// }
 
// int main(int argc, char **argv)
// {
//   ros::init(argc, argv, "image_listener");
//   ros::NodeHandle nh;
//   cv::namedWindow("view");
//   cv::startWindowThread();
//   image_transport::ImageTransport it(nh);
//   image_transport::Subscriber sub = it.subscribe("OutImage", 1, imageCallback);
//   //image_transport::Publisher pub = it.advertise("SpinOutImage", 1);
//   ros::spin();
//   cv::destroyWindow("view");
// }


