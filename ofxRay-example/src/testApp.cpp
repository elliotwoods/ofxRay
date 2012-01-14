#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	camera.setCursorDraw(true);
	
	plane = ofPlane(ofVec3f(1,1,0), ofVec3f(1,0,-1), ofVec3f(0,1,0), ofVec2f(3,2));
	ray.randomise();
	
	projector.position = ofVec3f(0,1,0);
	projector.rotation.makeRotate(90,0,1,0);
	i = j = 0;
	
	drawEnabled.insert(pair<string,bool>("ofGrid", true));	
	drawEnabled.insert(pair<string,bool>("ofRay", true));	
	drawEnabled.insert(pair<string,bool>("ofPlane", true));	
	drawEnabled.insert(pair<string,bool>("ofProjector", false));
	drawEnabledCursor = drawEnabled.begin();
}

//--------------------------------------------------------------
void testApp::update(){
	i+=32;
	if (i >= projector.width) {
		i = 0;
		j++;
	}
	if (j >= projector.height)
		j = 0;
	
	pRay = projector.castPixel(i, j);
	plane.getCornerRaysTo(ofVec3f(0,0,0), corners);
}

//--------------------------------------------------------------
void testApp::draw(){
	camera.begin();
	
	if (drawEnabled["ofGrid"]) {
		ofPushStyle();
		ofSetColor(155,100,100);
		ofDrawGrid(1.0f, 5.0f, true);
		ofPopStyle();
	}
	
	if (drawEnabled["ofRay"])
		ray.draw();
	
	if (drawEnabled["ofPlane"]) {
		plane.draw();
		for (int i=0; i<4; i++)
			corners[i].draw();
	}
	
	if (drawEnabled["ofProjector"]) {
		projector.draw();
		pRay.draw();
	}

	if (drawEnabled["ofRay"] && drawEnabled["ofRay"]) {
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
		ofCircle(0, 0, 0.1f);
		ofPopStyle();
		ofPopMatrix();
	}
	
	camera.end();
	
	drawSelection();
}

//--------------------------------------------------------------
void testApp::drawSelection() {
	int nItems = drawEnabled.size();
	string instruction = "[UP]/[DOWN] to select, [LEFT]/[RIGHT] to disable/enable";
	
	int stringWidth = 0;
	map<string, bool>::iterator it;
	for(it = drawEnabled.begin(); it != drawEnabled.end(); it++) {
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
	for(it = drawEnabled.begin(); it != drawEnabled.end(); it++) {
		result = drawEnabledCursor == it ? "> " : "  ";
		result += it->first;
		while (result.length() < stringWidth+2)
			result.push_back(' ');
		result += it->second ? " [O]" : " [ ]";
		ofDrawBitmapString(result, 40, y);
		y += 10;
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (ofGetKeyPressed(OF_KEY_SHIFT)) {
		if (key==OF_KEY_LEFT)
			ray.s += ofVec3f(-0.1,0,0);
		if (key==OF_KEY_RIGHT)
			ray.s += ofVec3f(0.1,0,0);
		if (key==OF_KEY_UP)
			ray.s += ofVec3f(0,0.1,0);
		if (key==OF_KEY_DOWN)
			ray.s += ofVec3f(0,-0.1,0);
	}
	
	if (key=='c')
		camera.toggleCursorDraw();
	
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