#pragma once

#include "define.h"

//�E�B���h�E�C�x���g�����N���X
class WIN_EVENT {
public:
	//�ϐ��錾
	WindowPos mouse; //���݂̃}�E�X���W
	WindowPos b_mouse; //�O�t���[�����̃}�E�X���W
	bool l_click; //���݂̍��N���b�N���(������Ă�����1)
	bool b_l_click; //�O�t���[����l_click�ϐ��̓��e

	//�֐��錾
	//�R���X�g���N�g
	WIN_EVENT();
	//�w��RECT���ɑ��݂��邩�ǂ���
	bool in(RECT area);
	bool in(POINT pos, int size);
	//�}�E�X�����N���b�N���ꂽ���ǂ���
	bool get_l_click();
	//�w��RECT���Ń}�E�X���N���b�N���ꂽ���ǂ���
	bool l_click_in(RECT area);
	//���t���[���Ăяo���֐�(�����̍Ō�ɌĂяo��)
	void loop();
};