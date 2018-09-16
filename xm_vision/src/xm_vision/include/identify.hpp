#ifndef IDENTIFY
#define IDENTIFY

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include <ssd_detect.hpp>
#include <math.h>
#include <xm_msgs/xm_People.h>
#include <xm_msgs/xm_Object.h>
#include <xm_msgs/xm_ObjectDetect.h>
#include <xm_msgs/xm_FollowPerson.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <Processor.h>
#include <Camera.h>
class identify
{
private:

  
    int wrong_flag = 0;

    std::vector<double> single_position;
    double person_x =0;
    double person_y =0;
    double person_z =0;
	std::vector<cv::Rect>people_position;
	std::vector<cv::Rect>people_position_tmp;

    std::vector<std::vector<int> > position_class_people;
    std::vector<Vision::Cordinate3D> targetCor;
    cv::Mat frame;
    int if_detected = 0;
	
        
public:
     identify();
    // ~idetify();
    bool callback(xm_msgs::xm_ObjectDetect::Request &req, 
                  xm_msgs::xm_ObjectDetect::Response &res);
    void task_start(ros::NodeHandle & n);
    int get_person_location(int , int );

};


#endif
