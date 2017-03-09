#pragma once

#include "define.h"
#include "AllProcess.h"

class ofApp : public ofBaseApp {

public:
	//コンストラクタ
	ofApp(int argc, LPSTR arg[]); //コンソール引数取得

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	//FBOコンストラクト
	ofFbo fbo;

	//ウィンドウ情報保持変数初期化
	WINDOW_INFO win_info;

	//コンストラクト
	AllProcess app;

	//実行時引数文字列
	LPSTR lpCmdLine;
};