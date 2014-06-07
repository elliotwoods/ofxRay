#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	camera.setCursorDraw(true);
	
	data.insert(pair<string, ofVec3f&>("center", center));
	data.insert(pair<string, ofVec3f&>("normal", normal));
	data.insert(pair<string, ofVec3f&>("up", up));
	data.insert(pair<string, ofVec3f&>("scale xy", scale));
	dataCursor = data.begin();
	
	center = ofVec3f(1.0f, 1.0f, 0.0f);
	normal = ofVec3f(1.0f, 1.0f, 1.0f).normalize();
	up = ofVec3f(0.0f, 1.0f, 0.0f).normalize();
	scale = ofVec2f(1.0f, 1.0f);
	
	updatePlane();
	plane.setInfinite(false);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	camera.begin();
	
	ofPushStyle();
	ofSetColor(200, 100, 100);
	ofDrawGrid(10,5,true);
	ofPopStyle();
	plane.draw();
	
	camera.end();
	
	drawSelection();
}

//--------------------------------------------------------------
void ofApp::drawSelection() {
	int nItems = data.size();
	string instruction = "[RETURN] to move selection, [UP]/[DOWN],[LEFT]/[RIGHT],[A]/[Z] to edit";
	
	int stringWidth = 0;
	map<string, ofVec3f&>::iterator it;
	for(it = data.begin(); it != data.end(); it++) {
		stringWidth = MAX(stringWidth, (it->first).length());
	}
	
	ofPushStyle();
	ofFill();
	ofSetColor(200, 100, 100);
	int y = ofGetHeight() - nItems*10 - 70;
	ofRect(20, y, MAX(stringWidth, instruction.length()) * 8 + 40, nItems*10 + 50);
	
	ofSetColor(255, 255, 255);
	y += 20;
	ofDrawBitmapString(instruction, 40, y);
	y += 20;
	ofSetColor(200, 200, 200);
	string result;
	for(it = data.begin(); it != data.end(); it++) {
		result = dataCursor == it ? "> " : "  ";
		result += it->first;
		while (result.length() < stringWidth+4)
			result.push_back(' ');
		result += ofToString(it->second, 2);
		ofDrawBitmapString(result, 40, y);
		y += 10;
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if (key==OF_KEY_LEFT)
		dataCursor->second.x += -0.1;
	if (key==OF_KEY_RIGHT)
		dataCursor->second.x += +0.1;
	
	if (key==OF_KEY_UP)
		dataCursor->second.y += +0.1;
	if (key==OF_KEY_DOWN)
		dataCursor->second.y += -0.1;
	
	if (key=='a')
		dataCursor->second.z += +0.1;
	if (key=='z')
		dataCursor->second.z += -0.1;
	
	updatePlane();
	
	
	if (key=='c')
		camera.toggleCursorDraw();
	if (key=='f')
		ofToggleFullscreen();
	
	if (key==OF_KEY_RETURN) {
		if (dataCursor == data.end())
			dataCursor = data.begin();
		else
			dataCursor++;
	}
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