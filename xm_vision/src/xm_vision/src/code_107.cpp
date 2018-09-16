#include <iostream>
#include <Processor.h>
#include <Camera.h>
#include <time.h>

std::vector<std::string> name_init();
int main()
{
    
    Vision::Processor processor("object");      //处理类初始化
    std::vector<std::string>vec = name_init();//清单名称初始化
    processor.setObjectName(vec);

    cv::VideoCapture capture(0);
    cv::Mat frame;
    cv::Mat result;
    capture >> frame;
    if(frame.empty())
    {
        std::cout<<"Open camera failed!!!!!!!!"<<std::endl;
        return 0;         
    }

    while(1)            
    {
        capture >> frame;
        processor.passMat(frame);
        result =  processor.get_107_DetectedImage();
        // cv::resize(result,result,cv::Size(result.cols/4,result.rows/4),0,0);  
        for(int i=0;i<=result.cols/100;i++)
            for(int j=0;j<=result.rows/100;j++)
            {
                cv::Point p1(i*100,j*100);
                cv::circle(result, p1, 3,cv::Scalar(255,0,0),-1); //第五个参数我设为-1，表明这是个实点。  
            } 
        cv::imshow("detect",result);
        cv::moveWindow("detect",0,0);
            
        char ch = cv::waitKey(1);
        if(ch == 'q')
        {
            processor.recordMessage();//在txt中记录下所有物品的信息
            cv::imwrite("/home/ye/Vision/data/result/detect_107_result.jpg",result);  
            break;
        }
         
    }
    std::cout<<"Detection task is finished!"<<std::endl;
    std::cout<<"Now,you can capture these objects!"<<std::endl;

    while(1)            //实时显示物品信息
    {
        capture >> frame;
        processor.passMat(frame);
        result =  processor.get_107_DetectedImage();
        // cv::resize(result,result,cv::Size(result.cols/4,result.rows/4),0,0);    
        for(int i=0;i<=result.cols/100;i++)
            for(int j=0;j<=result.rows/100;j++)
            {
                cv::Point p1(i*100,j*100);
                cv::circle(result, p1, 3,cv::Scalar(255,0,0),-1); //第五个参数我设为-1，表明这是个实点。  
            }
       
        
        cv::imshow("detect",result);
        cv::moveWindow("detect",0,0);
        char ch = cv::waitKey(1);
        if(ch == 'q')
            break;

         
    }

    std::cout<<"Task:detetct_all finish! "<<std::endl;
    return 0;

}


std::vector<std::string> name_init()
{
    std::vector<std::string>vec;
    std::string s;
    std::ifstream inf;
    inf.open("/home/ye/Vision/data/107_object_name.txt");
    while(std::getline(inf,s))
    {
    vec.push_back(s);
    }
    inf.close();    
    std::cout<<"the number of objects is :"<<vec.size();
    std::cout<<std::endl;
    return vec;
}
