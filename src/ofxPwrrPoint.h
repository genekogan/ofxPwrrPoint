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
    
    bool loadFromExported() {exported = true;}

    void setActive(bool active);
    void setPage(int page);
    void setPage(ofxPPSlide * slide);
    
    void preloadAssets();
    
    void update();
    void draw();
    
    int getIndex() {return index;}
    int getNumSlides() {return slides.size();}
    void printStats();
    
    ofxPPSlide * addSlide(string name);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseScrolled(float scrollX, float scrollY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void windowResized();
    
    template<typename L, typename M>
    void setBackgroundFunction(L *listener, M method) {
        ofAddListener(drawE, listener, method);
    }
    
    void setContentRectangle(int x, int y, int w, int h);
    
    ofEvent<ofxPPSlide*> drawE;
    
    void exportAssets();
    void exportScreenshots();

    void prevSegment();
    void nextSegment();
    
    void setupGui();
    void drawGui();
    void wrapNotes();
    
    bool exported;
    
protected:
    
    void mouseMoved(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseScrolled(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    void keyPressed(ofKeyEventArgs &evt);
    
    void exportScreenshotsSave();

    vector<ofxPPSlide*> slides;
    vector<string> notes;
    
    int index;
    bool preloaded;
    bool toExportSlides;
    ofRectangle content;
    
    ofTrueTypeFont notesFont;
    int notesFontSize, guiWindowWidth, guiWindowHeight;
};
