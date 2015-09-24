#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    if( !ofFile::doesFileExist("11to16.bin") ){
        ofSystemAlertDialog("Make sure you have 11to16.bin, xTable.bin and zTable.bin in your data folder!");
        ofExit();
    }
    ofBackground(0, 0, 0);
    
    // gui
    panel.setup("distance in mm", "settings.xml", 0, 0);
    panel.add(kinect.minDistance);
    panel.add(kinect.maxDistance);
    panel.add(step.set("step", 5, 3, 30));
    panel.add(enableDrawDebug.set("enableDrawDebug", false));
    panel.add(enableDrawWireFrame.set("enableDrawWireFrame", true));
    panel.add(enableDrawGuideLine.set("enableDrawGuideLine", false));
    panel.add(enableMouseInput.set("enableMouseInput", true));
    panel.add(cameraPosition.set("cameraPosition", ofVec3f(0, -4.f, -10.f), ofVec3f(-100, -100, -100), ofVec3f(100, 100, 100)));
    panel.add(cameraLookAt.set("cameraLookAt", ofVec3f(0, 0, 0), ofVec3f(-100, -100, -100), ofVec3f(100, 100, 100)));
    panel.loadFromFile("settings.xml");
    
    // kinect
    kinect.open();
    
    // caemera
    camera.setDistance(14);
    camera.setPosition(cameraPosition);
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    if (!enableMouseInput) camera.disableMouseInput();
    
    // bullet
    world.setup();
    world.enableGrabbing();
    world.setCamera(&camera);
    world.setGravity(ofVec3f(0, 0, 15.0));
    
    // light
    light.setPosition(0, -5, 0);
    
    // debug
    ofSetVerticalSync(false);
    ofSetFrameRate(0);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if( kinect.isFrameNew() ){
        texDepth.loadData(kinect.getDepthPixels());
        texRGB.loadData(kinect.getRgbPixels());
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
        // setup when kinect got ready
        // - init kinectBulletShape
        kinectBulletShape = shared_ptr< ofxBulletTriMeshShape >( new ofxBulletTriMeshShape() );
        kinectBulletShape->create( world.world, kinectMesh, ofVec3f(0,0,0), 0.f, ofVec3f(-10000, -10000, -10000), ofVec3f(10000,10000,10000) );
        kinectBulletShape->add();
        kinectBulletShape->enableKinematic();
        kinectBulletShape->setActivationState( DISABLE_DEACTIVATION );
        
        // - camera
        camera.setPosition(w/2.0, h/2.0, -10.f);
        camera.lookAt(ofVec3f(w/2.0, h/2.0, 0), ofVec3f(0, -1, 0));
    }
    
    if (h != 0 && kinectBulletShape != NULL) {
        kinectBulletShape->remove();
        kinectBulletShape->create( world.world, kinectMesh, ofVec3f(0,0,0), 0.f, ofVec3f(-10000, -10000, -10000), ofVec3f(10000,10000,10000) );
        kinectBulletShape->add();
        kinectBulletShape->enableKinematic();
        kinectBulletShape->setActivationState( DISABLE_DEACTIVATION );
        world.update(ofGetLastFrameTime()*2, 20);
    }
    
    // camera
    enableMouseInput ? camera.enableMouseInput() : camera.disableMouseInput();
}

bool ofApp::valueIsInKinectRange(float value){
    return (value > kinect.minDistance && value < kinect.maxDistance);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw mesh
    ofEnableDepthTest();{
        // ofPushMatrix();{
        camera.begin();{
            // ofTranslate(-rawDepthPixels.getWidth()/2.0, -rawDepthPixels.getHeight()/2.0, 0);
            ofSetLineWidth(1.f);
            if(enableDrawDebug) world.drawDebug();
            ofEnableLighting();{
                light.enable();{
                    // primitive
                    ofDrawBox(0.3, 0, 0, 0.5, 0.5, 0.5);
                    
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
                }light.disable();
            }ofDisableLighting();
            // if(enableDrawDebug) ofDrawAxis(0.5);
            // }ofPopMatrix();
        }camera.end();
        
    }ofDisableDepthTest();

    // gui
    panel.draw();
    
    // debug
    // - fps
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    // - center guide line
    if (enableDrawGuideLine) {
        ofSetColor(ofColor::lightBlue);
        ofLine(ofGetWidth()/2.0, 0, ofGetWidth()/2.0, ofGetHeight());
        ofLine(0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight()/2.0);
        ofSetColor(255);
    }
    
    // - depth data
    ofVec2f debugImageSize(1920/10.0, 1080/10.0);
    ofRect(panel.getPosition().x, panel.getHeight()+4, debugImageSize.x+2, debugImageSize.y+2);
    texDepth.draw(panel.getPosition().x+1, panel.getHeight()+5, debugImageSize.x, debugImageSize.y);

    // - RGB data
    ofRect(panel.getPosition().x, panel.getHeight()+5+debugImageSize.y+2, debugImageSize.x+2, debugImageSize.y+2);
    texRGB.draw(panel.getPosition().x+1, panel.getHeight()+5+debugImageSize.y+3, debugImageSize.x, debugImageSize.y);
    
    // - info
    //    ofDrawBitmapString("ofxKinectV2: Work in progress addon.\nBased on the excellent work by the OpenKinect libfreenect2 team\n\n-Only supports one Kinect v2 at a time. \n-Requires USB 3.0 port ( superspeed )\n-Requires patched libusb. If you have the libusb from ofxKinect ( v1 ) linked to your project it will prevent superspeed on Kinect V2", 10, 14);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':{
            // add one sphere
            shared_ptr< ofxBulletSphere > ss( new ofxBulletSphere() );
            ss->create( world.world, camera.getPosition()+100.0f*camera.getLookAtDir(), 0.0000051, 30.0);
            ss->add();
            ofVec3f frc(camera.getLookAtDir());
            frc.normalize();
            // ss->applyCentralForce(frc*0.01);
            spheres.push_back( ss );
        }
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        default:
            break;
    }
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
