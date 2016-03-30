#pragma once

#include "ofMain.h"
#include "ofxPwrrPoint.h"


class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    void drawBg(ofxPPSlide * & slide) {
        ofBackground(255);
        ofSetColor(0, 255, 0);
        ofFill();
        ofDrawRectangle(0, 0, ofGetWidth(), 56);
        ofSetColor(255);
        font.drawString(slide->getName(), 10, 42);
    }
    
    ofxPwrrPoint slideshow;
    ofTrueTypeFont font;
};
