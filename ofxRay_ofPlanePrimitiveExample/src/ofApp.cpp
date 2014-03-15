#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
//	plane.
	planePrimitive.setPosition(ofVec3f(30.0f, 50.0f, 100.0f));
	planePrimitive.setScale(3,2,1);
	planePrimitive.setResolution(5, 5);
	planePrimitive.rotate(45, 1, 2, 3);
	
	plane.setFrom(planePrimitive);
}

//--------------------------------------------------------------
void ofApp::update(){

	lookat		= camera.getLookAtDir();
	screenMouse = ofVec3f(ofGetMouseX(),ofGetMouseY(),0);
	worldMouse = camera.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 0.0f));
	worldMouseTransmissionVector = camera.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 1.0f)) - worldMouse;
	
	mouseRay.s = worldMouse;
	mouseRay.t = worldMouseTransmissionVector;
	
	ofVec3f intersectionPosition;
	bool doesIntersect = plane.intersect(mouseRay, intersectionPosition);
	
	cout << (doesIntersect ? "hits" : "misses") << " at world position " << intersectionPosition << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(100, 50);
	
	camera.begin();

	//this will be drawn as a dot at your mouse cursor
	// because it's a line going away from the camera
	// under your mouse
	mouseRay.draw();
	
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































