#include "ui_design.h"

UI_DESIGN::UI_DESIGN() {
	//�o�b�t�@�m��
	fbo.add(15, 15);
	fbo.add(15, 15);
	fbo.add(15, 15);
	fbo.add(15, 15);
	//�`��
	//x�|�C���^�[�J�[�\��
	fbo.change_c(x_pointer_cursor);
	ofSetColor(255, 255, 255, 255);
	ofRect(0, 0, 15, 15);
	fbo.change_c(-1);
	fbo.change_a(x_pointer_cursor);
	ofSetColor(0, 0, 0, 30);
	ofRect(0, 0, 15, 15);
	ofSetColor(0, 0, 0, 255);
	ofRect(7, 0, 1, 4);
	ofRect(7, 11, 1, 4);
	ofRect(5, 5, 5, 5);
	ofSetColor(0, 0, 0, 0);
	ofRect(6, 6, 3, 3);
	fbo.change_a(-1);
	//xy�|�C���^�[�J�[�\��
	fbo.change_c(xy_pointer_cursor);
	ofSetColor(255, 255, 255, 255);
	ofRect(0, 0, 15, 15);
	fbo.change_c(-1);
	fbo.change_a(xy_pointer_cursor);
	ofSetColor(0, 0, 0, 30);
	ofRect(0, 0, 15, 15);
	ofSetColor(0, 0, 0, 255);
	ofRect(7, 0, 1, 4);
	ofRect(7, 11, 1, 4);
	ofRect(0, 7, 4, 1);
	ofRect(11, 7, 4, 1);
	ofRect(5, 5, 5, 5);
	ofSetColor(0, 0, 0, 0);
	ofRect(6, 6, 3, 3);
	fbo.change_a(-1);
	//�Y�[���J�[�\��
	fbo.change_c(zoom_cursor);
	ofSetColor(255, 255, 255, 255);
	ofRect(0, 0, 15, 15);
	fbo.change_c(-1);
	fbo.change_a(zoom_cursor);
	ofSetColor(0, 0, 0, 255);
	ofRect(0, 0, 15, 15);
	ofSetColor(0, 0, 0, 30);
	ofRect(5, 5, 5, 5);
	fbo.change_a(-1);
	//�P�ʐ�
	fbo.change_c(unit_line);
	ofSetColor(255, 255, 255, 255);
	ofRect(0, 0, 15, 15);
	fbo.change_c(-1);
	fbo.change_a(unit_line);
	ofSetColor(0, 0, 0, 0);
	ofRect(0, 0, 15, 15);
	ofSetColor(0, 0, 0, 255);
	ofRect(0, 7, 15, 1);
	ofRect(7, 0, 1, 15);
	fbo.change_a(-1);
}