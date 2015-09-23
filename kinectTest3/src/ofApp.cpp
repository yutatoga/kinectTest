#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    if( !ofFile::doesFileExist("11to16.bin") ){
        ofSystemAlertDialog("Make sure you have 11to16.bin, xTable.bin and zTable.bin in your data folder!");
        ofExit();
    }
    ofBackground(30, 30, 30);
    
    // gui
    panel.setup("distance in mm", "settings.xml", 540, 100);
    panel.add(kinect.minDistance);
    panel.add(kinect.maxDistance);
    panel.loadFromFile("settings.xml");
    
    // kinect
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

    // add vertex to mesh and save indexes
    vector< vector<int> > indexes;
    int id = 0;
    for (int y = 0; y < h; y += step) {
        vector<int> tempVector;
        for (int x = 0; x < w; x += step) {
            kinectMesh.addVertex(ofVec3f(x, y, rawDepthPixels[x+y*w]));
            tempVector.push_back(id);
            id++;
        }
        indexes.push_back(tempVector);
    }
    
    // set triangle
    for (int y = 0; y < (int)indexes.size()-1; y ++) {
        for (int x = 0; x < (int)indexes[0].size()-1; x ++) {
            kinectMesh.addTriangle(indexes[y][x], indexes[y][x+1], indexes[y+1][x+1]);
            kinectMesh.addTriangle(indexes[y][x], indexes[y+1][x+1], indexes[y+1][x]);
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
