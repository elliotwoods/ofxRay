#pragma once
//
//  Projector.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxRay/Ray.h"
#include "ofxRay/Base.h"

namespace ofxRay {
	///A class to generate rays given parameters for a projector or camera
	///NB: ofProjector inherits from ofNode, and therefore ofNode stores its view matrix
	class Projector : public Base, public ofNode {
	public:
		Projector(int width=1024, int height=768);
		Projector(float throwRatio, const ofVec2f& lensOffset, int width, int height);
		Projector(const ofMatrix4x4 & projection, int width, int height);
		Projector(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height);

		////
		//ofGeometric
		////
		//
		void draw() const;
		void randomiseVectors(float amplitude=1.0f);
		////
		
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

		void setProjection(float throwRatio, const ofVec2f& lensOffset);
		void setProjection(const ofMatrix4x4& projection);

		///Set width of projector. Warning: we will forget our throwRatio and lensOffset
		void setWidth(int width);
		///Set height of projector. Warning: we will forget our throwRatio and lensOffset
		void setHeight(int height);
		int getWidth() const;
		int getHeight() const;

		ofMatrix4x4 getViewMatrix() const;
		ofMatrix4x4 getProjectionMatrix() const;

		bool isProjectionMatrixInfinite() const;
		ofMatrix4x4 getClippedProjectionMatrix() const; ///<If projection matrix is infinite, we enforce a (0.01...10.0) clipping
		
		void drawOnNearPlane(ofBaseHasTexture & image) const;
		
		ofVec2f getNormFromIndex(const uint index);

	protected:
		int width;
		int height;
		ofMatrix4x4 projection;
		static ofMesh* drawBox;

	private:
		static void makeBox();
	};
}
