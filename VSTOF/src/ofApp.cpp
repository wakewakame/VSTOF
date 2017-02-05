#include "ofApp.h"

ofApp::ofApp(int argc, LPSTR arg[]) {
	if (argc = 2) {
		lpCmdLine = arg[1];
	}else{
		ofExit(); //�ُ킪����ΏI��
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	//�E�B���h�E�T�C�Y���E�B���h�E���ێ��ϐ��ɑ��
	win_info.size.x = 854;
	win_info.size.y = 480;
	//�t���[�����[�g�ݒ�
	win_info.fps = 60;
	win_info.speed = 60.0 / double(win_info.fps);
	ofSetFrameRate(win_info.fps); //OF�̃t���[�����[�g�l�X�V
	//�E�B���h�E�n���h�����E�B���h�E���ێ��ϐ��ɑ��
	win_info.glDc = wglGetCurrentDC();
	win_info.hwnd = WindowFromDC(win_info.glDc);
	//�E�B���h�E�T�C�Y�ύX
	ofSetWindowShape(win_info.size.x, win_info.size.y);
	//�A���t�@�`�����l�����g�p�\��
	ofEnableAlphaBlending();
	//AllProcess������
	if (app.init(lpCmdLine, win_info)) {
		ofExit(); //�ُ킪����ΏI��
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	//���C�����[�`��
	if (app.loop()) {
		ofExit(); //�ُ킪����ΏI��
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	app.draw.gui.win_event.mouse.x = x;
	app.draw.gui.win_event.mouse.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	app.draw.gui.win_event.l_click = (button == 0);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (button == 0) {
		app.draw.gui.win_event.l_click = 0;
	}
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
