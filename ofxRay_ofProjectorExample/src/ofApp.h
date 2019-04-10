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
	void updateProjector();
	
	ofEasyCam camera;
	map<string, float&> data;
	map<string, float&>::iterator dataCursor;
	
	ofProjector projector;
	vector<glm::vec2> pixels;
	vector<ofRay> rays;
	
	ofVec3f position;
	ofVec2f resolution;
	float throwRatio;
	ofVec2f lensOffset;
};
