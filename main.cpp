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

    //connect to server
    TCPClient *c = new TCPClient();
    c->conn("localhost", 7000);
    
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
    //pld->showResults();

    vector <Line*> lines = pld->getDetectedLines();

    if(lines.size() > 0){

        fl->setLines(lines);
        fl->follow();
        fl->showResults();
        std::ostringstream ss;
        ss << fl->getReferenceDistanceInM();
        
        c->sendData("{\"command\":\"test\",\"args\": {\"x\":" + ss.str() + "}}");

    }

    //wait key to finish
    waitKey(0);
}

/*
void piCamera(){

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
    Camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    Camera.set(CV_CAP_PROP_FPS, 30); 
    Camera.setHorizontalFlip(true);
    Camera.setVerticalFlip(true);

    //Open camera
    if (!Camera.open()) {
        cerr << "Error opening the camera." << endl;
        return;
    }
    
    //create a window to show the image
    //namedWindow("Results", CV_WINDOW_AUTOSIZE);

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
    
    while(true) {        
        //frame capture
        Camera.grab();
        Camera.retrieve(frame);

        //set image
        pld->setImage(frame);

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
        
        //display image
        //imshow("Results", frame);
        //waitKey(1);
    }
    
    //stop the camera
    Camera.release();
}
*/

int main(){

    int op = -1;

    do{
        // ask user for use video or photo data
        cout << "Select the input:" << endl;
        cout << "\t[0] Exit \t[1] Video \t[2] Photo \t[3] Raspicam" << endl;
        cout << ">> ";
        //cin >> op;
        //fixed for build it in sublime
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
            //piCamera();
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