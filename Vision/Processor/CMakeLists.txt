#This part is used for compile faster_rcnn_cpp.cpp
cmake_minimum_required (VERSION 2.8)

project (ppp_test)

add_executable(ppp_test main.cpp)

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

include_directories (
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
find_library(PROCESSOR Processor /home/ye/Vision/Processor/lib/libprocessor)
find_library(SSD ssd /home/ye/Vision/Processor/lib/libssd)
target_link_libraries(ppp_test 
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
