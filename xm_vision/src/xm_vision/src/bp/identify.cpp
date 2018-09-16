#include "identify.hpp"
#include <ros/ros.h>

identify::identify()
{
    // processor = Vision::Processor("person");
    // Vision::Processor processor("person");
}

// identify::~identify()
// {

// }

void identify::task_start(ros::NodeHandle & n)
{
     cv::namedWindow("test",0);
     cv::Mat src;
    ros::ServiceServer service = n.advertiseService("get_position", &identify::callback, this);
    int cnt = 0;

    while(1)
    {
        if(if_detected)
        {
            break;
        }
        else
        {   cnt++;
            // std::cout<<"time out!"<<std::endl;
            // std::cout<<"Wait command!!!"<<std::endl;
            // if(cnt>=1000000)if_detected=1;
            // cv::Mat src;
            // cam.transColorImg().copyTo(src);
            // cv:: resize(src,src,cv::Size(src.cols/2,src.rows/2),0,0);  
            // cv::imshow("test",src);

            ros::spinOnce();


        }
    }
  

}


bool identify::callback(xm_msgs::xm_ObjectDetect::Request &req, 
                      xm_msgs::xm_ObjectDetect::Response &res)
{
    std::string obj_name_tmp;
    obj_name_tmp = req.object_name;
    xm_msgs::xm_Object obj_tmp;
    std::cout<<"33331111111111111111111111111111111111111"<<std::endl;
    if(obj_name_tmp == "person")
    {
        if(req.people_id == -1)
        {
             std::cout<<"11111111111111111111111111111111111111"<<std::endl;
            get_person_location(1, -1);
            // 需考虑bug，如果没有识别到人群的情况  后面添加
            // for(int i = 0; i < group_position.size(); i++)
            //std::cout<<"222222222222222222222222//2222221"<<std::endl;
            for(int i=0; i<targetCor.size();i++)
            {
            obj_tmp.name =i;                   //无用响应信息
            obj_tmp.pos.point.x = targetCor[i].x;//x 
            obj_tmp.pos.point.y = targetCor[i].y;//y   
            obj_tmp.pos.point.z = targetCor[i].z;//z
            obj_tmp.pos.header.frame_id = "camera_link";
            obj_tmp.pos.header.stamp = ros::Time::now();

			
            
            res.object.push_back(obj_tmp);
            }
			//if_detected = 1;
            return true;
        }
        else
        {
            get_person_location(2, req.people_id);
            obj_tmp.name = -1;
            
            obj_tmp.pos.point.x = single_position[0];//x  targetCor[0].y
            obj_tmp.pos.point.y = single_position[1];//y
            obj_tmp.pos.point.z = single_position[2];//z
           
            std::cout<<"obj_tmpX: "<<obj_tmp.pos.point.x<<std::endl;
            std::cout<<"obj_tmpY: "<<obj_tmp.pos.point.y<<std::endl;
            std::cout<<"obj_tmpZ: "<<obj_tmp.pos.point.z<<std::endl;
            
            obj_tmp.pos.header.frame_id = "camera_link";
            obj_tmp.pos.header.stamp = ros::Time::now();
            obj_tmp.state = wrong_flag;
            res.object.push_back(obj_tmp);
			//if_detected = 1;
            return true;
        }
    }

}

int identify::get_person_location(int command_num, int search_id)
{
    
    Vision::Processor processor("person");
    if(command_num == 1)                    //if command_num == 1，take photos of five persn and store
    {
        Vision::Camera cam;
        if(!cam.openCamera())
        {
    	    std::cout<<"camera open failed"<<std::endl;
            if_detected = 1;
	        return 0;
        }

        cv::Mat frame0;
        for(int i=1;i<=10;i++)                   //避免拍摄的前几张图片出现问题
        {
            cam.transColorImg().copyTo(frame0);
        }

        cam.transColorImg().copyTo(frame);
        

        // frame=cv::imread("/home/ye/pic/4.jpg",1);
        if(frame.empty())
        {
            std::cout<<"command1--图片读取失败"<<std::endl;
            if_detected = 1;                    //end the node
            
        }
        processor.passMat(frame);
        
        system("rm -rf /home/ye/Vision/data/people_identify/");
        system("mkdir /home/ye/Vision/data/people_identify/");
        
       
        cv::Mat dst=processor.getImage();
        cv::imwrite("/home/ye/Vision/data/people_identify/dst.jpg",frame);

        position_five_people = processor.five_people();    //获取面积最大五人矩形
        int position_size = position_five_people.size();
        std::cout<<"获取面积最大五人矩形　 执行完成"<<std::endl;
        std::cout<<"检测到　person 个数为 "<<position_size<<std::endl;

        targetCor.clear();
        cam.getCordinate3D(position_five_people,targetCor);
        cam.closeCamera();                      //使用后就关闭

        if(position_size > 5) position_size = 5;

        for(int i = 0; i < position_size; i++)
        {
            char buf[2];
            sprintf(buf, "%d", i);
            std::string int_to_name;
            int_to_name = buf;
            std::string single_face_path = "/home/ye/Vision/data/people_identify/" + int_to_name + ".jpg";
            // rect1 = position_five_people[i]
            // std::cout << rect1.x << std::endl << rect1.y <<std::endl ;
            // std::cout << rect1.height << std::endl << rect1.width << std::endl;
            std::cout << position_five_people[i].x << std::endl << position_five_people[i].y <<std::endl ;
            std::cout << position_five_people[i].height << std::endl << position_five_people[i].width << std::endl;
            cv::imwrite(single_face_path, frame(position_five_people[i]));
        }
        if_detected = 1;                                        //end the node
        return 0;
    }

    else if(command_num == 2)
    {
        
        /*******************/
        //静态图片测试
        /*******************/

        // frame=cv::imread("/home/ye/pic/4.jpg",1);            
        // if(frame.empty())
        // {
        //     std::cout<<"command2--图片读取失败"<<std::endl;
        //     if_detected = 1;                                 //end the node
        // }

        /*******************/
        //实时图片测试
        /*******************/
        Vision::Camera camera1;                                 
        if(!camera1.openCamera())
        {
            camera1.closeCamera();
            if_detected = 1;   
	        return 0;
        }
        camera1.transColorImg().copyTo(frame);                 
        cv::imwrite("/home/ye/Vision/data/people_identify/scene.jpg", frame);

        system("rm /home/ye/Vision/data/people_identify/id.txt");
        system("rm /home/ye/Vision/data/people_identify/message.txt");

        std::ofstream out("/home/ye/Vision/data/people_identify/id.txt");
        if (out.is_open())   
        {  
            out << search_id;   
            out.close();  
        }  

	    system("python /home/ye/xm_vision/src/xm_vision/src/face.py");

        int top, right, bottom, left;
        std::ifstream in("/home/ye/Vision/data/people_identify/message.txt");
        if (! in.is_open())  
        { 
            std::cout << "Error opening file"; 
            if_detected = 1;                //end the node
            exit (1);
        }  

        while (!in.eof() )  
        {  
            in>>top;
            in>>right;
            in>>bottom;
            in>>left;
        }

        std::cout<<"top:"<<top<<std::endl;//y
        std::cout<<"right:"<<right<<std::endl;//
        std::cout<<"bottom:"<<bottom<<std::endl;
        std::cout<<"left:"<<left<<std::endl;//x
        in.close();
        std::cout<<"读取　message.txt 完成"<<std::endl;
        
        if(top == -1 && right == -1 && top == -1 && bottom == -1)
        {
            single_position.push_back(-1);
            single_position.push_back(-1);
            single_position.push_back(-1);
            wrong_flag = -1;
            if_detected = 1;         //end the node
            std::cout<<"1111"<<std::endl;
            return 0;
        }
        if(top == -2 && right == -2 && top == -2 && bottom == -2)
        {
            single_position.push_back(-2);
            single_position.push_back(-2);
            single_position.push_back(-2);
            wrong_flag = -2;
            if_detected = 1;         //end the node 
            return 0;
        }

		
        
        std::vector<cv::Rect> rect;
        std::cout<<"2222"<<std::endl;
        int x = left;
        int y = top;
        int width = right - left;
        int height = bottom - top;
        cv::Rect rect1 = cv::Rect(x,y,width,height);
        rect.push_back(rect1);
        targetCor.clear();
        camera1.getCordinate3D(rect,targetCor);
        std::cout<<"获取目标person ３Ｄ 完成"<<std::endl;
        camera1.closeCamera();        
        std::cout<<"the size of targetCor is "<<targetCor.size()<<std::endl;

        single_position.clear();
       
        single_position.push_back(targetCor[0].x);   //single_position[0]
        single_position.push_back(targetCor[0].y);   //single_position[1]
        single_position.push_back(targetCor[0].z);  //single_position[2]
        //打印测试
        std::cout<<"X: "<<targetCor[0].x<<std::endl;
        std::cout<<"Y: "<<targetCor[0].y<<std::endl;
        std::cout<<"Z: "<<targetCor[0].z<<std::endl;


		if(std::isnan(targetCor[0].x) || std::isnan(targetCor[0].y) || std::isnan(targetCor[0].z))
  		{
		    wrong_flag = -1;        //未获取到深度信息
            if_detected = 1;
            return 0;
  		}

//rbg识别到目标，但深度图上未匹配到目标　对应措施
        // if(single_position[0] == -3 && single_position[1] == -3)    //后面补充
        // {
        //     if(single_position[2] == -3)//需要机器人向右转，图像过于偏右侧
        //         wrong_flag = -3;
        //     if(single_position[2] == -4)//需要机器人向左转，图像过于偏左侧
        //         wrong_flag = -4;
        //     if_detected = 1;
        //     return 0;
        // }

        if(single_position[0] == -10 )    //后面补充
        {
            wrong_flag = -3;        //左转
            if_detected = 1;
            return 0;
        }
        
        if(single_position[0] == 10)   
        {
            wrong_flag = -4;        //右转
            if_detected = 1;
            return 0;
        }            
               
        cv::imshow("regImage",frame(rect1));
        cv::waitKey(1000);

        if_detected = 1;
        return 0;
            
        // single_position这个double型的vector用于确认具体的某一个人的真实世界坐标
        // single_position
        // search_id为需要找的人的ID号
        }

}


