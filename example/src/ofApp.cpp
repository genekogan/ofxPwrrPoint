#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    font.load("AndaleMono.ttf", 32);
    
    // assign ofApp::drawBg to be run before the rest of the slides (good for making a wallpaper/banner)
    slideshow.setBackgroundFunction(this, &ofApp::drawBg);
    
    // create a slide
    ofxPPSlide *s1 = slideshow.addSlide("Hello world (press UP/DOWN keys)");
    
    // numbers are (x, y, w, h) where each is normalized screen (between 0 and 1).
    // so 0, 0, 0.5, 1.0 allocates the left half of the slide to the image
    s1->addImage("my image", "interference14.png", 0.0, 0.0, 0.5, 1.0);
    
    // right half of the slide. the bool is whether to play automatically
    s1->addMovie("my movie", "fingers.mov", true, 0.5, 0.0, 0.5, 1.0);
    
    // text
    s1->addText(font, ofColor(0, 0, 150), "lorem ipsum", 0.0, 0.1, 0.5); // no height for text, just keeps wrapping
    
    // another slide
    ofxPPSlide *s2 = slideshow.addSlide("mouse = scroll, drag = zoom");
    s2->addScrollableImage("big image", "grid_flowers2.jpg", 0.1, 0.1, 0.8, 0.8);

    // first assets dont load until you set up the page (to make loadtime faster)
    slideshow.setPage(0);
    
    // press up and down to go through slides
}

//--------------------------------------------------------------
void ofApp::update(){
    slideshow.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    slideshow.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
