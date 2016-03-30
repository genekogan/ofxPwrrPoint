#pragma once

#include "ofMain.h"

#include "ofxScrollableImage.h"
#include "ofxPPElement.h"
#include "ofxPPSlide.h"


class ofxPwrrPoint
{
public:
    ofxPwrrPoint();
    ~ofxPwrrPoint();
    
    void setActive(bool active);
    void setPage(int page);
    void setPage(ofxPPSlide * slide);
    
    void preloadAssets();
    
    void update();
    void draw();
    
    void printStats();
    
    ofxPPSlide * addSlide(string name);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void windowResized();
    
    template<typename L, typename M>
    void setBackgroundFunction(L *listener, M method) {
        ofAddListener(drawE, listener, method);
    }
    
    void setContentRectangle(int x, int y, int w, int h);
    
    ofEvent<ofxPPSlide*> drawE;
    
protected:
    
    void mouseMoved(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    void keyPressed(ofKeyEventArgs &evt);
    
    vector<ofxPPSlide*> slides;
    int index;
    bool preloaded;
    ofRectangle content;
};
