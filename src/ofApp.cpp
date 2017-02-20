#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    vc_StopChar.push_back('p');//0
    vc_StopChar.push_back('q');//1
    vc_StopChar.push_back('w');
    vc_StopChar.push_back('e');
    vc_StopChar.push_back('r');
    vc_StopChar.push_back('t');
    vc_StopChar.push_back('y');
    vc_StopChar.push_back('u');
    vc_StopChar.push_back('i');
    vc_StopChar.push_back('o');//9

    vs_SoundPath.push_back("garden.mp3");
    vs_SoundPath.push_back("morato.mp3");
    vs_SoundPath.push_back("thema.wav");
    
    vs_SoundPath.push_back("garden.mp3");
    vs_SoundPath.push_back("morato.mp3");
    
    vs_SoundPath.push_back("thema.wav");
    vs_SoundPath.push_back("garden.mp3");
    vs_SoundPath.push_back("morato.mp3");
    vs_SoundPath.push_back("thema.wav");
    vs_SoundPath.push_back("garden.mp3");
    
    vs_SoundPath.push_back("morato.mp3");
    vs_SoundPath.push_back("thema.wav");
    vs_SoundPath.push_back("garden.mp3");
    vs_SoundPath.push_back("morato.mp3");
    vs_SoundPath.push_back("thema.wav");
    /*vs_SoundPath.push_back("garden.mp3");
    vs_SoundPath.push_back("morato.mp3");
    vs_SoundPath.push_back("thema.wav");
*/
    gui.setup();

    i_SoundNum = vs_SoundPath.size();
    cout << i_SoundNum << " file num"<< endl;
    i_SoundSetId = 0;
    
    ofxFloatSlider* volumeBuf;
    volumeBuf = new ofxFloatSlider[FILE_NUM];
    
    for(int i=0; i < i_SoundNum;i++){
        ofSoundPlayer soundBuf;
        soundBuf.load(vs_SoundPath[i]);
        soundBuf.setVolume(1.0);
        vo_SoundPlayer.push_back(soundBuf);
        //ofxFloatSlider volumeBuf;
        gui.add(volumeBuf[i].setup(ofToString(i)+":"+vs_SoundPath[i], 0.0, 0.0, 1.0));
        //gui.add(volumeBuf.setup(vs_SoundPath[i], 0.0, 0.0, 1.0));
    }
    
    b_GuiDraw = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    cout << gui.getFloatSlider("0:garden.mp3").getParameter()<<endl;
    cout << gui.getFloatSlider("0:garden.mp3").getPosition().x <<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(b_GuiDraw){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'd':
            b_GuiDraw = !b_GuiDraw;
            break;
    }
    for(int i=0;i<KEY_NUM;i++){
        if(key == ('0'+i)){
            cout<<i<<endl;
            vo_SoundPlayer[i+i_SoundSetId*KEY_NUM].play();
        }
        if(key == (vc_StopChar[i])){
            cout<<vc_StopChar[i]<<endl;
            vo_SoundPlayer[i+i_SoundSetId*KEY_NUM].stop();
        }
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
