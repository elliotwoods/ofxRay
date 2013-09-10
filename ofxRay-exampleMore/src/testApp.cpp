#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	camera.setCursorDraw(true);
	
	projector.setPosition(ofVec3f(0,0,-5));
	projector.setOrientation(ofQuaternion(43, ofVec3f(1,3,0)));
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	camera.begin();
    projector.draw();
    
    // ray to center of projection
    ofRay projectionCenterRay = projector.getProjectionCenterRay();
    projectionCenterRay.color = ofColor::red;
    projectionCenterRay.draw();
    
    // ray along projector (perpendicular to projector)
    ofRay projectorRay = projector.getProjectorRay(20);
    projectorRay.color = ofColor::blue;
    projectorRay.draw();
    
    // plane at arbitrary distance
    ofPlane plane = projector.getProjectionPlaneAt(30);
    plane.color = ofColor::green;
    plane.draw();
    
//    projector.ofNode::draw();

	camera.end();
}

