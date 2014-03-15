#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// let the axis oclude
	ofEnableDepthTest();
	
	// set attributes to our displayed plane
	planePrimitive.setPosition(0 ,0, 0);
	planePrimitive.setScale(1,2,1);
	rotationAmount = 0.3;
 	
	// set attributes to our intersection plane
	center	= ofVec3f(0, 0, 0);
	normal	= ofVec3f(0, 0, 1).normalize();
	up		= ofVec3f(0, 1, 0).normalize();
	scale	= ofVec2f(100, 100);

//	plane.setFrom(planePrimitive);
	
	updatePlane();
	plane.setInfinite(false);
}

//--------------------------------------------------------------
void ofApp::update(){

	// the mouse position on screen coordinates
	screenMouse = ofVec3f(ofGetMouseX(),ofGetMouseY(),0);
	
	// the mouse position on world coordinates
	worldMouse	= camera.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 0.0f));
	
	// a point right in front of the mouse (used to get mouse direction)
	worldMouseEnd	= camera.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 1.0f));
	
	// a vector representing the mouse direction (from camera to infinity?)
	worldMouseTransmissionVector = worldMouseEnd - worldMouse;
	
	// set attributes to the ray
	mouseRay.s = worldMouse;
	mouseRay.t = worldMouseTransmissionVector;
	
	rotation.x += rotationAmount;
	rotation.y += rotationAmount;
	
	planePrimitive.setOrientation(rotation);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(100, 50);
	
	camera.begin();

	// this will be drawn as a dot at your mouse cursor
	// because it's a line going away from the camera
	// under your mouse
	mouseRay.draw();
	
	plane.draw();


	ofSetColor(0, 250, 250, 100);
	planePrimitive.draw(OF_MESH_FILL);
	planePrimitive.drawAxes(100);
	
//	ofDrawAxis(100);
	camera.end();
	
	drawLabels();
}

void ofApp::drawLabels() {
	
	string label = "mouse in world coordinates";
	label += " x = " + ofToString(worldMouse.x);
	label += " y = " + ofToString(worldMouse.y);
	label += " x = " + ofToString(worldMouse.z);
	int y = ofGetHeight() - 70;
	ofDrawBitmapStringHighlight(label, 40, y);

	//--------------------------------------------------------------
	label = "mouse in screen coordinates x =" + ofToString(screenMouse.x)+" y = "+ofToString(screenMouse.y);
	y += 20;
	ofDrawBitmapStringHighlight(label, 40, y);
	
	y += 20;
	ofVec3f intersectionPosition;
	doesIntersect = plane.intersect(mouseRay, intersectionPosition);
	label = doesIntersect ? "hits" : "misses";
	label += + " at world position " + ofToString(intersectionPosition);
	ofDrawBitmapStringHighlight(label, 40, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::updatePlane()	{
	plane.setCenter(center);
	plane.setNormal(normal);
	plane.setUp(up);
	plane.setScale(scale);
	
	normal = plane.getNormal();
	up = plane.getUp();
}































