//
//  ofProjector.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofProjector.h"

ofMesh* ofProjector::drawBox = 0;

ofProjector::ofProjector() {
	*this = ofProjector(1024, 768);
	/*
	this->width = 1024;
	this->height = 768;
	this->aspectRatio = this->width / this->height;
	this->throwRatio = 2.0f;
	
	randomisePose();
	 */
}

ofProjector::ofProjector(int width, int height) {
	this->width = width;
	this->height = height;
	this->aspectRatio = (float) this->width / (float) this->height;
	this->throwRatio = 2.0f;
	this->lensOffset = ofVec2f(0.0f, 0.5f);
	randomisePose();
	makeBox();
}

ofProjector::ofProjector(const ofMatrix4x4& viewProjection, int width, int height) {
	throw("Error: viewProjection decomposition not implemented");
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
	position = ofVec3f(ofRandom(-scale, scale),
					   ofRandom(-scale, scale),
					   ofRandom(-scale, scale));
	rotation.makeRotate(ofRandom(360.0f), 0.0f, 1.0f, 0.0f);
}

ofRay ofProjector::castPixel(int x, int y) const {
	return castCoordinate((float) x / (float) width * 2.0f - 1.0f, 1.0f - (float) y / (float) height * 2.0f);	
}

ofRay ofProjector::castCoordinate(float x, float y) const {
	ofMatrix4x4 matrix = this->getProjectionMatrix();
	matrix.preMult(this->getViewMatrix());
	ofVec4f PosW = ofVec4f(x, y, 1.0f, 1.0f) * matrix.getInverse();
	ofVec4f t = ofVec3f(PosW / PosW.w) - position;
	
	return ofRay(position, t, this->color);
}

ofMatrix4x4 ofProjector::getViewMatrix() const {
	ofMatrix4x4 view;
	view.makeRotationMatrix(rotation);
	view.preMultTranslate(-position);
	return view;
}

ofMatrix4x4 ofProjector::getProjectionMatrix() const {
	ofMatrix4x4 projection;
	
	///////
	//approx
	///////
	//
	ofLogWarning() << "getProjectionMatrix() is fudged until can circumvent ofMatrix4x4::makeFrustumMatrix issue";
	float fovx = atan(0.5f / throwRatio) / (atan(1.0f) * 8) * 360;
	float fovy = fovx / aspectRatio;
	projection.makePerspectiveMatrix(fovy, aspectRatio, 0.1, 10.0f);
	//
	///////
	
	
	//the following doesn't give sensible results:
	//projection.makeFrustumMatrix(-0.5f + lensOffset.x, +0.5f + lensOffset.x, -0.5 / aspectRatio + lensOffset.y,  -0.5 / aspectRatio + lensOffset.y, 0.1f, throwRatio);
	return projection;
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
