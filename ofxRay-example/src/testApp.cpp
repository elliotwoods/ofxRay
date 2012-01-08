#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	
	plane = ofPlane(ofVec3f(1,1,0), ofVec3f(1,0,-1), ofVec3f(0,1,0), ofVec2f(3,2));
	
	ray.randomise();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	camera.begin();
	ofPushStyle();
	ofSetColor(155,100,100);
	ofDrawGrid(5.0f);
	ofPopStyle();
	
	plane.draw();
	ray.draw();
	
	bool intersects;
	ofVec3f intersect;
	intersects = plane.intersect(ray, intersect);
	
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(intersect);
	if (intersects)
		ofFill();
	else
		ofNoFill();
	ofCircle(0, 0, 0.3f);
	ofPopStyle();
	ofPopMatrix();
	
	camera.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key==OF_KEY_LEFT)
		ray.s += ofVec3f(-0.1,0,0);
	if (key==OF_KEY_RIGHT)
		ray.s += ofVec3f(0.1,0,0);
	if (key==OF_KEY_UP)
		ray.s += ofVec3f(0,0.1,0);
	if (key==OF_KEY_DOWN)
		ray.s += ofVec3f(0,-0.1,0);
	
}