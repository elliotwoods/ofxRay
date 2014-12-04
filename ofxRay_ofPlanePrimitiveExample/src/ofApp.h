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
	
	ofPlanePrimitive planePrimitive;	// the plane to display
	
	//ofxRay objects
	ofPlane plane;	// the plane to check intersection
	ofRay mouseRay;
	bool doesIntersect;
	ofVec3f intersectionPosition;
	
	// vectors
	ofVec3f screenMouse;
	ofVec3f worldMouse;
	ofVec3f worldMouseEnd;
	ofVec3f worldMouseTransmissionVector;
	
	float rotationSpeedX;
	float rotationSpeedY;
	ofVec3f rotation;
};