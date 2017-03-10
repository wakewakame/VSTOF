#include "AllProcess.h"

bool AllProcess::init(LPSTR cmd, WINDOW_INFO win_info2) { //初期化時実行関数
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
	draw.set_p_value(&sm.smd->para);
	return 0;
}

bool AllProcess::loop() { //ループ中に呼び出す関数
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
	Debug::SetFpsTitle(win_info);
	return 0;
}

AllProcess::~AllProcess() {
	if (sm.smd != nullptr) {
		sm.smd->Ready = nullptr;
	}
}