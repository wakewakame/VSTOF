#include "FBO.h"

FBO::FBO() {
	now_index = -1;
	num = 0;
}

FBO::~FBO() {
	if (now_index != -1) {
		color[now_index].end();
		alpha[now_index].end();
	}
}

int FBO::add(int x, int y) {
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

void FBO::change_c(int index) {
	if (now_index != -1) {
		color[now_index].end();
	}
	if (index != -1) {
		color[index].begin();
	}
	now_index = index;
}

void FBO::change_a(int index) {
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

void FBO::draw_c(int x, int y, int index) {
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);
	color[index].draw(x, y);
}

POINT FBO::get_size(int index) {
	POINT p;
	p.x = color[index].getWidth();
	p.y = color[index].getHeight();
	return p;
}