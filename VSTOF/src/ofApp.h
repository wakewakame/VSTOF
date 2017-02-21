#pragma once

#include "ofMain.h"
#include "windows.h"
#include <vector>
#include <string>

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
//構造体宣言
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
//アニメーション変数管理クラス
class Animation {
private:
	//変数宣言
	double fps; //フレームレート
	std::vector<bool> motion; //アニメーションが開始されていれば1,それ意外は0
	std::vector<double> len; //アニメーションする時間(単位はsec)
	std::vector<double> t; //fpsに依存しない経過時間(0.0<=t<=len)
	std::vector<char> type; //アニメーションの変化の種類(0<=val)
	std::vector<bool*> swp; //sw=0ならtを0に近づけ、逆なら逆
	std::vector<bool> bsw; //前フレームのときのsw1

public:
	//変数宣言
	std::vector<double> m; //モーション座標(基本的に 0.0<=m<=1.0)
	std::vector<double> p; //(p/1.0)=(t/len)

						   //関数宣言
						   //fps指定
	void set_fps(double frame_rate) {
		fps = frame_rate;
	}
	//アニメーション配列の長さ
	int get_len() {
		return t.size();
	}
	//アニメーション変数の追加
	void add(double length, char effect_type, bool *sw) {
		motion.push_back(0);
		len.push_back(length);
		t.push_back(0.0);
		m.push_back(0.0);
		p.push_back(0.0);
		type.push_back(effect_type);
		swp.push_back(sw);
		bsw.push_back(*sw);
	}
	//毎フレーム呼び出し関数
	void loop() {
		for (int i = 0; i < t.size(); i++) {
			//前フレームとの値が違う場合、アニメーション開始
			if (*swp[i] != bsw[i]) {
				motion[i] = 1;
				bsw[i] = *swp[i];
			}
			//アニメーション処理
			if ((motion[i]) || (type[i] == 4)) {
				//経過時間計算
				if (*swp[i]) {
					t[i] += (1.0 / fps);
					if (t[i] >= len[i]) {
						t[i] = len[i];
						motion[i] = 0;
					}
				}
				else {
					t[i] -= (1.0 / fps);
					if (t[i] <= 0.0) {
						t[i] = 0.0;
						motion[i] = 0;
					}
				}
				//モーション計算
				p[i] = t[i] / len[i];
				switch (type[i]) {
				case 0:
					//y=x
					m[i] = p[i];
					break;
				case 1:
					//y=x^2
					m[i] = pow(p[i], 2);
					break;
				case 2:
					//y=-(x-1)^2+1
					m[i] = 1.0 - pow(p[i] - 1.0, 2);
					break;
				case 3:
					//y=3*(x^2)-2*(x^3)
					//x=0でyが極小値0となり、x=1でyが極大値1となる三次関数
					m[i] = 3 * pow(p[i], 2) - 2 * pow(p[i], 3);
					break;
				case 4:
					//指定時間を0.0-1.0区間ループ
					if (motion[i] == 0) {
						motion[i] = 1;
						t[i] = 0.0;
					}
					m[i] = p[i];
					break;
				}
			}
		}
	}
};
//フレームバッファクラス
class FBO {
public:
	/*
	///メモ///
	アルファフレームバッファは必要なときだけ生成する
	*/
	//フレームバッファクラスの配列
	std::vector<ofFbo> color;
	std::vector<ofFbo> alpha;
	//現在のフレームバッファの個数
	int num;
	//現在使用しているフレームバッファのインデックス(-1ならレンダーバッファ)
	int now_index;
	//仮フレームバッファクラスのコンストラクト
	ofFbo fbo_;
	//コンストラクタ
	FBO() {
		now_index = -1;
		num = 0;
	}
	//デストラクタ
	~FBO() {
		if (now_index != -1) {
			color[now_index].end();
			alpha[now_index].end();
		}
	}
	//フレームバッファの追加
	int add(int x, int y) {
		//カラーフレームバッファ
		color.push_back(fbo_); //配列にカラーフレームバッファクラス追加
		color[num].allocate(x, y, GL_RGBA); //カラーフレームバッファ生成
		color[num].begin();
		ofClear(255, 255, 255, 0); //カラーフレームバッファ初期化
		color[num].end();
		//アルファフレームバッファ
		alpha.push_back(fbo_); //配列にアルファフレームバッファクラス追加
		alpha[num].allocate(x, y, GL_ALPHA); //アルファフレームバッファ生成
		alpha[num].begin();
		ofClear(0, 0, 0, 255); //アルファフレームバッファ初期化
		alpha[num].end();
		num += 1;
		return num - 1;
	}
	//カラーフレームバッファの切り替え(index=-1ならレンダーバッファ)
	void change_c(int index) {
		if (now_index != -1) {
			color[now_index].end();
		}
		if (index != -1) {
			color[index].begin();
		}
		now_index = index;
	}
	//アルファフレームバッファの切り替え(index=-1ならレンダーバッファ)
	void change_a(int index) {
		if (now_index != -1) {
			alpha[now_index].end();
			//カラーフレームバッファにマスク適応
			ofSetColor(255, 255, 255, 255);
			color[now_index].begin();
			glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);
			alpha[now_index].draw(0, 0);
			color[now_index].end();
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		}
		if (index != -1) {
			alpha[index].begin();
			glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);
		}
		now_index = index;
	}
	//指定カラーバッファ描画
	void draw_c(int x, int y, int index) {
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);
		color[index].draw(x, y);
	}
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
	/*
	Animation animation; //アニメーション変数配列
	FBO fbo; //バッファフレーム配列
	*/
};
//グラフパラメータ保持
class GraphPara {
private:
	std::vector<std::vector<float>> para; //para[次元][インデックス] (1次元ならdim=0)
	std::vector<float> lim_min; //lim_min[次元]
	std::vector<float> lim_max; //lim_max[次元]
	std::vector<char> lim_mode; //lim_min,lim_max変数の有効無効(0=なし,1=下限あり,2=上限あり,3=上限下限あり)
	int min; //min
	int max; //max
	int active; //active[次元] 現在アクティブなパラメータのインデックス
	POINT mouse;
	frame *f;
public:
	//コンストラクト
	GraphPara() {
		min = -1;
		max = -1;
		active = -1;
	}
	//単位変換関数
	float percent(float a, float a_min, float a_max, float b_min, float b_max) {
		float b;
		b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
		return b;
	}
	int percent(int a, int a_min, int a_max, int b_min, int b_max) {
		return (int)percent((float)a, (float)a_min, (float)a_max, (float)b_min, (float)b_max);
	}
	//フレーム設定
	void set_frame(frame *set_f) {
		f = set_f;
	}
	//新規パラメータ追加関数(最小値と最大値のパラメータは必ず生成)
	void create(char mode, int argnum, ...) {
		va_list arg; //引数リスト
		//次元数が足りなければ追加
		while (para.size() < argnum) {
			std::vector<float> empty;
			para.push_back(empty);
			lim_min.push_back(-1.0f);
			lim_max.push_back(1.0f);
			lim_mode.push_back(0);
		}
		va_start(arg, argnum);
			for (int i = 0; i < argnum; i++) {
				para[i].push_back((float)va_arg(arg, double));
			}
		va_end(arg);
		switch (mode) {
		case 1:
			min = para[0].size() - 1;
			break;
		case 2:
			max = para[0].size() - 1;
			break;
		}
	}
	//パラメータ取得関数
	float get_para(int dim, int index) {
		return para[dim][index];
	}
	//最小値のパラメータの上限設定(必要があれば)
	void limit_min(float min, int dim) {
		lim_min[dim] = min;
		switch (lim_mode[dim]) {
		case 0:
			lim_mode[dim] = 1;
			break;
		case 2:
			lim_mode[dim] = 3;
			break;
		}
	}
	//最大値のパラメータの上限設定(必要があれば)
	void limit_max(float max, int dim) {
		lim_max[dim] = max;
		switch (lim_mode[dim]) {
		case 0:
			lim_mode[dim] = 2;
			break;
		case 1:
			lim_mode[dim] = 3;
			break;
		}
	}
	//最小値インデックス取得関数
	int get_imin() {
		return min;
	}
	//最大値インデックス取得関数
	int get_imax() {
		return max;
	}
	//フレーム座標変換関数
	POINT get_pos(int index) {
		POINT pos;
		if (get_active() == index) {
			pos = mouse;
		}else{
			pos.x = (int)(percent(
				para[0][index],
				para[0][min],
				para[0][max],
				(float)f->pos.left,
				(float)f->pos.right
			));
			pos.y = (int)(percent(
				para[1][index],
				para[1][min],
				para[1][max],
				(float)f->pos.bottom,
				(float)f->pos.top
			));
		}
		return pos;
	}
	//パラメータ座標変換関数
	void set_pos(POINT pos ,int index) {
		para[0][index] = percent(
			(float)pos.x,
			(float)f->pos.left,
			(float)f->pos.right,
			para[0][min],
			para[0][max]
		);
		para[1][index] = percent(
			(float)pos.y,
			(float)f->pos.bottom,
			(float)f->pos.top,
			para[1][min],
			para[1][max]
		);
	}
	//現在アクティブなパラメータのインデックス設定(ない場合はindex=-1)
	void set_active(int index) {
		if (index != -1) {
			active = index;
		}else{
			active = -1;
		}
	}
	//現在アクティブなパラメータのインデックス取得
	int get_active() {
		return active;
	}
	//指定範囲内に指定地が存在するかどうかを判定する関数
	bool in(RECT area, POINT pos) {
		if (
			(area.left <= pos.x) &&
			(area.top <= pos.y) &&
			(area.right >= pos.x) &&
			(area.bottom >= pos.y)
			) {
			return 1;
		}
		return 0;
	}
	bool in(POINT center, int size, POINT pos) {
		return in({
			center.x - size / 2,
			center.y - size / 2,
			center.x + size / 2,
			center.y + size / 2
		}, pos);
	}
	//ドラッグ時マウス追従関数
	void seek(int index, POINT n_mouse, bool l_click, int size) {
		mouse = n_mouse;
		if (in(get_pos(index), size, n_mouse) && l_click) {
			set_active(index);
		}
		if ((get_active() == index) && (!l_click)) {
			set_active(-1);
		}
		if (get_active() == index) {
			set_pos(n_mouse, index);
			limiter(index); //上限下限制御関数
		}
	}
	//パラメータ変更時関数関数
	void limiter(int index) {
		for (int i = 0; i < para.size(); i++) {
			//下限判定
			if (
				(para[i][index] < lim_min[i]) &&
				((lim_mode[i] == 1) || (lim_mode[i] == 3))
				) {
				para[i][index] = lim_min[i];
			}
			//上限判定
			if (
				para[i][index] > lim_max[i] &&
				((lim_mode[i] == 2) || (lim_mode[i] == 3))
				) {
				para[i][index] = lim_max[i];
			}
			//最小値<最大値の判定
			if (para[i][min] > para[i][min]) {
				if (active = min) {
					para[i][min] = para[i][max];
				}
				else {
					para[i][max] = para[i][min];
				}
			}
		}
	}
};
//パラメーター値構造体
struct VSTParameteresFrames {
	frame root;
	frame all;
	frame scroll;
	frame tone, fade;
	//音色系
	frame make_auto; //自動で音色を生成するか
	frame raw_wave_para; //下記インデントを束ねる
						 //生波形データ派生
	frame use_rawwave; //生波形データの使用をするかどうか
	frame rawwave; //生波形の使用部分波形(ファイルマッピングにするかも(´・ω・｀))
	frame base_pitch; //基音位置(手動変更可)
	frame tone_para; //下記インデントのフレームを束ねる
	frame change_natural; //音程変化を自然にするか
	frame overtones; //倍音グラフor共鳴スペクトルグラフ
	frame iovertones; //倍音の数
	frame wave_type; //基本波形の形
					 //再生中変更できると便利なパラメーター
	frame hostpar;
	frame vol;
	frame pitch; //MIDI値に加算
				 //波形の絶対値の上限
	frame wave_limit;
	//生成波形
	frame outwave; //出力される波形
				   //フェード系
	frame use_string_mode; //弦モードの使用をするかどうか
	frame use_fade_change; //なめらかな音程,音量の変化を使用するかどうか(2つ同時に音を出せない)
	frame fadein;
	frame fadein_vol;
	frame fadein_pitch;
	frame fadeout;
	frame fadeout_vol;
	frame fadeout_pitch;
	frame fadechange;
	frame fadechange_vol;
	frame fadechange_pitch;
};
//RECTのfloat型版
struct RECTF {
	float left;
	float top;
	float right;
	float bottom;
};
//各パラメーターの表示範囲
struct VSTParameteresLength {
	RECTF rawwave;
	RECTF base_pitch;
	RECTF overtones;
};
struct animation {
	float click; //クリックモーション
	float on; //マウスが重なったときのモーション
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

//共有メモリアクセスクラス
class ShareMem {
public:
	//変数初期化
	HANDLE MapHandle = nullptr; //マップのハンドル
	ShareMemData *smd = nullptr; //共有メモリデータポインタ
	int ShareMemSize = sizeof(ShareMemData); //マップのサイズ

											 //関数宣言
	bool Open(LPCTSTR MapName) { //共有メモリアクセス関数
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
};

//フレーム操作クラス
class Frames {
public:
	//関数宣言
	//フレーム追加関数(引数:親フレーム(なければnullptr),自フレーム,フレーム名,子フレームの配列方法(0なら縦,1なら横),子フレーム間の間隔,自フレームの長さ(mode=0なら高さ,1なら幅),自フレームを固定サイズにするon/off)
	void add(frame *parent, frame *self, std::string name, int length, bool lock) {
		self->parent = parent; //親フレームのポインタ
		self->num_child = 0; //子フレームの数初期化
		self->pos.left = 0; //フレーム左座標
		self->pos.top = 0; //フレーム上座標
		self->pos.right = 0; //フレーム右座標
		self->pos.bottom = 0; //フレーム下座標
		self->size = { 0,0 }; //フレームサイズ
		self->name = name; //フレームの名称
		self->mode = 0; //子フレームが縦並び=0,横並び=1
		self->gap = 0; //子フレーム間同士の隙間(px単位)
		self->length = length; //全フレームが初期値サイズ時の自フレームのサイズ
		self->lock = lock; //現在の自フレームの長さ(mode = 0なら縦幅, mode = 1なら横幅)の固定のon/off
		self->index = 0;
		self->lock_length = 0; //固定サイズの全子フレームと全gapの和(末端フレームは0を代入)
		//親フレームが未指定の場合、以下の処理を省く
		if (parent == nullptr) {
			return;
		}
		self->parent->childs.push_back(self); //親フレームに自フレーム追加登録
		self->index = self->parent->num_child;//同フレーム内の自フレームの割当番号(=0,1,2,3,...)
		self->parent->num_child += 1; //親フレーム情報更新
		return;
	}
	void set_parent(frame *self, bool mode, int gap) {
		self->mode = mode; //子フレームが横並びなら0,縦並びなら1
		self->gap = gap; //小フレーム間の隙間サイズ(px単位)
		return;
	}
	//全フレームの登録完了時に、末端フレームから全親フレームのlength等取得関数
	void get_length(frame *f) {
		if (f->num_child != 0) {
			f->length = 0; //全子フレームと全gapの和
			f->lock_length = 0; //固定サイズの全子フレームと全gapの和(末端フレームは0を代入)
								//子フレームの個数分ループ
			for (int i = 0; i < f->num_child; i++) {
				//子フレームが子フレームを持っていた場合、再帰
				if (f->childs[i]->num_child != 0) {
					get_length(f->childs[i]);
				}
				//親フレームの長さに子フレームの長さを足していく
				f->length += f->childs[i]->length;
				//もし子フレームが末端フレームで、固定サイズフレームの場合
				if (f->childs[i]->lock) {
					f->lock_length += f->childs[i]->length;
				}
			}
			//全gapを足す
			f->length += f->gap * (f->num_child + 1);
			f->lock_length += f->gap * (f->num_child + 1);
		}
		return;
	}
	//全フレームの位置算出(引数1,2,3:自フレームのポインタ,自フレームを描画する位置,0なら長さは初期値のまま)
	void sub_resize(frame *f, RECT pos) {
		f->size.x = pos.right - pos.left;
		f->size.y = pos.bottom - pos.top;
		f->pos = pos;
		//子フレームがあれば、子フレームの配置もしておく
		//lengthとsizeのサイズ変更
		if (f->num_child != 0) {
			if (f->mode) {
				//横並びの場合
				//子フレーム分ループ
				for (int i = 0; i < f->num_child; i++) {
					//i個目の子フレーム位置変数初期化
					RECT child_pos;
					//i個目の子フレーム位置算出
					//上下位置
					child_pos.top = f->pos.top + f->gap;
					child_pos.bottom = f->pos.bottom - f->gap;
					//左位置
					if (i == 0) {
						child_pos.left = f->pos.left + f->gap;
					}
					else {
						child_pos.left = f->childs[i - 1]->pos.right + f->gap;
					}
					//右位置
					if (f->childs[i]->lock) {
						//固定サイズフレームの場合は割合変換不要
						child_pos.right =
							child_pos.left +
							f->childs[i]->length;
					}
					else {
						//非固定サイズフレームの場合は割合計算
						child_pos.right =
							child_pos.left +
							Utility::Proportion(
								f->childs[i]->length,
								0,
								f->length - f->lock_length,
								0,
								f->size.x - f->lock_length
							);
					}
					//子フレーム位置設定
					resize(f->childs[i], child_pos);
				}
			}
			else {
				//縦並びの場合
				//子フレーム分ループ
				for (int i = 0; i < f->num_child; i++) {
					//i個目の子フレーム位置変数初期化
					RECT child_pos;
					//i個目の子フレーム位置算出
					//左右位置
					child_pos.left = f->pos.left + f->gap;
					child_pos.right = f->pos.right - f->gap;
					//上位置
					if (i == 0) {
						child_pos.top = f->pos.top + f->gap;
					}
					else {
						child_pos.top = f->childs[i - 1]->pos.bottom + f->gap;
					}
					//下位置
					if (f->childs[i]->lock) {
						//固定サイズフレームorスクロール可フレームの場合は割合変換不要
						child_pos.bottom =
							child_pos.top +
							f->childs[i]->length;
					}
					else {
						//非固定サイズフレームの場合は割合計算
						child_pos.bottom =
							child_pos.top +
							Utility::Proportion(
								f->childs[i]->length,
								0,
								f->length - f->lock_length,
								0,
								f->size.y - f->lock_length
							);
					}
					//子フレーム位置設定
					resize(f->childs[i], child_pos);
				}
			}
		}
		return;
	}
	//指定フレーム以下のフレームすべての再配置
	void resize(frame *f, RECT pos) {
		get_length(f);
		sub_resize(f, pos);
	}
	//子フレームのポインタからrootフレームのポインタ取得
	frame* get_root(frame *f) {
		frame *b_f; //フレームポインタ一時代入用変数
		b_f = f;
		while (true) {
			if (b_f->parent != nullptr) {
				b_f = b_f->parent;
			}else{
				return b_f;
			}
		}
	}
	//指定フレーム以下のすべてのフレームでdispose実行
	void all_dispose(frame *f) {
		dispose(f);
		for (int i = 0; i < f->childs.size(); i++) {
			all_dispose(f->childs[i]);
		}
	}
	//フレーム内でnewしたクラスをすべてdeleteする関数
	void dispose(frame *f) {
		for (int i = 0; i < f->data.size(); i++) {
			delete f->data[i];
		}
	}
};

//パラメーターの値やフレーム変数管理クラス
class Parameteres {
public:
	//コンストラクト
	Frames frames; //フレーム操作クラス

				   //変数初期化
	VSTParameteres *p_value; //パラメーター値変数群ポインタ
	VSTParameteresFrames p_frame; //パラメーターフレーム変数群
	VSTParameteresLength p_length; //パラメーター表示範囲変数群

	//関数宣言
	Parameteres() { //全パラメーター分のフレーム作成
		//フレーム生成
		//frames.add(frame *parent, frame *self, std::string name, int length, bool lock)
		frames.add(nullptr, &p_frame.root, "root", 0, 0);
		frames.add(&p_frame.root, &p_frame.all, "all", 0, 0);
		frames.add(&p_frame.root, &p_frame.scroll, "scroll", 16, 1);
		frames.add(&p_frame.all, &p_frame.tone, "tone", 0, 0); //音色設定フレーム生成
		frames.add(&p_frame.tone, &p_frame.make_auto, "make_auto", 100, 0); //自動で音色を生成するか
		frames.add(&p_frame.tone, &p_frame.raw_wave_para, "raw_wave_para", 0, 0); //下記インデントを束ねる
		frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, "use_rawwave", 100, 0); //生波形データの使用をするかどうか
		frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, "rawwave", 100, 0); //生波形の使用部分波形(ファイルマッピングにするかも(´・ω・｀))
		frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, "base_pitch", 100, 0); //基音位置(手動変更可)
		frames.add(&p_frame.tone, &p_frame.tone_para, "tone_para", 0, 0); //下記インデントのフレームを束ねる
		frames.add(&p_frame.tone_para, &p_frame.change_natural, "change_natural", 100, 0); //音程変化を自然にするか
		frames.add(&p_frame.tone_para, &p_frame.overtones, "overtones", 100, 0); //倍音グラフor共鳴スペクトルグラフ
		frames.add(&p_frame.tone_para, &p_frame.iovertones, "iovertones", 100, 0); //倍音の数
		frames.add(&p_frame.tone_para, &p_frame.wave_type, "wave_type", 100, 0); //基本波形の形
		frames.add(&p_frame.tone_para, &p_frame.hostpar, "hostpar", 0, 0); //下記インデントのフレームを束ねる
		frames.add(&p_frame.hostpar, &p_frame.vol, "vol", 100, 0); //音量
		frames.add(&p_frame.hostpar, &p_frame.pitch, "pitch", 100, 0); //音程(IDI値に加算)
		frames.add(&p_frame.tone, &p_frame.wave_limit, "wave_limit", 100, 0); //波形の絶対値の上限
		frames.add(&p_frame.tone, &p_frame.outwave, "outwave", 100, 0); //出力波形
		frames.add(&p_frame.all, &p_frame.fade, "fade", 0, 0); //フェード設定フレーム生成
		frames.add(&p_frame.fade, &p_frame.use_string_mode, "use_string_mode", 100, 0); //弦モードの使用をするかどうか
		frames.add(&p_frame.fade, &p_frame.use_fade_change, "use_fade_change", 100, 0); //なめらかな音程,音量の変化を使用するかどうか(2つ同時に音を出せない)
		frames.add(&p_frame.fade, &p_frame.fadein, "fadein", 0, 0);
		frames.add(&p_frame.fadein, &p_frame.fadein_vol, "fadein_vol", 100, 0);
		frames.add(&p_frame.fadein, &p_frame.fadein_pitch, "fadein_pitch", 100, 0);
		frames.add(&p_frame.fade, &p_frame.fadeout, "fadeout", 0, 0);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, "fadeout_vol", 100, 0);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, "fadeout_pitch", 100, 0);
		frames.add(&p_frame.fade, &p_frame.fadechange, "fadechange", 0, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, "fadechange_vol", 100, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, "fadechange_pitch", 100, 0);

		//set_parent(frame *self, bool mode, int gap)
		frames.set_parent(&p_frame.root, 1, 2);
		frames.set_parent(&p_frame.hostpar, 1, 2);
		frames.set_parent(&p_frame.fadein, 1, 2);
		frames.set_parent(&p_frame.fadeout, 1, 2);
		frames.set_parent(&p_frame.fadechange, 1, 2);
		frames.set_parent(&p_frame.fadechange, 1, 2);

		frames.get_length(&p_frame.root); //全フレームのlength等取得
	}
	//デストラクタ
	~Parameteres() {
		frames.all_dispose(&p_frame.root);
	}
};

//ウィンドウイベント処理クラス
class WIN_EVENT {
public:
	//変数宣言
	POINT mouse; //現在のマウス座標
	POINT b_mouse; //前フレーム時のマウス座標
	bool l_click; //現在の左クリック情報(押されていたら1)
	bool b_l_click; //前フレームのl_click変数の内容

	//関数宣言
	//コンストラクト
	WIN_EVENT() {
		l_click = 0;
		b_l_click = 0;
	}
	//指定RECT内に存在するかどうか
	bool in(RECT area) {
		if (
			(area.left <= mouse.x) &&
			(area.top <= mouse.y) &&
			(area.right >= mouse.x) &&
			(area.bottom >= mouse.y)
			) {
			return 1;
		}
		return 0;
	}
	bool in(POINT pos, int size) {
		return in({
			pos.x - size / 2,
			pos.y - size / 2,
			pos.x + size / 2,
			pos.y + size / 2
		});
	}
	//マウスが左クリックされたかどうか
	bool get_l_click() {
		return (l_click == 1 && b_l_click == 0);
	}
	//指定RECT内でマウスがクリックされたかどうか
	bool l_click_in(RECT area) {
		return (in(area) && get_l_click());
	}
	//毎フレーム呼び出し関数(処理の最後に呼び出す)
	void loop() {
		b_l_click = l_click;
		b_mouse = mouse;
	}
};
//GUIクラス
class GUI {
public:
	//変数宣言
	double move; //汎用アニメーション変数
	double fps; //フレームレート

	//コンストラクト
	WIN_EVENT win_event;
	Frames frames;

	//関数宣言
	//単位変換関数
	float percent(float a, float a_min, float a_max, float b_min, float b_max) {
		float b;
		b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
		return b;
	}
	int percent(int a, int a_min, int a_max, int b_min, int b_max) {
		return (int)percent((float)a, (float)a_min, (float)a_max, (float)b_min, (float)b_max);
	}
	//背景初期化
	void reset() {
		ofBackground(30, 30, 30);
	}
	//フレームレート指定
	void set_fps(double frame_rate) {
		fps = frame_rate;
	}
	//線のボックス描画関数
	void LineBox(RECT pos, int thick) {
		ofRect(pos.left, pos.top, pos.right - pos.left, thick); //上
		ofRect(pos.right - thick, pos.top + thick, thick, (pos.bottom - pos.top) - 2 * thick); //右
		ofRect(pos.left, pos.bottom - thick, pos.right - pos.left, thick); //下
		ofRect(pos.left, pos.top + thick, thick, (pos.bottom - pos.top) - 2 * thick); //左
	}
	//フレームの境界線描画
	void FrameLine(frame *root) {
		ofSetColor(255, 255, 255, 255);
		//自フレーム描画
		LineBox(root->pos, 1);
		//自フレームに子フレームがあれば全部描画
		if (root->num_child != 0) {
			for (int i = 0; i < root->num_child; i++) {
				FrameLine(root->childs[i]);
			}
		}
	}
	//フレームの名称描画
	void FrameName(frame *root) {
		ofSetColor(255, 255, 255, 255);
		//自フレームが子フレームを持ってた場合
		if (root->num_child != 0) {
			//自フレームに子フレームがあれば全部描画
			if (root->num_child != 0) {
				for (int i = 0; i < root->num_child; i++) {
					FrameName(root->childs[i]);
				}
			}
		}
		else {
			//子フレームを持っていない場合
			ofDrawBitmapString(root->name, root->pos.left + 4, root->pos.top + 12);
		}
	}
	//グラフ基盤UI(フレーム,波形配列,サンプル数,描画モード)
	void wave_glaph(frame *f, float *samples, int num_sample, char mode) {
		//描画効率化のため、ピクセルの数に合わせて描画
		int index = 0; //配列から値を参照するときのインデックス
		float percentage = 0.0f;
		float height; //今描画する波の高さ
		float b_height; //前フレームのheight
		for (int i = 0; i < f->size.x; i++) {
			percentage = percent((float)i, 0.0f, (float)f->size.x, 0.0f, (float)num_sample - 1); //一時代入
			index = (int)percentage; //波形配列のインデックス算出
			percentage = percentage - (float)index; //int,floatの性質上必ず正の数になる
													//(念の為)index+1>=num_sampleになったらループ脱出
			if (index + 1 >= num_sample) {
				break;
			}
			//今描画する波の高さ
			height =
				percent(samples[index], 0.0f, 1.0f, 0.0f, (float)f->size.y * 0.7f) * (1.0f - percentage) +
				percent(samples[index + 1], 0.0f, 1.0f, 0.0f, (float)f->size.y * 0.7f) * (percentage);
			//f->size.x<num_sampleの時は塗りつぶし処理特殊
			if (i % 2 == 0) {
				ofSetColor(255, 255, 255, 255);
				switch (mode) {
				case 0: //塗りつぶしなし
					break;
				case 1: //y=0.0を中心に塗りつぶし
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f - height,
						1,
						height
					);
					break;
				case 2: //波形から下を塗りつぶし
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f - height,
						1,
						f->size.y / 2.0f + height
					);
					break;
				case 3: //上限ライン描画
					ofSetColor(255, 0, 0, 255);
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f - f->size.y * 0.7f / 2.0f,
						1,
						1
					);
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f + f->size.y * 0.7f / 2.0f,
						1,
						1
					);
					break;
				}
			}
			if (i > 0) {
				ofSetColor(255, 255, 255, 255);
				ofLine(
					f->pos.left + i - 1,
					f->pos.bottom - f->size.y / 2.0f - b_height,
					f->pos.left + i,
					f->pos.bottom - f->size.y / 2.0f - height
				);
			}
			b_height = height; //前フレームのときのheight取得用
		}
	}
	//拡大縮小可能なグラフ描画関数
	void wave_gui(frame *f, float *samples, int num_sample, char mode) {
		GraphPara *para;
		if (f->data.size() == 0) {
			//インスタンス化
			f->data.push_back(new frame);
			f->data.push_back(new GraphPara);
			//初期化
				//フレームクラス
				frames.add(f, (frame*)(f->data[0]), "wave_glaph", 1, 0);
				frames.set_parent(f, f->mode, 4);
				f->childs[0]->pos = f->pos;
				f->childs[0]->size = f->size;
				f->childs[0]->length = f->length;
				//グラフパラメータクラス
				para = (GraphPara*)(f->data[1]); //クラスのポインタ代入
				para->set_frame(f->childs[0]); //フレームのポインタ代入
				para->create(1, 2, 0.0f, -1.0f); //最小値パラメータ生成
				para->create(2, 2, (float)num_sample, 1.0f); //最大値パラメータ生成
				para->create(0, 2, 30.0f, 0.0f); //テストパラメータ生成
				para->limit_min(0.0f, 0); //x軸の最大可動範囲設定
				para->limit_max((float)num_sample, 0); //x軸の最大可動範囲設定
				para->limit_min(-1.0f, 1); //y軸の最大可動範囲設定
				para->limit_max(1.0f, 1); //y軸の最大可動範囲設定
				frames.resize(frames.get_root(f), frames.get_root(f)->pos); //リサイズ
		}else{
			para = (GraphPara*)(f->data[1]);
		}
		//グラフのリサイズ
		
		//グラフの描画
		wave_glaph(f->childs[0], samples, num_sample, mode);
		//パラメータ操作カーソル描画
		cursor(para, para->get_imin(), 15); //グラフの最小値のパラメータ描画
		cursor(para, para->get_imax(), 15); //グラフの最大値のパラメータ描画
		cursor(para, 2, 15); //グラフの最大値のパラメータ描画
	}
	//パラメータカーソル描画関数
	void cursor(GraphPara *para, int index, int size) {
		POINT pos;
		para->seek(index, win_event.mouse, win_event.l_click, size);
		pos = para->get_pos(index);
		ofSetColor(255, 255, 255, 200);
		ofRect(pos.x - size / 2, pos.y - size / 2, size, size);
	}
	//スイッチUI
	void sw(frame *f, bool *sw) {
		//アニメーション変数確認
		bool sw2[3];
		bool loop = 1;
		Animation *animation;
		FBO *fbo;
		if (f->data.size() == 0) {
			//フレームにクラス追加
			f->data.push_back(new Animation);
			f->data.push_back(new FBO);
			//ポインタ代入
			animation = (Animation*)f->data[0];
			fbo = (FBO*)f->data[1];
			//クラス初期化
			animation->add(0.15, 3, &sw2[0]); //マウスをかざしたとき1
			animation->add(0.15, 3, &sw2[1]); //マウスをかざしたとき2
			animation->add(0.15, 3, sw); //クリックされたとき
			animation->add(1.0, 4, &loop);
			animation->set_fps(fps); //fps指定
			fbo->add(60, 60);
		}else{
			animation = (Animation*)f->data[0];
			fbo = (FBO*)f->data[1];
		}
		//フレームバッファに描画
		ofSetColor(255, 255, 255, 255);
		fbo->change_c(0);
		ofSetColor(255, 255, 255, 255);
		ofRect(0, 0, 60, 60);
		fbo->change_c(-1);
		fbo->change_a(0);
		ofClear(0, 0, 0, 255);
		for (int i = 0; i < 60; i++) {
			for (int j = 0; j < 60; j++) {
				ofSetColor(0, 0, 0, 255 - (((i + j + (int)(40.0*animation->m[3])) / 20) % 2) * 128);
				ofRect(j, i, j + 1, i + 1);
			}
		}
		ofSetColor(0, 0, 0, 0);
		ofRect(20, 20, 20, 20);
		fbo->change_a(-1);
		//スイッチイベント確認
		//クリックされたとき
		if (win_event.l_click_in({
			f->pos.left + 10,
			f->pos.top + 20,
			f->pos.left + 70,
			f->pos.top + 80
		})) {
			*sw = !(*sw);
		}
		//マウスをかざしたとき
		sw2[0] = win_event.in({
			f->pos.left + 10,
			f->pos.top + 20,
			f->pos.left + 70,
			f->pos.top + 80
		});
		sw2[1] = (animation->p[0] >= 0.5);
		//アニメーション確認
		animation->loop();
		//描画
		fbo->draw_c(f->pos.left + 10, f->pos.top + 20, 0);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(0, 128, 198, 255);
		move = 15.0*animation->m[0];
		ofRect(
			f->pos.left + 40 - (int)move,
			f->pos.top + 50 - (int)move,
			(int)move * 2,
			(int)move * 2
		);
		ofSetColor(255, 255, 255, 255);
		move = 15.0*animation->m[1];
		ofRect(
			f->pos.left + 40 - (int)move,
			f->pos.top + 50 - (int)move,
			(int)move * 2,
			(int)move * 2
		);
		ofSetColor(0, 128, 198, (int)(255.0*animation->p[1]));
		ofDrawBitmapString("click", f->pos.left + 21, f->pos.top + 53);
		ofSetColor(0, 128, 198, 255);
		move = 15.0*animation->m[2];
		ofRect(
			f->pos.left + 40 - (int)move,
			f->pos.top + 50 - (int)move,
			(int)move * 2,
			(int)move * 2
		);
	}
	//毎フレーム呼び出し関数
	void loop() {
		win_event.loop();
	}
	/*
	追加予定の関数
	スイッチ関数
	スライド可変抵抗関数
	各グラフパラメーター関数
	ttf,otfの自体で文字列描画関数
	*/
	//コンストラクト

};

//描画クラス
class Draw {
public:
	//変数宣言
	float fps;
	RECT win_size;
	bool a; //デバッグ用

			//コンストラクト
	Parameteres para;
	GUI gui;

	//関数宣言
	bool resize(WINDOW_INFO win_info) { //描画領域変更
										//win_infoのウィンドウサイズ更新
		GetClientRect(win_info.hwnd, &win_info.window);
		win_info.size.x = win_info.window.right - win_info.window.left;
		win_info.size.y = win_info.window.bottom - win_info.window.top;
		para.p_frame.root.pos.left = 0;
		para.p_frame.root.pos.top = 0;
		para.p_frame.root.pos.right = win_info.size.x;
		para.p_frame.root.pos.bottom = win_info.size.y;
		fps = win_info.fps;
		para.frames.resize(&para.p_frame.root, para.p_frame.root.pos); //全フレームの自動配置
		para.p_frame.all.pos.bottom = para.p_frame.all.length;
		para.frames.resize(&para.p_frame.all, para.p_frame.all.pos);
		gui.set_fps(fps);
		return 0;
	}
	void loop() { //ループ中に呼び出す関数
				  //画面初期化
		gui.reset();
		//フレームの境界線描画
		gui.FrameLine(&para.p_frame.root);
		//フレームの名称描画
		gui.FrameName(&para.p_frame.root);
		//各パラメーター描画
		{
			gui.wave_gui(&para.p_frame.make_auto, para.p_value->outwave, para.p_value->noutwave, 0);
			gui.sw(&para.p_frame.raw_wave_para, &a);
		}
		//毎フレーム呼び出し関数
		gui.loop();
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