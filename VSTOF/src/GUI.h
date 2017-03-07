//インクルード
#include "ofMain.h"
#include "Frame.h"
#include "win_event.h"

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

//GUIクラス
class GUI {
public:
	//---変数宣言---//
	double move; //汎用アニメーション変数
	double fps; //フレームレート

	//---コンストラクト---//
	WIN_EVENT win_event;
	Frames frames;
	UI_DESIGN ui;

	//---関数宣言---//
	//単位変換関数
	float percent(float a, float a_min, float a_max, float b_min, float b_max);
	int percent(int a, int a_min, int a_max, int b_min, int b_max);
	//背景初期化
	void reset();
	//フレームレート指定
	void set_fps(double frame_rate);
	//線のボックス描画関数
	void LineBox(RECT pos, int thick);
	//フレームの境界線描画
	void FrameLine(frame *root);
	//フレームの名称描画
	void FrameName(frame *root);
	//単位線描画関数(単位を描画する線,線の本来の単位での長さ,描画するときの線の始まり地点の値,描画するときの線の終わりの地点の値,単位線の数)
	void unit_line(RECT line, float length, float start, float finish, int num);
	//グラフ座標からウィンドウ座標に変換する関数
	GraphPos conversion(WindowPos);
	//グラフ基盤UI(フレーム,波形配列,サンプル数,描画モード)
	void wave_graph(frame *f, graph g, bool mode);
	//パラメータクラスをグラフプロパティクラスに変換する関数
	graph c_graph(float *samples, GraphPara *param);
	//グラフに単位線をつける関数
	void graph_unitline(frame *f, graph g, char *x, char *y);
	//マウスで書き換え可能なグラフ描画関数
	void wave_draw(frame *f, graph g, bool mode, bool active);
	//拡大縮小可能なグラフ描画関数
	void wave_gui(frame *f, float *samples, int num_sample, bool mode);
	//ボリューム基盤UI
	void volume(frame *f, float *val);
	//ボリュームUI
	void volume_gui(frame *f, float *val);
	//パラメータカーソル描画関数
	void cursor(GraphPara *param, int index, char mode);
	//スイッチUI
	void sw(frame *f, bool *sw);
	//毎フレーム呼び出し関数
	void loop();
	/*
	追加予定の関数
	スイッチ関数
	スライド可変抵抗関数
	各グラフパラメーター関数
	ttf,otfの自体で文字列描画関数
	*/
	//コンストラクト

};