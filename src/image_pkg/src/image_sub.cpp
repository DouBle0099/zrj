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
#include "std_msgs/String.h"
#include <unistd.h>

ros::Duration d(0.01);  

class PictureDealer
{
    public: 
        // PictureDealer()
        // :it(n_)
        // {
        //     pub_ = it.advertise/*<sensor_msgs::ImagePtr>*/("SpinOutImage", 1);    
        //     sub_ = it.subscribe("OutImage", 1, &PictureDealer::imageCallback, this);
        //     sub_1 = it.subscribe("OutImage", 1, &PictureDealer::imageCallback1, this);
        // }
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
	            //cv::imshow("dst",dst);
                cv::waitKey(1);
                ROS_INFO_STREAM("Thread1 [thread=" << boost::this_thread::get_id() << "]");
            }
                catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
            }
            //d.sleep();
            ROS_INFO_STREAM("Thread1 [thread=" << boost::this_thread::get_id() << "]");
        }
        void imageCallback1(const sensor_msgs::ImageConstPtr& msg)
        {
            sleep(1);
            ROS_INFO_STREAM("Thread2 [thread=" << boost::this_thread::get_id() << "]");
        }
     private:  
    //    ros::NodeHandle n_; 
    //    image_transport::ImageTransport it; 
          image_transport::Publisher pub_; 
    //    image_transport::Subscriber sub_; 
    //    image_transport::Subscriber sub_1;

};
int main(int argc, char **argv)  
{  
    ros::init(argc, argv, "image_listener");   
    ros::NodeHandle n_; //new
    image_transport::ImageTransport it(n_);
    PictureDealer picturedeal; 
    image_transport::Subscriber sub_ = it.subscribe("OutImage", 1, &PictureDealer::imageCallback, &picturedeal);
    image_transport::Subscriber sub_1 = it.subscribe("OutImage1", 1, &PictureDealer::imageCallback1, &picturedeal);
    image_transport::Publisher pub_ = it.advertise/*<sensor_msgs::ImagePtr>*/("SpinOutImage", 1);
    
     ros::AsyncSpinner spinner(2); 
     spinner.start();
     ros::Rate loop_rate(22);
     while (ros::ok())
    {
        loop_rate.sleep();
    }
    ros::waitForShutdown();    
    // ros::AsyncSpinner s(4);  
    // s.start();  
    // ros::Rate r(5);  
    //ros::spin();
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


