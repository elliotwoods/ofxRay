#pragma once

#include "ofMain.h"
//#define USE_GRAB_CAM/
#ifdef USE_GRAB_CAM
#include "ofxGrabCam.h"
#endif
#include "ofxRay.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void drawSelection();
	
	void keyPressed  (int key);
#ifdef USE_GRAB_CAM
	ofxGrabCam camera;
#else
	ofEasyCam camera;
#endif
	map<string, bool> drawEnabled;
	map<string, bool>::iterator drawEnabledCursor;
	
	ofxRay::Plane plane;
	ofxRay::Ray ray;
	ofxRay::Projector projector;
	ofxRay::Ray pRay;
	
	ofxRay::Ray corners[4];
	unsigned int i, j;
	
	void drawRayIntersection(ofRay & r, ofColor onIntersectColor);
	
	void randomiseRay();
	
};
