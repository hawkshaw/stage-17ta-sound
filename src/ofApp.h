#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

#define KEY_NUM 10
#define GUI_OFFSET_X 20

enum SOUND_STATE
{
    SOUND_STATE_STOP = 0,
    SOUND_STATE_FADE_IN,
    SOUND_STATE_PLAYING,
    SOUND_STATE_FADE_OUT,
};

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void loopCheckChanged();
		
    ofxPanel gui;
    ofxPanel guiLoop;
    ofxPanel guiFadeIn;
    ofxPanel guiFadeOut;
    vector<bool> vb_GuiLoopState;
    
    vector<string> vs_SoundPath;
    vector<string> vs_SoundTag;
    vector<ofSoundPlayer> vo_SoundPlayer;
    vector<char> vc_StopChar;
    vector<SOUND_STATE> ve_SoundState;
    int i_SoundNum;
    int i_SoundSetId;
    
    ofxFloatSlider volumeBuf0;
    ofxFloatSlider volumeBuf1;
    ofxFloatSlider volumeBuf2;
    bool b_GuiDraw;
    
    bool b_Black;
    
    ofxXmlSettings settings;
    
    ofVideoPlayer video;
    int videoW,videoH;
    ofxPanel guiVideo;
    ofxIntSlider videoShiftX;
    ofxIntSlider videoShiftY;
    ofxFloatSlider videoRotateX;
    ofxFloatSlider videoRotateY;
};
