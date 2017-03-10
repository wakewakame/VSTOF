#pragma once

#include "define.h"

//�t���[������N���X
class Frames {
public:
	//�֐��錾
	//�t���[���ǉ��֐�(����:�e�t���[��(�Ȃ����nullptr),���t���[��,�t���[����,�q�t���[���̔z����@(0�Ȃ�c,1�Ȃ牡),�q�t���[���Ԃ̊Ԋu,���t���[���̒���(mode=0�Ȃ獂��,1�Ȃ畝),���t���[�����Œ�T�C�Y�ɂ���on/off)
	void add(frame *parent, frame *self, std::string name, int length, bool lock);
	//�q�t���[���̕��ו��ݒ�֐�
	void set_parent(frame *self, bool mode, int gap);
	//�t���[���̉���L��
	void set_description(frame *self, std::string description);
	//�S�t���[���̓o�^�������ɁA���[�t���[������S�e�t���[����length���擾�֐�
	void get_length(frame *f);
	//�S�t���[���̈ʒu�Z�o(����1,2,3:���t���[���̃|�C���^,���t���[����`�悷��ʒu,0�Ȃ璷���͏����l�̂܂�)
	void sub_resize(frame *f, RECT pos);
	//�w��t���[���ȉ��̃t���[�����ׂĂ̍Ĕz�u
	void resize(frame *f, RECT pos);
	//�q�t���[���̃|�C���^����root�t���[���̃|�C���^�擾
	frame* get_root(frame *f);
	//�w��t���[���ȉ��̂��ׂẴt���[����dispose���s
	void all_dispose(frame *f);
	//�t���[������new�����N���X�����ׂ�delete����֐�
	void dispose(frame *f);
};