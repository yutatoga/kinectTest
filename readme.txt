kinectTest 1
>> sample code from https://github.com/ofTheo/ofxKinectV2

kinectTest2
>> point cloud with kinect raw depth data

kinectTest3
>> drawing triangle mesh with kinect raw depth data

kinectTest4
>> same feature with kinectTest3 but calculating indexes for triangles(almost same speed with kinectTest3??) 

kinectTest5
>> draw triangle mesh with kinect raw depth data only in the depth range.

kinectTest6
>> save all vertexes to mesh and make triangle with depth data only in the depth range

kinectTest7
>> use bullet and make ofxBulletTriMeshShape with kinectMesh

kinectTest8
>> set camera position and direction properly for kinect’s view and you can change camera position and camera target(you can see the target as yellow sphere) with cameraPosition slider and cameraLookAt sliders after disabling mouseInput for ofEasyCam by pressing enableMouseInput button. you can see simple guide line by pressing DrawGuideLine button, and you can reset all of GUI settings and reset ofEasyCam by pressing reset button. You can toggle fullscreen by pressing f key and add sphere by pressing space key. you can see debug spheres by pressing enableDrawDebugSpheres button to check whole kinect view.

kinectTest9
>> load assimp model and add it to ofxBulletWorldRigid. you can add ofxBulletCustomShape pressing m key.