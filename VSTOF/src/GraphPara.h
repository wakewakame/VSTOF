#pragma once

#include "define.h"

//グラフパラメータ保持
/*
使い方
パラメータを必要な分だけcreate関数で生成
次元は自動で確保してくれる
パラメータの可動域は必要があれば設定
ウィンドウのx,y座標に対応する次元の設定
*/
class GraphPara {
private:
	std::vector<Dimension> param; //各次元のパラメータ配列
	int active; //現在アクティブなパラメータのインデックス
	WindowPos mouse; //現在のマウス座標
	frame *f; //使用するフレームのポインタ
	int x_dim; //ウィンドウx座標に対応する次元番号
	int y_dim; //ウィンドウy座標に対応する次元番号
	std::vector<float> length_min;
	int height; //1次元のみの場合のパラメータのXorY座標
public:
	//コンストラクト
	GraphPara();
	//単位変換関数
	float percent(float a, float a_min, float a_max, float b_min, float b_max);
	int percent(int a, int a_min, int a_max, int b_min, int b_max);
	//フレーム設定
	void set_frame(frame *set_f);
	//新規パラメータ追加関数(最小値と最大値のパラメータは必ず生成)
	void create(int argnum, ...);
	//グラフの描画域の最小値パラメータのインデックス設定
	void set_min(int dim, int index);
	//グラフの描画域の最大値パラメータのインデックス
	void set_max(int dim, int index);
	//最大値最小値パラメータ間の最小の距離設定
	void set_length_min(int dim, float min);
	//パラメータ取得関数
	const float get_param(int dim, int index);
	//パラメータの可動域の最小値設定
	void limit_min(float min, int dim);
	//パラメータの可動域の最大値設定
	void limit_max(float max, int dim);
	//パラメータの可動域の最小値取得
	const float get_lim_min(int dim);
	//パラメータの可動域の最大値取得
	const float get_lim_max(int dim);
	//最小値パラメータ取得関数
	const float get_min(int dim);
	//最大値パラメータ取得関数
	const float get_max(int dim);
	//最小値インデックス取得関数
	const int get_imin(int dim);
	//最大値インデックス取得関数
	const int get_imax(int dim);
	//現在アクティブなパラメータのインデックス設定(ない場合はindex=-1)
	void set_active(int index);
	//現在アクティブなパラメータのインデックス取得
	const int get_active();
	//ウィンドウのx,y座標に対応する次元数の設定
	void set_window_dim(int set_x_dim, int set_y_dim);
	//ウィンドウのx座標に対応する次元数の取得
	const int get_x_dim();
	//ウィンドウのy座標に対応する次元数の取得
	const int get_y_dim();
	//フレーム座標変換関数
	const POINT get_pos(int index);
	//パラメータ座標変換関数
	void set_pos(WindowPos pos, int index);
	//指定範囲内に指定地が存在するかどうかを判定する関数
	const bool hit(RECT area, WindowPos pos);
	const bool hit(POINT center, POINT size, WindowPos pos);
	//ドラッグ時マウス追従関数
	void seek(int index, WindowPos n_mouse, bool l_click, bool b_l_click, POINT size);
	//パラメータ変更時関数関数
	void limiter(int index);
};