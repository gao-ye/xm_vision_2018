#include <iostream>
#include <Camera.h>
int main()

{
    
    Vision::Camera cam;
    cv::Mat frame;
    // cv::namedWindow("windows",0);
    // cv::namedWindow("test",0);
    if(!cam.openCamera())
    {
    	std::cout<<"camera open failed"<<std::endl;
	return 0;
    }

    // frame=cv::imread("/home/ye/pic/1.jpg");
    // cv:: resize(frame,frame,cv::Size(800,600));  
    // cv::imshow("test",frame);
    int pic_num = 120;
   // system("sudo rm     /home/ye/Vision/data/dataset/");
   // system("sudo mkdir  /home/ye/Vision/data/dataset");
    while (1)
    {
	    
	    cam.transColorImg().copyTo(frame);       
        // cv::imshow("windows", frame);
        cv:: resize(frame,frame,cv::Size(0.5*frame.cols,0.5*frame.rows));
        cv::imshow("test",frame);
        char ch =cv::waitKey(1);
        if(ch == 'c')
        {
            char buf[7];
            sprintf(buf,"%06d",pic_num);
            std::string name;
            name = buf;
            std::string path = "/home/ye/Vision/data/dataset/"+name+".jpg";
            cv::imwrite(path,frame);
            pic_num++;
            std::cout<<"picture have "<<pic_num<<std::endl;
        }
        if(ch == 'q')
	    break;    
    }

    cam.closeCamera();
    return 0;

         

}
