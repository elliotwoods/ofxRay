#pragma once
//
//  Base.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofMain.h"

#if OF_VERSION_MINOR < 9	
	#define ofDrawLine ofLine
	#define OFXRAY_GET_TEXTURE getTextureReference()
#else
	#define OFXRAY_GET_TEXTURE getTexture()
#endif

namespace ofxRay {
	class Base;
}
ostream& operator<<(ostream & os, const ofxRay::Base &);
istream& operator>>(istream & is, ofxRay::Base &);

namespace ofxRay {
	class Base {
	public:
		Base();
		Base(ofColor color);
	
		void randomiseColor();
		void randomise(float amplitude=1.0f);
	
		virtual void draw() const = 0;	
		virtual void randomiseVectors(float amplitude=1.0f) = 0;

		friend ostream & (::operator<<) (ostream &, const Base &);
		friend istream & (::operator>>) (istream &, Base &);

		ofColor color;
	};
}