#include "ofxPwrrPoint.h"


ofxPwrrPoint::ofxPwrrPoint() {
    index = 0;
    //setActive(true);
    exported = false;
    toExportSlides = false;
    guiWindowWidth = 1024;
    guiWindowHeight = 800;
    notesFontSize = 20;
    notesFont.load("/Users/gene/Code/of_v0.9.0_osx_release/addons/ofxLibwebsockets/example_server_binary/bin/data/myriad.ttf", notesFontSize);
    
    // mouse scroll can be done separately
    ofAddListener(ofEvents().mouseScrolled, this, &ofxPwrrPoint::mouseScrolled);
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
    if (ofGetFrameNum() == 1 && toExportSlides) {
        exportScreenshotsSave();
        toExportSlides = false;
    }
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

bool ofxPwrrPoint::mouseScrolled(float scrollX, float scrollY) {
    slides[index]->mouseScrolled(scrollX, scrollY);
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

void ofxPwrrPoint::mouseScrolled(ofMouseEventArgs &evt) {
    mouseScrolled(evt.scrollX, evt.scrollY);
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
    ofLog(OF_LOG_NOTICE, "slides "+ofToString(s)+", breakpoints "+ofToString(s2));
}

void ofxPwrrPoint::exportAssets() {
    for (auto s : slides) {
        s->exportAssets();
    }
}

void ofxPwrrPoint::exportScreenshots() {
    toExportSlides = true;
}

void ofxPwrrPoint::exportScreenshotsSave() {
    int slide = 0;
    bool finished = false;
    bool started = false;
    while (!finished) {
        ofNotifyEvent(drawE, slides[index], this);
        slides[index]->draw();
        slides[index]->exportVideosAndScrollableImages("export/media/slide"+ofToString(slide+1, 3, '0'));
        ofSaveScreen("export/slide"+ofToString(slide+1, 3, '0')+".png");
        slide++;
        nextSegment();
        if (index == 1) {
            started = true;
        }
        if (started && index == 0) {
            finished = true;
        }
    }
}

void ofxPwrrPoint::setupGui() {
    ofSetWindowShape(guiWindowWidth, guiWindowHeight);
    ofBackground(0);
}

void ofxPwrrPoint::drawGui() {
    notesFont.drawString(notes[index], 10, 30);
}

void ofxPwrrPoint::wrapNotes() {
    notes.clear();
    for (int idx=0; idx<slides.size(); idx++) {
        vector<string> slideNotes = slides[idx]->getNotes();
        string noteString = "";
        for (int i=0; i<slideNotes.size(); i++) {
            noteString += slideNotes[i];
            noteString += " \n\n";
        }
        string typeWrapped = "";
        string tempString = "";
        
        vector <string> words = ofSplitString(noteString, " ");
        for(int i=0; i<words.size(); i++) {
            string wrd = words[i];
            tempString += wrd + " ";
            int stringwidth = notesFont.stringWidth(tempString);
            if(stringwidth >= guiWindowWidth-120) {
                tempString = "";
                typeWrapped += "\n";
            }
            typeWrapped += wrd + " ";
        }
        notes.push_back(typeWrapped);
    }
}
