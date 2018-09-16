#include <Processor.h>
#include <Camera.h>
#include <time.h>
void rectangle(cv::Mat img)
{
    
}
int main()
{
    Vision::Camera cam;

    Vision::Processor processor("object");
    std::vector<std::string> vec;
    std::string s;
    std::ifstream inf;
    inf.open("/home/ye/Vision/data/object_name.txt");
    while (std::getline(inf, s))
    {
        vec.push_back(s);
    }
    inf.close();
    std::cout << "the number of objects is :" << vec.size() << std::endl;
    processor.setObjectName(vec);
    processor.setDetectName("tissue"); //检测目标
   
    std::vector<cv::Rect> detCordinate;
    if (!cam.openCamera())
    {
        std::cout << vec[0] << std::endl;
        std::cout << "camera open failed" << std::endl;
        return 0;
    }
    double start = clock();
    int count = 0;
    std::vector<Vision::Cordinate3D> targetCor;
    //cv::namedWindow("out", 0);
    cv::namedWindow("color",0);
    //cv::namedWindow("windows",0);
	//cv::namedWindow("registered",0);
	cv::namedWindow("Depth",0);
    cv::Mat depth;
    cv::Mat color;
	cv::Mat regis;
	cv::Mat Depth;
	cv::Mat frame;
	
    while (1)
    {

        count++;
        char ch;
		//cam.transRegisImg().copyTo(regis);
        //processor.passMat(regis);
		cam.transColorImg().copyTo(frame);
        processor.passMat(frame);
        detCordinate = processor.getBoundingBox();
        if(detCordinate.size() == 0)
	    {
		    std::cout<<"no object"<<std::endl;
		    //cv::imwrite("no_object.jpg",frame);
            cv::imshow("windows",frame);
            ch = cv::waitKey(1);
            if(ch == 'q')
                break;
		continue;
	    }
        cam.getCordinate3D(detCordinate, targetCor);
        //cam.transOutImg().copyTo(depth);
		
        for (int i = 0; i < targetCor.size(); i++)
        {
			if(std::isnan(targetCor[i].x) || std::isnan(targetCor[i].y) || std::isnan(targetCor[i].z))
				std::cout<<" nan hhhhh"<<std::endl;
			else
            std::cout << "x:" << targetCor[i].x << "y:" << targetCor[i].y << "z:" << targetCor[i].z << std::endl;
        }
        processor.drawRect();
        processor.getImage().copyTo(color);
		cam.transOutImg().copyTo(Depth);
        //cv::imshow("registered",regis);
		cv::imshow("Depth",Depth);
		cv::imshow("color",color);
        ch = cv::waitKey(1);
        if (ch == 'q')
            break;
    }
    double end = clock();
    std::cout << "fps : " << double(count / (end - start) * CLOCKS_PER_SEC) << std::endl;
    cam.closeCamera();
    return 0;
}
