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
    
    virtual void exportAssets(string prefix) {}
    virtual void exportAssets() {}
    virtual void loadFromExported() {exported = true;}
    virtual bool isVideo() {return false;}
    virtual bool isAudio() {return false;}
    virtual bool isScrollableImage() {return false;}
    
    virtual void keyPressed(int key);
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    virtual bool mouseScrolled(float scrollX, float scrollY);
    
    ofxPPSlide *parent;
    ofRectangle box;
    string name;
    float tx, ty, tw, th;
    bool mouseOver;
    bool loaded;
    bool exported;
};


class ofxPPRect : public ofxPPElement {
public:
    ofxPPRect(ofxPPSlide *parent, ofColor color, float x, float y, float w, float h);
    void draw();    
    ofColor color;
};

class ofxPPImage : public ofxPPElement {
public:
    ofxPPImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h);
    void setup();
    void start();
    void resize(ofRectangle content);
    void draw();

    void exportAssets(string prefix) {
        vector<string> p = ofSplitString(path, "/");
        string newPath = ofToDataPath(p[p.size()-1]);
        if (prefix != "") {
            newPath = ofToDataPath(prefix+"_"+p[p.size()-1]);
        }
        string cmd = "cp \""+path+"\" \""+newPath+"\";";
        ofLog() << cmd;
        ofSystem(cmd);
    }
    
    void exportAssets() {exportAssets("");};

    ofImage img;
    string path;
    ofRectangle rect;
};


class ofxPPScrollableImage : public ofxPPElement {
public:
    ofxPPScrollableImage(ofxPPSlide *parent, string name, string path, float x, float y, float w, float h);
    
    void setup();
//    bool mouseMoved(int mouseX, int mouseY);
    bool mouseScrolled(float scrollX, float scrollY);
    void start();
    void resize(ofRectangle content);
    void update();
    void draw();
    
    bool isScrollableImage() {return true;}

    void exportAssets(string prefix) {
        vector<string> p = ofSplitString(path, "/");
        string newPath = ofToDataPath(p[p.size()-1]);
        if (prefix != "") {
            newPath = ofToDataPath(prefix+"_"+p[p.size()-1]);
        }
        string cmd = "cp \""+path+"\" \""+newPath+"\";";
        ofLog() << cmd;
        ofSystem(cmd);
    }
    
    void exportAssets() {exportAssets("");};
    
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
    
    void setPosition(float t);
    void setPositionRandom();
    void setLoop(bool isLoop_);
    
    bool mouseMoved(int x, int y);
    bool mousePressed(int x, int y);
    
    bool isVideo() {return true;}

    void exportAssets(string prefix) {
        vector<string> p = ofSplitString(path, "/");
        string newPath = ofToDataPath(p[p.size()-1]);
        if (prefix != "") {
            newPath = ofToDataPath(prefix+"_"+p[p.size()-1]);
        }
        string cmd = "cp \""+path+"\" \""+newPath+"\";";
        ofLog() << cmd;
        ofSystem(cmd);
    }
    
    void exportAssets() {exportAssets("");};


    ofVideoPlayer movie;
    string path;
    float pct, pctNext;
    ofRectangle rect;
    ofRectangle pBar;
    bool autoPlay;
    bool movieLoaded;
    bool isOverPBar, isOverMovie;
    bool isSetRandom;
    bool isLoop;
};


class ofxPPSound : public ofxPPElement {
public:
    ofxPPSound(ofxPPSlide *parent, string name, string path, ofTrueTypeFont & font, bool autoPlay, bool isLoop, float x, float y, float w, float h);
    
    void setup();
    void start();
    void resize(ofRectangle content);
    void stop();
    void update();
    void draw();
    
    void setPosition(float t);
    void setPositionRandom();
    void setLoop(bool isLoop_);
    
    bool mouseMoved(int x, int y);
    bool mousePressed(int x, int y);
    
    bool isAudio() {return true;}
    
    void exportAssets(string prefix) {
        vector<string> p = ofSplitString(path, "/");
        string newPath = ofToDataPath(p[p.size()-1]);
        if (prefix != "") {
            newPath = ofToDataPath(prefix+"_"+p[p.size()-1]);
        }
        string cmd = "cp \""+path+"\" \""+newPath+"\";";
        ofLog() << cmd;
        ofSystem(cmd);
    }
    
    void exportAssets() {exportAssets("");};
    
    ofSoundPlayer sound;
    ofTrueTypeFont *font;
    string path;
    float pct, pctNext;
    ofRectangle rect;
    ofRectangle pBar;
    bool autoPlay;
    bool isPaused;
    bool soundLoaded;
    bool isOverPBar, isOverSound;
    bool isSetRandom;
    bool isLoop;
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

