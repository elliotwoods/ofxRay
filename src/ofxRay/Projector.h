#pragma once
//
//  Projector.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "Ray.h"
#include "Base.h"

namespace ofxRay {
	class Projector;
}
ostream& operator<<(ostream &, const ofxRay::Projector &);
istream& operator>>(istream & is, ofxRay::Projector &);

namespace ofxRay {
	typedef std::function<vector<glm::vec2>(const vector<glm::vec2>&)> UndistortFunction;

    class Plane;
    
	///A class to generate rays given parameters for a projector or camera
	///NB: ofProjector inherits from ofNode, and therefore ofNode stores its view matrix
	class Projector : public Base, public ofNode {
	public:
		Projector(int width=1024, int height=768);
		Projector(float throwRatio, const glm::vec2& lensOffset, int width, int height);
		Projector(const glm::mat4 & projection, int width, int height);
		Projector(const glm::mat4 & view, const glm::mat4 & projection, int width, int height);

		//--
		//ofGeometric
		//--
		//
		void draw() const;
		void randomiseVectors(float amplitude=1.0f);
		//
		//--
		
		///Choose a random pose, for the projector with scale factor
		void randomisePose(float scale=1.0f);
	
		glm::vec2 distortedPixelToCoordinate(const glm::vec2& xy) const;
		glm::vec2 undistortedPixelToCoordinate(const glm::vec2 & xy) const;
		
		///Generate a ray for the given pixel coordinates x,y within the projector's image
		Ray castPixel(int x, int y, bool isDistorted) const;
		Ray castPixel(const glm::vec2& xy, bool isDistorted) const;
		void castPixels(const vector<glm::vec2>& xy, vector<Ray>& rays, bool isDistorted) const;

		///Generate a ray for the given normalised coordinate x,y where {-1.0f<x,y<1.0f}
		Ray castCoordinate(const glm::vec2& xy) const;
		void castCoordinates(const vector<glm::vec2>& xy, vector<Ray>& rays) const;

        // Generate a ray for the projection center
        Ray getProjectionCenterRay() const;
        
        // Generate a ray for the projector itself (differs from the above if we have lens offset)
        Ray getProjectorRay(float distance) const;
        
        // returns a plane, perpendicular to the projector ray, at the specified distance
        Plane getProjectionPlaneAt(float distance, bool infinite = false) const;
        
        // convert world coordinates, to signed normalized (-1...1) coordinates
        glm::vec3 getNormalizedSCoordinateOfWorldPosition(glm::vec3 pointWorld) const;
        
        // convert world coordinates, to unsigned normalized (0...1) coordinates
        glm::vec3 getNormalizedUCoordinateOfWorldPosition(glm::vec3 pointWorld) const;

        // convert world coordinates to screen pixel coordinates
        glm::vec3 getScreenCoordinateOfWorldPosition(glm::vec3 pointWorld) const;
        
        // convert signed normalized (-1...1) coordiantes to world coordinates (pointNorm.z contains distance to projector plane)
        glm::vec3 getWorldPositionOfNormalizedSCoordinate(glm::vec3 pointNormS) const;
        
        // convert unsigned normalized (0...1) coordiantes to world coordinates (pointNorm.z contains distance to projector plane)
        glm::vec3 getWorldPositionOfNormalizedUCoordinate(glm::vec3 pointNormU) const;
        
        // convert screen pixel coordinates to world coordinates (pointScreen.z contains distance to projector plane)
        glm::vec3 getWorldPositionOfScreenCoordinate(glm::vec3 pointScreen) const;
        
        
//		void setView(const glm::mat4& m) {
//			ofNode::setTransformMatrix(m.getInverse());
//		}
		void setView(const glm::mat4&);
		void setProjection(float throwRatio, const glm::vec2& lensOffset);
		void setProjection(const glm::mat4& projection);

		///Set width of projector. Warning: we will forget our throwRatio and lensOffset
		void setWidth(int width);
		///Set height of projector. Warning: we will forget our throwRatio and lensOffset
		void setHeight(int height);
		int getWidth() const;
		int getHeight() const;

        float getThrowRatio() const;
        glm::vec2 getLensOffset() const;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

		bool isProjectionMatrixInfinite() const;
		glm::mat4 getClippedProjectionMatrix() const; ///<If projection matrix is infinite, we enforce a (0.01...10.0) clipping
		
		void drawOnNearPlane(ofBaseHasTexture &, bool nearPlaneFlipped = false) const;
		void drawOnNearPlane(ofTexture &, bool nearPlaneFlipped = false) const;
		
		void beginAsCamera(bool flipY = false) const;
		void endAsCamera() const;
		
		glm::vec2 getCoordinateFromIndex(const uint32_t index) const;
		glm::vec2 getCoordinateFromIndex(const uint32_t x, const uint32_t y) const;

		glm::vec2 getIndexFromCoordinate(const glm::vec2&) const;
		
		void setNearClip(float near = 0.5);
		void setFarClip(float far = 20.0f);

		void setUndistortFunction(const UndistortFunction &);

		friend ostream & (::operator<<) (ostream &, const Projector &);
		friend istream & (::operator>>) (istream &, Projector &);
	protected:
		int width;
		int height;
        
		glm::mat4 projection;
		static ofMesh* drawBox;

		float nearClip = 0.5f;
		float farClip = 20.0f;

		UndistortFunction undistortFunction;

	private:
		static void makeBox();

        float throwRatio;
        glm::vec2 lensOffset;
	};
}

