#####################################
cmake_minimum_required (VERSION 2.8) 
project (main)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)
find_package(raspicam REQUIRED)
find_package(OpenCV)
IF(OpenCV_FOUND AND raspicam_CV_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (main main.cpp)  
	target_link_libraries (main ${raspicam_CV_LIBS})
ELSE()
	MESSAGE(FATAL_ERROR "OPENCV OR RASPICAM NOT FOUND")
ENDIF()
#####################################
