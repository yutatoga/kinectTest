#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxBullet.h"
#include "ofxAssimpModelLoader.h"

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
    
    void setupWhenKinectIsReady();
    bool valueIsInKinectRange(float value);
    void resetPressed();
    
    ofxPanel panel;
    ofParameter<int> step;
    ofParameter<bool> enableDrawDebug;
    ofParameter<bool> enableDrawWireFrame;
    ofParameter<bool> enableDrawGuideLine;
    ofParameter<bool> enableMouseInput;
    ofParameter<ofVec3f> cameraPosition;
    ofParameter<ofVec3f> cameraLookAt;
    ofParameter<bool> enableDrawDebugSpheres;
    ofxButton reset;
    
    ofxKinectV2 kinect;
    ofTexture texDepth;
    ofTexture texRGB;
    ofFloatPixels rawDepthPixels;
    ofMesh kinectMesh;
    ofEasyCam camera;
    ofVec2f translateValue;
    
    // 3d model
    ofxAssimpModelLoader assimpModelLoader;
    
    // bullet
    ofxBulletWorldRigid world;
    shared_ptr<ofxBulletTriMeshShape> kinectBulletShape;
    vector<shared_ptr<ofxBulletSphere> > spheres;
    vector<ofxBulletCustomShape*> assimpModelBulletShapes;
    
    // light
    ofLight light;
    
    // debug
    // - debug spheres
    vector<ofSpherePrimitive> debugSpheres;
    // - camera target
    ofSpherePrimitive debugSphereCameraTarget;
};
