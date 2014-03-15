#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	center = ofVec3f(1.0f, 1.0f, 0.0f);
	normal = ofVec3f(1.0f, 1.0f, 1.0f).normalize();
	up = ofVec3f(0.0f, 1.0f, 0.0f).normalize();
	scale = ofVec2f(100.0f, 100.0f);
	
	updatePlane();
	plane.setInfinite(false);

//	plane.
	planePrimitive.setPosition(plane.getCenter());
	planePrimitive.setScale(1,2,1);
	planePrimitive.setResolution(5, 5);
}

//--------------------------------------------------------------
void ofApp::update(){

	lookat		= camera.getLookAtDir();
	screenMouse = ofVec3f(ofGetMouseX(),ofGetMouseY(),0);
	worldMouse	= camera.screenToWorld(screenMouse);
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(100, 50);
	
	camera.begin();


	plane.draw();

	ofSetColor(0, 250, 250, 100);
	planePrimitive.draw(OF_MESH_FILL);
//	planePrimitive.draw(OF_MESH_WIREFRAME);
	
	// draw grid
	ofPushStyle();
	ofSetColor(200, 100, 100);
	ofDrawGrid(100,1,true);
	ofPopStyle();

	camera.end();
	
	// draw labels
	drawLabels();
}

void ofApp::drawLabels() {
	string wm = "x =" + ofToString(worldMouse.x)+" y = "+ofToString(worldMouse.y);
	string instruction = "mouse in world coordinates " + wm;
	
	ofPushStyle();
	ofFill();
	ofSetColor(200, 100, 100);
	int y = ofGetHeight() - 70;
	ofSetColor(255, 255, 255);
	
	ofDrawBitmapStringHighlight(instruction, 40, y);

	ofPopStyle();

	//--------------------------------------------------------------
	string sm = "x =" + ofToString(screenMouse.x)+" y = "+ofToString(screenMouse.y);
	instruction = "mouse in screen coordinates " + sm;
	
	ofPushStyle();
	ofFill();
	ofSetColor(200, 100, 100);
	y += 30;
	ofSetColor(255, 255, 255);
	
	ofDrawBitmapStringHighlight(instruction, 40, y);
	
	ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	updatePlane();
	
	if (key=='c')
//		camera.toggleCursorDraw();
	if (key=='f')
		ofToggleFullscreen();
	
	if (key==OF_KEY_RETURN) {
/*		if (dataCursor == data.end())
			dataCursor = data.begin();
		else
			dataCursor++;
*/	}
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































