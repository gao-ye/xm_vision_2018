cmake_minimum_required(VERSION 2.8.3)
project(xm_vision)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  xm_msgs
)


catkin_package(
)
include_directories(
 include
  ${catkin_INCLUDE_DIRS}
)
include_directories (
    src
    include
    /home/ye/Vision/Processor/lib/libprocessor
    /home/ye/freenect2/include
    /home/ye/Vision/Processor/lib/libssd
    /home/ye/Vision/Camera/lib
    /home/ye/library/caffe-ssd/include
    /home/ye/library/caffe-ssd/build/lib	
    /usr/local/include 
    /usr/include/eigen3 
    /usr/include/python2.7
    /usr/local/cuda-8.0/include  )

add_library(identify src/identify.cpp)

add_executable(people_tracking     src/people_tracking.cpp)
add_executable(people_identify      src/people_identify.cpp)
add_executable(take_photos         src/take_photos.cpp)
add_executable(object_all          src/object_all.cpp)
add_executable(object_detect       src/object_detect.cpp)
add_executable(code_107       src/code_107.cpp)


find_library(PROCESSOR Processor /home/ye/Vision/Processor/lib/libprocessor)
find_library(SSD ssd /home/ye/Vision/Processor/lib/libssd)

 target_link_libraries( identify 
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1

  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )


 target_link_libraries( people_tracking
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )
 
 target_link_libraries( people_identify identify 
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1

  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )

  target_link_libraries( take_photos
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )

  target_link_libraries( object_all
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1
  #/usr/lib/x86_64-linux-gnu/libopencv_highgui.so
  #/usr/lib/x86_64-linux-gnu/libopencv_core.so
  #/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )

  target_link_libraries( object_detect
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )
   target_link_libraries( code_107
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )

add_dependencies(people_tracking ${PROJECT_NAME}_cpp)
add_dependencies(people_identify ${PROJECT_NAME}_genmsg)
add_dependencies(object_all ${PROJECT_NAME}_genmsg)
add_dependencies(object_detect ${PROJECT_NAME}_genmsg)


