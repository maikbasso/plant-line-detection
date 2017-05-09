/*
    @Author: Maik Basso <maik@maikbasso.com.br> 

    To compile and run program:
    g++ -std=c++11 pld-video.cpp -o pld-video `pkg-config --cflags --libs opencv` && ./pld-video

*/

//Include file for every supported OpenCV function
#include <opencv2/opencv.hpp>
#include <iostream>
//my libraries
#include "pld.h"

using namespace cv;
using namespace std;

int main(){
    Mat frame;
    // create a capture object
    VideoCapture cap;
    //from camera capture
    //cap.open(0);
    //from video file
    cap.open("videos/video-lines.mp4");

    if(!cap.isOpened()){
        cout << "Cannot open the capture or video file." << endl;
        return -1;
    }

    //create the line detection object
    PlantLineDetection *ld = new PlantLineDetection();

    //read frame per frame
    while(cap.read(frame)){
    	//resize image for input
    	Mat tempImage;
    	resize(frame, tempImage, Size(320,240) );

        //set image
        ld->setImage(tempImage);

        //detect
        ld->detect();

        //show results
        ld->showResults();

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
            break;
        }
    }

    //end the program without errors
    return 0;
}
