#pragma once

#include "ofMain.h"
#include "windows.h"
#include <vector>
#include <string>

#include "GUI.h"

//ウィンドウ情報保持変数
struct WINDOW_INFO {
	POINT size; //ウィンドウサイズ
	POINT b_size; //前フレームのウィンドウサイズ
	RECT window; //ウィンドウ左上右下座標(デスクトップ基準)
	HDC glDc; //OpenFrameworksのOpenGLのデバイスコンテキスト
	HWND hwnd; //OpenFrameworksの生成ウィンドウのハンドル
	int fps; //ウィンドウの描画フレームレート
	double speed; //60fpsを基準としたフレーム単位のアニメーション速度
};

//ユーティリティクラス
class Utility {
public:
	//割合変換関数(戻り値:変換後,引数1~5:変換前の値,変換前の最小値,変換前の最大値,変換後の最小値,変換後の最大値)
	static double Proportion(double i, double i_mix, double i_max, double j_min, double j_max) {
		double j;
		j = j_min + (i - i_mix) * ((j_max - j_min) / (i_max - i_mix));
		return j;
	}
	static int Proportion(int i, int i_mix, int i_max, int j_min, int j_max) {
		int j;
		j = int(Proportion(double(i), double(i_mix), double(i_max), double(j_min), double(j_max)));
		return j;
	}
};

//全ての処理を行うクラス
class AllProcess {
public:
	//変数初期化
	WINDOW_INFO win_info; //ウィンドウ情報保持変数初期化

						  //コンストラクト
	ShareMem sm; //共有メモリクラス
	Draw draw; //描画クラス
	std::string title; //ウィンドウタイトル文字列

			   //関数宣言
	bool init(LPSTR cmd, WINDOW_INFO win_info2) { //初期化時実行関数
												  //変数代入
		win_info = win_info2;
		//共有メモリ関連
		if (sm.Open(cmd)) {
			return 1;
		}
		if (sm.smd->Ready != nullptr) { //多重起動防止
			return 1;
		}
		else {
			sm.smd->Ready = win_info.hwnd;
		}
		//Draw関連
		if (draw.resize(win_info)) { //使用中のウィンドウサイズ更新
			return 1;
		}
		//その他
		//共有メモリデータをパラメータクラスの変数と同期
		draw.para.p_value = &sm.smd->para;
		return 0;
	}
	bool loop() { //ループ中に呼び出す関数
				  //Draw関連
				  //前フレームとウィンドウサイズ比較
		if (
			(win_info.size.x != win_info.b_size.x) ||
			(win_info.size.y != win_info.b_size.y)
			) {
			if (draw.resize(win_info)) { //使用中のウィンドウサイズ更新
				return 1;
			}
		}
		//GUI描画
		draw.loop();
		///debug///
		title = "";
		title += "fps:";
		title += std::to_string(ofGetFrameRate());
		SetWindowText(win_info.hwnd, (LPSTR)title.c_str());
		///debug///
		return 0;
	}
	//終了時処理
	~AllProcess() {
		if (sm.smd != nullptr) {
			sm.smd->Ready = nullptr;
		}
	}
};

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