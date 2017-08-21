/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

#include <opencv2/opencv.hpp>

using namespace cv;

class Line{
public:
	Point p1;
    Point p2;
    
    Line(Point p1, Point p2){
	    this->p1 = p1;
	    this->p2 = p2;
	}
};