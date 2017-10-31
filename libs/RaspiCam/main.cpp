/*
 * Author: Maik Basso
 * Email: maik@maikbasso.com.br
 * 
 * */
 
#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>

using namespace std;
using namespace cv;
 
int main () {
    raspicam::RaspiCam_Cv Camera;
    Mat frame;
    
    //set camera params
    int resolution[][2] = {{1920,1080},{1336,768},{1280,720},{1024,768},{800,600},{640,480},{320,240},{160,120},{100,133}};
    int resolutionNumber = 5;
    
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);   
	Camera.set(CV_CAP_PROP_FRAME_WIDTH, resolution[resolutionNumber][0]);
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT, resolution[resolutionNumber][1]);
	Camera.set(CV_CAP_PROP_FPS, 30);
	
	Camera.setHorizontalFlip(true);
    Camera.setVerticalFlip(true);
	
	/**
	 * Sets a property in the VideoCapture.
	* * 
	* Implemented properties:
	* CV_CAP_PROP_FRAME_WIDTH,CV_CAP_PROP_FRAME_HEIGHT,
	* CV_CAP_PROP_FORMAT: CV_8UC1 or CV_8UC3
	* CV_CAP_PROP_BRIGHTNESS: [0,100]
	* CV_CAP_PROP_CONTRAST: [0,100]
	* CV_CAP_PROP_SATURATION: [0,100]
	* CV_CAP_PROP_GAIN: (iso): [0,100]
	* CV_CAP_PROP_EXPOSURE: -1 auto. [1,100] shutter speed from 0 to 33ms
	* CV_CAP_PROP_WHITE_BALANCE_RED_V : [1,100] -1 auto whitebalance
	* CV_CAP_PROP_WHITE_BALANCE_BLUE_U : [1,100] -1 auto whitebalance
	**/

    //Open camera
    if (!Camera.open()) {
		cerr << "Error opening the camera" << endl;
		return -1;
	}
	
	//create a window to show the image
	namedWindow("Results", CV_WINDOW_AUTOSIZE);
    
	while(true) {
		//start time
		double timerBegin = getTickCount();
		
		//frame capture
        Camera.grab();
        Camera.retrieve(frame);
        
        //total time
        double totalTime = double(getTickCount()-timerBegin)/double(getTickFrequency());
        double fps = (float) ((float)(1)/totalTime);
        
        //statistics
        system("clear");
		cout << "########################################" << endl;
		cout << "######## Raspicam by Maik Basso ########" << endl;
		cout << "########################################" << endl << endl;
		cout << "\tFrame size: " << frame.size().width << "x" << frame.size().height << endl; 
		cout << "\tPixels per frame: " << (frame.size().width*frame.size().height) << endl;
		cout << "########################################" << endl;
		cout << "\tTime per frame: " << totalTime  << " s" << endl;
		cout << "\tFPS: " << fps << endl;
		cout << "########################################" << endl;
		
		//display image
		imshow("Results", frame);
		waitKey(1);
    }
    
    //stop the camera
    Camera.release();

    return 0;
}
