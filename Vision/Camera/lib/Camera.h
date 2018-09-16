#ifndef CAMERA_H
#define CAMERA_H
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <math.h>
namespace Vision
{

struct Cordinate3D
{
    float x;
    float y;
    float z;
};
class Camera
{
  private:
    enum State
    {
        Closed = 0,
        Open
    };           //illustrate the condition of the camera
    State state; // the condition of the camera
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = 0;
    libfreenect2::PacketPipeline *pipeline = 0;
    std::string serial;
    bool enable_rgb = true;
    bool enable_depth = true;
    int deviceID = 0;
    libfreenect2::SyncMultiFrameListener *listener;
    libfreenect2::FrameMap frames;
    libfreenect2::Registration *registration;
    libfreenect2::Frame *undistort;
    libfreenect2::Frame *registered;
    libfreenect2::Frame *rgb;
    libfreenect2::Frame *depth;
    libfreenect2::Frame *depth2rgb;
    std::thread *updateThread;
    std::mutex updateMutex;
    std::condition_variable initSig;
    static void trampoline(Camera *); //this function belong to the class
    cv::Mat rgbImg;
    cv::Mat depthImg;
    cv::Mat undistortImg;
    cv::Mat registeredImg;
    cv::Mat depth2rgbImg;
    cv::Mat outImg;
    float xRot = 0.3353; //rotation parameter at coordinate x
    float yRot = 0.3409; //rotation parameter at coordinate y
    //float xDif = -74.308; //bias parameter at coordinate x
	 float xDif = -86.02; //bias parameter at coordinate x
    float yDif = 25.089;  //bias parameter at coordinate y
    std::vector<Vision::Cordinate3D> targetCor;
    void update();
    /****************************************/
    // function name: map2D23D

    // input parameter: the bounding box of the detected object

    // returned value: the real world coordinate of the detect object

    /****************************************/
    Cordinate3D map2D23D(cv::Rect rect);

  public:
    Camera();
    ~Camera();
    bool openCamera();
    bool closeCamera();
    void getCordinate3D(std::vector<cv::Rect> detCordinate, std::vector<Vision::Cordinate3D> &targetCord);
    cv::Mat transUndisImg();
    cv::Mat transRegisImg();
    cv::Mat transColorImg();
    cv::Mat transDepthImg();
    cv::Mat transDepth2rgbImg();
    cv::Mat transOutImg();
};
}; // namespace Vision

#endif
