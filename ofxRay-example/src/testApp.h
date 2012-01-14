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

	ofxGrabCam camera;
	map<string, bool> drawEnabled;
	map<string, bool>::iterator drawEnabledCursor;
	
	ofPlane plane;
	ofRay ray;
	ofProjector projector;
	ofRay pRay;
	
	ofRay corners[4];
	unsigned int i, j;
};
