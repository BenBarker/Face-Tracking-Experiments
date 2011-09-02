//Created from the HaarFinder example that comes with openFrameworks
//http://www.openframeworks.cc

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//img.loadImage("test.jpg");
	
    //Setup haar finder
    finder.setup("haarcascade_frontalface_default.xml");
    finder.setScaleHaar(1.1); //Close to but above 1.0 for accuracy, higher for less accuracy.
    
    //Initialize key states
    keyIsPressed = 0;
    keyPressed_Space = false;
    keyPressed_A = false;
    keyPressed_D = false;
    keyPressed_S = false;
    keyPressed_F = false;
    
    //Particle init
    elapsedTime = 0;
    spawnRate    = 5;
    ofVec2f ps_loc(ofGetWidth()/2,ofGetHeight()/2);
	ps.init(0, ps_loc);
    previousCentroid.set(ps_loc); 
    
    
    //Camera and texture init
	camWidth 		= 640;	// try to grab at this size. 
	camHeight 		= 480;

	vidGrabber.setVerbose(false);
	vidGrabber.initGrabber(camWidth,camHeight);
	videoTexture.allocate(camWidth,camHeight, GL_RGB);	
    camImg.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
    debugImg.allocate(camWidth,camHeight,OF_IMAGE_COLOR);
    texScreen.allocate(camWidth,camHeight,GL_RGB);
	
}

//--------------------------------------------------------------
void testApp::update(){
	vidGrabber.grabFrame();	
	if (vidGrabber.isFrameNew()){
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
		camImg.setFromPixels(pixels,camWidth,camHeight,OF_IMAGE_COLOR,true);
        if (((ofGetElapsedTimeMillis() - elapsedTime) > 10) && (keyPressed_Space || keyPressed_D || keyPressed_S || keyPressed_F)){
            finder.findHaarObjects(camImg);
            elapsedTime = ofGetElapsedTimeMillis();
        }
        
        //Particle system
        ps.run();
        
        if (keyPressed_Space){
            for(int i = 0; i < finder.blobs.size(); i++) {
                ofRectangle cur = finder.blobs[i].boundingRect;
                //ofRect(cur.x, cur.y, cur.width, cur.height);
                ofVec2f currentCentroid(finder.blobs[i].centroid.x,finder.blobs[i].centroid.y);
                ofVec2f faceVelocity(previousCentroid.x - currentCentroid.x,previousCentroid.y - currentCentroid.y);
                
                //If you shake your head the particles go away
                if (currentCentroid.distance(previousCentroid) > 5){
                    spawnRate *= 0.55;
                }
                else{
                    spawnRate *= ofRandom(1.0,5.0);
                }
                //clamp it to something reasonable
                spawnRate = MIN(MAX(spawnRate,1),100);
                
                for(int j = 0; j < spawnRate; j++){
                    ofVec2f accel;
                    ofVec2f velocity;
                    ofVec2f location;
                    float radius = ofRandom(3,8);
                    velocity.set(0,0);
                    
                    //Make the particles dynamic in this case
                    if (keyPressed_A){
                        accel.set(0,0.5);
                        velocity.set(0,-1);
                        velocity -= (faceVelocity*0.8);
                    }
                    else{
                        accel.set(0.0,0.0);
                    }

                    location.set(int(currentCentroid.x + ofRandom(-cur.width,cur.width)*0.5),int(currentCentroid.y + ofRandom(-cur.height,cur.height)*0.5));
                    int colorIndex = (location.y * camWidth + location.x-1)*3;
                    int distanceAlphaValue = ofClamp((255 - (location.distance(currentCentroid) / (cur.width*0.5)) * 255),0,255);
                    ofColor particleColor(pixels[colorIndex],pixels[colorIndex+1],pixels[colorIndex+2],distanceAlphaValue);
                    //ofColor particleColor(ofRandom(128,255),0,0);
                    Particle newParticle(accel,velocity,location,radius,particleColor);
                    ps.addParticle(newParticle);
                }
                previousCentroid.set(finder.blobs[i].centroid.x,finder.blobs[i].centroid.y);
            }
        }
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0,0,0);
    //Draw camera image (background)
    ofSetColor(255,255,255,255);
    camImg.draw(0, 0);
	ofNoFill();
    ofDisableBlendMode();
    
    
    //Draw bounding box
    if(keyPressed_D){
        finder.draw(0,0);
    }
    
    //Draw face region in corner
    if (keyPressed_S){
        for(int i=0;i<finder.blobs.size();i++){
            ofRectangle cur = finder.blobs[i].boundingRect;
            texScreen.loadScreenData(cur.x,cur.y,cur.width,cur.height);
            texScreen.draw(0,0);
        }
    }
    
    //Rays effect
    float scaleFactor = 1.02; //How much it scales per iteration
    float scaleAmount = 1.00;
    float invScaleAmount = 1.0 - scaleAmount;
    int alphaAmount = 255;
    float scaleIters = 30;
    if (keyPressed_F){
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        for (int scaleI = 1; scaleI <= scaleIters;scaleI++){ //these loops should be reversed, only need one texture lookup
            scaleAmount *= scaleFactor;
            invScaleAmount = 1.0 - scaleAmount;
            alphaAmount = 32 - ((float(scaleI)/float(scaleIters)) * 32);
            ofSetColor(255,255,255,alphaAmount);
             for(int i = 0; i < finder.blobs.size(); i++) {
                ofRectangle cur = finder.blobs[i].boundingRect;
                //ofRect(cur.x, cur.y, cur.width, cur.height);
                texScreen.loadScreenData(cur.x,cur.y,cur.width,cur.height);
                 texScreen.draw(MAX(0,cur.x + (cur.width*invScaleAmount)*0.5), \
                                MAX(0,cur.y + (cur.height*invScaleAmount*0.4)), //Shift Y up so the beams go up.
                                cur.width*scaleAmount, \
                                cur.height*scaleAmount);
             }
        }
    }
    

    
    //Draw particles
    if (keyPressed_Space){
        ps.render();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    printf("key: %i\n",key);
    switch(key){
    //if (key == 32){
        case ' ':
            keyPressed_Space = !keyPressed_Space;
            break;
        case 'a':
            keyPressed_A = !keyPressed_A;
            break;
        case 'd':
            keyPressed_D = !keyPressed_D;
            break;
        case 'f':
            keyPressed_F = !keyPressed_F;
            break;
        case 's':
            keyPressed_S = !keyPressed_S;
            break;

        
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}