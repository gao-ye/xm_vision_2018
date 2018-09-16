#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>
#include <Processor.h>
#include <Camera.h>
#include <time.h>
#include <xm_msgs/xm_People.h>
#include <xm_msgs/xm_Object.h>
#include <xm_msgs/xm_ObjectDetect.h>
#include <xm_msgs/xm_FollowPerson.h>

int main(int argc, char **argv)
{

    ros::init(argc, argv, "people_tracking");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<xm_msgs::xm_FollowPerson>("follow", 1);
    ros::Rate loop_rate(5);
    xm_msgs::xm_FollowPerson pos_xyz;

    Vision::Camera cam;
    Vision::Processor processor("person");
    std::vector<std::string> vec;
    vec.push_back("person");
    processor.setObjectName(vec);
    cv::Mat frame;

    std::vector<cv::Rect> detCordinate;
    std::vector<Vision::Cordinate3D> targetCor;
    cv::namedWindow("color",0);
    cv::Mat color;
    int count = 0;

    if (!cam.openCamera())
    {
      std::cout << "camera open failed" << std::endl;
      return 0;
    }
    int if_writed = 0;
    while (1)
    {

      count++;
      char ch;
      cam.transColorImg().copyTo(frame);
      processor.passMat(frame);
      detCordinate.clear();
      targetCor.clear();
      detCordinate = processor.getBoundingBox();
      if(detCordinate.size() == 0)
	    {
		    std::cout<<"no object"<<std::endl;
		    continue;
	    }

      cam.getCordinate3D(detCordinate, targetCor);
      //std::cout<<"the size is "<<targetCor.size()<<std::endl;
      //std::cout << " x: " << targetCor[0].x << " y: " << targetCor[0].y << " z: " << targetCor[0].z << std::endl;
     
      processor.drawRect();
      processor.getImage().copyTo(color);
      cv::imshow("color",color);
      if( if_writed == 0)
      {
           cv::imwrite("/home/ye/Vision/data/result/find_people.jpg",color);
	   if_writed = 1;
      }
      

      ch = cv::waitKey(1);
      if (ch == 'q')
          break;
      pos_xyz.position.point.x = targetCor[0].x;
      pos_xyz.position.point.y = targetCor[0].y;
      pos_xyz.position.point.z = targetCor[0].z;
      pos_xyz.position.header.frame_id = "camera_link";
      pos_xyz.position.header.stamp = ros::Time::now();
      printf("people_position:%lf, %lf, %lf\n", pos_xyz.position.point.x, 
                                              pos_xyz.position.point.y, 
                                              pos_xyz.position.point.z);

      chatter_pub.publish(pos_xyz);
      ros::spinOnce();  
      loop_rate.sleep();
      count++;
      //  if(count >5)break;   //用来关闭异常摄像头
      //if(system("CLS"))system("clear");
      }
 
    cam.closeCamera();
    return 0;
}
