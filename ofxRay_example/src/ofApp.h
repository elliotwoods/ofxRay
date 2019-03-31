#pragma once

#include "ofMain.h"
#include "ofxRay.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void drawSelection();
	
	void keyPressed  (int key);

	ofEasyCam camera;

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
