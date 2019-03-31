#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	
	plane = ofPlane(glm::vec3(0,100,400),//Center 
					glm::vec3(1,0,-1),   //Normal
					glm::vec3(0,1,0),    //Up vector
					glm::vec2(100,100)); //Scale
	plane.color = ofColor::white;
	
	randomiseRay();
	ray.color = ofColor::yellow;
	ray.width = 3;
	
	projector.setPosition(glm::vec3(0,0,0));
	projector.setOrientation(glm::angleAxis(ofDegToRad(180), glm::vec3(0,1,0)));
	i = j = 0;
	
	drawEnabled.insert(pair<string,bool>("ofGrid", true));	
	drawEnabled.insert(pair<string,bool>("ofRay", true));	
	drawEnabled.insert(pair<string,bool>("ofPlane", true));	
	drawEnabled.insert(pair<string,bool>("ofProjector", false));
	drawEnabledCursor = drawEnabled.begin();
}
//--------------------------------------------------------------
void ofApp::randomiseRay(){
	float s = 100;
	ray.setStart({ofRandom(-s,s),ofRandom(-s,s),ofRandom(-s, s)});
	float t = 500;
	ray.setTranmissionVector({ofRandom(-t, t),ofRandom(-t,t),ofRandom(-t, t)});
}
//--------------------------------------------------------------
void ofApp::update(){
	i+=32;
	if (i >= projector.getWidth()) {
		i = 0;
		j++;
	}
	if (j >= projector.getHeight())
		j = 0;
	
	pRay = projector.castPixel(i, j);
	plane.getCornerRaysTo(glm::vec3(0,0,0), corners);
}
//--------------------------------------------------------------
void ofApp::drawRayIntersection(ofRay & r, ofColor onIntersectColor){
	glm::vec3 intersect;
	bool intersects = plane.intersect(r, intersect);
	
	ofPushStyle();
	if (intersects)
		ofSetColor(onIntersectColor);
	else
		ofSetColor(ofColor::white);
	ofDrawSphere(intersect, 10);
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::draw(){
	camera.begin();
	
	if (drawEnabled["ofGrid"]) {
		ofPushStyle();
		ofDrawGrid(50.0f, 10, true);
		ofPopStyle();
	}
	
	if (drawEnabled["ofRay"])
		ray.draw();
	
	if (drawEnabled["ofPlane"]) {
		ofPushStyle();
		ofSetLineWidth(2);
		plane.draw();
		ofPopStyle();
		for (int i=0; i<4; i++)
			corners[i].draw();
	}
	
	if (drawEnabled["ofProjector"]) {
		projector.draw();
		drawRayIntersection(pRay, ofColor::red);
		pRay.draw();
	}

	if (drawEnabled["ofRay"]) {
		drawRayIntersection(ray, ofColor::blue);
		ray.draw();	
		ofPushStyle();
		ofSetColor(0);
		ofDrawSphere(ray.getEnd(), 5);
		ofPopStyle();
	}
	
	camera.end();
	
	drawSelection();
	
	
}

//--------------------------------------------------------------
void ofApp::drawSelection() {
	int nItems = drawEnabled.size();
	string instruction = "[UP]/[DOWN] to select, [LEFT]/[RIGHT] to disable/enable";
	
	int stringWidth = 0;
	for(auto& it: drawEnabled){
		stringWidth = MAX(stringWidth, (it.first).length());
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
	for(auto& it: drawEnabled){
		result = (drawEnabledCursor->first == it.first) ? "> " : "  ";
		result += it.first;
		while (result.length() < stringWidth+2)
			result.push_back(' ');
		result += it.second ? " [O]" : " [ ]";
		ofDrawBitmapString(result, 40, y);
		y += 10;
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if (ofGetKeyPressed(OF_KEY_SHIFT)) {
		if (key==OF_KEY_LEFT)
			ray.s += ofVec3f(-1,0,0);
		if (key==OF_KEY_RIGHT)
			ray.s += ofVec3f(1,0,0);
		if (key==OF_KEY_UP)
			ray.s += ofVec3f(0,1,0);
		if (key==OF_KEY_DOWN)
			ray.s += ofVec3f(0,-1,0);
	}else{
		if (key==' ') {
			randomiseRay();
		}
		if (key==OF_KEY_UP) {
			if (drawEnabledCursor == drawEnabled.begin())
			{
				drawEnabledCursor = drawEnabled.end();
				drawEnabledCursor--;
			}
			else
				drawEnabledCursor--;
		}
		if (key==OF_KEY_DOWN) {
			if (drawEnabledCursor == drawEnabled.end())
				drawEnabledCursor = drawEnabled.begin();
			else
				drawEnabledCursor++;
		}
		if (key==OF_KEY_LEFT)
			if (drawEnabledCursor != drawEnabled.end())
				drawEnabledCursor->second = false;
		if (key==OF_KEY_RIGHT)
			if (drawEnabledCursor != drawEnabled.end())
				drawEnabledCursor->second = true;
	}
}
