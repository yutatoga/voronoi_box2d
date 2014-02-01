#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "TextureShape.h"
#include "ofxVoronoi.h"

//衝突を検出するためのリスナーのクラス
class MyContactListener : public b2ContactListener {
	
public:
	MyContactListener();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	
	ofSoundPlayer mySound;
};


class testApp : public ofBaseApp{
	
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
	//voronoi
	void generateTheVoronoi();
	//filter
	void loadLUT(string path);
	void applyLUT(ofPixelsRef pix);

	
	ofxBox2d box2d;
	
	// a vector of all the texture images
	vector <ofImage> textures;
	
	// a vector of all the texture shapes
	vector <ofPtr<TextureShape> > shapes;
	
	//voronoi
	ofRectangle      bounds;
	vector <ofPoint> pointVector;
	vector <ofColor> colorVector;
	ofxVoronoi       voronoi;
	
	//video grabber
	ofVideoGrabber videoGrabber;
	int videoGrabberWidth;
	int videoGrabberHeight;
	int videoDrawWidth;
	int videoDrawHeight;
	//filter
	bool doLUT;
	int dirLoadIndex;
	ofPoint lutPos;
	ofPoint thumbPos;
	bool LUTloaded;
	ofVec3f lut[32][32][32];
	ofImage lutImg;
	
	//sound
	MyContactListener contacts; //衝突感知のリスナー
	ofSoundPlayer playerMouseClick;
	
	bool firstMouseClick;
	bool secondMouseClick;
};
