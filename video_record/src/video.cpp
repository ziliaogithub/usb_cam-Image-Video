#include <ros/ros.h>  
#include <image_transport/image_transport.h>  
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>  
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

VideoWriter writer; 
 
//received image callback
void imageCallback(const sensor_msgs::ImageConstPtr& cam_image)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(cam_image,sensor_msgs::image_encodings::BGR8);
    }

    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception:%s",e.what());
        return;
    }

    Mat frame = cv_ptr->image;
    //imshow("image",frame);
  
    //将图像帧写入视频
    writer << frame; 
    cvWaitKey(2);
}


int main(int argc, char **argv)  
{  
     ros::init(argc, argv, "video_record_node");  
     ros::NodeHandle nh;    

     image_transport::ImageTransport it(nh);  
     image_transport::Subscriber sub = it.subscribe("/usb_cam/image_raw", 1, imageCallback); 

     //设置视频文件存储位置
     string filename;
     ros::param::param("~filename",filename,std::string("/home/uav_02/Videos/video_01.avi"));

     //设置并打开 VideoWriter
     writer.open(filename, CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(640, 480));

     ros::spin();  
     return 0;
} 
