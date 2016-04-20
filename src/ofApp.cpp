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
        
        // writing some stuff
        ofDrawBitmapString("average sky color at the moment", 20, ofGetHeight() - 240);
        ofDrawBitmapString("#" + ofToString(averageColor.getHex()) , 20, ofGetHeight() - 230);
        
        // getting relative coordinates
        ofVec2f popupOrigin = ofVec2f(20, ofGetHeight() - 220);
        ofVec2f popupMaxCoor = popupOrigin + ofVec2f(300, 200);
        
        
        // drawing the background
        ofSetColor(averageColor);
        ofDrawRectangle(popupOrigin, 300, 200);
        ofSetColor(255);
        
        if (nearest.size() != 0){
            
            
            // setup bg
            ofRectangle bgRect = ofRectangle(popupOrigin, 300, 200);
            
            //**** firstRect -----------------------------
            
            ofRectangle rectA = ofRectangle(popupOrigin + ofVec2f(nearest[0] * 1.5, nearest[0]* 0.5) * 0.1, 100, 100);
            ofRectangle rectAO = ofRectangle(popupOrigin, 80, 80);
            
            float offset = ofVec2f(rectA.getMaxX() - popupOrigin.x, rectA.getMaxY() - popupOrigin.y).length();
            offset = ofMap(offset, 0, 250, 120, 60);
            
            rectA.setSize(offset , offset);
            ofSetColor(0, 0, 0);
            ofDrawRectangle(rectA);
           
           // ofDrawRectangle(rectAO);
            
            
            ofBeginShape();
            ofVertex(popupOrigin.x, popupOrigin.y);
            ofVertex(rectAO.getMaxX(), popupOrigin.y);
            ofVertex(rectA.getMaxX(), rectA.getMinY());
            ofVertex(rectA.getMinX(), rectA.getMaxY());
            ofVertex(popupOrigin.x, rectAO.getMaxY());
           
            ofEndShape();
            
            //*** secondRect -----------------------------
            
            ofRectangle rectB = ofRectangle(ofVec2f(popupMaxCoor.x - 100, popupOrigin.y) + ofVec2f(-nearest[1] * 1.5, nearest[1]* 0.5) * 0.1, 100, 100);
            ofRectangle rectBO = ofRectangle(bgRect.getTopRight() - ofVec2f(80, 0), 80, 80);
            float offsetB = ofVec2f(bgRect.getTopRight() - rectB.getBottomLeft()).length();
            offsetB = ofMap(offsetB, 0, 250, 120, 60);
            rectB.setSize(offsetB, offsetB);
            
            rectB.setX(rectB.x - (offsetB - 100));
            
            ofDrawRectangle(rectB);
            ofDrawRectangle(rectBO);
            ofBeginShape();
            ofVertex(bgRect.getTopRight().x, bgRect.getTopRight().y);
            ofVertex(rectBO.getTopLeft().x, rectBO.getTopLeft().y);
            ofVertex(rectB.getTopLeft().x, rectB.getTopLeft().y);
            ofVertex(rectB.getBottomRight().x, rectB.getBottomRight().y);
            ofVertex(rectBO.getBottomRight().x, rectBO.getBottomRight().y);
            ofEndShape();
            
            
            //*** thirdRect -----------------------------
            ofRectangle rectC = ofRectangle(ofVec2f(popupOrigin.x, popupMaxCoor.y - 100) + ofVec2f(nearest[2] * 1.5, -nearest[2]* 0.5) * 0.1, 100, 100);
            ofRectangle rectCO = ofRectangle(bgRect.getBottomLeft() - ofVec2f(0, 80), 80, 80);
            
            
            float offsetC = ofVec2f(bgRect.getBottomLeft() - rectC.getTopRight()).length();
            offsetC = ofMap(offsetC, 0, 250, 120, 60);
 
            rectC.setSize(offsetC, offsetC);
            rectC.setY(rectC.y + (100 - offsetC));
            
            
            ofDrawRectangle(rectC);
            ofDrawRectangle(rectCO);
            
            ofBeginShape();
            ofVertex(bgRect.getBottomLeft().x, bgRect.getBottomLeft().y);
            ofVertex(rectCO.getTopLeft().x, rectCO.getTopLeft().y);
            ofVertex(rectC.getTopLeft().x, rectC.getTopLeft().y);
            ofVertex(rectC.getBottomRight().x, rectC.getBottomRight().y);
            ofVertex(rectCO.getBottomRight().x, rectCO.getBottomRight().y);
            ofEndShape();
            
            
            
            
            //*** fourthRect -----------------------------
            ofRectangle rectD = ofRectangle(ofVec2f(popupMaxCoor.x - 100, popupMaxCoor.y - 100) - ofVec2f(nearest[3] * 1.5, nearest[3]* 0.5) * 0.1, 100, 100);
            ofRectangle rectDO = ofRectangle(bgRect.getBottomRight() - ofVec2f(80, 80), 80, 80);

            float offsetD = ofVec2f(rectD.getTopLeft() - bgRect.getBottomRight()).length();
            offsetD = ofMap(offsetD, 0 , 250, 120, 60);
            rectD.setSize(offsetD, offsetD);
            
            rectD.setX(rectD.x + (100 - offsetD));
            rectD.setY(rectD.y + (100 - offsetD));
            
            
            ofBeginShape();
            ofVertex(bgRect.getBottomRight().x, bgRect.getBottomRight().y);
            ofVertex(rectDO.getTopRight().x, rectDO.getTopRight().y);
            ofVertex(rectD.getTopRight().x, rectD.getTopRight().y);
            ofVertex(rectD.getBottomLeft().x, rectD.getBottomLeft().y);
            ofVertex(rectDO.getBottomLeft().x, rectDO.getBottomLeft().y);
            
            ofEndShape();

            
            
            ofDrawRectangle(rectD);
            
            
            
        }
        
        
        
        
        
        ofSetColor(255);
        img.draw(20, ofGetWidth()/2 - 20, 90, 60);
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