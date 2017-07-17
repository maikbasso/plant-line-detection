/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include "line.hpp"
#include "pld.hpp"
#include "line-follower.hpp"

using namespace cv;
using namespace std;

void video(){
    Mat frame;
    // create a capture object
    VideoCapture cap;
    //from camera capture
    //cap.open(0);
    //from video file
    cap.open("../../videos/video-lines.mp4");

    if(!cap.isOpened()){
        cout << "Cannot open the capture or video file." << endl;
        return;
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
}

void photo(){

    // create the mat and open an image
    Mat image = imread("../../photos/test1.png");

    // Check for invalid input
    if(!image.data){
        cout << "Could not open or find the image." << endl;
        return;
    }
    
    // create the line detection object
    PlantLineDetection *pld = new PlantLineDetection();

    //create the follow line object
    LineFollower *fl = new LineFollower();
    
    //set parameters to line follower
    fl->setImageSize(320,240); // in px
    fl->setReferenceToFollow(320/2); // in px
    fl->setCameraApertureAngle(64); //66 <> 62 degrees from raspicam
    fl->setCameraAngleToGround(15); //in relation of the ground
    fl->setCameraHeight(2); //in meters
        
    //resize image for input
    Mat tempImage;
    resize(image, tempImage, Size(320,240) );

    //set image
    pld->setImage(tempImage);

    // detect
    pld->detect();

    // show results
    pld->showResults();

    vector <Line*> lines = pld->getDetectedLines();

    if(lines.size() > 0){

        fl->setLines(lines);
        fl->follow();
        fl->showResults();

    }

    //wait key to finish
    waitKey(0);
}

void piCamera(){

}

int main(){

    int op = -1;

    do{
        // ask user for use video or photo data
        cout << "Select the input data:" << endl;
        cout << "\t[0] Exit" << endl;
        cout << "\t[1] Video" << endl;
        cout << "\t[2] Photo" << endl;
        cout << "\t[3] Raspicam" << endl;
        cout << ">> ";
        //cin >> op;
        //fixed for build it in sublime cout << "2" << endl;
        op = 2; cout << "2" << endl;

        if(op == 1){
            cout << "Video input selected!" << endl;
            video();
            op = 0;
        }
        else if(op == 2){
            cout << "Photo input selected!" << endl;
            photo();
            op = 0;
        }
        else if(op == 3){
            cout << "Raspicam was selected as input!" << endl;
            piCamera();
            op = 0;
        }
        else if(op == 0){
            cout << "Bye Bye!" << endl;
        }
        else{
            cout << "Error: Select the correct data type!" << endl;
            op == -1;
        }

    }while(op != 0);

    // finish the program without errors
    return 0;
}