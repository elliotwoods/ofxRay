#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	
	data.insert(pair<string, float&>("position.x", position.x));
	data.insert(pair<string, float&>("position.y", position.y));
	data.insert(pair<string, float&>("position.z", position.z));
	data.insert(pair<string, float&>("resolution.x", resolution.x));
	data.insert(pair<string, float&>("resolution.y", resolution.y));
	data.insert(pair<string, float&>("throwRatio", throwRatio));
	data.insert(pair<string, float&>("lensOffset.x", lensOffset.x));
	data.insert(pair<string, float&>("lensOffset.y", lensOffset.y));
	
	dataCursor = data.begin();
	
	position = ofVec3f(1.0f, 1.0f, 0.0f);
	resolution = ofVec2f(1024, 768);
	throwRatio = 2.0f;
	lensOffset = ofVec2f(0.0f, 0.5f);
	
	updateProjector();
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
	
	projector.draw();
	
	for (const auto & ray : this->rays) {
		ray.draw();
	}
	
	camera.end();
	
	drawSelection();
}

//--------------------------------------------------------------
void ofApp::drawSelection() {
	int nItems = data.size();
	string instruction = "[UP]/[DOWN] to move selection, [LEFT]/[RIGHT] to edit";
	
	int stringWidth = 0;
	map<string, float&>::iterator it;
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
		dataCursor->second += -0.1;
	if (key==OF_KEY_RIGHT)
		dataCursor->second += +0.1;
	
	if (key==OF_KEY_UP) {
		if (dataCursor == data.begin())
			dataCursor = data.end();
		dataCursor--;
	}
	if (key==OF_KEY_DOWN) {
		dataCursor++;
		if (dataCursor == data.end())
			dataCursor = data.begin();
	}
	
	updateProjector();
	
	
	if (key=='f')
		ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::updateProjector()	{
	projector.setWidth(this->resolution.x);
	projector.setHeight(this->resolution.y);
	projector.setProjection(this->throwRatio, this->lensOffset);
	projector.setPosition(this->position);
	pixels.clear();
	for (int i = 0; i < this->resolution.x; i += 32) {
		for (int j = 0; j < this->resolution.y; j += 32) {
			pixels.push_back(glm::vec2(i, j));
		}
	}

	projector.castPixels(pixels, rays);
}
