#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"
#include "ofxRay.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void drawSelection();
	
	void keyPressed  (int key);
	void updatePlane();
	
	ofxGrabCam camera;
	map<string, ofVec3f&> data;
	map<string, ofVec3f&>::iterator dataCursor;
	
	ofPlane plane;
	
	ofVec3f center;
	ofVec3f normal;
	ofVec3f up;
	ofVec3f scale; // only 2D
};
