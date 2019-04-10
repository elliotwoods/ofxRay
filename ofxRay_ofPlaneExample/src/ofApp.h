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
	void updatePlane();
	
	ofEasyCam camera;
	map<string, glm::vec3&> data;
	map<string, glm::vec3&>::iterator dataCursor;
	
	ofPlane plane;
	
	glm::vec3 center;
	glm::vec3 normal;
	glm::vec3 up;
	glm::vec3 scale; // only 2D
};
