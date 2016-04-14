#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    std::string file = "nycbuidling.json";

    // Now parse the JSON
    bool parsingSuccessful = result.open(file);

    if (parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << result.size();
       

    }
    else
    {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
    
     for (Json::ArrayIndex i = 0; i < result.size() - 1; i += 1){
         
         ofVec2f diff = ofVec2f(result[i]["coordinates"][0].asFloat(),result[i]["coordinates"][1].asFloat()) - ofVec2f(result[i+1]["coordinates"][0].asFloat(),result[i+1]["coordinates"][1].asFloat());
         float diffLength = diff.length();
    
         if (diffLength > 0.00005){
             heights.push_back(result[i]["height"].asFloat());
             ofVec2f coord = ofVec2f((result[i]["coordinates"][0].asFloat() + 75)* 100,(result[i]["coordinates"][1].asFloat() - 39)* 100);
             coord.set(coord.x - 97, coord.y - 170);
             coord.set(coord.x * 100, coord.y * 100);
             coordinates.push_back(coord);
         }
         
     }




}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(255);
    ofSetColor(0, 0, 0);
    
    
    
    

    
    
    
    // draw directly from json
    /*
    for (Json::ArrayIndex i = 0; i < result.size() - 1; i += 2){
        std::string height  = ofToString(result[i]["height"]);
        std::string coordinateX  = ofToString(result[i]["coordinates"][0]);
        std::string coordinateY  = ofToString(result[i]["coordinates"][1]);
        float heightNum = ofToFloat(height);
        ofVec2f coordVec= ofVec2f((ofToFloat(coordinateX) + 75)* 100, ((ofToFloat(coordinateY))-39)* 100);
        coordVec.set(coordVec.x - 97, coordVec.y - 170);
        coordVec.set(coordVec.x * 100, coordVec.y * 100);
        
        //float xDiff = abs(ofToFloat(ofToString(result[i]["coordinates"][0])) - ofToFloat(ofToString(result[i + 1]["coordinates"][0])));
        //cout<<xDiff<<endl;
        
        ofVec2f mousePos = ofVec2f(ofGetMouseX(), ofGetMouseY());
        ofVec2f diff = coordVec - mousePos;
        
        //cout<<diff<<endl;
        if (diff.length() < 4)
        nearest.push_back(heightNum * 2);
        if (nearest.size() > 4){
            nearest.pop_front();
        }
        
        
        if (heightNum > 15.0){
            
            ofDrawRectangle(coordVec.x + offset_x, coordVec.y + offset_y, 2, heightNum * 0.1);
        }
    }
    
     */
    
    
    for (int i = 0; i < heights.size() ; i ++){
        
        
        
        // needs optimization, probably drawing to fbo first? so dont have to draw everytime
        ofVec2f mousePos = ofVec2f(ofGetMouseX(), ofGetMouseY());
        ofVec2f diff = ofVec2f(coordinates[i].x + offset_x, coordinates[i].y + offset_y )- mousePos;
        
        // cout<<diff<<endl;
        if (nearest.size() > 4){
            nearest.pop_front();
        }
        
        ofSetColor(0, 0, 0, heights[i] * 3.0);
        // right now is just bouncing between this range, but not he nearset
        if (diff.length() < 4){
            nearest.push_back(heights[i] * 2);
        }
        
        if (diff.length() < 20){
            ofSetColor(100 + heights[i], 0, 0, heights[i] * 3.0);
        }
        
        // draw the map
        ofDrawRectangle(coordinates[i].x + offset_x, coordinates[i].y + offset_y, 2, heights[i] * 0.1);

        
    }
    
    ofSetColor(0, 0, 0);
    
    
    ofDrawBitmapString("fps" + ofToString(ofGetFrameRate()), 20, 20);
    
    
    
    
    float nearAverage = 0;
    for (int i = 0 ; i < nearest.size(); i ++){
        ofDrawRectangle(10 + 25 * i, 80, 20, nearest[i]);
        nearAverage = (nearAverage + nearest[i]) / nearest.size();
    }
    
    ofDrawBitmapString("average height ", 20, 40);
    ofDrawBitmapString(ofToString(nearAverage), 20, 60);
    

    
    ofSetColor(255, 255, 255);
    
    


}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case OF_KEY_UP:
            offset_y += 5.0;
            break;
        case OF_KEY_DOWN:
            offset_y -= 5.0;
            break;
        case OF_KEY_LEFT:
            offset_x += 5.0;
            break;
        case OF_KEY_RIGHT:
            offset_x -= 5.0;
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
    ofVec2f mousePosDiff = ofVec2f((mousePosOrigin.x - x) * 0.1, (mousePosOrigin.y - y) * 0.1);
    offset_x -= mousePosDiff.x;
    offset_y -= mousePosDiff.y;
  

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mousePosOrigin.set(x, y);
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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