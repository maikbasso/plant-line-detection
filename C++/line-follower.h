/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

 //Include file for every supported OpenCV function
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class LineFollower{
private:
	vector<Line*> lines;
	Line *followedLine;
	int height;
	int width;
	void selectLineForFollow();
public:
	LineFollower();
    void setLines(vector<Line*> lines);
    void setSize(int width, int height);
    Line* getFollowedLine();
};

LineFollower::LineFollower(){
	this->followedLine = new Line(Point(0,0), Point(0,0));
	this->height = 0;
	this->width = 0;
}

void LineFollower::setLines(vector<Line*> lines){
	this->lines = lines;
}

void LineFollower::setSize(int width, int height){
	this->width = width;
	this->height = height;
}

Line* LineFollower::getFollowedLine(){
	this->selectLineForFollow();
	return this->followedLine;	
}

void LineFollower::selectLineForFollow(){
	int lineIndex = 0;
	int distance = 0;
	for(size_t i=0; i<this->lines.size(); i++){
		int d = (this->width/2) - abs((this->lines[i]->p1.x + this->lines[i]->p2.x) / 2);
		if(d <= distance){
			lineIndex = i;
			distance = d;
		}
	}
	this->followedLine = this->lines[lineIndex];
}