#pragma once

#include "define.h"

//フレーム操作クラス
class Frames {
public:
	//関数宣言
	//フレーム追加関数(引数:親フレーム(なければnullptr),自フレーム,フレーム名,子フレームの配列方法(0なら縦,1なら横),子フレーム間の間隔,自フレームの長さ(mode=0なら高さ,1なら幅),自フレームを固定サイズにするon/off)
	void add(frame *parent, frame *self, std::string name, int length, bool lock);
	//子フレームの並べ方設定関数
	void set_parent(frame *self, bool mode, int gap);
	//フレームの解説記入
	void set_description(frame *self, std::string description);
	//全フレームの登録完了時に、末端フレームから全親フレームのlength等取得関数
	void get_length(frame *f);
	//全フレームの位置算出(引数1,2,3:自フレームのポインタ,自フレームを描画する位置,0なら長さは初期値のまま)
	void sub_resize(frame *f, RECT pos);
	//指定フレーム以下のフレームすべての再配置
	void resize(frame *f, RECT pos);
	//子フレームのポインタからrootフレームのポインタ取得
	frame* get_root(frame *f);
	//指定フレーム以下のすべてのフレームでdispose実行
	void all_dispose(frame *f);
	//フレーム内でnewしたクラスをすべてdeleteする関数
	void dispose(frame *f);
};