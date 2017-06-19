/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

 //Include file for every supported OpenCV function
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

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