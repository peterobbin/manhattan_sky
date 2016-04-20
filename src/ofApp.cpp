#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    std::string file = "nycbuidling.json";

    // Now parse the JSON - data source - Mapzen
    // Special thanks -> Weili 
    bool parsingSuccessful = result.open(file);

    if (parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << result.size();
       

    }
    else
    {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
    
    // buffering all the json date in to vectors
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

    // initializaing
    loading=false;
    loaded = false;
    ofRegisterURLNotification(this);
    averageColor = ofColor(0,0,0);


}

//--------------------------------------------------------------
void ofApp::urlResponse(ofHttpResponse & response){
    if(response.status==200 && response.request.name == "tsingy_forest"){
        img.load(response.data);
        loading=false;
    }else{
        cout << response.status << " " << response.error << endl;
        if(response.status!=-1) loading=false;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    float addUpR;
    float addUpG;
    float addUpB;
    float sumOfPoints;
    
    
    // calculating color when new image is loaded
    
    if(img.isAllocated() && loaded){
        loaded = false;
        
        for (int y = 0; y< img.getHeight()/2; y++) {
            for (int x = 0; x < img.getWidth(); x++) {
                addUpR += img.getColor(x, y).r;
                addUpG += img.getColor(x, y).g;
                addUpB += img.getColor(x, y).b;
                
                sumOfPoints += 1;
            }
        }
        averageColor.set(addUpR/sumOfPoints, addUpG/sumOfPoints, addUpB/sumOfPoints);
        cout<<averageColor<<endl;
    
    }
    
    
    
    
    
    
    
    

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(255);
    ofSetColor(0, 0, 0);

    
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
    
    
//    ofPushMatrix();
//    ofTranslate(ofGetWidth()/3, ofGetWidth()/3);
//    ofDrawBox(-200, -200, 0, 200, 200, 200);
//    ofDrawBox(200, -200, 0, 200, 200, 200);
//    ofDrawBox(-200, 200, 0, 200, 200, 200);
//    ofDrawBox(200, 200, 0, 200, 200, 200);
//    ofTranslate(-ofGetWidth()/3, -ofGetWidth()/3);
//    ofPopMatrix();
    
    float nearAverage = 0;
    for (int i = 0 ; i < nearest.size(); i ++){
        ofDrawRectangle(10 + 25 * i, 80, 20, nearest[i]);
        nearAverage = (nearAverage + nearest[i]) / nearest.size();
    }
    
    ofDrawBitmapString("average height ", 20, 40);
    ofDrawBitmapString(ofToString(nearAverage), 20, 60);
    

    
    ofSetColor(255, 255, 255);
    
    // color average with img and color
    
    if(img.isAllocated()){
        ofSetColor(0, 0, 0);
        
        
        ofDrawBitmapString("average sky color at the moment", 20, ofGetHeight() - 120);
        ofDrawBitmapString("#" + ofToString(averageColor.getHex()) , 20, ofGetHeight() - 140);
        
       
        ofSetColor(averageColor);
        ofDrawRectangle(20, ofGetHeight() - 100, 200, 50);
        ofSetColor(255);
        
        
        img.draw(20, ofGetWidth()/2, 150, 100);
    }
    
    
    
    


}

//--------------------------------------------------------------
void ofApp::exit() {
    ofUnregisterURLNotification(this);
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
        case ' ':
            img.clear();
            // photo credit from http://www.sheratontribecaview.com/
            ofLoadURLAsync("http://cam.sheratontribecaview.com/sheraton-tribeca-new-york-hotel.jpg","tsingy_forest");
            loading =true;
            loaded = true;
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