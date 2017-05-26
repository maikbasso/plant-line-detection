/*
    @Author: Maik Basso <maik@maikbasso.com.br> 

    To compile and run program:
    g++ -std=c++11 pld-photo.cpp -o pld-photo `pkg-config --cflags --libs opencv` && ./pld-photo

*/

// Include file for every supported OpenCV function
#include <opencv2/opencv.hpp>
#include <iostream>
// my libraries
#include "pld.h"

// namespaces
using namespace cv;
using namespace std;

int main(){

    // create the mat
    Mat image;

    // open a image file
    image = imread("photos/teste1.png");

    // Check for invalid input
    if(!image.data){
        cout <<  "Could not open or find the image." << endl ;
        return -1;
    }
    
    // create the line detection object
    PlantLineDetection *ld = new PlantLineDetection();
        
    //resize image for input
    Mat tempImage;
    resize(image, tempImage, Size(320,240) );

    //set image
    ld->setImage(tempImage);

    // detect
    ld->detect();

    // show results
    ld->showResults();

    //wait key to finish
    waitKey(0);

    // end the program without errors
    return 0;
}