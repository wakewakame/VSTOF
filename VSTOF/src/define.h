#pragma once

#include "ofMain.h"
#include "windows.h"
#include <vector>
#include <string>

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

//共有メモリ構造体
struct ShareMemData {
	HWND Ready; //GUIクライアントが起動直後にTRUEにする(多重起動回避のため)
	HWND Host; //描画先ハンドル
	VSTParameteres para; //VSTのパラメーター群
};

struct Dimension {
	bool on_limit_min; //パラメータの可動域の最小値が有効かどうか
	bool on_limit_max; //パラメータの可動域の最大値が有効かどうか
	float limit_min; //パラメータの可動域の最小値
	float limit_max; //パラメータの可動域の最大値
	int min_index; //グラフの描画域の最小値パラメータのインデックス
	int max_index; //グラフの描画域の最大値パラメータのインデックス
	std::vector<float> val; //パラメータの値の配列
};

//フレーム構造体
struct frame {
	frame *parent; //親フレームのポインタ
	int num_child; //子フレームの数
	std::vector<frame*> childs; //全子フレームのポインタ配列
	int index;//同フレーム内の自フレームの割当番号(=0,1,2,3,...)
	RECT pos; //フレーム座標
	POINT size; //フレームサイズ(末端フレームのみ代入)
	std::string name; //フレームの名称
	bool mode; //子フレームが縦並び=0,横並び=1
	int gap; //子フレーム間同士の隙間(px単位)
	int length; //全フレームが初期値サイズ時の自フレームのサイズ
	bool lock; //各子フレームの長さ(mode=0なら縦幅,mode=1なら横幅)の固定on/off
	int lock_length; //固定サイズの全子フレームと全gapの和(末端フレームは0を代入)
	std::vector<void*> data; //汎用配列(クラスのポインタなど)
};

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
	static float Proportion(float i, float i_mix, float i_max, float j_min, float j_max) {
		float j;
		j = int(Proportion(double(i), double(i_mix), double(i_max), double(j_min), double(j_max)));
		return j;
	}
	static int Proportion(int i, int i_mix, int i_max, int j_min, int j_max) {
		int j;
		j = int(Proportion(double(i), double(i_mix), double(i_max), double(j_min), double(j_max)));
		return j;
	}
};

//グラフ上の座標
struct GraphPos {
	int x;
	float y;
};

//ウィンドウ上の座標
struct WindowPos {
	int x;
	int y;
};

//グラフのプロパティ
struct graph {
	float *samples; //波形本体
	int num_sample; //波形のサンプル数
	int start_index; //描画領域の始めの波形のサンプルのインデックス
	int end_index; //描画領域の終わりの波形のサンプルのインデックス
	float zero_index_val; //波形のサンプルが0の地点のグラフ上の単位のx座標
	float last_index_val; //波形のサンプルが最終地点のグラフ上の単位のx座標
	float val_min; //描画領域のサンプル値の最小値
	float val_max; //描画領域のサンプル値の最大値
	float val_lim_min; //描画領域のサンプル値の最小値の可動域の最小値
	float val_lim_max; //描画領域のサンプル値の最大値の可動域の最大値
};

