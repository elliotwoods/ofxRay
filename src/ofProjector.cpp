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
	this->viewProjInitFlag = false;
	randomisePose();
	makeBox();
}

/**
 * if we init with the viewProj matrix we can't decompose it to get just the view or
 * just the projection matrix (can we?) but we can 
 * presumably make everythine else work ok
 * should we just inherit this class and write new methods? probably yes
 */
/***EW***/ //We can just make the other one an identity matrix
ofProjector::ofProjector(const ofMatrix4x4& viewProjection, int width, int height) {
	//throw("Error: viewProjection decomposition not implemented");
	this->viewProjMatrix = viewProjection;
	this->width = width;
	this->height = height;
	this->aspectRatio = (float) this->width / (float) this->height;
	// how do we know what the throw and offset should be?
	this->throwRatio = 2.0f;
	this->lensOffset = ofVec2f(0.0f, 0.5f);
	this->viewProjInitFlag = true;
  
	// compute the position vec of the camera
	ofMatrix4x4 temp = this->viewProjMatrix.getInverse();
	ofVec4f temp4Vec = ofVec4f(0,0,0,1) * temp ;
	this->position = ofVec3f(temp4Vec / temp4Vec.w);
  
	// what does makeBox do? 
	makeBox(); 
}

ofMatrix4x4 ofProjector::getViewProjMatrix() const{
  return(this->viewProjMatrix);
}

void ofProjector::draw() const {
	ofPushStyle();
	ofSetColor(color);
	
	ofPushMatrix();
	// ccs, here we need to change this if we've init'd with the 
	// proj-view matrix
	if(viewProjInitFlag){
		// ccs, i think we need to call mult even with a single matrix
		// the stack is initially the identity...
		glMultMatrixf(getViewProjMatrix().getInverse().getPtr());
	} else {
		glMultMatrixf(getViewMatrix().getInverse().getPtr());
		glMultMatrixf(getProjectionMatrix().getInverse().getPtr());
	}
	drawBox->draw();

	ofDrawArrow(ofVec3f(0,0,-1),ofVec3f(0,2,-1), 1.0f);
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

// index starts from the top left (0,0)
// 
// index = Y * width  + x;
// so Y = index / width 
ofRay ofProjector::castPixelIndex(int index) const {
	int x, y;
	y = index / this->width;
	x = index % this->width;
	ofLogError() << "CastingPixels: " << index << "(x,y): " << x << " " << y << " (" << this->width << "," << this->height << ")";
	return castPixel(x, y);
}

ofRay ofProjector::castPixel(int x, int y) const {
	return castCoordinate( ((float) x / (float) width) * 2.0f - 1.0f, 1.0f - ((float) y / (float) height) * 2.0f);	
}

ofRay ofProjector::castCoordinate(float x, float y) const {
	ofMatrix4x4 matrix;
	if(viewProjInitFlag) {
		matrix = this->getViewProjMatrix();
	} else {
		matrix = this->getProjectionMatrix();
		matrix.preMult(this->getViewMatrix());
	}

	  ofVec4f PosW, s,t;
  
	  PosW = ofVec4f(x, y, 1.0f, 1.0f) * matrix.getInverse();
	  t = ofVec3f(PosW / PosW.w) - position;
	  s = position;
	  ofLogNotice() << "CastingRay: s" << s << "t: " << t;
  
	return ofRay(s, t, this->color);
}

ofMatrix4x4 ofProjector::getViewMatrix() const {
	ofMatrix4x4 view;
	view.makeRotationMatrix(rotation);
	view.preMultTranslate(-position);
	return view;
}

ofMatrix4x4 ofProjector::getProjectionMatrix() const {
	ofMatrix4x4 projection;
	
	////
	//approx
	////
	//
	ofLogWarning() << "getProjectionMatrix() is fudged until can circumvent ofMatrix4x4::makeFrustumMatrix issue";
	float fovx = atan(0.5f / throwRatio) / (atan(1.0f) * 8) * 360;
	float fovy = fovx / aspectRatio;
	projection.makePerspectiveMatrix(fovy, aspectRatio, 0.1, 10.0f);
	//
	////
	
	
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
