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
	bool lock_lenght; //各子フレームの長さ(mode=0なら縦幅,mode=1なら横幅)の固定on/off
};
//パラメーター値構造体
struct VSTParameteresFrames {
	frame root;
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
	void add(frame *parent, frame *self, std::string name, bool mode, int gap, int length, bool lock_length) { //フレームの追加
		self->parent = parent; //親フレームのポインタ
		self->num_child = 0; //子フレームの数初期化
		self->pos.left = 0; //フレーム左座標
		self->pos.top = 0; //フレーム上座標
		self->pos.right = 0; //フレーム右座標
		self->pos.bottom = 0; //フレーム下座標
		self->size.x = 0; //フレームxサイズ
		self->size.y = 0; //フレームyサイズ
		self->name = name; //フレームの名称
		self->mode = mode; //子フレームが縦並び=0,横並び=1
		self->gap = gap; //子フレーム間同士の隙間(px単位)
		self->length = length; //全フレームが初期値サイズ時の自フレームのサイズ
		self->lock_lenght = lock_length; //現在の自フレームの長さ(mode = 0なら縦幅, mode = 1なら横幅)の固定のon/off
		self->index = 0;
		//親フレームが未指定の場合、以下の処理を省く
		if (parent == nullptr) {
			return;
		}
		self->parent->childs.push_back(self); //親フレームに自フレーム追加登録
		self->index = self->parent->num_child;//同フレーム内の自フレームの割当番号(=0,1,2,3,...)
		self->parent->num_child += 1; //親フレーム情報更新
		return;
	}
	int GetFrameHight(frame *f) { //末端フレームの初期サイズから全フレームの初期yサイズ取得
		int sum = 0;
		//自フレームの所有する全ての子フレームの初期長取得
		for (int i = 0; i < f->num_child; i++) {
			if (f->childs[i]->num_child == 0) {
				sum += f->childs[i]->length;
			}else{
				//子フレームが子フレームを所持していた場合、再帰処理
				sum += GetFrameHight(f->childs[i]);
			}
		}
		sum += f->gap * (f->num_child + 1); //子フレーム間同士の隙間も含めた値を算出
		return sum; //フレームの高さを返す
	}
	bool MakeFramePos(frame *f) { //フレーム位置算出
		for (int i = 0; i < f->num_child; i++) {
			if (f->childs[i]->num_child == 0) {

			}else{

			}
		}
		/*
		if (f->child == NULL) { //自フレームが子フレームを持っているか確認
			return 1;
		}
		int x = f->x;
		bool mode = f->mode; //自フレームの子フレーム整列方法取得
		int num_x = f->pos.left; //自フレームの左座標から子フレーム配置
		int num_y = f->pos.top; //自フレームの上座標から子フレーム配置
		int num = 0; //子フレームの数カウント用変数
		frame *last; //子フレームを1つづつ代入する変数
		last = f->child; //自フレームの最初の子フレーム代入
		if (mode) { //自フレームの子フレームが横並びであれば子フレームの個数カウント
			while (true) {
				num += 1;
				if (last->next == NULL) { //次フレームがなければループ終了
					break;
				}
				last = last->next; //次のフレームに移動
			}
			x /= num; //フレームの横幅をリサイズ
		}
		last = f->child; //自フレームの最初の子フレーム代入
		while (true) {
			last->x = x; //フレーム横幅取得
			last->pos.left = num_x; //子フレームの左座標取得
			last->pos.top = num_y; //子フレームの上座標取得
			last->pos.right = last->pos.left + last->x; //子フレームの右座標算出
			last->pos.bottom = last->pos.top + last->y; //子フレームの下座標算出
			if (last->child != NULL) { //子フレームに子フレームがあればそれの位置検出
				MakeFramePos(last);
			}
			if (last->next != NULL) { //次フレームがあれば
				if (!mode) { //縦並びであれば
					num_y = last->pos.bottom; //次フレームの上座標更新
				}
				else { //横並びであれば
					num_x = last->pos.right; //次フレームの左座標更新
				}
				last = last->next; //次のフレームに移動
			}
			else { //次フレームがなければループ終了
				break;
			}
		}
		return 0;
		*/
	}
	void resize(frame *f, RECT size) {
		f->pos.left = size.left;
		f->pos.top = size.top;
		f->x = size.right - size.left;
		MakeFramePos(f);
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
		frames.add(NULL, &p_frame.root, 0, 0); //ルートフレーム生成
		frames.add(&p_frame.root, &p_frame.tone, 0, 0); //音色設定フレーム生成
		frames.add(&p_frame.tone, &p_frame.make_auto, 0, 100); //自動で音色を生成するか
		frames.add(&p_frame.tone, &p_frame.raw_wave_para, 0, 0); //下記インデントを束ねる
		frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, 0, 100); //生波形データの使用をするかどうか
		frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, 0, 100); //生波形の使用部分波形(ファイルマッピングにするかも(´・ω・｀))
		frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, 0, 100); //基音位置(手動変更可)
		frames.add(&p_frame.tone, &p_frame.tone_para, 0, 0); //下記インデントのフレームを束ねる
		frames.add(&p_frame.tone_para, &p_frame.change_natural, 0, 100); //音程変化を自然にするか
		frames.add(&p_frame.tone_para, &p_frame.overtones, 0, 100); //倍音グラフor共鳴スペクトルグラフ
		frames.add(&p_frame.tone_para, &p_frame.iovertones, 0, 100); //倍音の数
		frames.add(&p_frame.tone_para, &p_frame.wave_type, 0, 100); //基本波形の形
		frames.add(&p_frame.tone_para, &p_frame.hostpar, 1, 0); //下記インデントのフレームを束ねる
		frames.add(&p_frame.hostpar, &p_frame.vol, 0, 100); //音量
		frames.add(&p_frame.hostpar, &p_frame.pitch, 0, 100); //音程(IDI値に加算)
		frames.add(&p_frame.tone, &p_frame.wave_limit, 0, 100); //波形の絶対値の上限
		frames.add(&p_frame.tone, &p_frame.outwave, 0, 100); //出力波形
		frames.add(&p_frame.root, &p_frame.fade, 0, 0); //フェード設定フレーム生成
		frames.add(&p_frame.fade, &p_frame.use_string_mode, 0, 100); //弦モードの使用をするかどうか
		frames.add(&p_frame.fade, &p_frame.use_fade_change, 0, 100); //なめらかな音程,音量の変化を使用するかどうか(2つ同時に音を出せない)
		frames.add(&p_frame.fade, &p_frame.fadein, 1, 0);
		frames.add(&p_frame.fadein, &p_frame.fadein_vol, 0, 100);
		frames.add(&p_frame.fadein, &p_frame.fadein_pitch, 0, 0);
		frames.add(&p_frame.fade, &p_frame.fadeout, 1, 0);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, 0, 100);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, 0, 0);
		frames.add(&p_frame.fade, &p_frame.fadechange, 1, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, 0, 100);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, 0, 0);
		frames.GetFrameHight(&p_frame.root);
		//パラメーター表示範囲設定
		p_length.rawwave.left = 0.0f;
		p_length.rawwave.right = 1.0f;
		p_length.rawwave.top = -1.0f;
		p_length.rawwave.bottom = 1.0f;
	}
};

/*
//フレーム操作クラス
class Frames {
public:
//関数宣言
void add(frame *parent, frame *self, bool mode, int size) { //フレームの追加
self->parent = parent; //親フレームのポインタ
self->child = NULL; //一番初めの子フレームのポインタ
self->next = NULL; //次フレームのポインタ
self->mode = mode; //0:子フレームが縦並びフレーム,1:子フレームが横並び
self->pos.left = 0; //フレーム左座標
self->pos.top = 0; //フレーム上座標
self->pos.right = 0; //フレーム右座標
self->pos.bottom = 0; //フレーム下座標
self->y = size; //フレームの高さ
if (parent == NULL) { //親フレーム未指定の場合上記の値のままでOK
return;
}
if (parent->child == NULL) { //親フレームが子フレームを持っていなかった場合
parent->child = self; //親フレームに一番最初の子フレーム登録
}else{ //親フレームが子フレームを持っていたら
//親フレームの子フレーム列一番最後に自フレーム登録
frame *last = parent->child;
while (last->next != NULL) {
last = last->next;
}
last->next = self;
}
return;
}
int GetFrameHight(frame *f) { //末端フレームのサイズから全フレームのyサイズ取得
bool mode = f->mode; //自フレームの子フレーム整列方法取得
int num = 0; //自フレームの高さ代入用変数
if (f->child == NULL) { //指定されたフレームが子フレームを持たなかった場合
return 0; //高さは0になる
}
frame *last = f->child; //親フレームの全子フレームを1つづつ代入用変数
while (true) {
if (last->child != NULL) { //自フレームに子フレームが存在すれば
last->y = GetFrameHight(last); //子フレームのyサイズ取得
}
num += last->y; //上記で取得した子フレームの高さを加算
if (!mode) { //自フレームの子フレームが縦並びであれば
if (last->next != NULL) { //次フレームが存在した場合
last = last->next;//次の子フレームに移動
}else { //次フレームがなければループ終了
break;
}
}else { //横並びであればループ終了
break;
}
}
return num; //フレームの高さを返す
}
bool MakeFramePos(frame *f) { //フレーム位置算出
if (f->child == NULL) { //自フレームが子フレームを持っているか確認
return 1;
}
int x = f->x;
bool mode = f->mode; //自フレームの子フレーム整列方法取得
int num_x = f->pos.left; //自フレームの左座標から子フレーム配置
int num_y = f->pos.top; //自フレームの上座標から子フレーム配置
int num = 0; //子フレームの数カウント用変数
frame *last; //子フレームを1つづつ代入する変数
last = f->child; //自フレームの最初の子フレーム代入
if (mode) { //自フレームの子フレームが横並びであれば子フレームの個数カウント
while (true) {
num += 1;
if (last->next == NULL) { //次フレームがなければループ終了
break;
}
last = last->next; //次のフレームに移動
}
x /= num; //フレームの横幅をリサイズ
}
last = f->child; //自フレームの最初の子フレーム代入
while (true) {
last->x = x; //フレーム横幅取得
last->pos.left = num_x; //子フレームの左座標取得
last->pos.top = num_y; //子フレームの上座標取得
last->pos.right = last->pos.left + last->x; //子フレームの右座標算出
last->pos.bottom = last->pos.top + last->y; //子フレームの下座標算出
if (last->child != NULL) { //子フレームに子フレームがあればそれの位置検出
MakeFramePos(last);
}
if (last->next != NULL) { //次フレームがあれば
if (!mode) { //縦並びであれば
num_y = last->pos.bottom; //次フレームの上座標更新
}
else { //横並びであれば
num_x = last->pos.right; //次フレームの左座標更新
}
last = last->next; //次のフレームに移動
}else { //次フレームがなければループ終了
break;
}
}
return 0;
}
void resize(frame *f, RECT size) {
f->pos.left = size.left;
f->pos.top = size.top;
f->x = size.right - size.left;
MakeFramePos(f);
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
frames.add(NULL, &p_frame.root, 0, 0); //ルートフレーム生成
frames.add(&p_frame.root, &p_frame.tone, 0, 0); //音色設定フレーム生成
frames.add(&p_frame.tone, &p_frame.make_auto, 0, 100); //自動で音色を生成するか
frames.add(&p_frame.tone, &p_frame.raw_wave_para, 0, 0); //下記インデントを束ねる
frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, 0, 100); //生波形データの使用をするかどうか
frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, 0, 100); //生波形の使用部分波形(ファイルマッピングにするかも(´・ω・｀))
frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, 0, 100); //基音位置(手動変更可)
frames.add(&p_frame.tone, &p_frame.tone_para, 0, 0); //下記インデントのフレームを束ねる
frames.add(&p_frame.tone_para, &p_frame.change_natural, 0, 100); //音程変化を自然にするか
frames.add(&p_frame.tone_para, &p_frame.overtones, 0, 100); //倍音グラフor共鳴スペクトルグラフ
frames.add(&p_frame.tone_para, &p_frame.iovertones, 0, 100); //倍音の数
frames.add(&p_frame.tone_para, &p_frame.wave_type, 0, 100); //基本波形の形
frames.add(&p_frame.tone_para, &p_frame.hostpar, 1, 0); //下記インデントのフレームを束ねる
frames.add(&p_frame.hostpar, &p_frame.vol, 0, 100); //音量
frames.add(&p_frame.hostpar, &p_frame.pitch, 0, 100); //音程(IDI値に加算)
frames.add(&p_frame.tone, &p_frame.wave_limit, 0, 100); //波形の絶対値の上限
frames.add(&p_frame.tone, &p_frame.outwave, 0, 100); //出力波形
frames.add(&p_frame.root, &p_frame.fade, 0, 0); //フェード設定フレーム生成
frames.add(&p_frame.fade, &p_frame.use_string_mode, 0, 100); //弦モードの使用をするかどうか
frames.add(&p_frame.fade, &p_frame.use_fade_change, 0, 100); //なめらかな音程,音量の変化を使用するかどうか(2つ同時に音を出せない)
frames.add(&p_frame.fade, &p_frame.fadein, 1, 0);
frames.add(&p_frame.fadein, &p_frame.fadein_vol, 0, 100);
frames.add(&p_frame.fadein, &p_frame.fadein_pitch, 0, 0);
frames.add(&p_frame.fade, &p_frame.fadeout, 1, 0);
frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, 0, 100);
frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, 0, 0);
frames.add(&p_frame.fade, &p_frame.fadechange, 1, 0);
frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, 0, 100);
frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, 0, 0);
frames.GetFrameHight(&p_frame.root);
//パラメーター表示範囲設定
p_length.rawwave.left = 0.0f;
p_length.rawwave.right = 1.0f;
p_length.rawwave.top = -1.0f;
p_length.rawwave.bottom = 1.0f;
}
};
*/

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
		frame *last = root->child;
		while (true) {
			if (last->child != NULL) {
				FrameLine(last);
			}
			ofSetColor(0, 128, 198, 40);
			ofSetLineWidth(1);
			LIneBox(last->pos);
			if (last->next != NULL) {
				last = last->next;
			}
			else {
				break;
			}
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
		win_size.right = win_info.size.x;
		win_size.bottom = win_info.size.y;
		win_size.left = 0;
		win_size.top = 0;
		fps = win_info.fps;
		para.frames.resize(&para.p_frame.root, win_size);
		return 0;
	}
	void loop() { //ループ中に呼び出す関数
				  //画面初期化
		gui.reset();
		//GUIクラスの毎フレーム呼び出し関数
		gui.loop();
		//フレームの境界線描画
		gui.FrameLine(&para.p_frame.root);
		//各パラメーター描画

	}
};

//全ての処理を行うクラス
class AllProcess {
public:
	//変数初期化
	WINDOW_INFO win_info;

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
				  //win_infoのウィンドウサイズ更新
		GetWindowRect(win_info.hwnd, &win_info.window);
		win_info.size.x = win_info.window.right - win_info.window.left;
		win_info.size.y = win_info.window.bottom - win_info.window.top;
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
