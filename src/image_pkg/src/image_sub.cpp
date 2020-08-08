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
        void imageCallback(const sensor_msgs::ImageConstPtr& msg)
        {
            try
            {
                cv::Mat image=cv_bridge::toCvShare(msg, "bgr8")->image;
                cv::Mat dst;
	            cv::Point center(image.cols/2,image.rows/2); //指定旋转中心点
	            double angle = -90.0;  //旋转角度于此处修改
	            double scale = 1.0;  //缩放系数
	            cv::Mat rotMat = getRotationMatrix2D(center,angle,scale);
	            warpAffine(image,dst,rotMat,image.size());
                sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", dst).toImageMsg();//转化为ros消息支持的msg类型
                pub_.publish(msg);//此处规定发布消息
	            //cv::imshow("dst",dst); //imshow被调用时，多线程会开起失败
                cv::waitKey(1);
            }
                catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
            }
            ROS_INFO_STREAM("Thread1 [thread=" << boost::this_thread::get_id() << "]");
        }
        void imageCallback1(const sensor_msgs::ImageConstPtr& msg)
        {
            sleep(1);
            ROS_INFO_STREAM("Thread2 [thread=" << boost::this_thread::get_id() << "]");
        }
     private:  
          image_transport::Publisher pub_; 

};
int main(int argc, char **argv)  
{  
    ros::init(argc, argv, "image_listener");   
    ros::NodeHandle n_; 
    image_transport::ImageTransport it(n_);
    PictureDealer picturedeal; 
    image_transport::Subscriber sub_ = it.subscribe("OutImage", 1, &PictureDealer::imageCallback, &picturedeal);//类外构造 订阅者1 独占一个线程
    image_transport::Subscriber sub_1 = it.subscribe("OutImage1", 1, &PictureDealer::imageCallback1, &picturedeal);//订阅者2 独占一个线程
    image_transport::Publisher pub_ = it.advertise/*<sensor_msgs::ImagePtr>*/("SpinOutImage", 1);//发布者于此处
    
     ros::AsyncSpinner spinner(2); //为节点指定两个线程
     spinner.start();
     ros::Rate loop_rate(22);
     while (ros::ok())
    {
        loop_rate.sleep();
    }
    ros::waitForShutdown();    //等待关闭
    return 0;  
}  





 



