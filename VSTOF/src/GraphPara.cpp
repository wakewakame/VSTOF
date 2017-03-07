#include "GraphPara.h"

GraphPara::GraphPara() {
	active = -1;
}

float GraphPara::percent(float a, float a_min, float a_max, float b_min, float b_max) {
	float b;
	if ((a_max - a_min) != 0) {
		b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
		return b;
	}
	else {
		return 0.0f;
	}
}

int GraphPara::percent(int a, int a_min, int a_max, int b_min, int b_max) {
	return (int)percent((float)a, (float)a_min, (float)a_max, (float)b_min, (float)b_max);
}

void GraphPara::set_frame(frame *set_f) {
	f = set_f;
}

void GraphPara::create(int argnum, ...) {
	va_list arg; //引数リスト
				 //次元数が足りなければ追加
	while (param.size() < argnum) {
		Dimension empty;
		param.push_back(empty);
		length_min.push_back(0.0f);
		int dim = param.size() - 1;
		param[dim].on_limit_min = 0;
		param[dim].on_limit_max = 0;
		param[dim].limit_min = -1.0f;
		param[dim].limit_max = 1.0f;
		param[dim].min_index = -1;
		param[dim].max_index = -1;
	}
	va_start(arg, argnum);
	for (int i = 0; i < param.size(); i++) {
		param[i].val.push_back((float)va_arg(arg, double));
	}
	va_end(arg);
}

void GraphPara::set_min(int dim, int index) {
	param[dim].min_index = index;
}

void GraphPara::set_max(int dim, int index) {
	param[dim].max_index = index;
}

void GraphPara::set_length_min(int dim, float min) {
	length_min[dim] = min;
}

const float GraphPara::get_param(int dim, int index) {
	return param[dim].val[index];
}

void GraphPara::limit_min(float min, int dim) {
	param[dim].on_limit_min = 1;
	param[dim].limit_min = min;
}

void GraphPara::limit_max(float max, int dim) {
	param[dim].on_limit_max = 1;
	param[dim].limit_max = max;
}

const float GraphPara::get_lim_min(int dim) {
	return param[dim].limit_min;
}

const float GraphPara::get_lim_max(int dim) {
	return param[dim].limit_max;
}

const float GraphPara::get_min(int dim) {
	return param[dim].val[param[dim].min_index];
}

const float GraphPara::get_max(int dim) {
	return param[dim].val[param[dim].max_index];
}

const int GraphPara::get_imin(int dim) {
	return param[dim].min_index;
}

const int GraphPara::get_imax(int dim) {
	return param[dim].max_index;
}

void GraphPara::set_active(int index) {
	if (index != -1) {
		active = index;
	}
	else {
		active = -1;
	}
}

const int GraphPara::get_active() {
	return active;
}

void GraphPara::set_window_dim(int set_x_dim, int set_y_dim) {
	x_dim = set_x_dim;
	y_dim = set_y_dim;
}

void GraphPara::set_window_dim(int set_x_dim, int set_y_dim, int set_height) {
	set_window_dim(set_x_dim, set_y_dim);
	height = set_height;
}

const int GraphPara::get_x_dim() {
	return x_dim;
}

const int GraphPara::get_y_dim() {
	return y_dim;
}

const POINT GraphPara::get_pos(int index) {
	POINT pos;
	if (get_active() == index) {
		if (x_dim != -1) {
			pos.x = mouse.x;
		}
		else {
			pos.x = f->pos.left + height;
		}
		if (y_dim != -1) {
			pos.y = mouse.y;
		}
		else {
			pos.y = f->pos.top + height;
		}
	}
	else {
		if (x_dim != -1) {
			pos.x = (int)(percent(
				param[x_dim].val[index],
				get_min(x_dim),
				get_max(x_dim),
				(float)f->pos.left,
				(float)f->pos.right
			));
		}
		else {
			pos.x = f->pos.left + height;
		}
		if (y_dim != -1) {
			pos.y = (int)(percent(
				param[y_dim].val[index],
				get_min(y_dim),
				get_max(y_dim),
				(float)f->pos.bottom,
				(float)f->pos.top
			));
		}
		else {
			pos.y = f->pos.top + height;
		}
	}
	return pos;
}

void GraphPara::set_pos(POINT pos, int index) {
	if (x_dim != -1) {
		param[x_dim].val[index] = percent(
			(float)pos.x,
			(float)f->pos.left,
			(float)f->pos.right,
			get_min(x_dim),
			get_max(x_dim)
		);
	}
	if (y_dim != -1) {
		param[y_dim].val[index] = percent(
			(float)pos.y,
			(float)f->pos.bottom,
			(float)f->pos.top,
			get_min(y_dim),
			get_max(y_dim)
		);
	}
}

const bool GraphPara::hit(RECT area, POINT pos) {
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
const bool GraphPara::hit(POINT center, POINT size, POINT pos) {
	return hit({
		center.x - size.x / 2,
		center.y - size.y / 2,
		center.x + size.x / 2,
		center.y + size.y / 2
	}, pos);
}

void GraphPara::seek(int index, POINT n_mouse, bool l_click, POINT size) {
	mouse = n_mouse;
	if (hit(get_pos(index), size, n_mouse) && l_click && get_active() == -1) {
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

void GraphPara::limiter(int index) {
	for (int i = 0; i < param.size(); i++) {
		//下限判定
		if (
			(param[i].val[index] < get_lim_min(i)) &&
			param[i].on_limit_min
			) {
			param[i].val[index] = get_lim_min(i);
		}
		//上限判定
		if (
			(param[i].val[index] > get_lim_max(i)) &&
			param[i].on_limit_max
			) {
			param[i].val[index] = get_lim_max(i);
		}
		//最小値<最大値の判定
		if (abs(get_min(i) - get_max(i)) <= length_min[i]) {
			if (active = get_imin(i)) {
				param[i].val[get_imin(i)] = get_max(i) - length_min[i];
			}
			else {
				param[i].val[get_imax(i)] = get_min(i) + length_min[i];
			}
		}
	}
}