#pragma once

#include "define.h"

//���L�������A�N�Z�X�N���X
class ShareMem {
public:
	//�ϐ�������
	HANDLE MapHandle = nullptr; //�}�b�v�̃n���h��
	ShareMemData *smd = nullptr; //���L�������f�[�^�|�C���^
	int ShareMemSize = sizeof(ShareMemData); //�}�b�v�̃T�C�Y

	//�֐��錾
	~ShareMem();
	bool Open(LPCTSTR MapName);
};