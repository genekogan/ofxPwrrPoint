#include "ofxScrollableImage.h"


#define STRINGIFY(A) #A

ofxScrollableImage::ofxScrollableImage() {
    string shaderProgram = STRINGIFY(
                                     uniform sampler2DRect tex;
                                     uniform float scale;
                                     uniform vec2 offset;
                                     void main (void){
                                         vec2 pos = gl_TexCoord[0].st;
                                         pos += offset;
                                         pos *= scale;
                                         vec4 rTxt = texture2DRect(tex, pos);
                                         gl_FragColor = rTxt;
                                     }
                                     );
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();
}

void ofxScrollableImage::setup(string path, int width, int height) {
    loadImage(path);
    setSize(width, height);
}

void ofxScrollableImage::loadImage(string path) {
    this->path = path;
    img.load(path);
}

void ofxScrollableImage::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    fbo.allocate(width, height);
    maskFbo.allocate(width, height);
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
}

void ofxScrollableImage::setScale(float scale) {
    this->scale = scale;
    update();
}

void ofxScrollableImage::setPosition(float tx, float ty) {
    this->tx = tx;
    this->ty = ty;
    update();
}

void ofxScrollableImage::update() {
    float px = ofMap(tx, 0, 1, 0, max(0.0f, (img.getWidth()/scale - width)), true);
    float py = ofMap(ty, 0, 1, 0, max(0.0f, (img.getHeight()/scale - height)), true);
    fbo.begin();
    ofClear(0, 0, 0, 255);
    shader.begin();
    shader.setUniformTexture("tex", img.getTexture() , 1 );
    shader.setUniform1f("scale", scale);
    shader.setUniform2f("offset", px, py);
    maskFbo.draw(0,0);
    shader.end();
    fbo.end();
}

void ofxScrollableImage::draw(int x, int y) {
    ofPushMatrix();
    ofTranslate(x, y);
    fbo.draw(0, 0);
    ofPopMatrix();
}