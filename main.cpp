/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "PLD/line.hpp"
#include "PLD/pld.hpp"
#include "PLD/line-follower.hpp"
#include "Socket/client.hpp"

using namespace cv;
using namespace std;
using namespace raspicam;

void followLine(Mat frame, TCPClient *client, PlantLineDetection *pld, LineFollower *fl){
    
    //resize image for input
    Mat resizedFrame;
    resize(frame, resizedFrame, Size(320,240));

    //set image
    pld->setImage(resizedFrame);

    // detect
    pld->detect();

    // show results
    pld->showResults();

    vector <Line*> lines = pld->getDetectedLines();

    if(lines.size() > 0){

        fl->setLines(lines);
        fl->follow();
        fl->showResults();
        std::ostringstream ss;
        ss << fl->getReferenceDistanceInM();
        
        client->sendData("{\"command\":\"setPosition\", \"args\": {\"x\":" + ss.str() +", \"y\":0.5, \"z\":0}}");

    }
}

void photo(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){

    // create the mat and open an image
    Mat image = imread("../tests/photos/test1.png");

    // Check for invalid input
    if(!image.data){
        cout << "Could not open or find the image." << endl;
        return;
    }

    //follow line and send command to drone
    followLine(image, c, pld, fl);

    //wait key to finish
    waitKey(0);
}

void video(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){
    Mat frame;
    // create a capture object
    VideoCapture cap;
    //from video file
    cap.open("../tests/videos/video-lines.mp4");

    if(!cap.isOpened()){
        cout << "Cannot open the capture or video file." << endl;
        return;
    }

    //read frame per frame
    while(cap.read(frame)){

        //follow line and send command to drone
        followLine(frame, c, pld, fl);

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
            break;
        }
    }
}

void webcam(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){
    Mat frame;
    // create a capture object
    VideoCapture cap;
    //capture from webcam
    cap.open(0);

    if(!cap.isOpened()){
        cout << "Cannot open the capture or video file." << endl;
        return;
    }

    //read frame per frame
    while(cap.read(frame)){

        //follow line and send command to drone
        followLine(frame, c, pld, fl);

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
            break;
        }
    }
}

/*
void piCamera(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){

    //create camera object
    raspicam::RaspiCam_Cv Camera;
    Mat frame;
    
    //Implemented properties:
    //CV_CAP_PROP_FRAME_WIDTH,CV_CAP_PROP_FRAME_HEIGHT,
    //CV_CAP_PROP_FORMAT: CV_8UC1 or CV_8UC3
    //CV_CAP_PROP_BRIGHTNESS: [0,100]
    //CV_CAP_PROP_CONTRAST: [0,100]
    //CV_CAP_PROP_SATURATION: [0,100]
    //CV_CAP_PROP_GAIN: (iso): [0,100]
    //CV_CAP_PROP_EXPOSURE: -1 auto. [1,100] shutter speed from 0 to 33ms
    //CV_CAP_PROP_WHITE_BALANCE_RED_V : [1,100] -1 auto whitebalance
    //CV_CAP_PROP_WHITE_BALANCE_BLUE_U : [1,100] -1 auto whitebalance 
    //Suported resolutions: 2592x1944 - 1920x1080 - 1296x972 - 1296x730 - 640x480

    //configure all parameters
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);   
    Camera.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    Camera.set(CV_CAP_PROP_FPS, 30); 
    Camera.setHorizontalFlip(true);
    Camera.setVerticalFlip(true);

    //Open camera
    if (!Camera.open()) {
        cerr << "Error opening the camera." << endl;
        return;
    }
    
    while(true) {        
        //frame capture
        Camera.grab();
        Camera.retrieve(frame);

        //follow line and send command to drone
        followLine(frame, c, pld, fl);

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
            break;
        }
    }
    
    //stop the camera
    Camera.release();
}
*/

int main(){

    //connect to drone-server
    TCPClient *c = new TCPClient();
    c->conn("localhost", 7000);

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

    //set gimbal position
    c->sendData("{\"command\": \"rotateGimbal\", \"args\": {\"pitch\":15,\"roll\":0,\"yaw\":0}}");
    sleep(1);
    //arming motors and take off
    c->sendData("{\"command\": \"arm\", \"args\": {}}");
    sleep(1);
    c->sendData("{\"command\": \"setSpeed\", \"args\": {\"airSpeed\": 7,\"groundSpeed\": 5}}");
    sleep(1);
    c->sendData("{\"command\": \"takeOff\", \"args\": {\"z\": 2}}");

    //Awaits take-off
    sleep(10);

    //Select data imput
    //webcam(c, pld, fl);
    video(c, pld, fl);
    //photo(c, pld, fl);
    //piCamera(c, pld, fl);

    // finish the program without errors
    return 0;
}