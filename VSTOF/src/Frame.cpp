#include "Frame.h"

void Frames::add(frame *parent, frame *self, std::string name, int length, bool lock) {
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

void Frames::set_parent(frame *self, bool mode, int gap) {
	self->mode = mode; //子フレームが横並びなら0,縦並びなら1
	self->gap = gap; //小フレーム間の隙間サイズ(px単位)
	return;
}

void Frames::get_length(frame *f) {
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

void Frames::sub_resize(frame *f, RECT pos) {
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

void Frames::resize(frame *f, RECT pos) {
	get_length(f);
	sub_resize(f, pos);
}

frame* Frames::get_root(frame *f) {
	frame *b_f; //フレームポインタ一時代入用変数
	b_f = f;
	while (true) {
		if (b_f->parent != nullptr) {
			b_f = b_f->parent;
		}
		else {
			return b_f;
		}
	}
}

void Frames::all_dispose(frame *f) {
	dispose(f);
	for (int i = 0; i < f->childs.size(); i++) {
		all_dispose(f->childs[i]);
	}
}

void Frames::dispose(frame *f) {
	for (int i = 0; i < f->data.size(); i++) {
		delete f->data[i];
	}
}