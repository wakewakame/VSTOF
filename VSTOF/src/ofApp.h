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
	static double Proportion(double i,double i_mix,double i_max,double j_min,double j_max) {
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
		self->size = {0,0}; //フレームサイズ
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
	void resize(frame *f, RECT pos) {
		f->size.x = pos.right - pos.left;
		f->size.y = pos.bottom - pos.top;
		f->pos = pos;
		//子フレームがあれば、子フレームの配置もしておく
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
					}else{
						child_pos.left = f->childs[i-1]->pos.right + f->gap;
					}
					//右位置
					if (f->childs[i]->lock) {
						//固定サイズフレームの場合は割合変換不要
						child_pos.right =
							child_pos.left +
							f->childs[i]->length;
					}else{
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
			}else{
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
					}else{
						child_pos.top = f->childs[i-1]->pos.bottom + f->gap;
					}
					//下位置
					if (f->childs[i]->lock) {
						//固定サイズフレームorスクロール可フレームの場合は割合変換不要
						child_pos.bottom =
							child_pos.top +
							f->childs[i]->length;
					}else{
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
		frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, "fadeout_pitch" ,100, 0);
		frames.add(&p_frame.fade, &p_frame.fadechange, "fadechange", 0, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, "fadechange_vol", 100, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, "fadechange_pitch", 100, 0);

		//set_parent(frame *self, bool mode, int gap)
		frames.set_parent(&p_frame.root, 1, 0);
		frames.set_parent(&p_frame.hostpar, 1, 0);
		frames.set_parent(&p_frame.fadein, 1, 0);
		frames.set_parent(&p_frame.fadeout, 1, 0);
		frames.set_parent(&p_frame.fadechange, 1, 0);
		frames.set_parent(&p_frame.fadechange, 1, 0);
		
		frames.get_length(&p_frame.root); //全フレームのlength等取得
	}
};

//ウィンドウイベント処理クラス
class WIN_EVENT {
public:
	//変数宣言
	HWND hwnd; //現在指定されているウィンドウハンドル
	HWND m_hwnd; //マウス下の最前面ウィンドウハンドル
	POINT mouse_pos; //マウス座標
	MSG msg; //ウィンドウメッセージ代入変数

	//関数宣言
	//ウィンドウハンドル設定関数
	void SetHwnd(HWND h) {
		hwnd = h;
		return;
	}
	//マウスが自ウィンドウを操作しているかどうかの判定
	bool GetMouseOnWindow() {
		GetMousePos();
		return 0;
	}
	//マウスが指定範囲内を操作しているかどうかの判定
	bool GetMouseOnRect() {
		GetMousePos();
		return 0;
	}
	//マウスホイール変化量取得関数
	int GetScroll() {

		return 0;
	}
	//マウス座標取得関数
	void GetMousePos() {
		GetMessage(&msg, NULL, 0, 0);
		//mouse_pos = ; //マウス座標代入
		return;
	}
};

//GUIクラス
class GUI {
public:
	//関数宣言
	float percent(float a, float a_min, float a_max, float b_min, float b_max) { //単位変換関数
		float b;
		b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
		return b;
	}
	//背景初期化
	void reset() {
		ofBackground(30, 30, 30);
	}
	//線のボックス描画関数
	void LIneBox(RECT pos) {
		ofLine(pos.left, pos.top, pos.right, pos.top);
		ofLine(pos.right, pos.top, pos.right, pos.bottom);
		ofLine(pos.right, pos.bottom, pos.left, pos.bottom);
		ofLine(pos.left, pos.bottom, pos.left, pos.top);
	}
	//フレームの境界線描画
	void FrameLine(frame *root) {
		ofSetLineWidth(1.0f);
		ofSetColor(255, 255, 255, 240);
		//自フレーム描画
		LIneBox(root->pos);
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
		}else{
			//子フレームを持っていない場合
			ofDrawBitmapString(root->name, root->pos.left + 4, root->pos.top + 12);
		}
	}
	void rawwave(frame *f, RECTF len, float *samples, int num_sample, float *allrawwave, int num_allrawwave, POINT rlen) { //生波形データの使用部分選択

	}
	void loop() { //毎フレーム呼び出し関数

	}
	/*
	追加予定の関数
	スイッチ関数
	スライド可変抵抗関数
	各グラフパラメーター関数
	*/
	//コンストラクト

};

//描画クラス
class Draw {
public:
	//変数宣言
	float fps;
	RECT win_size;

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
		return 0;
	}
	void loop() { //ループ中に呼び出す関数
		//画面初期化
		gui.reset();
		//GUIクラスの毎フレーム呼び出し関数
		gui.loop();
		//フレームの境界線描画
		gui.FrameLine(&para.p_frame.root);
		//フレームの名称描画
		gui.FrameName(&para.p_frame.root);
		//各パラメーター描画

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
		}else {
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

	WINDOW_INFO win_info; //ウィンドウ情報保持変数初期化

						  //コンストラクト
	AllProcess app;

	//実行時引数文字列
	LPSTR lpCmdLine;
};
