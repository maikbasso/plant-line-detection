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
	Line* followedLine;
	int imageHeight;
	int imageWidth;
	float imageSizeInMeters[2];
	float referenceToFollow;
	float cameraApertureAngle;
	float cameraAngleToGround;
	float cameraHeight;
	float referenceDistanceInPx;
	float referenceDistanceInM;
	void selectLineForFollow();
	void calculateImageSizeInMeters();
	void calculateReferenceDistance();
public:
	LineFollower();
    void setLines(vector<Line*> lines);
    void setImageSize(int width, int height);
    void setReferenceToFollow(float reference);
    void setCameraApertureAngle(float cameraApertureAngle);
    void setCameraAngleToGround(float cameraAngleToGround);
    void setCameraHeight(float cameraHeight);
    float getReferenceDistanceInM();
    void showResults();
    void follow();
};

LineFollower::LineFollower(){}

void LineFollower::setLines(vector<Line*> lines){
	this->lines = lines;
}

void LineFollower::setImageSize(int width, int height){
	this->imageWidth = width;
	this->imageHeight = height;
}

void LineFollower::setReferenceToFollow(float reference){
	this->referenceToFollow = reference;
}

void LineFollower::setCameraApertureAngle(float cameraApertureAngle){
	this->cameraApertureAngle = cameraApertureAngle;
}

void LineFollower::setCameraAngleToGround(float cameraAngleToGround){
	this->cameraAngleToGround = cameraAngleToGround;
}

void LineFollower::setCameraHeight(float cameraHeight){
	this->cameraHeight = cameraHeight;
}

void LineFollower::follow(){
	this->selectLineForFollow();
	this->calculateImageSizeInMeters();
	this->calculateReferenceDistance();
}

void LineFollower::selectLineForFollow(){
	int lineIndex = 0;
	int distance = 0;
	for(size_t i=0; i<this->lines.size(); i++){
		int d = (this->imageWidth/2) - abs((this->lines[i]->p1.x + this->lines[i]->p2.x) / 2);
		if(d <= distance){
			lineIndex = i;
			distance = d;
		}
	}
	this->followedLine = this->lines[lineIndex];
}

void LineFollower::calculateImageSizeInMeters(){
	
	/*
	dt = math.tan((beta+(alpha/2))*(math.pi/180))*dz
	da = math.tan((beta-(alpha/2))*(math.pi/180))*dz
	dy = dt - da
	px = dy/h
	dx = px * w
	*/
	int w = this->imageWidth;
	int h = this->imageHeight;
	float dz = this->cameraHeight;
	float alpha = this->cameraApertureAngle;
	float beta = this->cameraAngleToGround;

	float dt = tan((beta+(alpha/2)) * (CV_PI/180)) * dz;
	float da = tan((beta-(alpha/2)) * (CV_PI/180)) * dz;
	float dy = dt - da;
	float px = dy / h;
	float dx = px * w;
	
	//float beta = this->cameraHeight*tan(this->cameraAngleToGround*(CV_PI/180));
	//float alpha = this->cameraHeight*tan(((this->cameraApertureAngle/2)+this->cameraAngleToGround)*(CV_PI/180));
	
	//float y = alpha - beta;
	//float x = (this->imageWidth*y)/this->imageHeight;
	
	this->imageSizeInMeters[0] = dx;
	this->imageSizeInMeters[1] = dy;

}

void LineFollower::calculateReferenceDistance(){
	this->referenceDistanceInPx = this->referenceToFollow - ((this->followedLine->p1.x + this->followedLine->p2.x)/2);
	this->referenceDistanceInM = (this->referenceDistanceInPx*this->imageSizeInMeters[0])/this->imageWidth;
}

void LineFollower::showResults(){
	cout << "Camera angle to ground: " << this->cameraAngleToGround << endl;
	cout << "Camera aperture angle: " << this->cameraApertureAngle << endl;
	cout << "Following line: p1(" << this->followedLine->p1.x << ", " << this->followedLine->p1.y << ")\t p2(" << this->followedLine->p2.x << ", " << this->followedLine->p2.y << ")" << endl;
	cout << "Image proportion in meters: " << this->imageSizeInMeters[0] << " x " << this->imageSizeInMeters[1] << endl;
    cout << "Distance in px: " << this->referenceDistanceInPx << endl;
    cout << "Distance in m: " << this->referenceDistanceInM << endl;
}

float LineFollower::getReferenceDistanceInM(){
	return this->referenceDistanceInM;
}