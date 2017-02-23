#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    gui.setup();
    gui.setPosition(GUI_OFFSET_X, 0);
    gui.setDefaultBackgroundColor(ofColor(20, 20, 20));
    guiLoop.setup();
    guiLoop.setPosition(GUI_OFFSET_X+gui.getWidth(), 0);
    guiLoop.setName("Loop");
    guiFadeIn.setup();
    guiFadeIn.setName("fade In");
    guiFadeOut.setup();
    guiFadeOut.setName("fade Out");
    guiMaxVolume.setup();
    guiMaxVolume.setName("max Volume");
    
    vc_StopChar.push_back('q');//1
    vc_StopChar.push_back('w');
    vc_StopChar.push_back('e');
    vc_StopChar.push_back('r');
    vc_StopChar.push_back('t');
    vc_StopChar.push_back('y');
    vc_StopChar.push_back('u');
    vc_StopChar.push_back('i');
    vc_StopChar.push_back('o');//9
    vc_StopChar.push_back('p');//0

    settings.loadFile("mySettings.xml");
    settings.pushTag("FILES");
    i_SoundNum = settings.getNumTags("FILE");
    cout << i_SoundNum << " file num"<< endl;
    
    i_SoundSetId = 0;
    
    ofxFloatSlider* volumeBuf;
    volumeBuf = new ofxFloatSlider[i_SoundNum];
    ofxToggle* toggleBuf;
    toggleBuf = new ofxToggle[i_SoundNum];
    
    ofxFloatSlider* fadeInBuf;
    fadeInBuf = new ofxFloatSlider[i_SoundNum];
    ofxFloatSlider* fadeOutBuf;
    fadeOutBuf = new ofxFloatSlider[i_SoundNum];
    
    ofxFloatSlider* maxVolumeBuf;
    maxVolumeBuf = new ofxFloatSlider[i_SoundNum];
    
    for(int i=0; i < i_SoundNum;i++){
        settings.pushTag("FILE", i);
        vs_SoundPath.push_back(ofToString(settings.getValue("PATH", "filepathdummy")));
        //ofxFloatSlider volumeBuf;
        char cbuf;
        cbuf = 'a' + i;
        vs_SoundTag.push_back(ofToString(cbuf)+":"+vs_SoundPath[i]);
        gui.add(volumeBuf[i].setup(vs_SoundTag[i], 0.0, 0.0, 1.0));
        guiLoop.add(toggleBuf[i].setup(vs_SoundTag[i],(bool)(settings.getValue("LOOP", 0))));
        guiFadeIn.add(fadeInBuf[i].setup(vs_SoundTag[i],(settings.getValue("FADE_IN_SEC", 0)),0.0,5.0));
        guiFadeOut.add(fadeOutBuf[i].setup(vs_SoundTag[i],(settings.getValue("FADE_OUT_SEC", 0)),0.0,5.0));
        guiMaxVolume.add(maxVolumeBuf[i].setup(vs_SoundTag[i],(settings.getValue("VOLUME", 1.0)),0.0,2.0));
        vb_GuiLoopState.push_back((bool)(settings.getValue("LOOP", 0)));
        ve_SoundState.push_back(SOUND_STATE_STOP);
        toggleBuf[i].setSize(toggleBuf[i].getHeight(), toggleBuf[i].getHeight());
        toggleBuf[i].setTextColor(ofColor(255, 255, 255, 0));
        fadeInBuf[i].setTextColor(ofColor(255, 255, 255, 0));
        fadeOutBuf[i].setTextColor(ofColor(255, 255, 255, 0));
        ofSoundPlayer soundBuf;
        soundBuf.load(vs_SoundPath[i]);
        soundBuf.setVolume(settings.getValue("VOLUME", 1.0));
        soundBuf.setLoop(toggleBuf[i]);
        vo_SoundPlayer.push_back(soundBuf);
        
        settings.popTag();
    }
    guiLoop.setSize(toggleBuf[0].getHeight(), guiLoop.getHeight());
    
    guiFadeIn.setPosition(guiLoop.getPosition()+ofPoint(guiLoop.getWidth(),0));
    guiFadeOut.setPosition(guiFadeIn.getPosition()+ofPoint(guiFadeIn.getWidth(),0));
    guiMaxVolume.setPosition(guiFadeOut.getPosition()+ofPoint(guiFadeOut.getWidth(),0));
    b_GuiDraw = true;
    b_Black=false;
    
    guiVideo.setup();

    guiVideo.add(videoShiftX.setup("ShiftX",0,-100,100));
    guiVideo.add(videoShiftY.setup("ShiftY",0,-100,100));
    guiVideo.add(videoRotateX.setup("RotateX",0,-10.0,10.0));
    guiVideo.add(videoRotateY.setup("RotateY",0,-10.0,10.0));
    guiVideo.setPosition(GUI_OFFSET_X,gui.getHeight());
    guiVideo.setDefaultBackgroundColor(ofColor(20, 20, 20));
    
    video.load("test.mp4");
    videoH=video.getHeight();
    videoW=video.getWidth();
}

//--------------------------------------------------------------
void ofApp::update(){
    //cout << gui.getFloatSlider("0:garden.mp3").getParameter()<<endl;
    //cout << gui.getFloatSlider("0:garden.mp3").getPosition().x <<endl;
    for(int i=0; i < i_SoundNum;i++){
        float volumeBuf;
        switch(ve_SoundState[i]){
            case SOUND_STATE_FADE_IN:
                volumeBuf = vo_SoundPlayer[i].getVolume()/guiMaxVolume.getFloatSlider(vs_SoundTag[i]);
                volumeBuf+= 1.0/(ofGetFrameRate()*guiFadeIn.getFloatSlider(vs_SoundTag[i]));
                if(volumeBuf>1.0){
                    volumeBuf=1.0;
                    ve_SoundState[i]=SOUND_STATE_PLAYING;
                }
                vo_SoundPlayer[i].setVolume(volumeBuf*guiMaxVolume.getFloatSlider(vs_SoundTag[i]));
                gui.getFloatSlider(vs_SoundTag[i])=volumeBuf;
                break;
            case SOUND_STATE_FADE_OUT:
                volumeBuf = vo_SoundPlayer[i].getVolume()/guiMaxVolume.getFloatSlider(vs_SoundTag[i]);
                volumeBuf-= 1.0/(ofGetFrameRate()*guiFadeOut.getFloatSlider(vs_SoundTag[i]));
                if(volumeBuf<0.0){
                    volumeBuf=0.0;
                    ve_SoundState[i]=SOUND_STATE_STOP;
                    vo_SoundPlayer[i].stop();
                }
                vo_SoundPlayer[i].setVolume(volumeBuf*guiMaxVolume.getFloatSlider(vs_SoundTag[i]));
                gui.getFloatSlider(vs_SoundTag[i])=volumeBuf;
                break;
            case SOUND_STATE_PLAYING:
                if(!vo_SoundPlayer[i].isPlaying()){
                    gui.getFloatSlider(vs_SoundTag[i])=0.0;
                    ve_SoundState[i]=SOUND_STATE_STOP;
                }
                break;
            default:
                break;
        }
    }
    loopCheckChanged();
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofPushMatrix();
    if(video.isPlaying()){
        ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
        ofTranslate(videoShiftX, videoShiftY);
        ofRotateY(videoRotateY);
        ofRotateX(videoRotateX);
        video.draw(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2, ofGetWindowWidth(), ofGetWindowWidth()*videoH/videoW);
    }
    ofPopMatrix();
    
    if(b_GuiDraw){
        ofShowCursor();
        ofEnableAlphaBlending();
        gui.draw();
        guiLoop.draw();
        guiFadeIn.draw();
        guiFadeOut.draw();
        guiMaxVolume.draw();
        guiVideo.draw();
        int startIdx;
        startIdx = i_SoundSetId * KEY_NUM;
        ofPushStyle();
        ofSetColor(255, 0, 0);
        for(int i=0; i< KEY_NUM; i++){
            if((startIdx + i)< i_SoundNum){
                int guiPosX = gui.getFloatSlider(vs_SoundTag[startIdx+i]).getPosition().x - 15;
                int guiPosY = gui.getFloatSlider(vs_SoundTag[startIdx+i]).getPosition().y + 12;
                ofDrawBitmapString((i+1)%10, guiPosX, guiPosY);
            }
        }
        ofSetColor(255);
        for(int i=0; i< i_SoundNum; i++){
            int guiPosX = guiMaxVolume.getFloatSlider(vs_SoundTag[i]).getPosition().x + guiFadeOut.getWidth()+5;
            int guiPosY = guiMaxVolume.getFloatSlider(vs_SoundTag[i]).getPosition().y + 12;
            ofDrawBitmapString(ve_SoundState[i], guiPosX, guiPosY);
        }
        ofPopStyle();
    }else{
        ofHideCursor();
    }
    if(b_Black){
        ofClear(0);
        ofHideCursor();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'd':
            b_GuiDraw = !b_GuiDraw;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'b':
            b_Black = !b_Black;
            break;
        case 'm':
            video.play();
            break;
        case 's':
            video.stop();
            break;
        case OF_KEY_DOWN:
            i_SoundSetId = min(i_SoundSetId+1,(int)(i_SoundNum/KEY_NUM));
            break;
        case OF_KEY_UP:
            i_SoundSetId = max(i_SoundSetId-1,0);
            break;
    }
    int soundIdx;
    for(int i=0;i<KEY_NUM;i++){
        soundIdx = i+i_SoundSetId*KEY_NUM;
        if(soundIdx < i_SoundNum){
            if(key == ('0' + ((i+1)%KEY_NUM))){
                //cout<<i<<endl;
                vo_SoundPlayer[soundIdx].play();
                if(guiFadeIn.getFloatSlider(vs_SoundTag[soundIdx])==0){
                    cout << "CI" << vs_SoundTag[soundIdx] <<endl;
                    gui.getFloatSlider(vs_SoundTag[soundIdx])=1.0;
                    vo_SoundPlayer[soundIdx].setVolume(1.0*guiMaxVolume.getFloatSlider(vs_SoundTag[soundIdx]));
                    ve_SoundState[soundIdx]=SOUND_STATE_PLAYING;
                }else{
                    cout << "FI" << vs_SoundTag[soundIdx] <<endl;
                    vo_SoundPlayer[soundIdx].setVolume(0.0);
                    ve_SoundState[soundIdx]=SOUND_STATE_FADE_IN;
                }
            }
            if(key == (vc_StopChar[i])){
                cout<<vc_StopChar[i]<<endl;
                if(guiFadeOut.getFloatSlider(vs_SoundTag[soundIdx])==0){
                    cout << "CO" << vs_SoundTag[soundIdx] <<endl;
                    vo_SoundPlayer[soundIdx].stop();
                    gui.getFloatSlider(vs_SoundTag[soundIdx])=0.0;
                    ve_SoundState[soundIdx]=SOUND_STATE_STOP;
                }else{
                    cout << "FO" << vs_SoundTag[soundIdx] <<endl;
                    ve_SoundState[soundIdx]=SOUND_STATE_FADE_OUT;
                }
            }
        }
    }
}

void ofApp::loopCheckChanged(){
    for(int i=0; i < i_SoundNum;i++){
        bool togglebuf = guiLoop.getToggle(vs_SoundTag[i]);
        if(vb_GuiLoopState[i] != togglebuf){
            vo_SoundPlayer[i].setLoop(togglebuf);
            vb_GuiLoopState[i] = togglebuf;
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
