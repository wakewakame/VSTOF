#pragma once

#include "Debug.h"
#include "define.h"
#include "ShareMem.h"
#include "Placement.h"

//�S�Ă̏������s���N���X
class AllProcess {
public:
	//�ϐ�������
	WINDOW_INFO win_info; //�E�B���h�E���ێ��ϐ�������

						  //�R���X�g���N�g
	ShareMem sm; //���L�������N���X
	Draw draw; //�`��N���X

					   //�֐��錾
	bool init(LPSTR cmd, WINDOW_INFO win_info2);
	bool loop();
	//�I��������
	~AllProcess();
};