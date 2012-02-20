//
//  ofProjector.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofProjector.h"

ofMesh* ofProjector::drawBox = 0;

ofProjector::ofProjector(int width, int height) {
	float aspectRatio= (float)width / (float)height;
	*this = ofProjector(2.0f, ofVec2f(0.0f, 0.5f), width, height);
}

ofProjector::ofProjector(float throwRatio, const ofVec2f& lensOffset, int width, int height) {
	this->width = width;
	this->height = height;
	this->setProjection(throwRatio, lensOffset);
	ofProjector::makeBox();
}

ofProjector::ofProjector(const ofMatrix4x4& projection, int width, int height) {
	this->projection = projection;
	this->width = width;
	this->height = height;
	ofProjector::makeBox();
}

void ofProjector::draw() const {
	ofPushStyle();
	ofSetColor(color);
	
	ofPushMatrix();
	glMultMatrixf(getViewMatrix().getInverse().getPtr());
	glMultMatrixf(getProjectionMatrix().getInverse().getPtr());
	drawBox->draw();
	ofPopMatrix();
	
	ofPopStyle();
}

void ofProjector::randomiseVectors(float amplitude) {
	randomisePose(amplitude);
}

void ofProjector::randomisePose(float scale) {
	((ofNode*)(this))->setGlobalPosition(ofVec3f(ofRandom(-scale, scale),
					   ofRandom(-scale, scale),
					   ofRandom(-scale, scale)));
	((ofNode*)(this))->setOrientation(ofQuaternion(ofRandom(360.0f), 0.0f, 1.0f, 0.0f));
}

ofRay ofProjector::castPixel(int x, int y) const {
	return castCoordinate((float) x / (float) width * 2.0f - 1.0f, 1.0f - (float) y / (float) height * 2.0f);	
}

ofRay ofProjector::castCoordinate(float x, float y) const {
	ofMatrix4x4 matrix = this->getProjectionMatrix();
	matrix.preMult(this->getViewMatrix());
	ofVec4f PosW = ofVec4f(x, y, 1.0f, 1.0f) * matrix.getInverse();
	ofVec4f t = ofVec3f(PosW / PosW.w) - this->getPosition();
	return ofRay(this->getPosition(), t, this->color);
}

void ofProjector::setProjection(float throwRatio, const ofVec2f& lensOffset) {
	ofMatrix4x4 projection;

	//throwRatio, aspectRatio
	const float aspectRatio = (float)width / (float)height;
	const float fovx = 2.0f * atan(0.5f / throwRatio) * 90.0f / atan(1.0f);
	const float fovy = fovx / aspectRatio;
	projection.makePerspectiveMatrix(fovy, aspectRatio, 0.1, 5000.0f);

	//lensOffset
	ofMatrix4x4 lensOffsetTransform;
	lensOffsetTransform.makeTranslationMatrix(-lensOffset.x * 2.0f, -lensOffset.y * 2.0f, 0.0f);
	projection *= lensOffsetTransform;

	this->setProjection(projection);
}

void ofProjector::setProjection(const ofMatrix4x4& projection) {
	this->projection = projection;
}

ofMatrix4x4 ofProjector::getViewMatrix() const {
	return this->getGlobalTransformMatrix();
}

ofMatrix4x4 ofProjector::getProjectionMatrix() const {
	return this->projection;
}

void ofProjector::makeBox() {
	if (drawBox != 0)
		return;
	
	vector<ofVec3f> vertices;
	vertices.push_back(ofVec3f(-1, -1, -1));
	vertices.push_back(ofVec3f(-1, -1, +1));
	vertices.push_back(ofVec3f(-1, +1, +1));
	vertices.push_back(ofVec3f(+1, +1, +1));
	vertices.push_back(ofVec3f(+1, -1, +1));
	vertices.push_back(ofVec3f(+1, -1, -1));
	vertices.push_back(ofVec3f(+1, +1, -1));
	vertices.push_back(ofVec3f(-1, +1, -1));
	
	vector<ofIndexType> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(0);
	
	indices.push_back(7);
	indices.push_back(2);
	
	indices.push_back(6);
	indices.push_back(3);
	
	indices.push_back(1);
	indices.push_back(4);
	
	indices.push_back(0);
	indices.push_back(5);

	drawBox = new ofMesh();
	drawBox->addVertices(vertices);
	drawBox->addIndices(indices);
	drawBox->setMode(OF_PRIMITIVE_LINES);
}
