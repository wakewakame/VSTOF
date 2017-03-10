#include "AllProcess.h"

bool AllProcess::init(LPSTR cmd, WINDOW_INFO win_info2) { //�����������s�֐�
											  //�ϐ����
	win_info = win_info2;
	//���L�������֘A
	if (sm.Open(cmd)) {
		return 1;
	}
	if (sm.smd->Ready != nullptr) { //���d�N���h�~
		return 1;
	}
	else {
		sm.smd->Ready = win_info.hwnd;
	}
	//Draw�֘A
	if (draw.resize(win_info)) { //�g�p���̃E�B���h�E�T�C�Y�X�V
		return 1;
	}
	//���̑�
	//���L�������f�[�^���p�����[�^�N���X�̕ϐ��Ɠ���
	draw.set_p_value(&sm.smd->para);
	return 0;
}

bool AllProcess::loop() { //���[�v���ɌĂяo���֐�
			  //Draw�֘A
			  //�O�t���[���ƃE�B���h�E�T�C�Y��r
	if (
		(win_info.size.x != win_info.b_size.x) ||
		(win_info.size.y != win_info.b_size.y)
		) {
		if (draw.resize(win_info)) { //�g�p���̃E�B���h�E�T�C�Y�X�V
			return 1;
		}
	}
	//GUI�`��
	draw.loop();
	Debug::SetFpsTitle(win_info);
	return 0;
}

AllProcess::~AllProcess() {
	if (sm.smd != nullptr) {
		sm.smd->Ready = nullptr;
	}
}