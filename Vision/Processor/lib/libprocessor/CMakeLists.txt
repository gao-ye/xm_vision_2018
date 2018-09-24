cmake_minimum_required (VERSION 2.8)

SET (SRC_LIST Processor.cpp)
include_directories ( 
    /home/ye/Vision/Processor/lib/libssd
    /home/ye/library/caffe-ssd/include
    /home/ye/library/caffe-ssd/build/lib
    #~/py-faster-rcnn/lib/nms 
    /share/apps/local/include
    /usr/local/include 
    /usr/include/python2.7
    /usr/local/cuda-8.0/include )

add_compile_options(-std=c++11)

add_library(Processor SHARED ${SRC_LIST})
