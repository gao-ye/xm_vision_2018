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

generate_messages(
    DEPENDENCIES
   std_msgs  # Or other packages containing msgs
 )

catkin_package(
)



include_directories (
    src
    include
    ${catkin_INCLUDE_DIRS}
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
add_executable(people_idetify      src/people_identify.cpp)
add_executable(take_photos         src/take_photos.cpp)
add_executable(hello               src/hello.cpp)
add_executable(object_all          src/object_all.cpp)
add_executable(object_detect       src/object_detect.cpp)


find_library(PROCESSOR Processor /home/ye/Vision/Processor/lib/libprocessor)
find_library(SSD ssd /home/ye/Vision/Processor/lib/libssd)

link_directories(
  ${catkin_LIBRARIES}
    /usr/local/lib
)

 target_link_libraries( people_tracking
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
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
	/home/ye/Vision/Camera/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
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
	/home/ye/Vision/Camera/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )

 target_link_libraries( hello
  ${catkin_LIBRARIES}
  ${PROCESSOR}
  ${SSD}
	/home/ye/freenect2/lib/libfreenect2.so
	/home/ye/Vision/Camera/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
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
	/home/ye/Vision/Camera/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
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
	/home/ye/Vision/Camera/lib/libcamera.so
  /home/ye/library/caffe-ssd/build/lib/libcaffe.so
  /usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
  /usr/local/lib/libopencv_imgproc.so
  /usr/lib/x86_64-linux-gnu/libglog.so
  /usr/lib/x86_64-linux-gnu/libboost_system.so
  /usr/lib/x86_64-linux-gnu/libboost_python.so
  /usr/lib/x86_64-linux-gnu/libpython2.7.so
 )

add_dependencies(people_tracking ${PROJECT_NAME}_genmsg)
add_dependencies(people_identify ${PROJECT_NAME}_genmsg)
add_dependencies(object_all ${PROJECT_NAME}_genmsg)
add_dependencies(object_detect ${PROJECT_NAME}_genmsg)


set(CMAKE_BUILD_TYPE Release)
