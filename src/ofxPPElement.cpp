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
    exported = false;
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

void ofxPPElement::keyPressed(int key) {
}

bool ofxPPElement::mouseMoved(int mouseX, int mouseY) {
    mouseOver = box.inside(mouseX, mouseY);
    return mouseOver;
}

bool ofxPPElement::mouseDragged(int mouseX, int mouseY) {
    return mouseOver;
}

bool ofxPPElement::mouseScrolled(float scrollX, float scrollY) {
    return mouseOver;
}

bool ofxPPElement::mousePressed(int mouseX, int mouseY) {
    return mouseOver;
}

bool ofxPPElement::mouseReleased(int mouseX, int mouseY) {
    return mouseOver;
}


//////////////////////////////////////////////////////////////////////

ofxPPRect::ofxPPRect(ofxPPSlide *parent, ofColor color, float x, float y, float w, float h) : ofxPPElement(parent, "rect", x, y, w, h) {
    this->color = color;
}

void ofxPPRect::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(color);
    ofDrawRectangle(box.getX(), box.getY(), box.getWidth(), box.getHeight());
    ofPopStyle();
    ofPopMatrix();
}


//////////////////////////////////////////////////////////////////////

ofxPPImage::ofxPPImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h) : ofxPPElement(parent, name, x, y, w, h) {
    this->path = path;
}

void ofxPPImage::setup() {
    if (exported) {
        vector<string> p = ofSplitString(path, "/");
        img.load(p[p.size()-1]);
    }
    else {
        img.load(path);
    }
    
    
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
    ofTranslate(floor(rect.getX()), floor(rect.getY()));
    img.draw(floor(box.getX()), floor(box.getY()));
    ofPopStyle();
    ofPopMatrix();
}

//////////////////////////////////////////////////////////////////////

ofxPPScrollableImage::ofxPPScrollableImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h) : ofxPPElement(parent, name, x, y, w, h) {
    this->path = path;
}

void ofxPPScrollableImage::setup() {
    if (exported) {
        vector<string> p = ofSplitString(path, "/");
        img.setup(p[p.size()-1], box.getWidth(), box.getHeight());
    }
    else {
        img.setup(path, box.getWidth(), box.getHeight());
    }
    maxScale = min(img.getImageWidth() / box.getWidth(), img.getImageHeight() / box.getHeight());
    tx = 0;
    ty = 0;
    //scale = 0.5*maxScale;
    scale = maxScale;
    img.setPosition(tx, ty);
    img.setScale(scale);
}

bool ofxPPScrollableImage::mouseScrolled(float scrollX, float scrollY) {
    if (mouseOver) {
        scale = ofClamp(scale - 0.015 * scrollY, 0.5, maxScale);
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
    isOverPBar = false;
    isOverMovie = false;
    isSetRandom = false;
    isLoop = true;
}

void ofxPPMovie::setup() {
    if (!movieLoaded) {
        if (exported) {
            vector<string> p = ofSplitString(path, "/");
            movie.load(p[p.size()-1]);
        }
        else {
            movie.load(path);
        }
        movie.setLoopState(isLoop ? OF_LOOP_NORMAL : OF_LOOP_NONE);
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
        pBar.set(x, y + h - 20, w, 20);
    }
    else {
        float y = 0;
        float h = bh;
        float w = iw * bh / ih;
        float x = 0.5 * (bw - w);
        rect.set(x, y, w, h);
        pBar.set(x, y + h - 20, w, 20);
    }
}

void ofxPPMovie::setPositionRandom() {
    setPosition(ofRandom(1));
}

void ofxPPMovie::setPosition(float t) {
    isSetRandom = true;
    pctNext = t;
}

void ofxPPMovie::setLoop(bool isLoop_) {
    isLoop = isLoop_;
    movie.setLoopState(isLoop ? OF_LOOP_NORMAL : OF_LOOP_NONE);
}

void ofxPPMovie::start() {
    if (!loaded) {
        setup();
        loaded = true;
    }
    if (autoPlay) {
        movie.play();
    }
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
    pct = (float) movie.getCurrentFrame() / movie.getTotalNumFrames();
    if (isSetRandom && movie.isInitialized() && movie.isPlaying()) {
        pct = pctNext;
        movie.setPosition(pct);
        isSetRandom = false;
    }
}

bool ofxPPMovie::mouseMoved(int x, int y) {
    float mx = x-box.getX();
    float my = y-box.getY();
    isOverPBar = pBar.inside(mx, my);
    isOverMovie = rect.inside(mx, my);
    return isOverPBar;
}

bool ofxPPMovie::mousePressed(int x, int y) {
    if (isOverPBar) {
        float t = (x - box.getX() - pBar.getX()) / pBar.getWidth();
        movie.setPosition(t);
    }
    else if (isOverMovie) {
        if (movie.isPlaying()) {
            movie.stop();
        }
        else {
            movie.play();
        }
    }
    return isOverPBar;
}

void ofxPPMovie::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(box.getX(), box.getY());
    movie.draw(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

    if (isOverPBar) {
        ofSetColor(0, 255, 0);
        ofDrawRectangle(pBar);
        ofSetColor(0, 0, 255);
        ofDrawRectangle(pBar.getX(), pBar.getY(), pBar.getWidth()*pct, pBar.getHeight());
    }
    
    ofPopStyle();
    ofPopMatrix();
}


//////////////////////////////////////////////////////////////////////

ofxPPSound::ofxPPSound(ofxPPSlide *parent, string name, string path, ofTrueTypeFont & font, bool autoPlay, bool isLoop, float x, float y, float w, float h) : ofxPPElement(parent, name, x, y, w, h) {
    this->autoPlay = autoPlay;
    this->isLoop = isLoop;
    this->path = path;
    this->font = &font;
    soundLoaded = false;
    isOverPBar = false;
    isOverSound = false;
    isSetRandom = false;
    isPaused = false;
    isLoop = true;
}

void ofxPPSound::setup() {
    if (!soundLoaded) {
        if (exported) {
            vector<string> p = ofSplitString(path, "/");
            sound.load(p[p.size()-1]);
        }
        else {
            sound.load(path);
        }
        sound.setLoop(isLoop);
        soundLoaded = true;
    }
    
    float w = box.getWidth();
    float h = box.getHeight();
    float pb = h;
    float x = 0;
    float y = 0;
    
    rect.set(x, y, w, h);
    pBar.set(x+pb, y, w-pb, h);
}

void ofxPPSound::setPositionRandom() {
    setPosition(ofRandom(1));
}

void ofxPPSound::setPosition(float t) {
    isSetRandom = true;
    pctNext = t;
}

void ofxPPSound::setLoop(bool isLoop_) {
    isLoop = isLoop_;
    sound.setLoop(isLoop);
}

void ofxPPSound::start() {
    if (!loaded) {
        setup();
        loaded = true;
    }
    if (autoPlay) {
        sound.play();
    }
}

void ofxPPSound::resize(ofRectangle content) {
    ofxPPElement::resize(content);
    if (loaded) {
        setup();
    }
}

void ofxPPSound::stop() {
    sound.stop();
}

void ofxPPSound::update() {
    pct = sound.getPosition();
    if (isSetRandom && sound.isLoaded() && sound.isPlaying()) {
        pct = pctNext;
        sound.setPosition(pct);
        isSetRandom = false;
    }
    ofSoundUpdate();
}

bool ofxPPSound::mouseMoved(int x, int y) {
    float mx = x-box.getX();
    float my = y-box.getY();
    isOverPBar = pBar.inside(mx, my);
    isOverSound = rect.inside(mx, my);
    return isOverPBar;
}

bool ofxPPSound::mousePressed(int x, int y) {
    if (isOverPBar) {
        float t = (x - box.getX() - pBar.getX()) / pBar.getWidth();
        sound.setPosition(t);
    }
    else if (isOverSound) {
        if (sound.isPlaying()) {
            isPaused = !isPaused;
            sound.setPaused(isPaused);
        }
        else {
            sound.play();
        }
    }
    return isOverPBar;
}

void ofxPPSound::draw() {
    //ofxPPElement::draw();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(box.getX(), box.getY());
    
    ofSetColor(0);
    ofDrawRectangle(rect);

    
    ofSetColor((isOverSound && !isOverPBar) ? ofColor(0, 255, 0) : ofColor(150));
    if (!sound.isPlaying() || isPaused) {
        ofDrawTriangle(rect.getX() + rect.getHeight()*0.1, rect.getY() + rect.getHeight()*0.1,
                       rect.getX() + rect.getHeight()*0.1, rect.getY() + rect.getHeight()*0.9,
                       rect.getX() + rect.getHeight()*0.9, rect.getY() + rect.getHeight()*0.5);
    } else {
        ofDrawRectangle(rect.getX() + rect.getHeight()*0.1, rect.getY() + rect.getHeight()*0.1, rect.getHeight() * 0.3, rect.getHeight() * 0.8);
        ofDrawRectangle(rect.getX() + rect.getHeight()*0.6, rect.getY() + rect.getHeight()*0.1, rect.getHeight() * 0.3, rect.getHeight() * 0.8);
    }
    
    ofSetColor(150);
    ofDrawRectangle(pBar);
    ofSetColor(0, 0, 255);
    ofDrawRectangle(pBar.getX(), pBar.getY(), pBar.getWidth()*pct, pBar.getHeight());

    ofSetColor(255);
    font->drawString(name, pBar.getX() + 0.05 * pBar.getWidth(), pBar.getY() + pBar.getHeight()*0.9);
    
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