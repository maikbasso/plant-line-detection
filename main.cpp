/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "PLD/line.hpp"
#include "PLD/pld.hpp"
#include "PLD/line-follower.hpp"
#include "Socket/client.hpp"

using namespace cv;
using namespace std;
using namespace raspicam;
using namespace std::chrono;

void followLine(Mat frame, TCPClient *client, PlantLineDetection *pld, LineFollower *fl);

void photo(TCPClient *c, PlantLineDetection *pld, LineFollower *fl);

void video(TCPClient *c, PlantLineDetection *pld, LineFollower *fl);

void webcam(TCPClient *c, PlantLineDetection *pld, LineFollower *fl);

void piCamera(TCPClient *c, PlantLineDetection *pld, LineFollower *fl);

void simulatedTests(TCPClient *c, PlantLineDetection *pld, LineFollower *fl);

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
    fl->setCameraApertureAngle(64); //66 <> 62 degrees from raspicam
    fl->setCameraAngleToGround(15); //in relation of the ground
    fl->setCameraHeight(2); //in meters

    //set gimbal position
    //c->sendData("{\"command\": \"rotateGimbal\", \"args\": {\"pitch\":15,\"roll\":0,\"yaw\":0}}");
    //sleep(1);
    //arming motors and take off
    //c->sendData("{\"command\": \"arm\", \"args\": {}}");
    //sleep(1);
    //c->sendData("{\"command\": \"setSpeed\", \"args\": {\"airSpeed\": 7,\"groundSpeed\": 5}}");
    //sleep(1);
    //c->sendData("{\"command\": \"takeOff\", \"args\": {\"z\": 2}}");

    //Awaits take-off
    //sleep(10);

    //Select data imput
    //webcam(c, pld, fl);
    //video(c, pld, fl);
    //photo(c, pld, fl);
    //piCamera(c, pld, fl);
    simulatedTests(c, pld, fl);

    // finish the program without errors
    return 0;
}

void simulatedTests(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){
    // terminal display
    ostringstream display;

    display << "Starting the tests" << endl;

    //all of resolutions of test
    int resolutions[][2] = {{160,120},{320,240},{640,480},{800,600},{1024,768},{1296,972},{1366,768},{1920,1080}};

    //number of tests per resolution
    int testsPerResolution = 1;

    display << "Total number of tests: " << (testsPerResolution*(sizeof(resolutions)/sizeof(*resolutions))) << endl;

    //image to simulate plantation
    Mat imageTest = imread("../tests/photos/test1.png");
    //Mat imageTest = imread("../tests/photos/test5.jpg");
    //Mat imageTest = imread("../tests/photos/test4.jpg");

    //test header
    ostringstream dataTestHeader;
    dataTestHeader << "test_number"; // test number
    dataTestHeader << "\tres_number"; // resolution number
    dataTestHeader << "\tres_label"; // resolution label
    dataTestHeader << "\tt_im_acquisition"; // time to image acquisition
    dataTestHeader << "\tt_pretreatment"; // pretreatment step
    dataTestHeader << "\tt_line_detection"; // line detection step
    dataTestHeader << "\tt_line_filter"; // line filter step
    dataTestHeader << "\tt_line_follower"; // line follower step
    dataTestHeader << "\tt_total_frame"; // all time
    dataTestHeader << "\tfps"; // fps
    dataTestHeader << "\tdetected_lines"; // all detected lines
    dataTestHeader << "\n";

    //results file id
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"../tests/results/test_%d-%m-%Y_%H-%M-%S.csv",timeinfo);
    string resultsFileId(buffer);

    //results file
    ofstream resultsFile;
    resultsFile.open(resultsFileId);

    display << "Created test file: " << resultsFileId << endl;

    //write in results file
    resultsFile << dataTestHeader.str();

    display << "Writing test header..." << endl;

    int testNumber = 0;

    //for all resolutions
    //for(int i=0; i < (sizeof(resolutions)/sizeof(*resolutions)); i++){
    int i = 1;
    {
        //resize image for input
        Mat imageTestResized;
        resize(imageTest, imageTestResized, Size(resolutions[i][0],resolutions[i][1]));

        //create camera object
        RaspiCam_Cv Camera;
        Mat frame;

        //configure all parameters
        Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);   
        Camera.set(CV_CAP_PROP_FRAME_WIDTH, resolutions[i][0]);
        Camera.set(CV_CAP_PROP_FRAME_HEIGHT, resolutions[i][1]);
        //Camera.set(CV_CAP_PROP_FPS, 30); 
        Camera.setHorizontalFlip(true);
        Camera.setVerticalFlip(true);

        //wait camera configure
        //usleep(5000000);

        //Open camera
        if (!Camera.open()) {
            cout << "Error opening the camera." << endl;
            return;
        }

        //capture the first frame
        Camera.grab();
        Camera.retrieve(frame);

        int count = 0;

        while(count < testsPerResolution) {

            auto t_im_acquisition_i = high_resolution_clock::now();

            //frame capture
            //Camera.grab();
            Camera.retrieve(frame);

            auto t_im_acquisition_f = high_resolution_clock::now();
            
            //follow line and send command to drone
            //reference to follow
            fl->setReferenceToFollow(imageTestResized.size().width/2);

            //set image
            pld->setImage(imageTestResized);

            // detect
            //pld->detect();

            auto t_pretreatment_i = high_resolution_clock::now();
            pld->pretreatmentRGB();
            //pld->pretreatmentHSV();
            auto t_pretreatment_f = high_resolution_clock::now();

            auto t_line_detection_i = high_resolution_clock::now();
            pld->lineDetection();
            auto t_line_detection_f = high_resolution_clock::now();

            auto t_line_filter_i = high_resolution_clock::now();
            pld->reduceLinesForImageSpace();
            pld->lineFilter();
            auto t_line_filter_f = high_resolution_clock::now();

            // show results
            pld->showResults();

            auto t_line_follower_i = high_resolution_clock::now();

            vector <Line*> lines = pld->getDetectedLines();
            int numberOfLines = lines.size();
            cout << "Detected lines: " << numberOfLines << endl;
            if(numberOfLines > 0){

                fl->setLines(lines);
                fl->follow();
                //fl->showResults();
                ostringstream ss;
                ss << fl->getReferenceDistanceInM();
                
            }
            auto t_line_follower_f = high_resolution_clock::now();


            auto t_im_acquisition = duration_cast<milliseconds>(t_im_acquisition_f - t_im_acquisition_i);
            auto t_pretreatment = duration_cast<milliseconds>(t_pretreatment_f - t_pretreatment_i);
            auto t_line_detection = duration_cast<milliseconds>(t_line_detection_f - t_line_detection_i);
            auto t_line_filter = duration_cast<milliseconds>(t_line_filter_f - t_line_filter_i);
            auto t_line_follower = duration_cast<milliseconds>(t_line_follower_f - t_line_follower_i);
            auto t_total = t_im_acquisition + t_pretreatment + t_line_detection + t_line_filter + t_line_follower;
            auto t_total_frame = duration_cast<milliseconds>(t_total);

            //save test result
            ostringstream dataTest;
            dataTest << testNumber++; // test number
            dataTest << "\t" << (i+1); // resolution number
            dataTest << "\t" << resolutions[i][0] << "x" << resolutions[i][1]; // resolution label
            dataTest << "\t" << t_im_acquisition.count(); // time to image acquisition
            dataTest << "\t" << t_pretreatment.count(); // pretreatment step
            dataTest << "\t" << t_line_detection.count(); // line detection step
            dataTest << "\t" << t_line_filter.count(); // line filter step
            dataTest << "\t" << t_line_follower.count(); // line follower step
            dataTest << "\t" << t_total_frame.count(); // all time
            dataTest << "\t" << (1000/t_total_frame.count()); // fps
            dataTest << "\t" << numberOfLines; // all detected lines
            dataTest << "\n";

            resultsFile << dataTest.str();

            //write in results file
            //system("clear");
            cout << display.str() << "Writing test number: " << testNumber << endl;

            cout << "Results file:" << endl << "tn\trn\trl\ttim\ttp\ttld\ttlf\ttlf\tttf\tfps\tdl\n" << dataTest.str() << endl;

            resultsFile.flush();


            count++;

            //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
            //if(waitKey(30) == 27) {
            //    break;
            //}
        }

        //stop the camera
        Camera.release();
    }

    //close the results file
    resultsFile.close();

    /**/
}

void followLine(Mat frame, TCPClient *client, PlantLineDetection *pld, LineFollower *fl){

    //reference to follow
    fl->setReferenceToFollow(frame.size().width/2);

    //set image
    pld->setImage(frame);

    // detect
    //pld->detect();

    pld->pretreatmentRGB();
    pld->lineDetection();
    pld->reduceLinesForImageSpace();
    pld->lineFilter();

    // show results
    pld->showResults();

    vector <Line*> lines = pld->getDetectedLines();

    if(lines.size() > 0){

        fl->setLines(lines);
        fl->follow();
        fl->showResults();
        ostringstream ss;
        ss << fl->getReferenceDistanceInM();
        
        string message = "{\"command\":\"setPosition\", \"args\": {\"x\":" + ss.str() +", \"y\":0.5, \"z\":0}}";
        //client->sendData(message);
        cout << message << endl;
    }
}

void piCamera(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){

    //create camera object
    RaspiCam_Cv Camera;
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
        cout << "Error opening the camera." << endl;
        return;
    }

    //create a window to show the image
    //namedWindow("Results", CV_WINDOW_AUTOSIZE);

    while(true) {        
        //frame capture
        Camera.grab();
        Camera.retrieve(frame);
        //follow line and send command to drone
        followLine(frame, c, pld, fl);

        //display image
        //imshow("Results", frame);
        //waitKey(1);

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
            break;
        }
    }
    
    //stop the camera
    Camera.release();
}

void photo(TCPClient *c, PlantLineDetection *pld, LineFollower *fl){

    // create the mat and open an image
    Mat image = imread("../tests/photos/test1.png");

    // Check for invalid input
    if(!image.data){
        cout << "Could not open or find the image." << endl;
        return;
    }

    //resize image for input
    Mat resizedFrame;
    resize(image, resizedFrame, Size(320,240));

    //follow line and send command to drone
    followLine(resizedFrame, c, pld, fl);

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

        //resize image for input
        Mat resizedFrame;
        resize(frame, resizedFrame, Size(320,240));

        //follow line and send command to drone
        followLine(resizedFrame, c, pld, fl);

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

        //resize image for input
        Mat resizedFrame;
        resize(frame, resizedFrame, Size(320,240));

        //follow line and send command to drone
        followLine(resizedFrame, c, pld, fl);

        //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if(waitKey(30) == 27) {
            break;
        }
    }
}