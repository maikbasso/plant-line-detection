#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
//my libraries
#include "pld.h"

using namespace cv;
using namespace std;

int main(){
    Mat frame, resultsFrame;
    // open the video file for reading
    VideoCapture cap("../videos/video-lines.mp4");
    //for camera capture
    //VideoCapture cap;
    //cap.open(0);

    PlantLineDetection *ld = new PlantLineDetection();

    if(!cap.isOpened()){
        cout << "Cannot open the capture or video file." << endl;
        return -1;
    }

    //select the start time video in ms
    //cap.set(CV_CAP_PROP_POS_MSEC, 300);

    //show the video properties
    cout << "Video properties: " << endl << "============================" << endl;
    cout << "Capture format: " << cap.get(CV_CAP_PROP_FORMAT) << endl;
    cout << "Capture mode: " << cap.get(CV_CAP_PROP_MODE) << endl;
    cout << "Frame size: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Frame per seconds: " << cap.get(CV_CAP_PROP_FPS) << endl;
    cout << "============================" << endl << "Press ESC for exit." << endl << endl;

    //create a window called "Results"
    namedWindow("Results", CV_WINDOW_AUTOSIZE);

    //read frame per frame
    while(cap.read(frame)){
    	//resize image for input
    	Mat tempImage;
    	resize(frame,tempImage, Size(320,180) );

        //set image
        ld->setImage(tempImage);

        //detect
        ld->detect();

        //get results
        resultsFrame = ld->getResultsFrame();

        //show the frame in "MyVideo" window
        imshow("Results", resultsFrame);

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
                break;
       }
    }

    return 0;
}