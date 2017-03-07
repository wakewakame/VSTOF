#include "ShareMem.h"

//音色情報
struct VSTParameteres { //パラメーター値構造体
						//音色系
	bool make_auto; //自動で音色を生成するか
	bool use_rawwave; //生波形データの使用をするかどうか
	bool change_natural; //音程変化を自然にするか
	float overtones[512]; //倍音グラフor共鳴スペクトルグラフ
	int iovertones; //倍音の数
					//生波形データ派生
	float rawwave[4096]; //生波形の使用部分波形(ファイルマッピングにするかも(´・ω・｀))
	int nrawwave; //生波形のサンプル数
	float base_pitch; //基音位置(手動変更可)
					  //生成波形
	float outwave[4096]; //出力される波形
	int noutwave; //出力波形のサンプル数
				  //フェード系
	bool use_string_mode; //弦モードの使用をするかどうか
	bool use_fade_change; //なめらかな音程,音量の変化を使用するかどうか(2つ同時に音を出せない)
	float fadein_vol[6];
	float fadein_pitch[6];
	float fadeout_vol[6];
	float fadeout_pitch[6];
	float fadechange_vol[6];
	float fadechange_pitch[6];
	//波形の絶対値の上限
	float wave_limit;
	//基本波形の形
	char wave_type;
	//再生中変更できると便利なパラメーター
	float vol;
	float pitch; //MIDI値に加算
};

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
~ShareMem() { //共有メモリクローズ
	UnmapViewOfFile(smd); //共有メモリデータポインタ開放
}