#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>
#include <Processor.h>
#include <Camera.h>
#include <time.h>
#include <fstream>  //ifstream
#include <string>    
#include <xm_msgs/xm_Object.h>
#include <xm_msgs/xm_ObjectDetect.h>

int if_detected = 0;

std::vector<std::string> name_init();
bool callback(xm_msgs::xm_ObjectDetect::Request &req, 
                      xm_msgs::xm_ObjectDetect::Response &res);
int main(int argc, char **argv)
{ 

  int count = 0;
  ros::init(argc, argv, "object_detect");  
  ros::NodeHandle n;
 ros::ServiceServer service = n.advertiseService("get_position", &callback);


    while(1)
    {
        count++;
        if(if_detected)
        {
            break;
        }
        else
        {
            std::cout<<"time out!"<<std::endl;
            std::cout<<count<<std::endl;
            
            ros::spinOnce();
        }

    }
  return 0;
}

bool callback(xm_msgs::xm_ObjectDetect::Request &req, 
                      xm_msgs::xm_ObjectDetect::Response &res)
{
    Vision::Camera cam;
    Vision::Processor processor("object");
    std::vector<std::string>vec = name_init();//清单名称初始化
    processor.setObjectName(vec);

    std::string obj_name_tmp;
    obj_name_tmp = req.object_name;
    xm_msgs::xm_Object obj_tmp;
    int count = 1;
    
    std::cout<<obj_name_tmp<<std::endl;
    processor.setDetectName(obj_name_tmp)  ; //检测目标

    cv::Mat frame;
    std::vector<cv::Rect> detCordinate;
    std::vector<Vision::Cordinate3D> targetCor;
    // cv::namedWindow("windows",0);
    if(!cam.openCamera())
    {
    	std::cout<<"camera open failed"<<std::endl;
        cam.closeCamera();
	    return false;
    }
    std::cout<<"hello"<<std::endl;
    // frame = cv::imread("/home/ye/pic/1.jpg");
    // if (frame.empty())
    // {

    //     std::cout<<"imread failed"<<std::endl;
    //     return false;
    // }
    // else {
    //     std::cout<<"imread Success"<<std::endl;
    // }



    if(req.people_id == 2) 
    {
        while(/*!if_detected &&*/count <= 20)  //检测多少次停止
        {
            count++;
            targetCor.clear();
            detCordinate.clear();
            cam.transColorImg().copyTo(frame);

            processor.passMat(frame);
            detCordinate = processor.getTwoBoundingBox();
            std::cout<<detCordinate.size()<<std::endl;
	        if(detCordinate.size() != 2)
            {
                std::cout<<"did not find two objects at the same time!"<<std::endl;
                continue;

            }
            else
            {
	            cam.getCordinate3D(detCordinate,targetCor);
	            for(int i = 0; i<targetCor.size() ; i++)
  	            {
		            std::cout<<"x:"<<targetCor[i].x<<"  y:"<<targetCor[i].y<<"  z:"<<targetCor[i].z<<std::endl;
	            }
                processor.drawRect();
                break;
            }

        }


        cam.closeCamera();
        cv::Mat result = processor.getImage();
        cv::imwrite("/home/ye/Vision/data/result/object_one.jpg",result);
        cv::resize(result,result,cv::Size(result.cols/2,result.rows/2),0,0);    
        cv::imshow("windows", result);
        cv::moveWindow("windows",0,0);
        cv::waitKey(500);
        
        for(int i=0;i<2;i++)
        {
        obj_tmp.pos.point.x = targetCor[i].x;//x   //此处默认返回检测到的第一个任务物品的位置，后续优化
        obj_tmp.pos.point.y = targetCor[i].y;//y
        obj_tmp.pos.point.z = targetCor[i].z;//z
        obj_tmp.pos.header.frame_id = "camera_link";
        obj_tmp.pos.header.stamp = ros::Time::now();
        res.object.push_back(obj_tmp);
        }
        if_detected = 1;   
        std::cout<<"detetction success! "<<std::endl;
        return true;
    }
    else
    {
         while(/*!if_detected &&*/count <= 30)  //检测多少次停止
        {
        count++;
        targetCor.clear();
        detCordinate.clear();
        cam.transColorImg().copyTo(frame);

        processor.passMat(frame);
        detCordinate = processor.getBoundingBox();
        std::cout<<detCordinate.size()<<std::endl;
	    if(detCordinate.size() == 0)
        {
            std::cout<<"no object"<<std::endl;
            continue;

        }
	    cam.getCordinate3D(detCordinate,targetCor);
	    for(int i = 0; i<targetCor.size() ; i++)
  	    {
		    std::cout<<"x:"<<targetCor[i].y<<"  y:"<<targetCor[i].x<<"  z:"<<targetCor[i].z<<std::endl;
	    }
        processor.drawRect();
        char ch;
        ch = cv::waitKey(1);
        if (ch == 'q')
          break;
        cv::Mat result;
        processor.getImage().copyTo(result);
        cv::imshow("windows", result);
            if(detCordinate.size() != 0)
        {
            break;

        }

        }

        cam.closeCamera();
    
        cv::Mat result = processor.getImage();
        cv::imwrite("/home/ye/Vision/data/result/object.jpg",result);
        cv::resize(result,result,cv::Size(result.cols/2,result.rows/2),0,0);    
        cv::imshow("windows", result);
        cv::moveWindow("windows",0,0);
        cv::waitKey(1000);
        // obj_tmp.pos.point.y = 1;//x   //此处默认返回检测到的第一个任务物品的位置，后续优化
        // obj_tmp.pos.point.z = 1;//y
        // obj_tmp.pos.point.x = 1;//z
        obj_tmp.pos.point.x = targetCor[0].x;//x   //此处默认返回检测到的第一个任务物品的位置，后续优化
        obj_tmp.pos.point.y = targetCor[0].y;//y
        obj_tmp.pos.point.z = targetCor[0].z;//z
        obj_tmp.pos.header.frame_id = "camera_link";
        obj_tmp.pos.header.stamp = ros::Time::now();
    
        if_detected = 1;
        res.object.push_back(obj_tmp);
        std::cout<<"12"<<std::endl;
        return true;
    }




}

std::vector<std::string> name_init()
{
    std::vector<std::string>vec;
    std::string s;
    std::ifstream inf;
    inf.open("/home/ye/Vision/data/object_name.txt");
    while(std::getline(inf,s))
    {
    vec.push_back(s);
    }
    inf.close();    
    std::cout<<"the number of objects is :"<<vec.size();
    std::cout<<std::endl;
    return vec;
}
