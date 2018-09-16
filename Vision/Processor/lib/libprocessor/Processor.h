#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include <ssd_detect.hpp>
#include <math.h>
#define PEOPLE "person"
#define OBJECT "object"
namespace Vision
{
    struct Cordinate2D
    {
      float x;
      float y;
    };

    class Processor
    {
        private:
          Detector detector;
          Cordinate2D cor2d;
          cv::Mat img;
          cv::Mat detected_img;
          std::string detName; //name of object to be detected
          const std::string peopleModel = "/home/ye/Vision/models/people/coco.prototxt";
          const std::string peopleWeightFile = "/home/ye/Vision/models/people/coco.caffemodel";
          const std::string peopleMeanFile = "";
          const std::string peopleMeanValue = "128,128,128";
    
		  const std::string objModel = "/home/ye/Vision/models/object/obj.prototxt";
          const std::string objWeightFile = "/home/ye/Vision/models/object/obj.caffemodel";
          const std::string objMeanFile = "";
          const std::string objMeanValue = "104,117,123" ;
          double threshold = 0.3;
          std::vector<std::string> name ;
          std::vector<std::string> detected_name ;
          std::vector<cv::Rect> detCordinate;
        
        public:
          Processor();
          Processor(std::string detType);
	      void setObjectName(std::vector<std::string> objName);
          void setDetectName(std::string detectName);
          void passMat(cv::Mat img);
		  
          std::vector<cv::Rect> getBoundingBox();
          cv::Mat getImage();
          cv::Mat getDetectedImage();
          
          std::vector<std::string>  getAllObject();

	
    };
}; // namespace Vision

#endif
