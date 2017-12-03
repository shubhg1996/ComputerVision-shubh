#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream>
#include <std_msgs/Int16.h>

cv::VideoCapture cap;

void msgCallback(const std_msgs::Int16::ConstPtr& msg)
  {
    if (msg->data == 1)
    {
        cap.release();
    }
  }
int main(int argc, char** argv)
{    
    int count = 0;
    int camera = argv[1][0]-48;
    ros::init(argc, argv, "camera_driver");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    //image_transport::Publisher pub = it.advertise("nissan/camera0_raw", 1);
    image_transport::Publisher pub = it.advertise("image_raw", 1);
    ros::Subscriber sub = nh.subscribe("toggle", 1000, msgCallback);
    cap.open(camera);
    //cap.open("/home/swati/catkin_ws/igvc.mp4");
    //cap.set(3, 640);
    //cap.set(4, 480);
 
     cv::namedWindow("here");
     cv::startWindowThread();
    if(!cap.isOpened()) 
    return 1;
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
    ros::Rate loop_rate(30);
    
    while (nh.ok())
    {
        cap >> frame;
    
        if(!frame.empty())
        {
            cv::imshow("here", frame);
            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
            pub.publish(msg);
        }
         ros::spinOnce();
          loop_rate.sleep();
          if(!cap.isOpened()) return 0;
    }
 return 0;
}
