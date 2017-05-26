/*
    @Author: Maik Basso <maik@maikbasso.com.br> 

*/

 //Include file for every supported OpenCV function
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Image{
public:
    Mat data;
    string name;
};

class Line{
public:
    Line(Point p1, Point p2);
    Point p1;
    Point p2;
};
Line::Line(Point p1, Point p2){
    this->p1 = p1;
    this->p2 = p2;
}

class PlantLineDetection{
private:
    Image *images[7];
    int height;
    int width;
    vector <Line*>lines;
    vector <Line*>filteredLines;

    // define the adaptative thresholds and properties
    int heightThreshold;
    int colorThreshold[2];
    int maxLineDistance;

    //results display properties
    bool printResultsImage;
    bool saveFrames;
    int resultFrameSize[2];
    int grid[2];

    // for benchmark
    int64 startTime;
    int64 stopTime;
    float fps;
    int frameCounter;

    //methods
    void pretreatment();
    void lineDetection();
    void reduceLinesForImageSpace();
    void lineFilter();
    bool isParallelLines(Point p1, Point p2, Point p3, Point p4);
    int lineDistance(Point p1, Point p2, Point p3, Point p4);
    Point intersectPoint(Point p1, Point p2, Point p3, Point p4);

public:
    PlantLineDetection();
    void setImage(Mat image);
    void detect();
    void showResults();
};

PlantLineDetection::PlantLineDetection(){
    //set default values
    height = 0;
    width = 0;
    heightThreshold = 0;
    colorThreshold[0] = 60;
    colorThreshold[1] = 255;
    maxLineDistance = 20;//px
    printResultsImage = true;
    saveFrames = true;
    resultFrameSize[0] = 260;
    resultFrameSize[1] = 180;
    grid[0] = 3;
    grid[1] = 3;
    fps = 0;
    frameCounter = 0;

    // load the images from the algorithm
    for(int i=0; i < 7; i++){
        this->images[i] = new Image();
    }

    if(this->printResultsImage == true){
        //create a window called "Results"
        namedWindow("Results", CV_WINDOW_AUTOSIZE);
    }

}

void PlantLineDetection::setImage(Mat frame){
    //get current time
    this->startTime = getTickCount();
    //number of frame
    this->frameCounter++;
    //get image size
    this->width = frame.size[1];
    this->height = frame.size[0];
    //set the frame results
    this->images[0]->data = frame.clone();
    this->images[0]->name = "Original Image";

    this->images[1]->data = Mat::zeros(this->height, this->width, CV_8U);
    this->images[1]->name = "Green Extraction";

    this->images[2]->data = Mat::zeros(this->height, this->width, CV_8U);
    this->images[2]->name = "Threshold Filter";

    this->images[3]->data = Mat::zeros(this->height, this->width, CV_8U);
    this->images[3]->name = "Object Detection";

    this->images[4]->data = Mat::zeros(this->height, this->width, CV_8U);
    this->images[4]->name = "Skeletonization";

    this->images[5]->data = frame.clone();
    this->images[5]->name = "Hough Lines";

    this->images[6]->data = frame.clone();
    this->images[6]->name = "Final Result";
}

void PlantLineDetection::detect(){
    this->pretreatment();
    this->lineDetection();
    this->reduceLinesForImageSpace();
    this->lineFilter();
    //get current time
    this->stopTime = getTickCount();
}

void PlantLineDetection::pretreatment(){
    for(int y=(this->height-1); y >= 0; y--){

        bool isLine = false;
        int x1,x2 = 0;

        for(int x=(this->width-1); x >= 0; x--){
            //get the pixelIm1 data
            Vec3b pixel = this->images[0]->data.at<Vec3b>(Point(x,y));
            int B = (int)pixel.val[0];
            int G = (int)pixel.val[1];
            int R = (int)pixel.val[2];

            // green extraction (method by GUO-QUAN JIANG)
            int greyLevel = 0;
            if ((2*G) <= (R + B)){
                greyLevel = 0;
            }
            else if((2*G) >= R+B+255){
                greyLevel = 255;
            }
            else{
                greyLevel = (2*G)-R-B;
            }

            //MPRI
            // int bottom = G + R;
            // int top = G - R;
            // if(bottom == 0){
            //     bottom = 1;
            // }
            // int MPRI = top / bottom;

            // greyLevel = MPRI;

            // if(greyLevel > 255){
            //     greyLevel = 255;
            // }
            // else if(greyLevel < 0){
            //     greyLevel = 0;
            // }
            
            this->images[1]->data.at<uchar>(Point(x,y)) = greyLevel;

            // threshold filter
            if(this->colorThreshold[0] <= greyLevel && greyLevel <= this->colorThreshold[1]){
                greyLevel = 255;
            }
            else{
                greyLevel = 0;
            }

            this->images[2]->data.at<uchar>(Point(x,y)) = greyLevel;

            // detect the object
            if(isLine == false && greyLevel == 255){
                x1 = x2 = x;
                isLine = true;
            }
            else if(isLine == true && greyLevel == 255){
                x2 = x;
            }
            else if(isLine == true && greyLevel == 0){
                this->images[3]->data.at<uchar>(Point(x1,y)) = 255;
                this->images[3]->data.at<uchar>(Point(x2,y)) = 255;
                if(x2 != 0){
                    int meanx = (x1+x2) / 2;
                    this->images[4]->data.at<uchar>(Point(meanx,y)) = 255;
                }
                isLine = false;
                x1 = x2 = 0;
            }
        }
    }

    //cvSobel(this->images[2]->data, this->images[3]->data);

    //threshold(this->images[1]->data, this->images[2]->data, 100, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);

    //Mat img = this->images[4]->data;
    //Canny(this->images[4]->data, img, 50, 200, 3);
    //dilate(this->images[4]->data, img, Mat(), Point(-1, -1), 2, 1, 1);
    //Canny(img, img, 50, 200, 3);
}

void PlantLineDetection::lineDetection(){
    //clear the previews line properties
    this->lines.clear();
    this->filteredLines.clear();
    //the vector of all polar lines
    vector<Vec2f> lines;
    // detect lines with hough transform
    HoughLines(this->images[4]->data, lines, 1, CV_PI/180, 30, 0, 0);
    //HoughLines(this->images[4]->data, lines, 1, CV_PI/180, 30, 0, 0);
    for(size_t i = 0; i < lines.size(); i++){
        //get the rho and theta values
        float rho = lines[i][0];
        float theta = lines[i][1];
        //Check if this lines are in the desired range
        if(theta>CV_PI/180*1 && theta<CV_PI/180*180){
            //calculate a and b
            double a = cos(theta);
            double b = sin(theta);
            //calculate the generic point
            double x0 = a*rho;
            double y0 = b*rho;
            //declare the points of line
            Point p1, p2;
            //calculate the points p1 and p2
            p1.x = cvRound(x0 + 1000*(-b));
            p1.y = cvRound(y0 + 1000*(a));
            p2.x = cvRound(x0 - 1000*(-b));
            p2.y = cvRound(x0 - 1000*(a));
            //save line
            this->lines.push_back(new Line(p1,p2));
        }
    }
}

void PlantLineDetection::reduceLinesForImageSpace(){
    for(size_t i=0; i < this->lines.size(); i++){

        int x1 = this->lines[i]->p1.x;
        int y1 = this->lines[i]->p1.y;
        int x2 = this->lines[i]->p2.x;
        int y2 = this->lines[i]->p2.y;

        // calculate the new points on image with equation of the line
        int y3 = 2;
        int x3 = (x2*y3+x1*y2-x2*y1-x1*y3)/(y2-y1);

        int y4 = this->height - y3;
        int x4 = (x2*y4+x1*y2-x2*y1-x1*y4)/(y2-y1);

        //replace the line points
        this->lines[i]->p1.x = x3;
        this->lines[i]->p1.y = y3;
        this->lines[i]->p2.x = x4;
        this->lines[i]->p2.y = y4;

        //print all lines
        line(this->images[5]->data, this->lines[i]->p1, this->lines[i]->p2, Scalar(255,0,0), 1, CV_AA);
    }
}

void PlantLineDetection::lineFilter(){
    vector <Line*>lastLine;
    for(size_t i=0; i < this->lines.size(); i++){

        if(lastLine.size() == 0){
            line(this->images[6]->data, this->lines[i]->p1, this->lines[i]->p2, Scalar(255,0,0), 1, CV_AA);
        }
        else{
            
            int count = 0;

            for(size_t n=0; n < lastLine.size(); n++){

                Point p = this->intersectPoint(lastLine[n]->p1,lastLine[n]->p2, this->lines[i]->p1, this->lines[i]->p2);
                
                if(0 < p.x && p.x < this->width && 0 < p.y && p.y < this->height){
                    count++;
                }

                if(this->isParallelLines(lastLine[n]->p1,lastLine[n]->p2, this->lines[i]->p1, this->lines[i]->p2) == true){
                    if(this->lineDistance(lastLine[n]->p1,lastLine[n]->p2, this->lines[i]->p1, this->lines[i]->p2) < this->maxLineDistance){
                        count++;
                    }
                }
            }

            if(count == 0){
                this->filteredLines.push_back(this->lines[i]);
                line(this->images[6]->data, this->lines[i]->p1, this->lines[i]->p2, Scalar(255,0,0), 1, CV_AA);
            }
        }

        lastLine.push_back(this->lines[i]);
    }
}

int PlantLineDetection::lineDistance(Point p1, Point p2, Point p3, Point p4){
    if(p1.x < p3.x){
        return p3.x - p1.x;
    }
    if(p1.x >= p3.x){
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

void PlantLineDetection::showResults(){
    if(this->printResultsImage == true){
        int numCols = this->grid[0];
        int numRows = this->grid[1];
        Mat resultsImage = Mat::zeros(numRows*this->resultFrameSize[1], numCols*this->resultFrameSize[0], this->images[0]->data.type());
        int countx = 0;
        int county = 0;
        for(int i=0; i < 7; i++){
            Mat currentImage;
            if(this->images[i]->data.type() == CV_8UC1){
                cvtColor(this->images[i]->data, currentImage, CV_GRAY2BGR);
            }
            else{
                currentImage = this->images[i]->data.clone();
            }
            //save frames
            if(this->saveFrames){
                string frameName = this->images[i]->name;
                frameName.erase(std::remove(frameName.begin(),frameName.end(),' '),frameName.end());
                imwrite("frames/" + frameName + ".png", currentImage);
            }
            //create temp image
            Mat tempImage = Mat::zeros(this->resultFrameSize[1], this->resultFrameSize[0], this->images[0]->data.type());
            //resize the image
            resize(currentImage,tempImage, Size(tempImage.cols,tempImage.rows) );
            //write name in frame
            ostringstream convert;   // stream used for the conversion
            convert << i;
            putText(tempImage, convert.str() + "-" + this->images[i]->name, Point(25, 25), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1, 5);
            //copy temp image to resultsImage
            tempImage.copyTo(resultsImage(cv::Rect(countx*this->resultFrameSize[0], county*this->resultFrameSize[1],tempImage.cols, tempImage.rows)));
            //grid design
            countx++;
            if(countx == numCols){
                county++;
                countx = 0;
            }
        }
        //show the frame in "Results" window
        imshow("Results", resultsImage);
    }
    //system("clear");
    double timeInSeconds = (this->stopTime - this->startTime) / getTickFrequency();
    cout << "=========================================" << endl;
    cout << "Plant Line Detection (PLD) by Maik Basso" << endl;
    cout << "=========================================" << endl;
    cout << "OpenCV version   = " << CV_VERSION << endl;
    //show the plant line detection properties
    cout << "=========================================" << endl;
    cout << "Frame resolution = " << this->width << " x " << this->height << endl;
    cout << "Frame counter    = " << this->frameCounter << endl;
    cout << "Time per frame   = " << timeInSeconds << " s" << endl;
    cout << "FPS              = " << (1 / timeInSeconds) << endl;
    cout << "All lines        = " << this->lines.size() << endl;
    cout << "Filtered lines   = " << this->filteredLines.size() << endl;
    // instructions for close the program
    cout << "=========================================" << endl;
    cout << "CTRL+C for close in terminal." << endl << "ESC for close in GUI interface." << endl;
    cout << "=========================================" << endl;
}
