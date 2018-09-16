#include <Processor.h>
#include <Camera.h>
#include <time.h>
void rectangle(cv::Mat img)
{
    
}
int main()
{
  Vision::Camera cam;
  Vision::Processor processor("person");
  std::vector<std::string> vec;
  vec.push_back("person");
  processor.setObjectName(vec);


    cv::Mat frame;
    std::vector<cv::Rect> detCordinate;
    if (!cam.openCamera())
    {
        //std::cout << vec[0] << std::endl;
        std::cout << "camera open failed" << std::endl;
        return 0;
    }
    int count = 0;
    std::vector<Vision::Cordinate3D> targetCor;
    cv::namedWindow("color",0);
    cv::namedWindow("windows",0);
	cv::namedWindow("depth",0);
	cv::Mat regis;
	cv::Mat dep;
	cv::Mat color;
	cv::Mat depth;
    while (1)
    {

        count++;
        char ch;
        cam.transColorImg().copyTo(frame);
		cam.transDepthImg().copyTo(dep);
        processor.passMat(frame);
		detCordinate.clear();
        targetCor.clear();
        detCordinate = processor.getBoundingBox();
        if(detCordinate.size() == 0)
	    {
		    std::cout<<"no object"<<std::endl;
            cv::imshow("windows",frame);
            ch = cv::waitKey(1);
            if(ch == 'q')
                break;
		continue;
	    }
        cam.getCordinate3D(detCordinate, targetCor);
        cam.transOutImg().copyTo(depth);
		cam.transRegisImg().copyTo(regis);
        for (int i = 0; i < targetCor.size(); i++)
        {
            std::cout << "x:" << targetCor[i].x << "y:" << targetCor[i].y << "z:" << targetCor[i].z << std::endl;
        }
        processor.drawRect();
        processor.getImage().copyTo(color);
        cv::imshow("color",color);
		cv::imshow("depth",dep);
        ch = cv::waitKey(1);
        if (ch == 'q')
            break;
    }
 
    
    cam.closeCamera();
    return 0;
}
