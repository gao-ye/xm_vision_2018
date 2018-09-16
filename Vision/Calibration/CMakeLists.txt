#This part is used for compile faster_rcnn_cpp.cpp
cmake_minimum_required (VERSION 2.8)

project (calibrate)

add_executable(calibrate calibrate.cpp)

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
	/home/ye/Vision/Camera
	/home/ye/freenect2/include
	/usr/local/include
  )

link_directories(/home/ye/Vision/Camera/lib
	/home/ye/freenect2/lib)
target_link_libraries(calibrate /home/ye/Vision/Camera/lib/lib/libcamera.so
	/home/ye/freenect2/lib/libfreenect2.so
	/usr/local/lib/libopencv_highgui.so
	/usr/local/lib/libopencv_core.so
    	/usr/local/lib/libopencv_imgproc.so
	/usr/local/lib/libopencv_imgcodecs.so
	/usr/local/lib/libopencv_shape.so
	/usr/local/lib/libopencv_videoio.so
	/usr/local/lib/libopencv_core.so.3.1
    )
