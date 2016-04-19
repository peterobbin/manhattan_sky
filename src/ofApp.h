#pragma once


#include "ofMain.h"
#include "ofxJSON.h"


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void urlResponse(ofHttpResponse & response);

    ofxJSONElement result;

    
    deque<float> nearest;
    
    
    
    deque<float> heights;
    deque<ofVec2f> coordinates;
    
    float offset_x;
    float offset_y;
    
    ofVec2f mousePosOrigin;
    
    

    
    ofColor averageColor;
    
    ofImage img;
    bool loading;
    bool loaded;

};
