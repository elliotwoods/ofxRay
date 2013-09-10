#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"
#include "ofxRay.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	ofxGrabCam camera;
	ofProjector projector;
};
