#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

class Image{
    public:
        Mat image;
        string name;
};

class PlantLineDetection{
	private:
        Image *images[7];
        int height = 0;
        int width = 0;

        // define the adaptative thresholds and properties
        int heightThreshold = 0;
        int colorThreshold[2] = {70, 255};
        int maxLineDistance = 20;//px

        //results display properties
        int resultFrameSize[2] = {220, 160};
        //int grid[2] = {3,3};
        int grid[2]= {3,3};

        // for benchmark
        time_t startTime;
        time_t stopTime;
        float fps = 0;
        int frameCounter = 0;

	public:
		PlantLineDetection();
		void setImage(Mat image);
		//void detect();
		bool isParallelLines(Point p1, Point p2, Point p3, Point p4);
		float lineDistance(Point p1, Point p2, Point p3, Point p4);
		Point intersectPoint(Point p1, Point p2, Point p3, Point p4);
		Mat getResultsFrame();
		void drawText(Mat & image, string text, int x, int y);
};

PlantLineDetection::PlantLineDetection(){}


void PlantLineDetection::setImage(Mat frame){
    //get current time
    time(&this->startTime);
    //seconds = difftime(timer,mktime(&y2k));
    //get image size
    this->width = frame.size[1];
    this->height = frame.size[0];
    // create the blank image 8 bits
    Mat blankImage = Mat::zeros(this->width, this->height, frame.type());
    // load the images from the algorithm
    for(int i=0; i < 7; i++) this->images[i] = new Image();
    this->images[0]->image = frame;
    this->images[0]->name = "Original Image";
    this->images[1]->image = blankImage;
    this->images[1]->name = "Green Extraction";
    this->images[2]->image = blankImage;
    this->images[2]->name = "Threshold Filter";
    this->images[3]->image = blankImage;
    this->images[3]->name = "Object Detection";
    this->images[4]->image = blankImage;
    this->images[4]->name = "Skeletonization";
    this->images[5]->image = frame;
    this->images[5]->name = "Hough Lines";
    this->images[6]->image = frame;
    this->images[6]->name = "Final Result";
}

Mat PlantLineDetection::getResultsFrame(){
    int numCols = this->grid[0];
    int numRows = this->grid[1];
    Mat resultsImage = Mat::zeros(numCols*this->resultFrameSize[0], (numRows*this->resultFrameSize[1]), this->images[0]->image.type());
    int countx = 0;
    int county = 0;
    //namedWindow("Results",CV_WINDOW_AUTOSIZE);
    //imshow("Results", this->images[0]->image);
    cout << "size of the images: " << sizeof(this->images);
//    for(int i=0; i < sizeof(this->images); i++){
////        Size size(this->resultFrameSize[0],this->resultFrameSize[1]);
////        Mat tempImage = Mat::zeros(this->resultFrameSize[0], this->resultFrameSize[1], this->images[i]->image.type());
////        resize(this->images[i]->image,tempImage,size);
////        this->drawText(tempImage, this->images[i]->name, 5, 5);
////        //tempImage.copyTo(resultsImage(cv::Rect(countx*this->resultFrameSize[0],county*this->resultFrameSize[1],tempImage.cols, tempImage.rows)));
////        tempImage.copyTo(resultsImage(cv::Rect(0, 0,tempImage.cols, tempImage.rows)));
////        countx++;
////        if(countx == numCols){
////            county++;
////            countx = 0;
////        }
//
//    }
    //system("clear");
    cout << "frame resolution = " << this->width << "x" << this->height << endl;
    cout << "frame counter =" << this->frameCounter << endl;
    cout << "time per frame = " << (this->stopTime- this->startTime) << endl;
    cout << "heightThreshold = " << this->heightThreshold << endl;
    cout << "colorThreshold = " << this->colorThreshold[0] << "x" << this->colorThreshold[1] << endl;
    cout << "fps = " << this->fps << endl;
    cout << "numberOfLines = " << 0 << endl;
    cout << "finalLines = " << 0 << endl;

    //resultsImage.paste(varStatus,(0, resultsImage.size[1]-heightVar))

    return resultsImage;
}

float PlantLineDetection::lineDistance(Point p1, Point p2, Point p3, Point p4){
    if(p1.x<p3.x){
        return p3.x - p1.x;
    }
    if(p1.x>=p3.x){
        return p1.x - p3.x;
    }
}

bool PlantLineDetection::isParallelLines(Point p1, Point p2, Point p3, Point p4){
    float m1=0, m2=0;

    if((p2.x-p1.x) != 0){
        m1 = (p2.y-p1.y)/(p2.x-p1.x);
    }
    if((p4.x-p3.x) != 0){
        m2 = (p4.y-p3.y)/(p4.x-p3.x);
    }

    if(m1==m2){
        return true;
    }

    return false;
}

Point PlantLineDetection::intersectPoint(Point p1, Point p2, Point p3, Point p4){
    int a1, a2, b1, b2, c1, c2;

    //calculate the coeficients for line 1 and 2
    a1 = p1.y - p2.y;
    b1 = p2.x - p1.x;
    c1 = (p1.x*p2.y) - (p2.x*p1.y);

    a2 = p3.y - p4.y;
    b2 = p4.x - p3.x;
    c2 = (p3.x*p4.y) - (p4.x*p3.y);

    //set the matrix a and b
    int a[2][2] = {{a1,b1},{a2,b2}};
    int b[2][1] = {{-c1},{-c2}};

    //create the opencv matrix
    Mat A = Mat(2,2, CV_32FC1, a);
    Mat B = Mat(2,1, CV_32FC1, b);

    //determinant
    if (determinant(A) != 0){
        //solve the eq ax=b
        Mat x = A.inv() * B;
        return Point(x);
    }
    else{
        //discards the line by placing the intersection point outside the image
        return Point(-1,-1);
    }
}

void PlantLineDetection::drawText(Mat & image, string text, int x, int y){
    putText(image, text,
            Point(x, y),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}
