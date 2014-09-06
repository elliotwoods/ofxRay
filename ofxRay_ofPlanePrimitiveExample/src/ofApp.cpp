#include "ofApp.h"

// Example by Jordi Puig

//--------------------------------------------------------------
void ofApp::setup(){
	
	// let the axis oclude
	ofEnableDepthTest();
	
	// set attributes to our displayed plane
	planePrimitive.setPosition(0 ,0, 0);
	planePrimitive.setScale(1,2,1);
	
	//set the rotation speeds
	rotationSpeedX = 0.33;
	rotationSpeedY = 0.2;
 		
	// get the attributes from the plane primitive
	// and apply them to the intersection plane
	plane.setFrom(planePrimitive);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	// the mouse position on screen coordinates
	screenMouse = ofVec3f(ofGetMouseX(),ofGetMouseY(),0);
	
	// the mouse position on world coordinates
	worldMouse = camera.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 0.0f));
	
	// a point right in front of the mouse (used to get mouse direction)
	worldMouseEnd = camera.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 1.0f));
	
	// a vector representing the mouse direction (from camera to infinity?)
	worldMouseTransmissionVector = worldMouseEnd - worldMouse;
	
	// set attributes to the ray
	mouseRay.s = worldMouse;
	mouseRay.t = worldMouseTransmissionVector;
	
	// rotate the plane
	rotation.x += rotationSpeedX;
	rotation.y += rotationSpeedY;
	planePrimitive.setOrientation(rotation);
	
	// update intersection plane
	plane.setFrom(planePrimitive);
	
	// check for intersection
	// all the good stuff is done here!
	doesIntersect = plane.intersect(mouseRay, intersectionPosition);

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackgroundGradient(100, 50);
	
	camera.begin();
	
	ofDrawGrid(1000.0f, 1.0f);
	
	// this will be drawn as a dot at your mouse cursor
	// because it's a line going away from the camera
	// under your mouse
	mouseRay.draw();
	
	plane.draw();
	
	ofSetColor(0, 250, 250, 100);
	planePrimitive.draw(OF_MESH_FILL);
	planePrimitive.drawAxes(100);
	
	camera.end();
	
	drawLabels();
}

void ofApp::drawLabels(){
	
	string label;
	int y = ofGetHeight() - 70;
	label = doesIntersect ? "hits" : "misses";
	label += + " at world position " + ofToString(intersectionPosition);
	ofDrawBitmapStringHighlight(label, 40, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}





















