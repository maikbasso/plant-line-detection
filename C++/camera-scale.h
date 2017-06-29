/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

class CameraScale{
private:
	float apertureAngle;
	float cameraAngle;
	float height;
	int imageHeight;
	int imageWidth;
	float sizeInMeters[2];
public:
	CameraScale();
	void setCameraAperture(float aperture);
	void setHeight(float height);
	void setImageSize(int imageWidth, int imageHeight);
	void setCameraAngle(float cameraAngle);
	float* getSizeInMeters();
};

CameraScale::CameraScale(){
	this->apertureAngle = 0;
	this->cameraAngle = 0;
	this->height = 0;
	this->imageHeight = 0;
	this->imageWidth = 0;
	this->sizeInMeters[0] = 0;//x
	this->sizeInMeters[1] = 0;//y
}

void CameraScale::setCameraAperture(float aperture){
	this->apertureAngle = aperture;
}

void CameraScale::setHeight(float height){
	this->height = height;
}

void CameraScale::setImageSize(int imageWidth, int imageHeight){
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
}

void CameraScale::setCameraAngle(float cameraAngle){
	this->cameraAngle = cameraAngle;
}

float* CameraScale::getSizeInMeters(){
	float lambda = this->cameraAngle - (this->apertureAngle/2);
	
	float opositCameraAngle = this->height*tan(cameraAngle*(CV_PI/180));
	float postCameraAngle = this->height*tan(((this->apertureAngle/2)+cameraAngle)*(CV_PI/180));
	
	float Y = postCameraAngle - opositCameraAngle;
	float X = (this->imageWidth*Y)/this->imageHeight; // regra de tres
	
	this->sizeInMeters[0] = X; //x
	this->sizeInMeters[1] = Y; //y

	return this->sizeInMeters;
}