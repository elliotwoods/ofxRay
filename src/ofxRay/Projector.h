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
    
    class Plane;
    
	///A class to generate rays given parameters for a projector or camera
	///NB: ofProjector inherits from ofNode, and therefore ofNode stores its view matrix
	class Projector : public Base, public ofNode {
	public:
		Projector(int width=1024, int height=768);
		Projector(float throwRatio, const ofVec2f& lensOffset, int width, int height);
		Projector(const ofMatrix4x4 & projection, int width, int height);
		Projector(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height);

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
	
		///Undistort the given coordinate (pass through for Projector)
		virtual ofVec2f undistortCoordinate(const ofVec2f & xy) const { return xy;}
		
		///Generate a ray for the given pixel coordinates x,y within the projector's image
		Ray castPixel(int x, int y) const;
		Ray castPixel(const ofVec2f& xy) const;
		void castPixels(const vector<ofVec2f>& xy, vector<Ray>& rays) const;

		///Generate a ray for the given normalised coordinate x,y where {-1.0f<x,y<1.0f}
		Ray castCoordinate(const ofVec2f& xy) const;
		void castCoordinates(const vector<ofVec2f>& xy, vector<Ray>& rays) const;

        // Generate a ray for the projection center
        Ray getProjectionCenterRay() const;
        
        // Generate a ray for the projector itself (differs from the above if we have lens offset)
        Ray getProjectorRay(float distance) const;
        
        // returns a plane, perpendicular to the projector ray, at the specified distance
        Plane getProjectionPlaneAt(float distance, bool infinite = false) const;
        
        // convert world coordinates, to signed normalized (-1...1) coordinates
        ofVec3f getNormalizedSCoordinateOfWorldPosition(ofVec3f pointWorld) const;
        
        // convert world coordinates, to unsigned normalized (0...1) coordinates
        ofVec3f getNormalizedUCoordinateOfWorldPosition(ofVec3f pointWorld) const;

        // convert world coordinates to screen pixel coordinates
        ofVec3f getScreenCoordinateOfWorldPosition(ofVec3f pointWorld) const;
        
        // convert signed normalized (-1...1) coordiantes to world coordinates (pointNorm.z contains distance to projector plane)
        ofVec3f getWorldPositionOfNormalizedSCoordinate(ofVec3f pointNormS) const;
        
        // convert unsigned normalized (0...1) coordiantes to world coordinates (pointNorm.z contains distance to projector plane)
        ofVec3f getWorldPositionOfNormalizedUCoordinate(ofVec3f pointNormU) const;
        
        // convert screen pixel coordinates to world coordinates (pointScreen.z contains distance to projector plane)
        ofVec3f getWorldPositionOfScreenCoordinate(ofVec3f pointScreen) const;
        
        
		void setView(const ofMatrix4x4& m) {
			ofNode::setTransformMatrix(m.getInverse());
		}

		void setProjection(float throwRatio, const ofVec2f& lensOffset);
		void setProjection(const ofMatrix4x4& projection);

		///Set width of projector. Warning: we will forget our throwRatio and lensOffset
		void setWidth(int width);
		///Set height of projector. Warning: we will forget our throwRatio and lensOffset
		void setHeight(int height);
		int getWidth() const;
		int getHeight() const;

        float getThrowRatio() const;
        ofVec2f getLensOffset() const;

		ofMatrix4x4 getViewMatrix() const;
		ofMatrix4x4 getProjectionMatrix() const;

		bool isProjectionMatrixInfinite() const;
		ofMatrix4x4 getClippedProjectionMatrix() const; ///<If projection matrix is infinite, we enforce a (0.01...10.0) clipping
		
		void drawOnNearPlane(ofBaseHasTexture & image, bool nearPlaneFlipped = false) const;
		
		void beginAsCamera() const;
		void endAsCamera() const;
		
		ofVec2f getCoordinateFromIndex(const uint32_t index) const;
		ofVec2f getCoordinateFromIndex(const uint32_t x, const uint32_t y) const;

		ofVec2f getIndexFromCoordinate(const ofVec2f&) const;
		
		static void setDefaultNear(float defaultNear = 0.5);
		static void setDefaultFar(float defaultFar = 20.0f);
	protected:
		int width;
		int height;
        
		ofMatrix4x4 projection;
		static ofMesh* drawBox;

		static float defaultNear;
		static float defaultFar;
	private:
		static void makeBox();

        float throwRatio;
        ofVec2f lensOffset;

	};
}
