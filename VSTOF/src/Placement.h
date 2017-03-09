#pragma once

#include "define.h"
#include "Frame.h"
#include "ShareMem.h"
#include "GUI.h"

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

//パラメーターの値やフレーム変数管理クラス
class Parameteres {
public:
	//コンストラクト
	Frames frames; //フレーム操作クラス

	//変数初期化
	VSTParameteres *p_value; //パラメーター値変数群ポインタ
	VSTParameteresFrames p_frame; //パラメーターフレーム変数群

	//関数宣言
	//コンストラクタ
	Parameteres();
	//デストラクタ
	~Parameteres();

	//ツリー構造のCUI描画(DEGUG用)
	void draw_node_name(frame *f) {
		std::string tree = "[frame nodes tree viewer]\n";
		std::vector<int> gap;
		tree = draw_node_name(f, gap, tree);
		std::cout << tree << std::endl;
	}
	string draw_node_name(frame *f, std::vector<int> gap, std::string tree) {
		bool pop_back = true;
		if (f->parent != nullptr) {
			if (f->parent->parent != nullptr) {
				if (f->parent->parent->childs[f->parent->parent->childs.size() - 1] == f->parent) {
					if (gap.size() >= 2) {
						gap[gap.size() - 2] += gap[gap.size() - 1] + 2;
						gap.pop_back();
						pop_back = false;
					}
				}
			}
		}
		if (f->parent != nullptr) {
			tree += "\n";
			for (int i = 0; i < gap.size(); i++) {
				for (int j = 0; j < gap[i]; j++) {
					tree += " ";
				}
				if (i == (gap.size() - 1)) {
					break;
				}
				tree += "┃";
			}
			if (f->parent->childs[f->parent->childs.size() - 1] == f) {
				tree += "┗";
			}else {
				tree += "┣";
			}
		}
		tree += "[";
		tree += f->name;
		tree += "]";
		gap.push_back(0);
		for (int i = 0; i < f->childs.size(); i++) {
			tree = draw_node_name(f->childs[i], gap, tree);
		}
		if (pop_back) {
			gap.pop_back();
		}
		return tree;
	}
};

//描画クラス
class Draw {
public:
	//変数宣言
	float fps;
	RECT win_size;
	bool a; //デバッグ用
	float b; //デバッグ用

	//コンストラクト
	Parameteres para;
	GUI gui;

	//関数宣言
	bool resize(WINDOW_INFO win_info);
	void loop();
};