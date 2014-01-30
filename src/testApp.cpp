#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	ofDisableAntiAliasing();
	
	// find all the texture files and load them
	ofDirectory dir;
	ofDisableArbTex();
	int n = dir.listDir("textures");
	for (int i=0; i<n; i++) {
		textures.push_back(ofImage(dir.getPath(i)));
	}
	printf("%i Textures Loaded\n", (int)textures.size());
	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	//voronoi
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	
	ofSetVerticalSync(true);
	voronoi.setBounds(0, 0, ofGetWidth(), ofGetHeight());
	
	generateTheVoronoi();
	
	ofEnablePointSprites();
}

//--------------------------------------------------------------
void testApp::update(){
	//voronoi
	generateTheVoronoi();
	meshVector.clear();
	
	//box2d
	box2d.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	//draw voronoi
	ofPushStyle();
	for(int i=0; i<voronoi.cells.size(); i++) {
		ofPath path;
		path.setFillColor(colorVector[i]);
		path.setStrokeColor(ofColor(255, 255, 255));
		path.setStrokeWidth(1);
		for(int j=0; j<voronoi.cells[i].pts.size(); j++) {
			path.lineTo(voronoi.cells[i].pts[j]);
		}
		path.draw();
	}
	ofPopStyle();
	
	//draw the raw points
	ofPushStyle();
	ofSetColor(255);
	for(int i=0; i<pointVector.size(); i++) {
		ofCircle(pointVector[i], 3);
	}
	ofPopStyle();
	
	// box2d
	for (int i=0; i<shapes.size(); i++) {
		shapes[i].get()->draw();
	}
	
	// some debug information
	string info = "Textures from subtlepatterns.com\n";
	info += "Press c to clear everything\n";
	
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
	
	//guideline
	ofDrawBitmapString("fps   : "+ofToString(ofGetFrameRate()), 10, 50);
	ofDrawBitmapString("point : "+ofToString(pointVector.size()), 10, 60);
	ofPushStyle();
	ofSetColor(0, 0, 255);
	ofLine(0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight()/2.0);
	ofLine(ofGetWidth()/2.0, 0, ofGetWidth()/2.0, ofGetHeight());
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::generateTheVoronoi() {
	voronoi.clear();
	for(int i=0; i<pointVector.size(); i++) {
		voronoi.addPoint(pointVector[i]);
	}
	voronoi.generateVoronoi();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'c') {
		shapes.clear();
	}
	
	if (key == ' ') {
		pointVector.push_back(ofPoint(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())));
		generateTheVoronoi();
		colorVector.push_back(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)));
		
		// add some circles every so often
		for (int i =0; i<voronoi.cells.size(); i++) {
			shapes.push_back(ofPtr<TextureShape>(new TextureShape));
			shapes.back().get()->setTexture(&textures[(int)ofRandom(textures.size())]);
			shapes.back().get()->setup(box2d, (ofGetWidth()/2)+ofRandom(-20, 20), 130, ofRandom(10, 50), voronoi.cells[i].pts);
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	int pointNumber = ofRandom(10, 50);
	for (int i=0; i<pointNumber; i++) {
		float theta = ofRandom(0, 2*pi);
		float amplitude = 100*pow(ofRandom(0, 1), 2);
		pointVector.push_back(ofPoint(sin(theta)*amplitude+x, cos(theta)*amplitude+y));
		colorVector.push_back(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)));
	}
	generateTheVoronoi();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
