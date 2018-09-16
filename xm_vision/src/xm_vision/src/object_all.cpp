#include <iostream>
#include <Processor.h>
#include <Camera.h>
#include <time.h>

std::vector<std::string> name_init();
int main()
{
    
    Vision::Camera cam;
    if(!cam.openCamera())
    {
    	std::cout<<"camera open failed"<<std::endl;
        std::cout<<"Task:detetct_all failed! "<<std::endl;
	    return 0;
    }
    
    Vision::Processor processor("object");      //处理类初始化
    std::vector<std::string>vec = name_init();//清单名称初始化
    processor.setObjectName(vec);

    int count = 1;
    int obj_num=0;
    cv::Mat frame;
    cv::Mat result_img;
    std::vector<std::string> detect_name;
    std::vector<std::string> result_name;
    std::vector<cv::Rect> detCordinate;

    while (count<=30)       //检测30次,记录下检测结果最好，即检测出数目最多的一次，
    {
	    count++;
	    cam.transColorImg().copyTo(frame);
        processor.passMat(frame);
        detect_name = processor.getAllObject();
        if(detect_name.size() > obj_num)
        {
            obj_num = detect_name.size();
            result_img =  processor.getDetectedImage();  //当前检测结果图片
            result_name = detect_name;                   //当前检测结果名字
        }

        std::cout<<"the number of objects detected is "<<obj_num<<std::endl;
    }

    std::ofstream outfile("/home/ye/Vision/data/result/out.txt", ios::trunc ); //将检测到物品名称保存txt
    for(int i=0;i<result_name.size();i++)
    {
        outfile << result_name[i]; 
        outfile << std::endl;
    }
    outfile.close();

    cv::imwrite("/home/ye/Vision/data/result/detect_all_result.jpg",result_img);
    cv::resize(result_img,result_img,cv::Size(result_img.cols/4,result_img.rows/4),0,0);    
    cv::imshow("detect",result_img);
    cv::moveWindow("detect",0,0);
    cv::waitKey(3000);
    cam.closeCamera();
    std::cout<<"Task:detetct_all finish! "<<std::endl;
    std::cout<<"the number of objects detected is "<<obj_num<<std::endl;
    return 0;

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
