#include "Processor.h"

namespace Vision{
    Processor::Processor()
    {

    }

    Processor::Processor(std::string detType)   // 加载模型 people  or object （people检测任务在此处完成）
    {
        if(detType == PEOPLE)
        {
            detector = Detector(peopleModel,peopleWeightFile,peopleMeanFile,peopleMeanValue);
            this->detName = detType;
	    threshold = 0.1;
        }
        else 
	{
		detector = Detector(objModel,objWeightFile,objMeanFile,objMeanValue);
		threshold = 0.2;	
	}

    }

    void Processor::setDetectName(std::string detectName)  //初始化 加载检测任务 people or object names
    {
    	this->detName = detectName;
    }	

    void Processor::setObjectName(std::vector<std::string> objNames)//初始化 加载清单
    {
    	this->name.assign(objNames.begin(),objNames.end());
    }	

    void Processor::passMat(cv::Mat img)
    {
        this->img = img.clone();
    }



    vector<cv::Rect> Processor::getBoundingBox()
    {
        if(detCordinate.size() != 0)
            detCordinate.clear(); 
        std::vector<vector<float> > detections = detector.Detect(this->img);	
	    if(detections.size() == 0) 
		    return detCordinate;

	
	    if(this->detName == PEOPLE)     //此处对people 面积进行比较
	    {
	    	        for(int i = 0 ; i <detections.size() ; i++ )
        		{
	        		int id;
            			id = detections[i][1];
            			if(name[id-1] == detName && detections[i][2] >= threshold)    
            			{
                			int x1 = static_cast<int>(detections[i][3] * img.cols);
                			int y1 = static_cast<int>(detections[i][4] * img.rows);
                			int x2 = static_cast<int>(detections[i][5] * img.cols);
                			int y2 = static_cast<int>(detections[i][6] * img.rows);

                			cv::Rect rect(cv::Point(x1,y1), cv::Point(x2,y2));
                			detCordinate.push_back(rect);
            			}
        		}
		    	cv::Rect rect;
		    	int area = 0 ;
		    	for(int i = 0 ; i < detCordinate.size() ; i++ )
		    	{
			    int temp_area = detCordinate[i].height * detCordinate[i].width;
			    if(temp_area > area)
			    {
				    area = temp_area;
				    rect = detCordinate[i];
			    }		
		    	}
		    	detCordinate.clear();
		    	detCordinate.push_back(rect);
	    }
	else 
	{
		std::vector<int> candidate;
		for (int i = 0 ; i < detections.size() ; i++)
		{
			int id ;
			id = detections[i][1];
			if(name[id-1] == detName && detections[i][2] >= threshold)
			{
				candidate.push_back(i);
			}
		}
		if(candidate.size() == 0)
			return detCordinate;
		int optimal = candidate[0];
		int temp_score = detections[optimal][2];
		for (int i = 0 ; i < candidate.size() ; i++)
		{
			if ( detections[candidate[i]][2] > temp_score) 
			{
				optimal = candidate[i];
				temp_score = detections[candidate[i]][2];
			}
		}
		int x1 = static_cast<int>(detections[optimal][3] * img.cols);
        	int y1 = static_cast<int>(detections[optimal][4] * img.rows);
        	int x2 = static_cast<int>(detections[optimal][5] * img.cols);
        	int y2 = static_cast<int>(detections[optimal][6] * img.rows);

        	cv::Rect rect(cv::Point(x1,y1), cv::Point(x2,y2));
        	detCordinate.push_back(rect);
	}
        return detCordinate;
    }



 


    std::vector<std::string>  Processor::getAllObject()
    { 
        if(detected_name.size() != 0)
            detected_name.clear(); 
        std::vector<vector<float> > detections = detector.Detect(this->img);	
	    if(detections.size() == 0) 
            return detected_name;
		this->detected_img = this->img.clone();
        for(int i = 0 ; i <detections.size() ; i++ )
        {
	        int id;
		    id = detections[i][1];
            if(detections[i][2] >= threshold)
            {
                int x1 = static_cast<int>(detections[i][3] * img.cols);
                int y1 = static_cast<int>(detections[i][4] * img.rows);
                int x2 = static_cast<int>(detections[i][5] * img.cols);
                int y2 = static_cast<int>(detections[i][6] * img.rows);

                cv::Rect rect(cv::Point(x1,y1), cv::Point(x2,y2));
                cv::rectangle(detected_img,rect,cv::Scalar(255,0,0),5);
                cv::putText(detected_img,cv::String(name[id-1]),cv::Point(x1,y1-10),1,2,cv::Scalar(255,0,0),2);
                detected_name.push_back(name[id-1]);

            }
        }
        return detected_name;
    }

    void Processor::drawRect()
    {
        if(detCordinate.size() == 0)
            return ;
        for(int i=0;i<detCordinate.size();i++)
        {
            int center_x = detCordinate[i].x + detCordinate[i].width / 2;
            int center_y = detCordinate[i].y + detCordinate[i].height / 2;
            cv::rectangle(img,detCordinate[i],cv::Scalar(255,0,0),5);
            //cv::putText(img,cv::String(detName),cv::Point(detCordinate[i].x,detCordinate[i].y-10),5,4,cv::Scalar(255,0,0),3);
            cv::circle(img,cv::Point(center_x,center_y),5,cv::Scalar(0,0,255),-1,8);
        }
    }

    cv::Mat Processor::getImage()
    {
        return this->img;
    }

    cv::Mat Processor::getDetectedImage()
    {
        return this->detected_img;
    }






}
