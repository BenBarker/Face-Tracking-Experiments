#pragma once

#include "ofMain.h"
#include "ofVec2f.h"
#include "ofxCvHaarFinder.h"
#include "ParticleSystem.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

        //Image and grabber variables
		ofImage             camImg;
        unsigned char * 	testPatternPixels;
        ofImage             debugImg;
        ofTexture           texScreen;
        ofVideoGrabber      vidGrabber;
        ofTexture			videoTexture;
        ofxCvHaarFinder     finder;
    
        //Particle related variables
        ParticleSystem      ps;
        ofVec2f             previousCentroid;
        int                 spawnRate;
        int 				camWidth;
        int 				camHeight;
        char                eventString[255];
        int                 keyIsPressed;
        int                 elapsedTime;
    
        //Event related variables
        bool                keyPressed_Space;
        bool                keyPressed_D;
        bool                keyPressed_S;
        bool                keyPressed_F;
        bool                keyPressed_A;
};
