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

    //get the video properties
    double capMode = cap.get(CV_CAP_PROP_MODE);
    double capFormat = cap.get(CV_CAP_PROP_FORMAT);
    double fps = cap.get(CV_CAP_PROP_FPS);
    int frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);


    //show the video properties
    cout << "Video properties: " << endl << "============================" << endl;
    cout << "Capture format: " << capFormat << endl;
    cout << "Capture mode: " << capMode << endl;
    cout << "Frame size: " << frameWidth << "x" << frameHeight << endl;
    cout << "Frame per seconds: " << fps << endl;
    cout << "============================" << endl << "Press ESC for exit." << endl << endl;

    //create a window called "Results"
    //namedWindow("Results",CV_WINDOW_AUTOSIZE);

    //read frame per frame
    while(cap.read(frame)){
        ld->setImage(frame);
        resultsFrame = ld->getResultsFrame();
        //show the frame in "MyVideo" window
        //imshow("Results", resultsFrame);
        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
                break;
       }
    }

    return 0;
}
