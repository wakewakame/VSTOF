#include "ofApp.h"

ofApp::ofApp(int argc, LPSTR arg[]) {
	if (argc = 2) {
		lpCmdLine = arg[1];
	}else{
		ofExit(); //異常があれば終了
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	//ウィンドウサイズをウィンドウ情報保持変数に代入
	win_info.size.x = 854;
	win_info.size.y = 480;
	//フレームレート設定
	win_info.fps = 60;
	win_info.speed = 60.0 / double(win_info.fps);
	ofSetFrameRate(win_info.fps); //OFのフレームレート値更新
	//ウィンドウハンドルをウィンドウ情報保持変数に代入
	win_info.glDc = wglGetCurrentDC();
	win_info.hwnd = WindowFromDC(win_info.glDc);
	//ウィンドウサイズ変更
	ofSetWindowShape(win_info.size.x, win_info.size.y);
	//アルファチャンネルを使用可能に
	ofEnableAlphaBlending();
	//AllProcess初期化
	if (app.init(lpCmdLine, win_info)) {
		ofExit(); //異常があれば終了
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	//メインルーチン
	if (app.loop()) {
		ofExit(); //異常があれば終了
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
