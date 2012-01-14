//
//  ofPlane.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofPlane.h"

ofMesh* ofPlane::viewGrid = 0;
ofMesh* ofPlane::viewPlane = 0;

ofPlane::ofPlane() {
	infinite = true;
	makeGrid();
	randomiseVectors();
}

ofPlane::ofPlane(float a, float b, float c, float d) {
	infinite = true;
	makeGrid();
}

ofPlane::ofPlane(ofVec3f center, ofVec3f normal) {
	c = center;
	n = normal;
	infinite = true;
	makeGrid();
}

ofPlane::ofPlane(ofVec3f center, ofVec3f normal, ofVec3f up, ofVec2f scale) {
	c = center;
	n = normal.normalize();
	infinite = false;
	this->up = up.normalize();
	this->scale = scale;
	
	makeGrid();
}

void ofPlane::draw() const {
	ofPushMatrix();
	ofTranslate(c);
	ofQuaternion q = ofQuaternion();
	q.makeRotate(ofVec3f(0,0,1), n);
	float angle, x, y, z;
	q.getRotate(angle, x, y, z);
	ofRotate(angle, x, y, z);
	
	
	ofPushStyle();
	ofSetColor(color);
	
	if (!this->infinite) {
		//align the up vector
		ofVec3f upT = ofVec3f(0,1,0) * q;
		ofQuaternion qU;
		qU.makeRotate(upT, this->up);
		qU.getRotate(angle, x, y, z);
		ofRotate(angle, x, y, z);
		ofScale(scale.x, scale.y, 1);
	}
	viewGrid->draw();
	
	if (this->infinite) {
		ofEnableAlphaBlending();
		ofSetColor(color.r, color.g, color.b, 50);
		viewPlane->draw();	
	}
		
	ofPopStyle();
	
	ofPopMatrix();
}

void ofPlane::randomiseVectors(float amplitude) {
	c = ofVec3f(ofRandom(-1.0f, 1.0f),
				ofRandom(-1.0f, 1.0f),
				ofRandom(-1.0f, 1.0f)) * amplitude;
	n = ofVec3f(ofRandom(-1.0f, 1.0f),
				ofRandom(-1.0f, 1.0f),
				ofRandom(-1.0f, 1.0f)) * amplitude;
	n.normalize();
}

bool ofPlane::intersect(const ofRay &ray, ofVec3f &position) const {
	//check if line is parallel to plane
	//if so, die young
	if (ray.t.dot(n) == 0.0f)
		return false;
	
	float u = n.dot(c - ray.s) / n.dot(ray.t);	
	position = ray.s + u * ray.t;
	
	
	if (!ray.infinite) {
		//check within line segment
		if (u < 0.0f || u > 1.0f)
			return false;
	}
	
	if (!this->infinite) {
		//check within plane segment
		
		//define up as ray, find distance between this ray and ray<> plane intersection point to get x position in plane
		float x = getUpRay().distanceTo(position);
		float y = sqrt((c - position).lengthSquared() - x*x);
		
		//if length along this ray < height and distance of point to ray < width then we're in the plane
		if (abs(x) <= scale.x && abs(y) <= scale.y)
			return true;
		else
			return false;
		
	} else
		return true;
}

const ofVec3f& ofPlane::getC() const{
	return c;
}

const ofVec3f& ofPlane::getN() const {
	return n;
}

bool ofPlane::getInfinite() const {
	return infinite;
}

void ofPlane::setC(const ofVec3f &c) {
	this->c = c;
}

void ofPlane::setN(const ofVec3f &n) {
	this->n = n;
}

void ofPlane::setInfinite(const bool b) {
	infinite = b;
}

void ofPlane::getCornerRaysTo(const ofVec3f &target, ofRay* rays) const {
	ofVec3f up = this->up * scale.y;
	ofVec3f right = getRight() * scale.x;
	ofVec3f corner;
	
	corner = c - up - right;
	*rays++ = ofRay(corner, target - corner, color, false);

	corner = c - up + right;
	*rays++ = ofRay(corner, target - corner, color, false);
	
	corner = c + up + right;
	*rays++ = ofRay(corner, target - corner, color, false);
	
	corner = c + up - right;
	*rays++ = ofRay(corner, target - corner, color, false);
}

void ofPlane::getCornerRaysFrom(const ofVec3f &source, ofRay* rays) const {
	ofVec3f up = this->up * scale.y;
	ofVec3f right = getRight() * scale.x;
	ofVec3f corner;
	
	corner = c - up - right;
	*rays++ = ofRay(source, corner - source, color, false);
	
	corner = c - up + right;
	*rays++ = ofRay(source, corner - source, color, false);
	
	corner = c + up + right;
	*rays++ = ofRay(source, corner - source, color, false);
	
	corner = c + up - right;
	*rays++ = ofRay(source, corner - source, color, false);
}

void ofPlane::makeGrid() {
	if (viewGrid != 0)
		return;
	
	vector<ofVec3f> vertices((2.0f / 0.25 + 1)*4);
	int i=0;
	for (float y=-1.0f; y<=1.0f; y+=0.25f) {
		vertices[i++] = ofVec3f(-1.0f, y, 0.0f);
		vertices[i++] = ofVec3f(+1.0f, y, 0.0f);
	}
	cout << endl;
	for (float x=1.0f; x>=-1.0f; x-=0.25f) {
		vertices[i++] = ofVec3f(x, 1.0f, 0.0f);
		vertices[i++] = ofVec3f(x, -1.0f, 0.0f);
	}
	
	viewGrid = new ofMesh();
	viewGrid->addVertices(vertices);
	viewGrid->setMode(OF_PRIMITIVE_LINES);
	
	viewPlane = new ofMesh();
	vertices.resize(4);
	vertices[0] = ofVec3f(-1000.0f, -1000.0f, 0.0f);
	vertices[1] = ofVec3f(+1000.0f, -1000.0f, 0.0f);
	vertices[2] = ofVec3f(-1000.0f, +1000.0f, 0.0f);
	vertices[3] = ofVec3f(-1000.0f, +1000.0f, 0.0f);
	viewPlane->addVertices(vertices);
	viewPlane->setMode(OF_PRIMITIVE_TRIANGLE_FAN);	
}

ofVec3f ofPlane::getUp() const {
	return up;
}

ofVec3f ofPlane::getRight() const {
	return up.crossed(n).normalize();
}

ofRay ofPlane::getUpRay() const {
	return ofRay(c, up);
}

ofRay ofPlane::getRightRay() const {
	return ofRay(c, getRight());
}
