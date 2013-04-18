//
//  Projector.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxRay/Projector.h"

namespace ofxRay {
	ofMesh* Projector::drawBox = 0;

	Projector::Projector(int width, int height) {
		float aspectRatio= (float)width / (float)height;
		*this = Projector(2.0f, ofVec2f(0.0f, 0.5f), width, height);
	}

	Projector::Projector(float throwRatio, const ofVec2f& lensOffset, int width, int height) {
		this->width = width;
		this->height = height;
		this->setProjection(throwRatio, lensOffset);
		Projector::makeBox();
	}

	Projector::Projector(const ofMatrix4x4& projection, int width, int height) {
		this->projection = projection;
		this->width = width;
		this->height = height;
		Projector::makeBox();
	}

	void Projector::draw() const {
		ofPushStyle();
		ofSetColor(color);
		ofSetLineWidth(3.0f);
	
		ofPushMatrix();
		glMultMatrixf(getViewMatrix().getInverse().getPtr());
		glMultMatrixf(getClippedProjectionMatrix().getInverse().getPtr());
		drawBox->draw();
		ofPopMatrix();
	
		ofPopStyle();
	}

	void Projector::randomiseVectors(float amplitude) {
		randomisePose(amplitude);
	}

	void Projector::randomisePose(float scale) {
		((ofNode*)(this))->setGlobalPosition(ofVec3f(ofRandom(-scale, scale),
						   ofRandom(-scale, scale),
						   ofRandom(-scale, scale)));
		((ofNode*)(this))->setOrientation(ofQuaternion(ofRandom(360.0f), 0.0f, 1.0f, 0.0f));
	}

	Ray Projector::castPixel(int x, int y) const {
		return castPixel(ofVec2f(x, y));	
	}

	Ray Projector::castPixel(const ofVec2f& xy) const {
		return castCoordinate(ofVec2f(	(xy.x + 0.5f)/ (float) width * 2.0f - 1.0f,
										1.0f - (xy.y + 0.5f) / (float) height * 2.0f)	);
	}

	void Projector::castPixels(const vector<ofVec2f>& xy, vector<Ray>& rays) const {
		vector<ofVec2f> xyNormalised;
		xyNormalised.reserve(xy.size());
		vector<ofVec2f>::const_iterator in;
	
		for (in = xy.begin(); in != xy.end(); in++)
			xyNormalised.push_back(ofVec2f(	(in->x + 0.5f)/ (float) width * 2.0f - 1.0f,
											1.0f - (in->y + 0.5f) / (float) height * 2.0f));
		castCoordinates(xyNormalised, rays);
	}

	Ray Projector::castCoordinate(const ofVec2f& xy) const {
		ofVec2f xyUndistorted = this->undistortCoordinate(xy);
		ofMatrix4x4 matrix = this->getProjectionMatrix();
		matrix.preMult(this->getViewMatrix());
		ofVec4f PosW = ofVec4f(xyUndistorted.x, xyUndistorted.y, 1.0f, 1.0f) * matrix.getInverse();
		ofVec3f t = ofVec3f(PosW / PosW.w) - this->getPosition();
		return Ray(this->getPosition(), t, ofColor(255.0f * (xyUndistorted.x + 1.0f) / 2.0f, 255.0f * (xyUndistorted.x + 1.0f) / 2.0f, 0.0f));
	}

	void Projector::castCoordinates(const vector<ofVec2f>& xy, vector<Ray>& rays) const {
		ofMatrix4x4 matrix = this->getProjectionMatrix();
		matrix.preMult(this->getViewMatrix());

		ofVec4f s = this->getPosition();
		ofVec4f PosW;
		ofVec3f t;
	
		rays.clear();
		rays.reserve(xy.size());

		vector<ofVec2f>::const_iterator it;
		for (it = xy.begin(); it != xy.end(); it++) {
			 PosW = ofVec4f(it->x, it->y, 1.0f, 1.0f) * matrix.getInverse();
			 t = (PosW / PosW.w) - s;
			 rays.push_back(Ray(s, t, ofColor(255.0f * (it->x + 1.0f) / 2.0f, 255.0f * (it->y + 1.0f) / 2.0f, 0.0f)));
		}
	}

	void Projector::setProjection(float throwRatio, const ofVec2f& lensOffset) {
		ofMatrix4x4 projection;

		//throwRatio, aspectRatio
		const float aspectRatio = (float)width / (float)height;
		const float fovx = 2.0f * atan(0.5f / throwRatio) * 90.0f / atan(1.0f);
		const float fovy = fovx / aspectRatio;
		projection.makePerspectiveMatrix(fovy, aspectRatio, 0.1f, 50.0f);

		//lensOffset
		ofMatrix4x4 lensOffsetTransform;
		lensOffsetTransform.makeTranslationMatrix(-lensOffset.x * 2.0f, -lensOffset.y * 2.0f, 0.0f);
		projection *= lensOffsetTransform;

		this->setProjection(projection);
	}

	void Projector::setProjection(const ofMatrix4x4& projection) {
		this->projection = projection;
	}

	void Projector::setWidth(int width) {
		this->width = width;
	}

	void Projector::setHeight(int height) {
		this->height = height;
	}

	int Projector::getWidth() const {
		return this->width;
	}

	int Projector::getHeight() const {
		return this->height;
	}

	bool Projector::isProjectionMatrixInfinite() const {
		return this->projection(2, 2) == 0.0f;
	}

	ofMatrix4x4 Projector::getViewMatrix() const {
		return this->getGlobalTransformMatrix().getInverse();
	}

	ofMatrix4x4 Projector::getProjectionMatrix() const {
		return this->projection;
	}

	ofMatrix4x4 Projector::getClippedProjectionMatrix() const {
		if ( this->isProjectionMatrixInfinite() ) {
			const float n(0.01f); //near
			const float f(10.0f); //far
			ofMatrix4x4 projection = this->getProjectionMatrix();
			projection(2, 2) = - (f + n) / (f - n);
			projection(3, 2) = - f * n / (f - n);
			return projection;
		} else 
			return this->getProjectionMatrix();
	}

	void Projector::drawOnNearPlane(ofBaseHasTexture &image) const {
		ofMesh plane;
		
		ofMatrix4x4 inversed;
		inversed.makeInvertOf(this->getViewMatrix() * this->getProjectionMatrix());
		
		plane.addVertex(ofVec3f(-1.0f, +1.0f, -1.0f) * inversed);
		plane.addTexCoord(ofVec2f(0,0));
		plane.addVertex(ofVec3f(+1.0f, +1.0f, -1.0f) * inversed);
		plane.addTexCoord(ofVec2f(this->getWidth(),0));
		plane.addVertex(ofVec3f(-1.0f, -1.0f, -1.0f) * inversed);
		plane.addTexCoord(ofVec2f(0,this->getHeight()));
		plane.addVertex(ofVec3f(+1.0f, -1.0f, -1.0f) * inversed);
		plane.addTexCoord(ofVec2f(this->getWidth(),this->getHeight()));
		
		plane.addIndex(0);
		plane.addIndex(1);
		plane.addIndex(2);
		plane.addIndex(2);
		plane.addIndex(1);
		plane.addIndex(3);
		
		image.getTextureReference().bind();
		plane.draw();
		image.getTextureReference().unbind();
	}
	
	void Projector::makeBox() {
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

	ofVec2f Projector::getNormFromIndex(const uint index) {
		uint x = index % width;
		uint y = index / width;
		return ofVec2f(2.0f * (float(x) + 0.5) / float(width) - 1.0f,
				2.0f * (float(y) + 0.5) / float(height) - 1.0f);
	}
}

