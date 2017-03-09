#include "ShareMem.h"

bool ShareMem::Open(LPCTSTR MapName) { //共有メモリアクセス関数
	if (MapName == nullptr) { //コンソール引数がなければエラー
		return 1;
	}
	MapHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapName); //ファイル名からマップハンドル取得
	if (MapHandle == NULL) { //マップが存在していなければエラー
		return 1;
	}
	smd = (ShareMemData *)MapViewOfFile(MapHandle, FILE_MAP_ALL_ACCESS, 0, 0, ShareMemSize); //マップ内容同期
	if (smd == NULL) { //マップ内容取得ができていなければエラー
		return 1;
	}
	return 0;
}

ShareMem::~ShareMem() { //共有メモリクローズ
	UnmapViewOfFile(smd); //共有メモリデータポインタ開放
}