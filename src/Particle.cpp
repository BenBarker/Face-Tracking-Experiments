// Simple Particle System
// Based on code by
// Daniel Shiffman <http://www.shiffman.net>

// A simple Particle class

#include "Particle.h"
#include "ofVec2f.h"
//--------------------------------------------------------------
// One constructor

#define PARTICLE_AGE 30.0
Particle::Particle(ofVec2f a, ofVec2f v, ofVec2f l, float r_,ofColor col){
	acc = a;
	vel = v;
	loc = l;
	r = r_;
    color = col;
	timer = PARTICLE_AGE;
}

//--------------------------------------------------------------
// Another constructor (the one we are using here)
Particle::Particle(ofVec2f l) {
	acc.set(0,0.10);
	vel.set(ofRandom(-1,1),ofRandom(-2,0));
	loc = l;
    color = ofColor(255,255,255,255);
	r = 5.0;
	timer = PARTICLE_AGE;
}

//--------------------------------------------------------------
// Method to update location
void Particle::update() {            
	vel += acc;
	loc += vel;
	timer -= 1.0;
    color.a *= 0.95;
}

//--------------------------------------------------------------
// Method to display
void Particle::render() {
	ofSetColor(color);
	ofFill();
    ofSetCircleResolution(6);
	ofCircle(loc.x,loc.y,r);
}

//--------------------------------------------------------------
// Is the particle still useful?
bool Particle::dead() {
	if (timer <= 0.0) {
		return 1;
	} else {
		return 0;
	}
}



