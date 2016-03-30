#pragma once

#include "ofMain.h"
#include "ofxScrollableImage.h"
#include "ofxPPElement.h"



class ofxPPSlide {
public:
    ofxPPSlide(string name);
    
    string getName() {return name;}
    
    void addImage(string name_, string path, float x, float y, float w, float h);
    void addScrollableImage(string name_, string path, float x, float y, float w, float h);
    void addMovie(string name_, string path, bool autoPlay, float x, float y, float w, float h);
    void addText(ofTrueTypeFont & font, ofColor textColor, string text, float x, float y, float w);
    void addAction(ofxPPElement *element);
    
    void addBreakpoint();
    vector<int> & getBreakpoints() {return breakpoints;}
    
    void loadAssets();
    
    void update();
    void draw();
    
    void start();
    void stop();
    
    void prev();
    void next();
    
    void setContentRectangle(int x, int y, int w, int h);
    void resize(ofRectangle content);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);

    ofRectangle content;

protected:
    
    int getNextBreakpointSlide();
    
    vector<ofxPPElement*> elements;
    vector<int> breakpoints;
    string name;
    int index, end;
};
