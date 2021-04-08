//
//  Projector.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxRay/Projector.h"
#include "ofxRay/Plane.h"

#include <glm/gtx/matrix_decompose.hpp>

ostream& operator<<(ostream & os, const ofxRay::Projector & projector) {
	const auto & node = (ofNode &)projector;

	os << node.getOrientationQuat();//.asVec4(); // handle that no oF serialisation operator for quat
	os << ";\n";
	os << node.getPosition();
	os << ";\n";
	os << node.getScale();
//	os << node.getLocalTransformMatrix();
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
		glm::quat orientation;
		glm::vec3 position;
		glm::vec3 scale;

		is >> orientation; // handle that no oF serialisation operator for quat
		is.ignore(2);
		is >> position;
		is.ignore(2);
		is >> scale;
		is.ignore(2);

		node.setOrientation(orientation);
		node.setPosition(position);
		node.setScale(scale);
//		node.setTransformMatrix(transform);
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

		glm::mat4 viewMatrix;
		is >> viewMatrix;
		is.ignore(2);
	}

	is >> (ofxRay::Base &)projector;

	return is;
}

namespace ofxRay {
	ofMesh* Projector::drawBox = 0;

	Projector::Projector(int width, int height) {
		*this = Projector(2.0f, glm::vec2(0.0f, 0.5f), width, height);
	}

	Projector::Projector(float throwRatio, const glm::vec2& lensOffset, int width, int height) {
		this->width = width;
		this->height = height;
		this->setProjection(throwRatio, lensOffset);
		Projector::makeBox();
	}

	Projector::Projector(const glm::mat4& projection, int width, int height) {
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
		ofMultMatrix(glm::inverse(getViewMatrix()));
		ofMultMatrix(glm::inverse(getClippedProjectionMatrix()));
		drawBox->draw();
		ofDrawLine(glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(2.0f,0.0f,-1.0f));
		ofDrawLine(glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,2.0f,-1.0f));
		ofPopMatrix();
	
		ofPopStyle();
	}

	void Projector::randomiseVectors(float amplitude) {
		randomisePose(amplitude);
	}

	void Projector::randomisePose(float scale) {
		((ofNode*)(this))->setGlobalPosition(glm::vec3(ofRandom(-scale, scale),
						   ofRandom(-scale, scale),
						   ofRandom(-scale, scale)));
		((ofNode*)(this))->setOrientation(ofQuaternion(ofRandom(360.0f), 0.0f, 1.0f, 0.0f));
	}

	glm::vec2 Projector::pixelToCoordinate(const glm::vec2 & xy) const {
		return glm::vec2((xy.x + 0.5f) / (float)width * 2.0f - 1.0f
			, 1.0f - (xy.y + 0.5f) / (float)height * 2.0f);
	}

	Ray Projector::castPixel(int x, int y) const {
		return castPixel(glm::vec2(x, y));	
	}

	Ray Projector::castPixel(const glm::vec2& xy) const {
		return castCoordinate(this->pixelToCoordinate(xy));
	}

	void Projector::castPixels(const vector<glm::vec2>& xy, vector<Ray>& rays) const {
		vector<glm::vec2> xyNormalised;
		xyNormalised.reserve(xy.size());
		vector<glm::vec2>::const_iterator in;
	
		for (in = xy.begin(); in != xy.end(); in++) {
			xyNormalised.push_back(this->pixelToCoordinate(*in));
		}
		castCoordinates(xyNormalised, rays);     
	}

	Ray Projector::castCoordinate(const glm::vec2& xy) const {
		glm::vec2 xyUndistorted = this->undistortCoordinate(xy);
		glm::mat4 matrix = this->getClippedProjectionMatrix();
//		matrix.preMult(this->getViewMatrix());
		matrix = matrix * this->getViewMatrix(); 
		//sometimes we need to switch that +1.0f for -1.0f in the z
		//opengl should be -1.0f, directx should be +1.0f, i think opencv might be +?
		
		glm::vec4 PosW = glm::inverse(matrix) * glm::vec4(xyUndistorted.x, xyUndistorted.y, +1.0f, 1.0f) ;
		glm::vec3 t = glm::vec3(PosW / PosW.w) - this->getPosition();
		return Ray(this->getPosition(), t, ofColor(255.0f * (xyUndistorted.x + 1.0f) / 2.0f, 255.0f * (xyUndistorted.x + 1.0f) / 2.0f, 0.0f), true);
	}

	void Projector::castCoordinates(const vector<glm::vec2>& xy, vector<Ray>& rays) const {
		glm::mat4 inverseMatrix = glm::inverse(this->getClippedProjectionMatrix() * this->getViewMatrix());
		
		glm::vec4 s = glm::vec4(this->getPosition(), 0);
		glm::vec4 PosW;
		glm::vec3 t;
	
		rays.clear();
		rays.reserve(xy.size());

		
		for (auto& it:xy){// it = xy.begin(); it != xy.end(); it++) {
			//we're using OpenGL standard here, i.e. -1.0f is far plane
			//in DirectX, +1.0f is far plane
			auto xyUndistorted = this->undistortCoordinate(it);
			PosW = inverseMatrix * glm::vec4(xyUndistorted.x, xyUndistorted.y, +1.0f, 1.0f);
			t = (PosW / PosW.w) - s;
			rays.push_back(Ray(s, t, ofColor(255.0f * (it.x + 1.0f) / 2.0f, 255.0f * (it.y + 1.0f) / 2.0f, 0.0f), true));
		}
	}
    
    Ray Projector::getProjectionCenterRay() const {
        return Ray(castCoordinate(glm::vec2(0, 0)));
    }
    
    Ray Projector::getProjectorRay(float distance) const {
        return Ray(getPosition(), getLookAtDir() * distance, false); // TODO: default distance?
    }

    Plane Projector::getProjectionPlaneAt(float distance, bool infinite) const {
        Plane plane(getPosition() + getLookAtDir() * distance, -getLookAtDir()); // TODO: not working?
        // TODO:
        if(!infinite) {} // find corners?
        plane.setInfinite(infinite);
        plane.setScale(glm::vec2(distance / throwRatio, distance / throwRatio * height / width));  // TODO: better way of doing this?
        // TODO: account for lens offset?

        return plane;
    }


    
    glm::vec3 Projector::getNormalizedSCoordinateOfWorldPosition(glm::vec3 pointWorld) const {
		auto tmp = getClippedProjectionMatrix() * getViewMatrix() * glm::vec4(pointWorld,1);
		return glm::vec3(tmp)/tmp.w;
    }
    
    glm::vec3 Projector::getNormalizedUCoordinateOfWorldPosition(glm::vec3 pointWorld) const {
        glm::vec3 normS(getNormalizedSCoordinateOfWorldPosition(pointWorld));
        return glm::vec3(ofMap(normS.x, -1, 1, 0, 1), ofMap(normS.y, 1, -1, 0, 1), normS.z);
    }
    
    glm::vec3 Projector::getScreenCoordinateOfWorldPosition(glm::vec3 pointWorld) const {
        return getNormalizedUCoordinateOfWorldPosition(pointWorld) * glm::vec3(width, height, 1);
    }
    
    glm::vec3 Projector::getWorldPositionOfNormalizedSCoordinate(glm::vec3 pointNormS) const {
		auto tmp = glm::inverse(getClippedProjectionMatrix() * getViewMatrix()) * glm::vec4(pointNormS,1);
		return glm::vec3(tmp)/tmp.w;
    }
    
    glm::vec3 Projector::getWorldPositionOfNormalizedUCoordinate(glm::vec3 pointNormU) const {
        glm::vec3 pointNormS(ofMap(pointNormU.x, 0, 1, -1, 1), ofMap(pointNormU.y, 0, 1, -1, 1), pointNormU.z);
        return getWorldPositionOfNormalizedSCoordinate(pointNormS);
    }

    glm::vec3 Projector::getWorldPositionOfScreenCoordinate(glm::vec3 pointScreen) const {
        return getWorldPositionOfNormalizedUCoordinate(pointScreen / glm::vec3(width, height, 1));
    }
    
	void Projector::setView(const glm::mat4& viewMatrix) {
		auto viewInverse = glm::inverse(viewMatrix);
		{
			glm::vec3 scale;
			glm::quat orientation;
			glm::vec3 translation, skew;
			glm::vec4 perspective;

			glm::decompose(viewInverse
				, scale
				, orientation
				, translation
				, skew
				, perspective);

			this->setOrientation(orientation);
			this->setPosition(translation);
		}
	}

	void Projector::setProjection(float throwRatio, const glm::vec2& lensOffset) {
		
        
        this->throwRatio = throwRatio;
        this->lensOffset = lensOffset;

		//throwRatio, aspectRatio
		const float aspectRatio = (float)width / (float)height;
		const float fovy = 2.0f * atan(0.5f / (throwRatio * aspectRatio));
		
		
		glm::mat4 projection = glm::perspective(fovy, aspectRatio, 0.1f, 50.0f);
		projection = glm::translate(glm::mat4(), {-lensOffset.x* 2.0f, -lensOffset.y * 2.0f, 0.f}) * projection;
		
		this->projection = projection;
	}

	void Projector::setProjection(const glm::mat4& projection) {
		this->throwRatio = projection[0][0] / 2.0f;
		this->lensOffset = - glm::vec2(projection[2][0], projection[2][1]) / 2.0f;
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
    
    glm::vec2 Projector::getLensOffset() const {
        return lensOffset;
    }


	bool Projector::isProjectionMatrixInfinite() const {
		return this->projection[3][2] == 0.0f;
	}

	glm::mat4 Projector::getViewMatrix() const {
		return glm::inverse(this->getGlobalTransformMatrix());
	}

	glm::mat4 Projector::getProjectionMatrix() const {
		return this->projection;
	}

	glm::mat4 Projector::getClippedProjectionMatrix() const {
		if ( this->isProjectionMatrixInfinite() ) {
			const float n(this->nearClip);
			const float f(this->farClip);
			glm::mat4 projection = this->getProjectionMatrix();
			projection[2][2] *= (f + n) / (f - n);
			projection[3][2] = - (f * n / (f - n));
			return projection;
		} else 
			return this->getProjectionMatrix();
	}

	void Projector::drawOnNearPlane(ofBaseHasTexture & image, bool nearPlaneFlipped) const {
		this->drawOnNearPlane(image.getTexture());
	}

	void Projector::drawOnNearPlane(ofTexture & texture, bool nearPlaneFlipped) const {
		ofMesh plane;

		glm::mat4 inversed = glm::inverse(this->getClippedProjectionMatrix() * this->getViewMatrix());
		
		float z = nearPlaneFlipped ? 1.0f : -1.0f;
		plane.addVertex(glm::vec3(-1.0f, +1.0f, z));
		plane.addTexCoord(glm::vec2(0, 0));
		plane.addVertex(glm::vec3(+1.0f, +1.0f, z));
		plane.addTexCoord(glm::vec2(this->getWidth(), 0));
		plane.addVertex(glm::vec3(-1.0f, -1.0f, z));
		plane.addTexCoord(glm::vec2(0, this->getHeight()));
		plane.addVertex(glm::vec3(+1.0f, -1.0f, z));
		plane.addTexCoord(glm::vec2(this->getWidth(), this->getHeight()));

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
	
		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(-1, -1, -1));
		vertices.push_back(glm::vec3(-1, -1, +1));
		vertices.push_back(glm::vec3(-1, +1, +1));
		vertices.push_back(glm::vec3(+1, +1, +1));
		vertices.push_back(glm::vec3(+1, -1, +1));
		vertices.push_back(glm::vec3(+1, -1, -1));
		vertices.push_back(glm::vec3(+1, +1, -1));
		vertices.push_back(glm::vec3(-1, +1, -1));
	
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

	glm::vec2 Projector::getCoordinateFromIndex(const uint32_t pixelIndex) const {
		uint32_t x = pixelIndex % width;
		uint32_t y = pixelIndex / width;
		return getCoordinateFromIndex(x, y);
	}

	glm::vec2 Projector::getCoordinateFromIndex(const uint32_t x, const uint32_t y) const {
		return glm::vec2(2.0f * (float(x) + 0.5) / float(width) - 1.0f,
				1.0f - 2.0f * (float(y) + 0.5) / float(height));
	}
	
	glm::vec2 Projector::getIndexFromCoordinate(const glm::vec2& coord) const {
		glm::vec2 result = coord;
		result.y *= -1.0f;
		result += 1.0f;
		result *= glm::vec2(this->width, this->height) / 2.0f;
		return result;
	}

	void Projector::setNearClip(float nearClip) {
		this->nearClip = nearClip;
	}

	void Projector::setFarClip(float farClip) {
		this->farClip = farClip;
	}
}
