#pragma once

#include "ofMain.h"
#include "ofxScrollableImage.h"
#include "ofxPPElement.h"



class ofxPPSlide {
public:
    ofxPPSlide(string name);
    
    bool loadFromExported() {exported = true;}
    bool atBeginning() {return index == 0;}
    bool atEnd() {return index == breakpoints.size();}
    
    string getName() {return name;}
    
    ofxPPRect * addRect(ofColor color, float x, float y, float w, float h);
    ofxPPImage * addImage(string name_, string path, float x, float y, float w, float h);
    ofxPPScrollableImage * addScrollableImage(string name_, string path, float x, float y, float w, float h);
    ofxPPMovie * addMovie(string name_, string path, bool autoPlay, float x, float y, float w, float h);
    ofxPPText * addText(ofTrueTypeFont & font, ofColor textColor, string text, float x, float y, float w);
    void addAction(ofxPPElement *element);
    
    void addBreakpoint();
    void addNote(string note) {notes.push_back(note);}

    vector<int> & getBreakpoints() {return breakpoints;}
    vector<string> & getNotes() {return notes;}
    
    void loadAssets();
    
    void update();
    void draw();
    
    void start();
    void stop();
    
    void prev();
    void next();
    
    void setContentRectangle(int x, int y, int w, int h);
    void resize(ofRectangle content);
    
    void keyPressed(int key);
    bool mouseMoved(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseScrolled(float scrollX, float scrollY);

    template<typename L, typename M>
    void setBackgroundFunction(L *listener, M method) {
        ofAddListener(drawE, listener, method);
    }
    
    void exportAssets();
    void exportVideosAndScrollableImages(string prefix);

    ofEvent<ofxPPSlide*> drawE;

protected:
    
    int getNextBreakpointSlide();
    
    vector<ofxPPElement*> elements;
    vector<int> breakpoints;
    vector<string> notes;
    string name;
    int index, end;
    bool exported;
    ofRectangle content;
};
