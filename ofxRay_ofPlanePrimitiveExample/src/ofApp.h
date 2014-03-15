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
	void updatePlane();
	
	ofEasyCam camera;
	
	ofPlanePrimitive planePrimitive;	// the plane to display
	
	//ofxRay objects
	ofPlane plane;	// the plane to check intersection
	ofRay mouseRay;
	bool doesIntersect;
	
	// plane attributes
	ofVec3f center;
	ofVec3f normal;
	ofVec3f up;
	ofVec3f scale; // only 2D
	
	// vectors
	ofVec3f screenMouse;
	ofVec3f worldMouse;
	ofVec3f worldMouseEnd;
	ofVec3f worldMouseTransmissionVector;
	
	float rotationX;
	float rotationY;
	ofVec3f rotation;
};