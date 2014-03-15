#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"
#include "ofxRay.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void drawSelection();
	
	void keyPressed  (int key);
	void updateProjector();
	
	ofxGrabCam camera;
	map<string, float&> data;
	map<string, float&>::iterator dataCursor;
	
	ofProjector projector;
	vector<ofVec2f> pixels;
	vector<ofRay> rays;
	
	ofVec3f position;
	ofVec2f resolution;
	float throwRatio;
	ofVec2f lensOffset;
};
