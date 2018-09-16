#include "Camera.h"

namespace Vision
{
Camera::Camera() : listener(nullptr)
{
  if (freenect2.enumerateDevices() == 0)
  {
    std::cout << "no device connected!" << std::endl;
    return;
  }
  if (serial == "")
  {
    serial = freenect2.getDefaultDeviceSerialNumber();
  }

  pipeline = new libfreenect2::OpenGLPacketPipeline();
  dev = freenect2.openDevice(serial, pipeline);

  listener = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color |
                                                      libfreenect2::Frame::Depth |
                                                      libfreenect2::Frame::Ir);
  dev->setColorFrameListener(listener);
  dev->setIrAndDepthFrameListener(listener);
  state = Closed;
  if (dev == 0)
  {
    std::cout << "failure opening device" << std::endl;
    return;
  }
  if (!dev->start())
  {
    std::cout << "Camera failed to open!" << std::endl;
  }
  state = Closed;
  registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
  undistort = new libfreenect2::Frame(512, 424, 4);
  registered = new libfreenect2::Frame(512, 424, 4);
  depth2rgb = new libfreenect2::Frame(1920, 1082, 4);
}
Camera::~Camera()
{
  dev->stop();

  delete registration;
  delete updateThread;
}
void Camera::update()
{
  while (state == Open)
  {
    listener->waitForNewFrame(frames);

    std::unique_lock<std::mutex> lock(updateMutex);

    rgb = frames[libfreenect2::Frame::Color];
    depth = frames[libfreenect2::Frame::Depth];

    cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbImg);
    cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthImg);
    depthImg = depthImg; //the maximal range of kinect2 depth camera is 4096 mm. http://answers.opencv.org/question/76468/opencvkinect-onekinect-for-windows-v2linuxlibfreenect2/
    registration->apply(rgb, depth, undistort, registered, true, depth2rgb);
    cv::Mat(undistort->height, undistort->width, CV_32FC1, undistort->data).copyTo(undistortImg);
    cv::Mat(registered->height, registered->width, CV_8UC4, registered->data).copyTo(registeredImg);
    cv::Mat(depth2rgb->height, depth2rgb->width, CV_32FC1, depth2rgb->data).copyTo(depth2rgbImg);
    listener->release(frames);
    initSig.notify_all();
    lock.unlock();
  }
}
bool Camera::openCamera()
{
  state = Open;
  updateThread = new std::thread(trampoline, this);

  std::unique_lock<std::mutex> lock(updateMutex);
  initSig.wait(lock);

  return true;
}
void Camera::trampoline(Camera *camera)
{
  camera->update();
}
bool Camera::closeCamera()
{
  if (state == Closed)
  {
    std::cout << "Camera has already been closed!" << std::endl;
  }
  else
  {
    state = Closed;

    updateThread->join();
  }
  return true;
}
void Camera::getCordinate3D(std::vector<cv::Rect> detCordinate, std::vector<Vision::Cordinate3D> &targetCord)
{
  std::unique_lock<std::mutex> lock(updateMutex);
  int size = detCordinate.size();
  std::cout << " Cor size:" << size << std::endl;
  
  registeredImg.copyTo(outImg);
  for (int i = 0; i < size; i++)
  {
    Vision::Cordinate3D * temp = new Vision::Cordinate3D();
	Vision::Cordinate3D tem = map2D23D(detCordinate[i]);
    temp->x = tem.x;
	temp->y = tem.y;
	temp->z = tem.z;
	std::cout<<"inside"<<temp->x<<" "<<temp->y<<" "<<temp->z<<std::endl;
	if(std::isnan(temp->x) || std::isnan(temp->y) || std::isnan(temp->z))
		std::cout<<"temp is nan"<<std::endl;
    targetCor.push_back(*temp);
	//targetCor.insert(i,temp);
  }
  targetCord.swap(targetCor);
}
Cordinate3D Camera::map2D23D(cv::Rect rect)
{
  Cordinate3D cord;
  cord.x = 0;
  cord.y = 0;
  cord.z = 0;
  int x = rect.x * xRot + xDif;
  int width = rect.width * xRot;
  int y = rect.y * yRot + yDif;
  int height = rect.height * yRot;
  int max_x = x + width;
  int max_y = y + height;
  if (x > 512)
    x = 512;
  if (y < 0)
    y = 0;
  if (y > 424)
    y = 424;
  if (max_x < 0)
    max_x = 0;
  if (max_x > 512)
    max_x = 512;
  if (max_y < 0)
    max_y = 0;
  if (max_y > 424)
    max_y = 424;
  if (x == 512) //turn left
    {
      cord.x = 10;
      cord.y = 10;
      cord.z = 10;
	  return cord;    
	}
    if (max_x == 0) //turn right
    {
      cord.x = -10;
      cord.y = -10;
      cord.z = -10;
	  return cord;    
	}
    if (y == 424) //too high
    {
      cord.x = -5;
      cord.y = -5;
      cord.z = -5;
	  return cord;
    }
    if (max_y == 0) //too low
    {
      cord.x = 5;
      cord.y = 5;
      cord.z = 5;
      return cord;
    }
  if(x<0)
  {
      cord.x = 10;
      cord.y = 10;
      cord.z = 10;
	  return cord;    	
  }

  int thickness = -1;
  int lineType = 8;
  int center_x = x + width / 2;
  int center_y = y + height / 2;
  cv::circle(outImg,cv::Point(center_x,center_y),4,cv::Scalar(0,255,0),thickness,lineType);
  double diagnol = width > height ? height : width;
  int count_point = 0;
//  std::cout<<"outside loop"<<std::endl;
  for (int i = 0; i < max_x; i++)
  {
    for (int j = 0; j < max_y; j++)
    {
      
      if (sqrt((center_x - i) * (center_x - i) + (center_y - j) * (center_y - j)) <= (diagnol*0.2))
      {
//		std::cout<<"inside loop"<<std::endl;
        float x1, y1, z1;
        //registration->getPointXYZ(undistort, i, j, x1, y1, z1);
        registration->getPointXYZ(undistort, j, i, x1, y1, z1);
//		std::cout<<"loop" << x1 << " " << y1 << " " <<z1<<std::endl;
        if (std::isnan(x1) || std::isnan(y1) || std::isnan(z1))
          continue;        
		if(x1 <= -2 || y1 <= -2 || x1 >= 2 || y1 >= 2 || z1 >= 3)
          continue;
        cv::circle(outImg,cv::Point(i,j),1,cv::Scalar(255,0,0),thickness,lineType);
//		std::cout<<"inside coord"<<cord.x<<" "<<cord.y<<" "<<cord.z<<" "<<std::endl;
        cord.x += x1;
        cord.y += y1;
        cord.z += z1;
        count_point++;
      }	
    }
	if(cord.x >10000 || cord.y > 10000 || cord.z > 10000)
		break;
  }

  cord.x = cord.x / count_point;
  cord.y = cord.y / count_point;		
  cord.z = cord.z / count_point;

  /*if(std::isnan(cord.x) || std::isnan(cord.y) || std::isnan(cord.z))
  {
	float x, y, z;
    registration->getPointXYZ(undistort, center_y, center_x, x, y, z);
	std::cout<<"center point" << x << " " << y << " " <<z<<std::endl;
	cord.x = x ;
	cord.y = y ;
	cord.z = z ;
  }*/
  return cord;
  

//regis图片
/*  int x = rect.x;
  int y = rect.y;
  int width = rect.width;
  int height = rect.height;
  int max_x = x + width;
  int center_x = (x+max_x) / 2;
  int max_y = y + height;
  int center_y = (y + max_y) / 2;
  int num = 0;
  int thickness = -1;
  int lineType = 8;
  cv::circle(outImg,cv::Point(center_x,center_y),4,cv::Scalar(0,255,0),thickness,lineType);
  int diagnol = height > width ? width : height;
  for(int i = x ; i < x + width ; i++ )
  {
  	for(int j = y ; j < y + height ; j++ )
	{
		if (sqrt((center_x - i) * (center_x - i) + (center_y - j) * (center_y - j)) <= (diagnol*0.2))
        {
		float x1, y1, z1;
    	//registration->getPointXYZ(undistort, center_x, center_y, x1, y1, z1);
		registration->getPointXYZ(undistort, center_y, center_x, x1, y1, z1);
        if (std::isnan(x1) || std::isnan(y1) || std::isnan(z1))
          continue;        
		if(x1 >= 2 || y1 >= 2 || z1 >= 3)
          continue;
        cv::circle(outImg,cv::Point(i,j),1,cv::Scalar(255,0,0),thickness,lineType);
		cord.x += x1;
		cord.y += y1;
		cord.z += z1;
		num ++ ;
		if(cord.x > 10000 || cord.y > 10000 || cord.z > 10000)
		{
			break;
	    }
		}
	}
	if(cord.x > 10000 || cord.y > 10000 || cord.z > 10000)
	{
		break;
	}
  }
  cord.x = cord.x / num;
  cord.y = cord.y / num;		
  cord.z = cord.z / num;
  return cord;          */
}
cv::Mat Camera::transColorImg()
{
  std::unique_lock<std::mutex> lock(updateMutex);
  return rgbImg;
}
cv::Mat Camera::transDepthImg()
{
  std::unique_lock<std::mutex> lock(updateMutex);
  return depthImg;
}
cv::Mat Camera::transUndisImg()
{
  std::unique_lock<std::mutex> lock(updateMutex);
  return undistortImg;
}
cv::Mat Camera::transRegisImg()
{
  std::unique_lock<std::mutex> lock(updateMutex);
  return registeredImg;
}
cv::Mat Camera::transDepth2rgbImg()
{
  std::unique_lock<std::mutex> lock(updateMutex);
  return depth2rgbImg;
}
cv::Mat Camera::transOutImg()
{
  std::unique_lock<std::mutex> lock(updateMutex);
  return outImg;
}
} // namespace Vision
