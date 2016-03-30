#pragma once

#include "ofMain.h"


class ofxScrollableImage
{
public:
    ofxScrollableImage();
    
    void setup(string path, int width, int height);
    void loadImage(string path);

    void setSize(int width, int height);
    void setScale(float scale);
    void setPosition(float tx, float ty);
    
    string getImagePath() {return path;}
    int getImageWidth() {return img.getWidth();}
    int getImageHeight() {return img.getHeight();}
    int getWidth() {return width;}
    int getHeight() {return height;}

    void draw(int x, int y);
    
private:
    void update();
    
    ofFbo fbo;
    ofFbo maskFbo;
    ofShader shader;
    int width, height;
    float scale, tx, ty;
    string path;
    ofImage img;
};