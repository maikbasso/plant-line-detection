# plant-line-detection
Autonomous system for unmanned aerial vehicles (UAV) focusing on precision agriculture.

# Install dependencies for Python:
	$ sudo apt-get install libopencv-dev python-opencv
	$ sudo apt-get install python-matplotlib

# Compiling the c ++ code
  - In C++ folder, run the command:
  	1. For photo: `$ g++ -std=c++11 pld-photo.cpp -o pld-photo `pkg-config --cflags --libs opencv` && ./pld-photo`
  	2. For video: `$ g++ -std=c++11 pld-video.cpp -o pld-video `pkg-config --cflags --libs opencv` && ./pld-video`
