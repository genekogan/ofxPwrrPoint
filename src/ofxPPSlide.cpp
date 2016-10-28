#include "ofxPPSlide.h"


ofxPPSlide::ofxPPSlide(string name) {
    this->name = name;
    setContentRectangle(0, 0, ofGetWidth(), ofGetHeight());
    index = 0;
    exported = false;
}

void ofxPPSlide::setContentRectangle(int x, int y, int w, int h) {
    content.set(x, y, w, h);
    resize(content);
}

ofxPPRect * ofxPPSlide::addRect(ofColor color, float x, float y, float w, float h) {
    ofxPPRect *rect = new ofxPPRect(this, color, x, y, w, h);
    elements.push_back(rect);
    return rect;
}

ofxPPImage * ofxPPSlide::addImage(string name_, string path, float x, float y, float w, float h) {
    ofxPPImage *img = new ofxPPImage(this, name_, path, x, y, w, h);
    if (exported) {
        img->loadFromExported();
    }
    elements.push_back(img);
    return img;
}

ofxPPScrollableImage * ofxPPSlide::addScrollableImage(string name_, string path, float x, float y, float w, float h) {
    ofxPPScrollableImage *img = new ofxPPScrollableImage(this, name_, path, x, y, w, h);
    if (exported) {
        img->loadFromExported();
    }
    elements.push_back(img);
    return img;
}

ofxPPMovie * ofxPPSlide::addMovie(string name_, string path, bool autoPlay, float x, float y, float w, float h) {
    ofxPPMovie *mov = new ofxPPMovie(this, name_, path, autoPlay, x, y, w, h);
    if (exported) {
        mov->loadFromExported();
    }
    elements.push_back(mov);
    return mov;
}

ofxPPText * ofxPPSlide::addText(ofTrueTypeFont & font, ofColor textColor, string text, float x, float y, float w) {
    ofxPPText *txt = new ofxPPText(this, font, textColor, text, x, y, w);
    if (exported) {
        txt->loadFromExported();
    }
    elements.push_back(txt);
    return txt;
}

void ofxPPSlide::addAction(ofxPPElement *element) {
    if (exported) {
        element->loadFromExported();
    }
    elements.push_back(element);
}

void ofxPPSlide::addBreakpoint() {
    breakpoints.push_back(elements.size());
}

int ofxPPSlide::getNextBreakpointSlide() {
    int end = elements.size();
    if (breakpoints.size() > 0 && index < breakpoints.size()) {
        end = breakpoints[index];
    }
    return end;
}

bool ofxPPSlide::mouseDragged(int mouseX, int mouseY) {
    int end = getNextBreakpointSlide();
    bool mouse = false;
    for (int i=0; i<end; i++) {
        if (elements[i]->mouseDragged(mouseX, mouseY)) {
            mouse = true;
        }
    }
    return mouse;
}

bool ofxPPSlide::mouseScrolled(float scrollX, float scrollY) {
    int end = getNextBreakpointSlide();
    bool mouse = false;
    for (int i=0; i<end; i++) {
        if (elements[i]->mouseScrolled(scrollX, scrollY)) {
            mouse = true;
        }
    }
    return mouse;
}

void ofxPPSlide::keyPressed(int key) {
    int end = getNextBreakpointSlide();
    bool mouse = false;
    for (int i=0; i<elements.size(); i++) {
        elements[i]->keyPressed(key);
    }
}

bool ofxPPSlide::mouseMoved(int mouseX, int mouseY) {
    int end = getNextBreakpointSlide();
    bool mouse = false;
    for (int i=0; i<elements.size(); i++) {
        elements[i]->mouseMoved(mouseX, mouseY);
    }
}

bool ofxPPSlide::mousePressed(int mouseX, int mouseY) {
    int end = getNextBreakpointSlide();
    bool mouse = false;
    for (int i=0; i<end; i++) {
        if (elements[i]->mousePressed(mouseX, mouseY)) {
            mouse = true;
        }
    }
}

bool ofxPPSlide::mouseReleased(int mouseX, int mouseY) {
    int end = getNextBreakpointSlide();
    bool mouse = false;
    for (int i=0; i<end; i++) {
        if (elements[i]->mouseReleased(mouseX, mouseY)) {
            mouse = true;
        }
    }
    return mouse;
}

void ofxPPSlide::loadAssets() {
    for (int i=0; i<elements.size(); i++) {
        //elements[i]->loadAssets();
    }
}

void ofxPPSlide::update() {
    int end = getNextBreakpointSlide();
    for (int i=0; i<end; i++) {
        elements[i]->update();
    }
}

void ofxPPSlide::draw() {
    ofxPPSlide *thisSlide = this;
    ofNotifyEvent(drawE, thisSlide, this);
    int end = getNextBreakpointSlide();
    for (int i=0; i<end; i++) {
        elements[i]->draw();
    }
}

void ofxPPSlide::start() {
    int end = getNextBreakpointSlide();
    for (int i=0; i<end; i++) {
        elements[i]->start();
    }
}

void ofxPPSlide::stop() {
    int end = getNextBreakpointSlide();
    for (int i=0; i<end; i++) {
        elements[i]->stop();
    }
}

void ofxPPSlide::prev() {
    if (breakpoints.size() > 0) {
        index = (index + breakpoints.size()) % (breakpoints.size()+1);
    }
}

void ofxPPSlide::next() {
    if (breakpoints.size() > 0) {
        if (index < breakpoints.size()) {
            int i1 = breakpoints[index];
            int i2 = index+1 == breakpoints.size() ? elements.size() : breakpoints[index+1];
            for (int i=i1; i<i2; i++) {
                elements[i]->start();
            }
        }
        index = (index + 1) % (breakpoints.size()+1);
    }
}

void ofxPPSlide::resize(ofRectangle content) {
    this->content = content;
    for (int i=0; i<elements.size(); i++) {
        elements[i]->resize(content);
    }
}

void ofxPPSlide::exportAssets() {
    for (auto e : elements) {
        e->exportAssets();
    }
}

void ofxPPSlide::exportVideosAndScrollableImages(string prefix) {
    for (auto e : elements) {
        if (e->isVideo()) {
            e->exportAssets(prefix);
        }
        if (e->isScrollableImage()) {
            e->exportAssets(prefix);
        }
    }
}