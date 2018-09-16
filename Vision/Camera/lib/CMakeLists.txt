cmake_minimum_required (VERSION 2.8)

SET (SRC_LIST Camera.cpp)
include_directories ( 
	/home/ye/freenect2/include
	/home/ye/Vision
	/usr/local/include
	/usr/include
        /share/apps/local/include
	/usr/include/python2.7	
 )

link_libraries( "/home/ye/freenect2/lib/libfreenect2.so"
	
)

add_definitions(-std=c++11)

add_library(camera SHARED ${SRC_LIST})
