#pragma once

#include "ofMain.h"

#include "ofxRay.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void drawLabels();
	
	void keyPressed  (int key);
	
	ofEasyCam camera;
	ofPlanePrimitive planePrimitive;

	//ofxRay::Plane
	ofPlane plane;
	ofRay mouseRay;
	
	
	ofVec3f lookat;
	ofVec3f screenMouse;
	ofVec3f worldMouse;
	ofVec3f worldMouseTransmissionVector;
};
