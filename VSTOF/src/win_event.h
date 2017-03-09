#pragma once

#include "define.h"

//ウィンドウイベント処理クラス
class WIN_EVENT {
public:
	//変数宣言
	WindowPos mouse; //現在のマウス座標
	WindowPos b_mouse; //前フレーム時のマウス座標
	bool l_click; //現在の左クリック情報(押されていたら1)
	bool b_l_click; //前フレームのl_click変数の内容

	//関数宣言
	//コンストラクト
	WIN_EVENT();
	//指定RECT内に存在するかどうか
	bool in(RECT area);
	bool in(POINT pos, int size);
	//マウスが左クリックされたかどうか
	bool get_l_click();
	//指定RECT内でマウスがクリックされたかどうか
	bool l_click_in(RECT area);
	//毎フレーム呼び出し関数(処理の最後に呼び出す)
	void loop();
};