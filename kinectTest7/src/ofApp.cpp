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
    panel.add(step.set("step", 5, 3, 100));
    panel.add(enableDrawDebug.set("enableDrawDebug", false));
    panel.add(enableDrawWireFrame.set("enableDrawWireFrame", true));
    panel.loadFromFile("settings.xml");
    
    // kinect
    kinect.open();
    
    // caemera
    camera.setDistance(14);
    camera.setPosition(ofVec3f(0, -4.f, -10.f));
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    camera.enableMouseInput();
    
    // bullet
    world.setup();
    world.enableGrabbing();
    world.setCamera(&camera);
    world.setGravity( ofVec3f(0, 25., 0) );
    
    // light
    light.setPosition( 0, -5, 0 );
    
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
    float depth;
    
    // add vertex to mesh and save indexes
    vector< vector<int> > indexes;
    int id = 0;
    for (int y = 0; y < h; y += step) {
        vector<int> tempVector;
        for (int x = 0; x < w; x += step) {
            float distance = rawDepthPixels[x+y*w];
            kinectMesh.addVertex(ofVec3f(x, y, distance));
            if (distance > kinect.minDistance && distance < kinect.maxDistance) {
                tempVector.push_back(id);
            }else{
                tempVector.push_back(-1); // set -1 for out of range
            }
            id++;
        }
        indexes.push_back(tempVector);
    }
    
    // set triangle
    for (int y = 0; y < h-step; y += step) {
        for (int x = 0; x < w-step; x += step) {
            if (indexes[y/step][x/step] != -1 &&
                indexes[y/step][x/step+1] !=  -1 &&
                indexes[y/step+1][x/step+1] != -1 &&
                indexes[y/step+1][x/step] != -1) {
                kinectMesh.addTriangle(indexes[y/step][x/step], indexes[y/step][x/step+1], indexes[y/step+1][x/step+1]);
                kinectMesh.addTriangle(indexes[y/step][x/step], indexes[y/step+1][x/step+1], indexes[y/step+1][x/step]);
            }
        }
    }

    // bullet
    if (h != 0 && kinectBulletShape == NULL ) {
        kinectBulletShape = shared_ptr< ofxBulletTriMeshShape >( new ofxBulletTriMeshShape() );
        kinectBulletShape->create( world.world, kinectMesh, ofVec3f(0,0,0), 0.f, ofVec3f(-10000, -10000, -10000), ofVec3f(10000,10000,10000) );
        kinectBulletShape->add();
        kinectBulletShape->enableKinematic();
        kinectBulletShape->setActivationState( DISABLE_DEACTIVATION );
    }

    if (h != 0 && kinectBulletShape != NULL) {
        kinectBulletShape->remove();
        kinectBulletShape->create( world.world, kinectMesh, ofVec3f(0,0,0), 0.f, ofVec3f(-10000, -10000, -10000), ofVec3f(10000,10000,10000) );
        kinectBulletShape->add();
        kinectBulletShape->enableKinematic();
        kinectBulletShape->setActivationState( DISABLE_DEACTIVATION );
        world.update();
    }
}

bool ofApp::valueIsInKinectRange(float value){
    return (value > kinect.minDistance && value < kinect.maxDistance);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw mesh
    ofEnableDepthTest();{
        camera.begin();{
            ofSetLineWidth(1.f);
            if(enableDrawDebug) world.drawDebug();
            ofEnableLighting();{
                light.enable();{
                    ofPushMatrix();{
                        // - kinect mesh
                        kinectMesh.setMode(OF_PRIMITIVE_TRIANGLES);
                        glLineWidth(int(1));
                        ofSetColor(255);
                        enableDrawWireFrame ? kinectMesh.drawWireframe() : kinectMesh.drawFaces();
                        
                        // - spheres
                        ofSetHexColor( 0xC4EF02 );
                        for( int i = 0; i < spheres.size(); i++ ) {
                            spheres[i]->draw();
                        }
                        ofSetColor(255);
                    }
                    ofPopMatrix();
                }light.disable();
            }ofDisableLighting();
            // if(enableDrawDebug) ofDrawAxis(0.5);
        }camera.end();
    }ofDisableDepthTest();
    
    // gui
    panel.draw();
    
    // debug
    // - fps
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    // - depth data
//    texDepth.draw(10, 100);
    // - RGB data
//    texRGB.draw(10, 110 + texDepth.getHeight(), 1920/4, 1080/4);
    // - info
//    ofDrawBitmapString("ofxKinectV2: Work in progress addon.\nBased on the excellent work by the OpenKinect libfreenect2 team\n\n-Only supports one Kinect v2 at a time. \n-Requires USB 3.0 port ( superspeed )\n-Requires patched libusb. If you have the libusb from ofxKinect ( v1 ) linked to your project it will prevent superspeed on Kinect V2", 10, 14);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // add one sphere
    shared_ptr< ofxBulletSphere > ss( new ofxBulletSphere() );
    ss->create( world.world, camera.getPosition(), 0.05, 30.0);
    ss->add();
    
    ofVec3f frc = -camera.getPosition();
    frc.normalize();
    ss->applyCentralForce( frc * 4000 );

    
    spheres.push_back( ss );
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
