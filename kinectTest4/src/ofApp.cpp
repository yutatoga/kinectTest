#include "ofApp.h"

int xyToId (int x, int y, int step, int width){
    return ((int)(y*((width-1)/step+1)+x)/step);
}

//--------------------------------------------------------------
void ofApp::setup(){
    if( !ofFile::doesFileExist("11to16.bin") ){
        ofSystemAlertDialog("Make sure you have 11to16.bin, xTable.bin and zTable.bin in your data folder!");
        ofExit();
    }
    
    ofBackground(30, 30, 30);
    
    panel.setup("distance in mm", "settings.xml", 540, 100);
    panel.add(kinect.minDistance);
    panel.add(kinect.maxDistance);
    panel.loadFromFile("settings.xml");
    
    kinect.open();
    
    // debug
    ofSetVerticalSync(false);
    ofSetFrameRate(0);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if( kinect.isFrameNew() ){
        texDepth.loadData( kinect.getDepthPixels() );
        texRGB.loadData( kinect.getRgbPixels() );
        rawDepthPixels = kinect.getRawDepthPixels();
    }
    
    // mesh
    kinectMesh.clear();
    int w = rawDepthPixels.getWidth();
    int h = rawDepthPixels.getHeight();
    int step = 10;
    float depth;
    
    // add vertex to mesh
    for (int y = 0; y < h; y += step) {
        vector<int> tempVector;
        for (int x = 0; x < w; x += step) {
            kinectMesh.addVertex(ofVec3f(x, y, rawDepthPixels[x+y*w]));
        }
    }
    
    // add triangle
    for (int y = 0; y < h-step; y += step) {
        for (int x = 0; x < w-step; x += step) {
            kinectMesh.addTriangle(xyToId(x, y, step, w), xyToId(x+step, y, step, w), xyToId(x+step, y+step, step, w));
            kinectMesh.addTriangle(xyToId(x, y, step, w), xyToId(x+step, y+step, step, w), xyToId(x, y+step, step, w));
        }
    }
}


bool ofApp::valueIsInKinectRange(float value){
    return (value > kinect.minDistance && value < kinect.maxDistance);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("ofxKinectV2: Work in progress addon.\nBased on the excellent work by the OpenKinect libfreenect2 team\n\n-Only supports one Kinect v2 at a time. \n-Requires USB 3.0 port ( superspeed )\n-Requires patched libusb. If you have the libusb from ofxKinect ( v1 ) linked to your project it will prevent superspeed on Kinect V2", 10, 14);
    
    texDepth.draw(10, 100);
    texRGB.draw(10, 110 + texDepth.getHeight(), 1920/4, 1080/4);
    
    // draw mesh
    camera.begin();{
        ofPushMatrix();{
            ofScale(1, -1, -1);
            ofEnableDepthTest();{
                kinectMesh.setMode(OF_PRIMITIVE_TRIANGLES);
                glLineWidth(int(1));
                kinectMesh.drawWireframe();
            }ofDisableDepthTest();
        }
        ofPopMatrix();
    }camera.end();
    
    // gui
    panel.draw();
    
    // debug
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
