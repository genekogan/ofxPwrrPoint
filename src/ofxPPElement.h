#pragma once

#include "ofMain.h"
#include "ofxScrollableImage.h"

class ofxPPSlide;

class ofxPPElement {
public:
    ofxPPElement(ofxPPSlide *parent, string name, float x, float y, float w, float h);

    virtual void resize(ofRectangle content);
    
    virtual void start() {}
    virtual void stop() {}
    virtual void update() {}
    virtual void draw();
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    
    ofxPPSlide *parent;
    ofRectangle box;
    string name;
    float tx, ty, tw, th;
    bool mouseOver;
    bool loaded;
};


class ofxPPImage : public ofxPPElement {
public:
    ofxPPImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h);
    void setup();
    void start();
    void resize(ofRectangle content);
    void draw();
    
    ofImage img;
    string path;
    ofRectangle rect;
};


class ofxPPScrollableImage : public ofxPPElement {
public:
    ofxPPScrollableImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h);
    
    void setup();
    bool mouseMoved(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    void start();
    void resize(ofRectangle content);
    void update();
    void draw();
    
    ofxScrollableImage img;
    float tx, ty, scale, maxScale;
    float py;
    string path;
};


class ofxPPMovie : public ofxPPElement {
public:
    ofxPPMovie(ofxPPSlide *parent, string name, string path, bool autoPlay, float x, float y, float w, float h);

    void setup();
    void start();
    void resize(ofRectangle content);
    void stop();
    void update();
    void draw();
    
    ofVideoPlayer movie;
    string path;
    ofRectangle rect;
    bool autoPlay;
    bool movieLoaded;
};


class ofxPPText : public ofxPPElement {
public:
    ofxPPText(ofxPPSlide *parent, ofTrueTypeFont & font, ofColor textColor, string text, float x, float y, float w);
    
    void wrapString();
    void resize(ofRectangle content);
    void draw();
    
    ofColor textColor;
    ofTrueTypeFont *font;
    string text, origText;
    float textHeight;
};

