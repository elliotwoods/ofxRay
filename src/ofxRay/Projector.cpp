//
//  Projector.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxRay/Projector.h"
#include "ofxRay/Plane.h"

ostream& operator<<(ostream & os, const ofxRay::Projector & projector) {
	const auto & node = (ofNode &)projector;

	os << node.getOrientationQuat().asVec4(); // handle that no oF serialisation operator for quat
	os << ";\n";
	os << node.getPosition();
	os << ";\n";
	os << node.getLocalTransformMatrix();
	os << ";\n";

	os << projector.width << ", " << projector.height;
	os << ";\n";
	os << projector.throwRatio;
	os << ";\n";
	os << projector.lensOffset;
	os << ";\n";
	os << projector.nearClip << ", " << projector.farClip;
	os << ";\n";
	os << projector.getClippedProjectionMatrix();
	os << ";\n";
	os << projector.getViewMatrix();
	os << ";\n";


	os << (const ofxRay::Base &) projector;

	return os;
}

istream& operator>>(istream & is, ofxRay::Projector & projector) {
	auto & node = (ofNode &)projector;

	{
		ofQuaternion orientation;
		ofVec3f position;
		ofMatrix4x4 transform;

		is >> (ofVec4f&) orientation; // handle that no oF serialisation operator for quat
		is.ignore(2);
		is >> position;
		is.ignore(2);
		is >> transform;
		is.ignore(2);

		node.setOrientation(orientation);
		node.setPosition(position);
		node.setTransformMatrix(transform);
	}
	
	{
		is >> projector.width;
		is.ignore(2);
		is >> projector.height;
		is.ignore(2);
		is >> projector.throwRatio;
		is.ignore(2);
		is >> projector.lensOffset;
		is.ignore(2);
		is >> projector.nearClip;
		is.ignore(2);
		is >> projector.farClip;
		is.ignore(2);
		is >> projector.projection;
		is.ignore(2);

		ofMatrix4x4 viewMatrix;
		is >> viewMatrix;
		is.ignore(2);
	}

	is >> (ofxRay::Base &)projector;

	return is;
}

namespace ofxRay {
	ofMesh* Projector::drawBox = 0;

	Projector::Projector(int width, int height) {
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
		ofMultMatrix(getViewMatrix().getInverse());
		ofMultMatrix(getClippedProjectionMatrix().getInverse());
		drawBox->draw();
		ofDrawLine(ofVec3f(0.0f,0.0f,-1.0f), ofVec3f(2.0f,0.0f,-1.0f));
		ofDrawLine(ofVec3f(0.0f,0.0f,-1.0f), ofVec3f(0.0f,2.0f,-1.0f));
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

	ofVec2f Projector::pixelToCoordinate(const ofVec2f & xy) const {
		return ofVec2f((xy.x + 0.5f) / (float)width * 2.0f - 1.0f
			, 1.0f - (xy.y + 0.5f) / (float)height * 2.0f);
	}

	Ray Projector::castPixel(int x, int y) const {
		return castPixel(ofVec2f(x, y));	
	}

	Ray Projector::castPixel(const ofVec2f& xy) const {
		return castCoordinate(this->pixelToCoordinate(xy));
	}

	void Projector::castPixels(const vector<ofVec2f>& xy, vector<Ray>& rays) const {
		vector<ofVec2f> xyNormalised;
		xyNormalised.reserve(xy.size());
		vector<ofVec2f>::const_iterator in;
	
		for (in = xy.begin(); in != xy.end(); in++) {
			xyNormalised.push_back(this->pixelToCoordinate(*in));
		}
		castCoordinates(xyNormalised, rays);
	}

	Ray Projector::castCoordinate(const ofVec2f& xy) const {
		ofVec2f xyUndistorted = this->undistortCoordinate(xy);
		ofMatrix4x4 matrix = this->getClippedProjectionMatrix();
		matrix.preMult(this->getViewMatrix());
		//sometimes we need to switch that +1.0f for -1.0f in the z
		//opengl should be -1.0f, directx should be +1.0f, i think opencv might be +?
		ofVec4f PosW = ofVec4f(xyUndistorted.x, xyUndistorted.y, +1.0f, 1.0f) * matrix.getInverse();
		ofVec3f t = ofVec3f(PosW / PosW.w) - this->getPosition();
		return Ray(this->getPosition(), t, ofColor(255.0f * (xyUndistorted.x + 1.0f) / 2.0f, 255.0f * (xyUndistorted.x + 1.0f) / 2.0f, 0.0f), true);
	}

	void Projector::castCoordinates(const vector<ofVec2f>& xy, vector<Ray>& rays) const {
		ofMatrix4x4 matrix = this->getClippedProjectionMatrix();
		matrix.preMult(this->getViewMatrix());

		ofVec4f s = this->getPosition();
		ofVec4f PosW;
		ofVec3f t;
	
		rays.clear();
		rays.reserve(xy.size());

		vector<ofVec2f>::const_iterator it;
		for (it = xy.begin(); it != xy.end(); it++) {
			//we're using OpenGL standard here, i.e. -1.0f is far plane
			//in DirectX, +1.0f is far plane
			PosW = ofVec4f(it->x, it->y, -1.0f, 1.0f) * matrix.getInverse();
			t = (PosW / PosW.w) - s;
rays.push_back(Ray(s, t, ofColor(255.0f * (it->x + 1.0f) / 2.0f, 255.0f * (it->y + 1.0f) / 2.0f, 0.0f), true));
		}
	}
    
    Ray Projector::getProjectionCenterRay() const {
        return Ray(castCoordinate(ofVec2f(0, 0)));
    }
    
    Ray Projector::getProjectorRay(float distance) const {
        return Ray(getPosition(), getLookAtDir() * distance, false); // TODO: default distance?
    }

    Plane Projector::getProjectionPlaneAt(float distance, bool infinite) const {
        Plane plane(getPosition() + getLookAtDir() * distance, -getLookAtDir()); // TODO: not working?
        // TODO:
        if(!infinite) {} // find corners?
        plane.setInfinite(infinite);
        plane.setScale(ofVec2f(distance / throwRatio, distance / throwRatio * height / width));  // TODO: better way of doing this?
        // TODO: account for lens offset?

        return plane;
    }


    
    ofVec3f Projector::getNormalizedSCoordinateOfWorldPosition(ofVec3f pointWorld) const {
        return pointWorld * getViewMatrix() * getClippedProjectionMatrix();
    }
    
    ofVec3f Projector::getNormalizedUCoordinateOfWorldPosition(ofVec3f pointWorld) const {
        ofVec3f normS(getNormalizedSCoordinateOfWorldPosition(pointWorld));
        return ofVec3f(ofMap(normS.x, -1, 1, 0, 1), ofMap(normS.y, 1, -1, 0, 1), normS.z);
    }
    
    ofVec3f Projector::getScreenCoordinateOfWorldPosition(ofVec3f pointWorld) const {
        return getNormalizedUCoordinateOfWorldPosition(pointWorld) * ofVec3f(width, height, 1);
    }
    
    ofVec3f Projector::getWorldPositionOfNormalizedSCoordinate(ofVec3f pointNormS) const {
        ofMatrix4x4 inverseCamera;
        inverseCamera.makeInvertOf(getViewMatrix() * getClippedProjectionMatrix());
        return pointNormS * inverseCamera;
    }
    
    ofVec3f Projector::getWorldPositionOfNormalizedUCoordinate(ofVec3f pointNormU) const {
        ofVec3f pointNormS(ofMap(pointNormU.x, 0, 1, -1, 1), ofMap(pointNormU.y, 0, 1, -1, 1), pointNormU.z);
        return getWorldPositionOfNormalizedSCoordinate(pointNormS);
    }

    ofVec3f Projector::getWorldPositionOfScreenCoordinate(ofVec3f pointScreen) const {
        return getWorldPositionOfNormalizedUCoordinate(pointScreen / ofVec3f(width, height, 1));
    }
    
	void Projector::setProjection(float throwRatio, const ofVec2f& lensOffset) {
		ofMatrix4x4 projection;
        
        this->throwRatio = throwRatio;
        this->lensOffset = lensOffset;

		//throwRatio, aspectRatio
		const float aspectRatio = (float)width / (float)height;
		const float fovy = 2.0f * atan(0.5f / (throwRatio * aspectRatio)) * RAD_TO_DEG;
		projection.makePerspectiveMatrix(fovy, aspectRatio, 0.1f, 50.0f);

		//lensOffset
		ofMatrix4x4 lensOffsetTransform;
		lensOffsetTransform.makeTranslationMatrix(-lensOffset.x * 2.0f, -lensOffset.y * 2.0f, 0.0f);
		projection *= lensOffsetTransform;

		this->projection = projection;
	}

	void Projector::setProjection(const ofMatrix4x4& projection) {
		this->throwRatio = projection(0, 0) / 2.0f;
		this->lensOffset = - ofVec2f(projection(2, 0), projection(2, 1)) / 2.0f;
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
    
    float Projector::getThrowRatio() const {
        return throwRatio;
    }
    
    ofVec2f Projector::getLensOffset() const {
        return lensOffset;
    }


	bool Projector::isProjectionMatrixInfinite() const {
		return this->projection(3, 2) == 0.0f;
	}

	ofMatrix4x4 Projector::getViewMatrix() const {
		return this->getGlobalTransformMatrix().getInverse();
	}

	ofMatrix4x4 Projector::getProjectionMatrix() const {
		return this->projection;
	}

	ofMatrix4x4 Projector::getClippedProjectionMatrix() const {
		if ( this->isProjectionMatrixInfinite() ) {
			const float n(this->nearClip);
			const float f(this->farClip);
			ofMatrix4x4 projection = this->getProjectionMatrix();
			projection(2, 2) *= (f + n) / (f - n);
			projection(3, 2) = - (f * n / (f - n));
			return projection;
		} else 
			return this->getProjectionMatrix();
	}

	void Projector::drawOnNearPlane(ofBaseHasTexture & image, bool nearPlaneFlipped) const {
		this->drawOnNearPlane(image.getTexture());
	}

	void Projector::drawOnNearPlane(ofTexture & texture, bool nearPlaneFlipped) const {
		ofMesh plane;

		ofMatrix4x4 inversed;
		inversed.makeInvertOf(this->getViewMatrix() * this->getClippedProjectionMatrix());

		float z = nearPlaneFlipped ? 1.0f : -1.0f;
		plane.addVertex(ofVec3f(-1.0f, +1.0f, z));
		plane.addTexCoord(ofVec2f(0, 0));
		plane.addVertex(ofVec3f(+1.0f, +1.0f, z));
		plane.addTexCoord(ofVec2f(this->getWidth(), 0));
		plane.addVertex(ofVec3f(-1.0f, -1.0f, z));
		plane.addTexCoord(ofVec2f(0, this->getHeight()));
		plane.addVertex(ofVec3f(+1.0f, -1.0f, z));
		plane.addTexCoord(ofVec2f(this->getWidth(), this->getHeight()));

		plane.addIndex(0);
		plane.addIndex(1);
		plane.addIndex(2);
		plane.addIndex(2);
		plane.addIndex(1);
		plane.addIndex(3);

		texture.bind();
		ofPushMatrix();
		ofMultMatrix(inversed);
		plane.draw();
		ofPopMatrix();
		texture.unbind();
	}
	
	void Projector::beginAsCamera(bool flipY) const {
		ofPushView();
		ofSetMatrixMode(ofMatrixMode::OF_MATRIX_PROJECTION);
		ofLoadIdentityMatrix();
		if (flipY) {
			ofScale(1.0f, -1.0f, 1.0f);
		}
		ofMultMatrix(getClippedProjectionMatrix());
		ofSetMatrixMode(ofMatrixMode::OF_MATRIX_MODELVIEW);
		ofLoadMatrix(getViewMatrix());
	}
	
	void Projector::endAsCamera() const {
		ofPopView();
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

	ofVec2f Projector::getCoordinateFromIndex(const uint32_t pixelIndex) const {
		uint32_t x = pixelIndex % width;
		uint32_t y = pixelIndex / width;
		return getCoordinateFromIndex(x, y);
	}

	ofVec2f Projector::getCoordinateFromIndex(const uint32_t x, const uint32_t y) const {
		return ofVec2f(2.0f * (float(x) + 0.5) / float(width) - 1.0f,
				1.0f - 2.0f * (float(y) + 0.5) / float(height));
	}
	
	ofVec2f Projector::getIndexFromCoordinate(const ofVec2f& coord) const {
		ofVec2f result = coord;
		result.y *= -1.0f;
		result += 1.0f;
		result *= ofVec2f(this->width, this->height) / 2.0f;
		return result;
	}

	void Projector::setNearClip(float nearClip) {
		this->nearClip = nearClip;
	}

	void Projector::setFarClip(float farClip) {
		this->farClip = farClip;
	}
}