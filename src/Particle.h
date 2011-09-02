// Simple Particle System
// Based on code by
// Daniel Shiffman <http://www.shiffman.net>

// A simple Particle class

#ifndef _PARTICLE
#define _PARTICLE

#include "ofMain.h"
#define OF_ADDON_USING_OFXVECTORMATH
//#include "ofAddons.h"
#include "ofVec2f.h"


class Particle {

	public:
	
		ofVec2f loc;
		ofVec2f vel;
		ofVec2f acc;
        ofColor color;
		float r;
		float timer;

		// One constructor
		Particle(ofVec2f a, ofVec2f v, ofVec2f l, float r_,ofColor col);

		// Another constructor (the one we are using here)
		Particle(ofVec2f l);

		// Method to update location
		void update();

		// Method to display
		void render();

		// Is the particle still useful?
		bool dead();
};
#endif


