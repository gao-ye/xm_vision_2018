#include <lib/Camera.h>
#include <opencv2/opencv.hpp>

float xrot = 0.3433;
float yrot = 0.3393;
float xdif = -86.02;
float ydif = 26.43;
#define xRot xrot
#define yRot yrot
#define xDif xdif
#define yDif ydif 
void DrawCircle(cv::Mat img, cv::Point center,int thickness,int radius)
{
	int lineType = 8;
	
	cv::circle(img, center, radius , cv::Scalar(0,0,255) , thickness,lineType);
}
int main()
{
	//Vision::Camera cam;
	cv::Mat rgb;
	cv::Mat regist;
	rgb = cv::imread("/home/ye/Vision/Camera/build/rgb.jpg");
	regist = cv::imread("/home/ye/Vision/Camera/build/reg.jpg");
	int row, col;
	for (row = 600; row < 900 ; row+=100)
	{
		for ( col = 790 ; col < 1090 ; col+=100)
		{
			DrawCircle(rgb,cv::Point(col,row),4,5);
			DrawCircle(regist,cv::Point(col*xRot+xDif,row*yRot+yDif),2,2);
			std::cout<<"row col"<<row<<" "<<col<<std::endl;
		}
	}

	cv::imshow("rgb",rgb);
	
	cv::imshow("regist",regist);
	/*int max_c = 1920 * xRot + xDif;
	int max_r = 1080 * yRot + yDif;
	for ( row = 440 * yRot + yDif; row < max_r ; row += 1.0/yRot)
	{
		for ( col = 860 * xRot + xDif; col < max_c ; col += 1.0/xRot)
		{
			DrawCircle(regist,cv::Point(col,row));
			std::cout<<"row col"<<row<<" "<<col<<std::endl;
		}
	}*/
	
	cv::imshow("regist",regist);
	cv::waitKey();
	return 0;
}
