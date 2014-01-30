//
//  TextureShape.h
//  example-ShapeTexturing
//
//  Created by Todd Vanderlin on 7/16/13.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxVoronoi.h"

//static int hexColors[4] = {0x31988A, 0xFDB978, 0xFF8340, 0xE8491B};
static int hexColors[4] = {0x333333, 0x222222, 0x111111, 0x00000};
class TextureShape {
  
public:
	
	ofImage    *    texturePtr;
	ofMesh          mesh;
	ofColor         color;
	ofxBox2dPolygon polyShape;
	
	TextureShape() {
		texturePtr = NULL;
	}
	void setup(ofxBox2d &world, float cx, float cy, float r, vector<ofVec2f> pts) {
		
		//色指定
		color.setHex(hexColors[(int)ofRandom(4)]);

		
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		//ここで、点をつくる。
		//ランダム多角形の場合
//		int   nPts  = 8;
		float scale = r / (float)texturePtr->getWidth();
//		for (int i=0; i<nPts; i++) {
//			float n = ofMap(i, 0, nPts-1, 0.0, TWO_PI);
//			float x = cos(n);
//			float y = sin(n);
//			float d = ofRandom(-r/2, r/2);
//			polyShape.addVertex(ofPoint(cx + (x * r + d), cy + (y * r + d)));
//			mesh.addTexCoord(ofPoint(0, 0));
//			mesh.addTexCoord(ofPoint(x * scale, y * scale));
//		}
		
		//ボロノイ図の場合
		ofLog(OF_LOG_NOTICE, "toga:"+ofToString(pts.size()));
		ofPushStyle();
		  int nPts = pts.size();
			for(int i=0; i<nPts; i++) {
				float n = ofMap(i, 0, nPts-1, 0.0, TWO_PI);
				float x = cos(n);
				float y = sin(n);
				float d = ofRandom(-r/2, r/2);
				polyShape.addVertex(pts[i].x, pts[i].y);
				mesh.addTexCoord(ofPoint(0, 0));
				mesh.addTexCoord(ofPoint(x * scale, y * scale));
				ofLog(OF_LOG_NOTICE, "OK loop:"+ofToString(i));
			}
		ofLog(OF_LOG_NOTICE, "OK 1");
		polyShape.setPhysics(0.3, 0.5, 0.1);
		ofLog(OF_LOG_NOTICE, "OK 2");
		polyShape.create(world.getWorld());
		ofLog(OF_LOG_NOTICE, "OK 3");
		ofPopStyle();
		ofLog(OF_LOG_NOTICE, "OK FINAL");
		
	}
	void setTexture(ofImage * texture) {
		texturePtr = texture;
		texturePtr->getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		texturePtr->getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
	}
	void draw() {
		
		mesh.clearVertices();
		vector<ofPoint> &pts = polyShape.getPoints();
		ofPoint center       = polyShape.getCentroid2D();
		for (int i=0; i<pts.size(); i++) {
			mesh.addVertex(center);
			mesh.addVertex(pts[i]);
		}
		mesh.addVertex(center);
		mesh.addVertex(pts.front());
		
		ofSetColor(color);
		texturePtr->bind();
		mesh.draw();
		texturePtr->unbind();
		
	}
	
};