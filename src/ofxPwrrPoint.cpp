#include "ofxPwrrPoint.h"


ofxPwrrPoint::ofxPwrrPoint() {
    index = 0;
    setActive(true);
    exported = false;
}

ofxPwrrPoint::~ofxPwrrPoint() {
    setActive(false);
}

void ofxPwrrPoint::setActive(bool active) {
    if (active) {
        ofAddListener(ofEvents().mouseMoved, this, &ofxPwrrPoint::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &ofxPwrrPoint::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &ofxPwrrPoint::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &ofxPwrrPoint::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &ofxPwrrPoint::keyPressed);
    }
    else {
        ofRemoveListener(ofEvents().mouseMoved, this, &ofxPwrrPoint::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxPwrrPoint::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxPwrrPoint::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxPwrrPoint::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &ofxPwrrPoint::keyPressed);
    }
}

void ofxPwrrPoint::setContentRectangle(int x, int y, int w, int h) {
    content.set(x, y, w, h);
    for (int i=0; i<slides.size(); i++) {
        slides[i]->setContentRectangle(x, y, w, h);
    }
}

void ofxPwrrPoint::preloadAssets() {
    preloaded = true;
    for (int i=0; i<slides.size(); i++) {
        slides[i]->loadAssets();
    }
}

void ofxPwrrPoint::setPage(int index_) {
    slides[index]->stop();
    this->index = index_;
    slides[index]->start();
}

void ofxPwrrPoint::update() {
    slides[index]->update();
}

void ofxPwrrPoint::draw() {
    ofNotifyEvent(drawE, slides[index], this);
    slides[index]->draw();
}

ofxPPSlide * ofxPwrrPoint::addSlide(string name) {
    ofxPPSlide *newSlide = new ofxPPSlide(name);
    if (exported) {
        newSlide->loadFromExported();
    }
    slides.push_back(newSlide);
    return newSlide;
}

void ofxPwrrPoint::setPage(ofxPPSlide * slide) {
    for (int i=0; i<slides.size(); i++) {
        if (slide == slides[i]) {
            setPage(i);
            return;
        }
    }
}

void ofxPwrrPoint::prevSegment() {
    if (slides[index]->atBeginning()) {
        setPage((index - 1 + slides.size()) % slides.size());
    }
    else {
        slides[index]->prev();
    }
}

void ofxPwrrPoint::nextSegment() {
    if (slides[index]->atEnd()) {
        setPage((index + 1) % slides.size());
    }
    else {
        slides[index]->next();
    }
}

bool ofxPwrrPoint::mouseMoved(int mouseX, int mouseY) {
    slides[index]->mouseMoved(mouseX, mouseY);
}

bool ofxPwrrPoint::mouseReleased(int mouseX, int mouseY) {
    slides[index]->mouseReleased(mouseX, mouseY);
}

bool ofxPwrrPoint::mouseDragged(int mouseX, int mouseY) {
    slides[index]->mouseDragged(mouseX, mouseY);
}

bool ofxPwrrPoint::mousePressed(int mouseX, int mouseY) {
    slides[index]->mousePressed(mouseX, mouseY);
}

bool ofxPwrrPoint::keyPressed(int key) {
    if (key == OF_KEY_LEFT) {
        prevSegment();
    }
    else if (key == OF_KEY_RIGHT) {
        nextSegment();
    }
    else if (key == OF_KEY_UP) {
        setPage((index - 1 + slides.size()) % slides.size());
    }
    else if (key == OF_KEY_DOWN) {
        setPage((index + 1) % slides.size());
    }
    else {
        slides[index]->keyPressed(key);
    }
}

void ofxPwrrPoint::mouseMoved(ofMouseEventArgs &evt){
    mouseMoved(evt.x, evt.y);
}

void ofxPwrrPoint::mousePressed(ofMouseEventArgs &evt){
    mousePressed(evt.x, evt.y);
}

void ofxPwrrPoint::mouseDragged(ofMouseEventArgs &evt){
    mouseDragged(evt.x, evt.y);
}

void ofxPwrrPoint::mouseReleased(ofMouseEventArgs &evt){
    mouseReleased(evt.x, evt.y);
}

void ofxPwrrPoint::keyPressed(ofKeyEventArgs &evt){
    keyPressed(evt.key);
}

void ofxPwrrPoint::windowResized() {
    for (int i=0; i<slides.size(); i++) {
        slides[i]->resize(content);
    }
}

void ofxPwrrPoint::printStats() {
    int s = 0;
    int s2 = 0;
    for (int i=0; i<slides.size(); i++) {
        s += 1;
        s2 += (1 + slides[i]->getBreakpoints().size());
    }
    cout << "slides " << s << " " << s2 << endl;
}
