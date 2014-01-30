#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFullscreen(true);
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
	
	//video grabber
	videoGrabberWidth = 240;
	videoGrabberHeight = 180;
	float horizontalZoomRatio = ofGetWidth()/1280.0;
	float verticalZoomRatio = ofGetHeight()/960.0;
	if (horizontalZoomRatio > verticalZoomRatio) {
		videoDrawWidth = ofGetWidth();
		videoDrawHeight = 960.0*ofGetWidth()/1280.0;
	}else{
		videoDrawHeight = ofGetHeight();
		videoDrawWidth = 1280.0*ofGetHeight()/960.0;
	}

	
	videoGrabber.setVerbose(true);
	videoGrabber.initGrabber(videoGrabberWidth,videoGrabberHeight);
	//filer
	// find all filter files and load them
	dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
	if (dir.size()>0) {
		dirLoadIndex = 7; //select filter here
		loadLUT(dir.getPath(dirLoadIndex));
		doLUT = true;
	}else{
		doLUT = false;
	}
	lutImg.allocate(videoGrabberWidth, videoGrabberHeight, OF_IMAGE_COLOR);
	
	thumbPos.set(lutImg.getWidth()*0.5f-80, -lutImg.getHeight()*0.5f - 60, 0);
	lutPos.set(-lutImg.getWidth()*0.5f, -lutImg.getHeight()*0.5f, 0);
}

//--------------------------------------------------------------
void testApp::update(){
	//voronoi
	generateTheVoronoi();
	
	//box2d
	box2d.update();
	
	//video grabber
	videoGrabber.update();
	//filter
	if (doLUT) {
		if (videoGrabber.isFrameNew()){
			applyLUT(videoGrabber.getPixelsRef());
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	//video grabber
	//videoGrabber.draw((ofGetWidth()-videoGrabberWidth)/2.0, (ofGetHeight()-videoGrabberHeight)/2.0);
	
	if(doLUT){
		ofPushMatrix();
//		ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f, 0);
		lutImg.draw((ofGetWidth()-videoDrawWidth)/2.0, (ofGetHeight()-videoDrawHeight)/2.0, videoDrawWidth, videoDrawHeight);
//		ofRect(thumbPos.x-3, thumbPos.y-3, 166, 126);
//		videoGrabber.draw(thumbPos.x, thumbPos.y, 160, 120);
		ofPopMatrix();
	}else {
		videoGrabber.draw(0,0);
	}

	
	
	
	//draw voronoi
//	ofPushStyle();
//	for(int i=0; i<voronoi.cells.size(); i++) {
//		ofPath path;
//		path.setFillColor(colorVector[i]);
//		path.setStrokeColor(ofColor(255, 255, 255));
//		path.setStrokeWidth(1);
//		for(int j=0; j<voronoi.cells[i].pts.size(); j++) {
//			path.lineTo(voronoi.cells[i].pts[j]);
//		}
//		path.draw();
//	}
//	ofPopStyle();
	
	//draw the raw points
//	ofPushStyle();
//	ofSetColor(255);
//	for(int i=0; i<pointVector.size(); i++) {
//		ofCircle(pointVector[i], 3);
//	}
//	ofPopStyle();
	
	// box2d
	ofPushStyle();
	for (int i=0; i<shapes.size(); i++) {
		shapes[i].get()->draw();
	}
	ofPopStyle();
	
	// some debug information
	string info = "Textures from subtlepatterns.com\n";
	info += "Press c to clear everything\n";
		
	//guideline
	ofPushStyle();
	ofSetColor(0, 255, 255);
	ofDrawBitmapString(info, 10, 15);
	ofDrawBitmapString("fps   : "+ofToString(ofGetFrameRate()), 10, 50);
	ofDrawBitmapString("point : "+ofToString(pointVector.size()), 10, 60);
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
void testApp::loadLUT(string path){
	LUTloaded=false;
	
	ofFile file(path);
	string line;
	for(int i = 0; i < 5; i++) {
		getline(file, line);
		ofLog() << "Skipped line: " << line;
	}
	for(int z=0; z<32; z++){
		for(int y=0; y<32; y++){
			for(int x=0; x<32; x++){
				ofVec3f cur;
				file >> cur.x >> cur.y >> cur.z;
				lut[x][y][z] = cur;
			}
		}
	}
	
	LUTloaded = true;
}

//--------------------------------------------------------------
void testApp::applyLUT(ofPixelsRef pix){
	if (LUTloaded) {
		
		for(int y = 0; y < pix.getHeight(); y++){
			for(int x = 0; x < pix.getWidth(); x++){
				
				ofColor color = pix.getColor(x, y);
				
				int lutPos [3];
				for (int m=0; m<3; m++) {
					lutPos[m] = color[m] / 8;
					if (lutPos[m]==31) {
						lutPos[m]=30;
					}
				}
				
				ofVec3f start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				ofVec3f end = lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1];
				
				for (int k=0; k<3; k++) {
					float amount = (color[k] % 8) / 8.0f;
					color[k]= (start[k] + amount * (end[k] - start[k])) * 255;
				}
				
				lutImg.setColor(x, y, color);
				
			}
		}
		
		lutImg.update();
	}
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
	// add some circles every so often
	for (int i =0; i<voronoi.cells.size(); i++) {
		shapes.push_back(ofPtr<TextureShape>(new TextureShape));
		shapes.back().get()->setTexture(&textures[(int)ofRandom(textures.size())]);
		shapes.back().get()->setup(box2d, (ofGetWidth()/2)+ofRandom(-20, 20), 130, ofRandom(10, 50), voronoi.cells[i].pts);
	}
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
