#include "ofxPPElement.h"


ofxPPElement::ofxPPElement(ofxPPSlide *parent, string name, float x, float y, float w, float h) {
    this->parent = parent;
    this->name = name;
    this->tx = x;
    this->ty = y;
    this->tw = w;
    this->th = h;
    loaded = false;
    box.set(tx * ofGetWidth(), ty * ofGetHeight(), tw * ofGetWidth(), th * ofGetHeight());
    mouseOver = false;
}

void ofxPPElement::resize(ofRectangle content) {
    float x_ = ofMap(tx, 0, 1, content.getX(), content.getX() + content.getWidth());
    float y_ = ofMap(ty, 0, 1, content.getY(), content.getY() + content.getHeight());
    float w_ = ofMap(tw, 0, 1, 0, content.getWidth());
    float h_ = ofMap(th, 0, 1, 0, content.getHeight());
    box.set(x_, y_, w_, h_);
}

void ofxPPElement::draw() {
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(0, 255, 0);
    ofNoFill();
    ofDrawRectangle(box);
    ofPopStyle();
    ofPopMatrix();
}

bool ofxPPElement::mouseMoved(int mouseX, int mouseY) {
    mouseOver = box.inside(mouseX, mouseY);
    return mouseOver;
}

bool ofxPPElement::mouseDragged(int mouseX, int mouseY) {
    return mouseOver;
}

bool ofxPPElement::mousePressed(int mouseX, int mouseY) {
    return mouseOver;
}

bool ofxPPElement::mouseReleased(int mouseX, int mouseY) {
    return mouseOver;
}

//////////////////////////////////////////////////////////////////////

ofxPPImage::ofxPPImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h) : ofxPPElement(parent, name, x, y, w, h) {
    this->path = path;
}

void ofxPPImage::setup() {
    img.load(path);
    
    float iw = img.getWidth();
    float ih = img.getHeight();
    
    float bw = box.getWidth();
    float bh = box.getHeight();
    
    if (iw / ih > bw / bh) {
        float x = 0;
        float w = bw;
        float h = ih * bw / iw;
        float y = 0.5 * (bh - h);
        rect.set(x, y, w, h);
    }
    else {
        float y = 0;
        float h = bh;
        float w = iw * bh / ih;
        float x = 0.5 * (bw - w);
        rect.set(x, y, w, h);
    }
    img.resize(rect.getWidth(), rect.getHeight());
}

void ofxPPImage::start() {
    if (!loaded) {
        setup();
        loaded = true;
    }
}

void ofxPPImage::resize(ofRectangle content) {
    ofxPPElement::resize(content);
    if (loaded) {
        setup();
    }
}

void ofxPPImage::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(rect.getX(), rect.getY());
    img.draw(box.getX(), box.getY());
    ofPopStyle();
    ofPopMatrix();
}

//////////////////////////////////////////////////////////////////////

ofxPPScrollableImage::ofxPPScrollableImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h) : ofxPPElement(parent, name, x, y, w, h) {
    this->path = path;
}

void ofxPPScrollableImage::setup() {
    img.setup(path, box.getWidth(), box.getHeight());
    maxScale = min(img.getImageWidth() / box.getWidth(), img.getImageHeight() / box.getHeight());
    tx = 0;
    ty = 0;
    scale = 0.5*maxScale;
    img.setPosition(tx, ty);
    img.setScale(scale);
}

bool ofxPPScrollableImage::mouseMoved(int mouseX, int mouseY) {
    ofxPPElement::mouseMoved(mouseX, mouseY);
}

bool ofxPPScrollableImage::mouseDragged(int mouseX, int mouseY) {
    if (mouseOver) {
        scale = ofClamp(scale + 0.005 * (mouseY - py), 0.5, maxScale);
    }
}

void ofxPPScrollableImage::start() {
    if (!loaded) {
        setup();
        loaded = true;
    }
}
void ofxPPScrollableImage::resize(ofRectangle content) {
    ofxPPElement::resize(content);
    if (loaded) {
        setup();
    }
}

void ofxPPScrollableImage::update() {
    if (mouseOver) {
        tx = ofMap(ofGetMouseX() - box.getX(), 0, box.getWidth(), 0, 1, true);
        ty = ofMap(ofGetMouseY() - box.getY(), 0, box.getHeight(), 0, 1, true);
        //cout << tx << " " <<ty << endl;
        img.setPosition(tx, ty);
        img.setScale(scale);
    }
    py = ofGetMouseY();
}

void ofxPPScrollableImage::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(box.getX(), box.getY());
    img.draw(0, 0);
    ofPopStyle();
    ofPopMatrix();
}

//////////////////////////////////////////////////////////////////////

ofxPPMovie::ofxPPMovie(ofxPPSlide *parent, string name, string path, bool autoPlay, float x, float y, float w, float h) : ofxPPElement(parent, name, x, y, w, h) {
    this->autoPlay = autoPlay;
    this->path = path;
    movieLoaded = false;
}

void ofxPPMovie::setup() {
    if (!movieLoaded) {
        movie.load(path);
        movie.setLoopState(OF_LOOP_NORMAL);
        movieLoaded = true;
    }
    
    float iw = movie.getWidth();
    float ih = movie.getHeight();
    
    float bw = box.getWidth();
    float bh = box.getHeight();
    
    if (iw / ih > bw / bh) {
        float x = 0;
        float w = bw;
        float h = ih * bw / iw;
        float y = 0.5 * (bh - h);
        rect.set(x, y, w, h);
    }
    else {
        float y = 0;
        float h = bh;
        float w = iw * bh / ih;
        float x = 0.5 * (bw - w);
        rect.set(x, y, w, h);
    }
}

void ofxPPMovie::start() {
    if (!loaded) {
        setup();
        loaded = true;
    }
    movie.play();
}

void ofxPPMovie::resize(ofRectangle content) {
    ofxPPElement::resize(content);
    if (loaded) {
        setup();
    }
}

void ofxPPMovie::stop() {
    movie.stop();
}

void ofxPPMovie::update() {
    movie.update();
}

void ofxPPMovie::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(box.getX(), box.getY());
    movie.draw(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
    ofPopStyle();
    ofPopMatrix();
}

//////////////////////////////////////////////////////////////////////

ofxPPText::ofxPPText(ofxPPSlide *parent, ofTrueTypeFont & font, ofColor textColor, string text, float x, float y, float w) : ofxPPElement(parent, text, x, y, w, 0.2) {
    this->font = &font;
    this->textColor = textColor;
    this->origText = text;
    this->text = text;
    textHeight = this->font->getLineHeight();
    wrapString();
}

void ofxPPText::wrapString() {
    string typeWrapped = "";
    string tempString = "";
    vector <string> words = ofSplitString(origText, " ");
    for(int i=0; i<words.size(); i++) {
        string wrd = words[i];
        tempString += wrd + " ";
        int stringwidth = font->stringWidth(tempString);
        if(stringwidth >= box.getWidth()) {
            tempString = "";
            typeWrapped += "\n";
        }
        typeWrapped += wrd + " ";
    }
    text = typeWrapped;
}

void ofxPPText::resize(ofRectangle content) {
    ofxPPElement::resize(content);
    wrapString();
}

void ofxPPText::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(textColor);
    font->drawString(text, box.getX(), box.getY() + textHeight);
    ofPopStyle();
    ofPopMatrix();
}