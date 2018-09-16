#include "lib/Camera.h"

using namespace Vision;
int main()
{
	Camera cam;
	if(!cam.openCamera())
	{
		std::cout<<"cam failure"<<std::endl;
		return 0;
	}
	std::cout<<"successful open"<<std::endl;
	cv::Mat rgb;
	cv::Mat depth;
	cv::namedWindow("depth",0);
	cv::namedWindow("colorImg",0);
	cv::Rect rect(cv::Point(860,440), cv::Point(960,640));
	std::vector<Vision::Cordinate3D> targetCor;
	std::vector<cv::Rect> detCor;
	detCor.push_back(rect);
	
	while(1)
	{
		cam.transColorImg().copyTo(rgb);
		//cv::rectangle(rgb, rect, cv::Scalar(0,0,255));
		//cv::resize(rgb,rgb,rgb.size(),0.2,0.2);
		cv::imshow("colorImg",rgb);
		cam.transDepthImg().copyTo(depth);
		cv::imshow("depth",depth);
		cv::imwrite("depth.jpg",depth);
		cv::imwrite("colorImg.jpg",rgb);
		cam.getCordinate3D(detCor,targetCor);
		std::cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
		for( int i = 0 ; i < targetCor.size() ; i++)
		{
			std::cout<<"x:"<<targetCor[i].x<<" "<<"y:"<<targetCor[i].y<<" "<<"z:"<<targetCor[i].z<<std::endl;
		}
		std::cout<<targetCor.size()<<std::endl;
		targetCor.clear();
		char ch = cv::waitKey(1);
		if(ch == 'q')
			break;
	}
	//cv::imwrite("rgb.jpg",rgb);
	//cv::imwrite("reg.jpg",depth);
	cam.closeCamera();
	return 0;
}
