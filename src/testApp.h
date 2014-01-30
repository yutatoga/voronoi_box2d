#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "TextureShape.h"
#include "ofxVoronoi.h"

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
	void generateTheVoronoi();	//voronoi
	
	
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
	
	vector<ofMesh> meshVector;
	
	
};
