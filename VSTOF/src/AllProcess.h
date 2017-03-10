#pragma once

#include "Debug.h"
#include "define.h"
#include "ShareMem.h"
#include "Placement.h"

//全ての処理を行うクラス
class AllProcess {
public:
	//変数初期化
	WINDOW_INFO win_info; //ウィンドウ情報保持変数初期化

						  //コンストラクト
	ShareMem sm; //共有メモリクラス
	Draw draw; //描画クラス

					   //関数宣言
	bool init(LPSTR cmd, WINDOW_INFO win_info2);
	bool loop();
	//終了時処理
	~AllProcess();
};