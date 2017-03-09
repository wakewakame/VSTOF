#pragma once

#include "define.h"

//共有メモリアクセスクラス
class ShareMem {
public:
	//変数初期化
	HANDLE MapHandle = nullptr; //マップのハンドル
	ShareMemData *smd = nullptr; //共有メモリデータポインタ
	int ShareMemSize = sizeof(ShareMemData); //マップのサイズ

	//関数宣言
	~ShareMem();
	bool Open(LPCTSTR MapName);
};