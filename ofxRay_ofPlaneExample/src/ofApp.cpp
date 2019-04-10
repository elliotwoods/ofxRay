#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	
	data.insert(pair<string, glm::vec3&>("center", center));
	data.insert(pair<string, glm::vec3&>("normal", normal));
	data.insert(pair<string, glm::vec3&>("up", up));
	data.insert(pair<string, glm::vec3&>("scale xy", scale));
	dataCursor = data.begin();
	
	center = glm::vec3(50.0f, 50.0f, 0.0f);
	normal = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
	up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	scale = glm::vec3(200.0f, 200.0f,1);
	
	updatePlane();
	plane.setInfinite(false);
	
	plane.color = ofColor::white;
	
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	camera.begin();
	
	ofDrawGrid(50,5,true);
	ofPushStyle();
	ofSetLineWidth(2);
	plane.draw();
	ofPopStyle();
	camera.end();
	
	drawSelection();
}

//--------------------------------------------------------------
void ofApp::drawSelection() {
	int nItems = data.size();
	string instruction = "[RETURN] to move selection, [UP]/[DOWN],[LEFT]/[RIGHT],[A]/[Z] to edit";
	
	int stringWidth = 0;
	map<string, glm::vec3&>::iterator it;
	for(it = data.begin(); it != data.end(); it++) {
		stringWidth = MAX(stringWidth, (it->first).length());
	}
	
	ofPushStyle();
	ofFill();
	ofSetColor(200, 100, 100);
	int y = ofGetHeight() - nItems*10 - 70;
	ofDrawRectangle(20, y, MAX(stringWidth, instruction.length()) * 8 + 40, nItems*10 + 50);
	
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
