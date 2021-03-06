#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool valueIsInKinectRange(float value);
    
    ofxPanel panel;
    ofParameter<int> step;
    
    ofxKinectV2 kinect;
    ofTexture texDepth;
    ofTexture texRGB;
    ofFloatPixels rawDepthPixels;
    ofMesh kinectMesh;
    ofEasyCam camera;
};
