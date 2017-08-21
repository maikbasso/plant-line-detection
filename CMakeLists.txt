cmake_minimum_required(VERSION 2.8)
project( PLD )
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)
find_package(raspicam REQUIRED)
find_package( OpenCV REQUIRED )
unset(LD_LIBRARY_PATH)
include_directories( libs )
add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} ) # use this in pc
#target_link_libraries( main ${raspicam_CV_LIBS} ) # use this in the Raspberry Pi