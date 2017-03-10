#include "GUI.h"

float GUI::percent(float a, float a_min, float a_max, float b_min, float b_max) {
	float b;
	b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
	return b;
}

int GUI::percent(int a, int a_min, int a_max, int b_min, int b_max) {
	return (int)percent((float)a, (float)a_min, (float)a_max, (float)b_min, (float)b_max);
}

void GUI::reset() {
	ofBackground(30, 30, 30);
}

void GUI::set_fps(double frame_rate) {
	fps = frame_rate;
}

void GUI::LineBox(RECT pos, int thick) {
	ofRect(pos.left, pos.top, pos.right - pos.left, thick); //上
	ofRect(pos.right - thick, pos.top + thick, thick, (pos.bottom - pos.top) - 2 * thick); //右
	ofRect(pos.left, pos.bottom - thick, pos.right - pos.left, thick); //下
	ofRect(pos.left, pos.top + thick, thick, (pos.bottom - pos.top) - 2 * thick); //左
}

void GUI::FrameLine(frame *root) {
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

void GUI::FrameName(frame *root) {
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
		ofDrawBitmapString(root->description, root->pos.left + 4, root->pos.top + 24);
	}
}

void GUI::unit_line(RECT line, float length, float start, float finish, int num) {
	float zoom = length / (finish - start); //拡大率
	float gap_x = (float)(line.right - line.left) / (float)num; //zoom=1のときの単位線同士のx幅
	float gap_y = (float)(line.bottom - line.top) / (float)num; //zoom=1のときの単位線同士のy幅
	float unit_zoom = zoom / pow(2.0f, floor(log2(zoom))); //単位線の拡大率
	float unit_gap_x = gap_x * unit_zoom; //単位線同士のx幅
	float unit_gap_y = gap_y * unit_zoom; //単位線同士のy幅
	float first_x = 0.0f; //描画する単位線のx軸における始めの位置
	float first_y = 0.0f; //描画する単位線のy軸における始めの位置
	if (line.right - line.left != 0.0f) {
		first_x = (line.right - line.left) * start / (finish - start);
		first_x = unit_gap_x * ceil(first_x / unit_gap_x) - first_x;
	}
	if (line.bottom - line.top != 0.0f) {
		first_y = (line.bottom - line.top) * start / (finish - start);
		first_y = unit_gap_y * ceil(first_y / unit_gap_y) - first_y;
	}
	for (int i = 0; i < num; i++) {
		float x = first_x + (float)line.left + unit_gap_x * (float)i;
		float y = first_y + (float)line.top + unit_gap_y * (float)i;
		POINT size = ui.fbo.get_size(ui.unit_line);
		if (
			(abs(line.left - x) > abs(line.right - line.left)) ||
			(abs(line.top - y) > abs(line.bottom - line.top))
			) {
			continue;
		}
		ui.fbo.draw_c((int)x - size.x / 2, (int)y - size.y / 2, ui.unit_line);
	}
	ofLine(line.left, line.top, line.right, line.bottom);
}

GraphPos GUI::conversion_g(frame *f, graph g, WindowPos w_pos) {
	GraphPos g_pos;
	g_pos.x = percent(w_pos.x ,f->pos.left, f->pos.right, g.start_index, g.end_index);
	g_pos.y = percent((float)w_pos.y, (float)(f->pos.bottom), (float)(f->pos.top), g.val_min, g.val_max);
	return g_pos;
}
WindowPos GUI::conversion_w(frame *f, graph g, GraphPos g_pos) {
	WindowPos w_pos;
	w_pos.x = percent(g_pos.x, g.start_index, g.end_index, f->pos.left, f->pos.right);
	w_pos.y = (int)percent((float)g_pos.y, g.val_min, g.val_max, (float)(f->pos.bottom), (float)(f->pos.top));
	return w_pos;
}


void GUI::wave_graph(frame *f, graph g, bool mode) {
	//描画効率化のため、ピクセルの数に合わせて描画
	int index = 0; //配列から値を参照するときのインデックス
	float percentage = 0.0f;
	float height; //今描画する波の高さ
	float b_height; //前フレームのheight
	float zero = percent(0.0f, g.val_min, g.val_max, (float)(f->pos.bottom), (float)(f->pos.top)); //グラフy座標が0のときのフレームy座標
	for (int i = 0; i < f->size.x; i++) {
		percentage = percent((float)i, 0.0f, (float)f->size.x, (float)g.start_index, (float)g.end_index); //一時代入
		index = (int)percentage; //波形配列のインデックス算出
		percentage = percentage - (float)index; //int,floatの性質上必ず正の数になる
												//(念の為)index+1>=num_sampleになったらループ脱出
		if (index + 1 >= g.num_sample) {
			break;
		}
		//今描画する波の高さ
		height =
			percent(g.samples[index], g.val_min, g.val_max, (float)(f->pos.bottom), (float)(f->pos.top)) * (1.0f - percentage) +
			percent(g.samples[index + 1], g.val_min, g.val_max, (float)(f->pos.bottom), (float)(f->pos.top)) * (percentage);
		//グラフ描画
		if (i > 0) {
			ofSetColor(255, 255, 255, 255);
			ofLine(
				f->pos.left + i - 1,
				b_height,
				f->pos.left + i,
				height
			);
		}
		//塗りつぶし
		if (mode) {
			if ((i / 10) % 2 == 0) {
				ofSetColor(255, 255, 255, 255);
				ofLine(
					f->pos.left + i,
					zero,
					f->pos.left + i,
					height
				);
			}
		}
		b_height = height; //前フレームのときのheight取得用
	}
}

graph GUI::c_graph(float *samples, GraphPara *param) {
	graph req;
	req.samples = samples;
	req.start_index = (int)(param->get_min(param->get_x_dim()));
	req.end_index = (int)(param->get_max(param->get_x_dim()));
	req.zero_index_val = param->get_lim_min(param->get_x_dim());
	req.last_index_val = param->get_lim_max(param->get_x_dim());
	req.val_min = param->get_min(param->get_y_dim());
	req.val_max = param->get_max(param->get_y_dim());
	req.val_lim_min = param->get_lim_min(param->get_y_dim());
	req.val_lim_max = param->get_lim_max(param->get_y_dim());
	req.num_sample = (int)(req.last_index_val - req.zero_index_val) + 1;
	return req;
}

void GUI::graph_unitline(frame *f, graph g, char *x, char *y) {
	//変数宣言
	float x_min = g.start_index; //xの描画領域内の最小値取得
	float x_max = g.end_index; //xの描画領域内の最大値取得
	float y_min = g.val_min; //yの描画領域内の最小値取得
	float y_max = g.val_max; //yの描画領域内の最大値取得
	float x_lim_length = (float)(g.num_sample);
	float y_lim_length = (g.val_lim_max - g.val_lim_min);
	WindowPos axis = conversion_w(f, g, {0,0.0f}); //xy軸のウィンドウ座標
	//軸位置制御
	if (axis.x < f->pos.left) {
		axis.x = f->pos.left;
	}
	if (axis.x > f->pos.right) {
		axis.x = f->pos.right;
	}
	if (axis.y < f->pos.top) {
		axis.y = f->pos.top;
	}
	if (axis.y > f->pos.bottom) {
		axis.y = f->pos.bottom;
	}
	//軸描画
	unit_line(
	{
		(int)axis.x,
		f->pos.bottom,
		(int)axis.x,
		f->pos.top
	},
		y_lim_length,
		y_min,
		y_max,
		8
	);
	unit_line(
	{
		f->pos.left,
		(int)axis.y,
		f->pos.right,
		(int)axis.y
	},
		x_lim_length,
		x_min,
		x_max,
		8
	);
	//単位文字列描画
	ofDrawBitmapString(x, f->pos.left, f->pos.top);
	ofDrawBitmapString(y, f->pos.right, f->pos.bottom);
}

void GUI::wave_draw(frame *f, graph g, bool mode, bool active) {
	if (!active) {
		wave_graph(f, g, mode);
		return;
	}
	if (win_event.in(f->pos) && win_event.l_click) {
		GraphPos start = conversion_g(f, g, win_event.b_mouse);
		GraphPos end = conversion_g(f, g, win_event.mouse);
		if (start.x == end.x) {
			if ((start.x >= 0) || (start.x <= g.num_sample - 1)) {
				g.samples[start.x] = start.y;
			}
		}else{
			int index;
			bool minus = (start.x > end.x);
			float p;
			for (int i = 0; i < abs(start.x - end.x) + 1; i++) {
				p = (float)i / (float)abs(start.x - end.x);
				index = start.x + (1-2*minus)*i;
				if ((index < 0) || (index > g.num_sample - 1)) {
					break;
				}
				g.samples[index] =
					start.y * (1.0f - p) +
					end.y * p;
			}
		}
	}
	wave_graph(f, g, mode);
}

void GUI::wave_gui(frame *f, float *samples, int num_sample, bool mode) {
	GraphPara *param;
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
		param = (GraphPara*)(f->data[1]); //クラスのポインタ代入
		param->set_frame(f->childs[0]); //フレームのポインタ代入
										//パラメータ追加
		param->create(2, 0.0f, -1.0f);
		param->create(2, (float)num_sample - 1, 1.0f);
		param->create(2, 30.0f, 0.3f);
		//パラメータの役割設定
		param->set_min(0, 0);
		param->set_max(0, 1);
		param->set_min(1, 0);
		param->set_max(1, 1);
		//パラメータ可動域設定
		param->limit_min(0.0f, 0);
		param->limit_max((float)num_sample - 1, 0);
		param->limit_min(-2.0f, 1);
		param->limit_max(2.0f, 1);
		param->set_length_min(0, 0.001f);
		param->set_length_min(1, 0.001f);
		//ウィンドウのx,y座標に対応する次元数の設定
		param->set_window_dim(0, 1);
		//フレームリサイズ
		frames.resize(frames.get_root(f), frames.get_root(f)->pos);
	}
	else {
		param = (GraphPara*)(f->data[1]);
	}
	//グラフの描画
	graph g = c_graph(samples, param);
	graph_unitline(f->childs[0], g, "ms", "dB");
	//パラメータ操作カーソル描画
	cursor(param, 0, ui.zoom_cursor); //グラフの最小値のパラメータ描画
	cursor(param, 1, ui.zoom_cursor); //グラフの最大値のパラメータ描画
	cursor(param, 2, ui.xy_pointer_cursor); //テストパラメータ描画
	//グラフの書き換え
	wave_draw(f->childs[0], g, mode, param->get_active() == -1);
}

void GUI::volume(frame *f, float *val) {
	GraphPara *param;
	int p1 = f->pos.left;
	int p2 = f->pos.right;
	int height = f->size.y / 2;
	if (f->data.size() == 0) {
		//グラフパラメータクラス
		f->data.push_back(new GraphPara);
		//クラスのポインタ代入
		param = (GraphPara*)(f->data[0]);
		//フレームのポインタ代入
		param->set_frame(f);
		//パラメータ追加
		param->create(1, 0.0f);
		param->create(1, 1.0f);
		param->create(1, *val);
		//パラメータの役割設定
		param->set_min(0, 0);
		param->set_max(0, 1);
		//パラメータ可動域設定
		param->limit_min(0.0f, 0);
		param->limit_max(1.0f, 0);
		//ウィンドウのx座標に対応する次元数の設定
		param->set_window_dim(0, -1, height);
	}
	else {
		param = (GraphPara*)(f->data[0]);
	}
	ofSetColor(255, 255, 255, 255);
	ofRect(p1, f->pos.top + height, p2 - p1, 1);
	ofRect(p1, f->pos.top + height - 2, 1, 5);
	ofRect(p2 - 1, f->pos.top + height - 2, 1, 5);
	cursor(param, 2, ui.x_pointer_cursor); //パラメータ描画
	*val = param->get_param(0, 2);
}

void GUI::volume_gui(frame *f, float *val) {
	frame *vol_f;
	if (f->data.size() == 0) {
		f->data.push_back(new frame);
	}
	vol_f = (frame*)(f->data[0]);
	vol_f->size.x = 200;
	vol_f->size.y = 15;
	vol_f->pos.left = f->pos.left + 4;
	vol_f->pos.top = f->pos.top + 20;
	vol_f->pos.right = vol_f->pos.left + vol_f->size.x;
	vol_f->pos.bottom = vol_f->pos.top + vol_f->size.y;
	volume(vol_f, val);
}

void GUI::cursor(GraphPara *param, int index, char mode) {
	POINT pos;
	POINT size = ui.fbo.get_size(mode);
	param->seek(index, win_event.mouse, win_event.l_click, size);
	pos = param->get_pos(index);
	ui.fbo.draw_c(pos.x - size.x / 2, pos.y - size.y / 2, mode);
}

void GUI::sw(frame *f, bool *sw) {
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
	}else {
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

void GUI::loop() {
	win_event.loop();
}