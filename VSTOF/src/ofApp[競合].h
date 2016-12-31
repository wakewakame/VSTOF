#pragma once

#include "ofMain.h"
#include "windows.h"
#include <vector>
#include <string>

//�E�B���h�E���ێ��ϐ�
struct WINDOW_INFO {
	POINT size; //�E�B���h�E�T�C�Y
	POINT b_size; //�O�t���[���̃E�B���h�E�T�C�Y
	RECT window; //�E�B���h�E����E�����W(�f�X�N�g�b�v�)
	HDC glDc; //OpenFrameworks��OpenGL�̃f�o�C�X�R���e�L�X�g
	HWND hwnd; //OpenFrameworks�̐����E�B���h�E�̃n���h��
	int fps; //�E�B���h�E�̕`��t���[�����[�g
	double speed; //60fps����Ƃ����t���[���P�ʂ̃A�j���[�V�������x
};
//�\���̐錾
struct VSTParameteres { //�p�����[�^�[�l�\����
						//���F�n
	bool make_auto; //�����ŉ��F�𐶐����邩
	bool use_rawwave; //���g�`�f�[�^�̎g�p�����邩�ǂ���
	bool change_natural; //�����ω������R�ɂ��邩
	float overtones[512]; //�{���O���tor���X�y�N�g���O���t
	int iovertones; //�{���̐�
					//���g�`�f�[�^�h��
	float rawwave[4096]; //���g�`�̎g�p�����g�`(�t�@�C���}�b�s���O�ɂ��邩��(�L�E�ցE�M))
	int nrawwave; //���g�`�̃T���v����
	float base_pitch; //��ʒu(�蓮�ύX��)
					  //�����g�`
	float outwave[4096]; //�o�͂����g�`
	int noutwave; //�o�͔g�`�̃T���v����
				  //�t�F�[�h�n
	bool use_string_mode; //�����[�h�̎g�p�����邩�ǂ���
	bool use_fade_change; //�Ȃ߂炩�ȉ���,���ʂ̕ω����g�p���邩�ǂ���(2�����ɉ����o���Ȃ�)
	float fadein_vol[6];
	float fadein_pitch[6];
	float fadeout_vol[6];
	float fadeout_pitch[6];
	float fadechange_vol[6];
	float fadechange_pitch[6];
	//�g�`�̐�Βl�̏��
	float wave_limit;
	//��{�g�`�̌`
	char wave_type;
	//�Đ����ύX�ł���ƕ֗��ȃp�����[�^�[
	float vol;
	float pitch; //MIDI�l�ɉ��Z
};
//���L�������\����
struct ShareMemData {
	HWND Ready; //GUI�N���C�A���g���N�������TRUE�ɂ���(���d�N������̂���)
	HWND Host; //�`���n���h��
	VSTParameteres para; //VST�̃p�����[�^�[�Q
};
//�t���[���\����
struct frame {
	frame *parent; //�e�t���[���̃|�C���^
	int num_child; //�q�t���[���̐�
	std::vector<frame*> childs; //�S�q�t���[���̃|�C���^�z��
	int index;//���t���[�����̎��t���[���̊����ԍ�(=0,1,2,3,...)
	RECT pos; //�t���[�����W
	POINT size; //�t���[���T�C�Y(���[�t���[���̂ݑ��)
	std::string name; //�t���[���̖���
	bool mode; //�q�t���[�����c����=0,������=1
	int gap; //�q�t���[���ԓ��m�̌���(px�P��)
	int length; //�S�t���[���������l�T�C�Y���̎��t���[���̃T�C�Y
	bool lock_lenght; //�e�q�t���[���̒���(mode=0�Ȃ�c��,mode=1�Ȃ牡��)�̌Œ�on/off
};
//�p�����[�^�[�l�\����
struct VSTParameteresFrames {
	frame root;
	frame tone, fade;
	//���F�n
	frame make_auto; //�����ŉ��F�𐶐����邩
	frame raw_wave_para; //���L�C���f���g�𑩂˂�
						 //���g�`�f�[�^�h��
	frame use_rawwave; //���g�`�f�[�^�̎g�p�����邩�ǂ���
	frame rawwave; //���g�`�̎g�p�����g�`(�t�@�C���}�b�s���O�ɂ��邩��(�L�E�ցE�M))
	frame base_pitch; //��ʒu(�蓮�ύX��)
	frame tone_para; //���L�C���f���g�̃t���[���𑩂˂�
	frame change_natural; //�����ω������R�ɂ��邩
	frame overtones; //�{���O���tor���X�y�N�g���O���t
	frame iovertones; //�{���̐�
	frame wave_type; //��{�g�`�̌`
					 //�Đ����ύX�ł���ƕ֗��ȃp�����[�^�[
	frame hostpar;
	frame vol;
	frame pitch; //MIDI�l�ɉ��Z
				 //�g�`�̐�Βl�̏��
	frame wave_limit;
	//�����g�`
	frame outwave; //�o�͂����g�`
				   //�t�F�[�h�n
	frame use_string_mode; //�����[�h�̎g�p�����邩�ǂ���
	frame use_fade_change; //�Ȃ߂炩�ȉ���,���ʂ̕ω����g�p���邩�ǂ���(2�����ɉ����o���Ȃ�)
	frame fadein;
	frame fadein_vol;
	frame fadein_pitch;
	frame fadeout;
	frame fadeout_vol;
	frame fadeout_pitch;
	frame fadechange;
	frame fadechange_vol;
	frame fadechange_pitch;
};
//RECT��float�^��
struct RECTF {
	float left;
	float top;
	float right;
	float bottom;
};
//�e�p�����[�^�[�̕\���͈�
struct VSTParameteresLength {
	RECTF rawwave;
	RECTF base_pitch;
	RECTF overtones;
};
struct animation {
	float click; //�N���b�N���[�V����
	float on; //�}�E�X���d�Ȃ����Ƃ��̃��[�V����
};

//���L�������A�N�Z�X�N���X
class ShareMem {
public:
	//�ϐ�������
	HANDLE MapHandle = nullptr; //�}�b�v�̃n���h��
	ShareMemData *smd = nullptr; //���L�������f�[�^�|�C���^
	int ShareMemSize = sizeof(ShareMemData); //�}�b�v�̃T�C�Y

											 //�֐��錾
	bool Open(LPCTSTR MapName) { //���L�������A�N�Z�X�֐�
		if (MapName == nullptr) { //�R���\�[���������Ȃ���΃G���[
			return 1;
		}
		MapHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapName); //�t�@�C��������}�b�v�n���h���擾
		if (MapHandle == NULL) { //�}�b�v�����݂��Ă��Ȃ���΃G���[
			return 1;
		}
		smd = (ShareMemData *)MapViewOfFile(MapHandle, FILE_MAP_ALL_ACCESS, 0, 0, ShareMemSize); //�}�b�v���e����
		if (smd == NULL) { //�}�b�v���e�擾���ł��Ă��Ȃ���΃G���[
			return 1;
		}
		return 0;
	}
	~ShareMem() { //���L�������N���[�Y
		UnmapViewOfFile(smd); //���L�������f�[�^�|�C���^�J��
	}
};

//�t���[������N���X
class Frames {
public:
	//�֐��錾
	void add(frame *parent, frame *self, std::string name, bool mode, int gap, int length, bool lock_length) { //�t���[���̒ǉ�
		self->parent = parent; //�e�t���[���̃|�C���^
		self->num_child = 0; //�q�t���[���̐�������
		self->pos.left = 0; //�t���[�������W
		self->pos.top = 0; //�t���[������W
		self->pos.right = 0; //�t���[���E���W
		self->pos.bottom = 0; //�t���[�������W
		self->size.x = 0; //�t���[��x�T�C�Y
		self->size.y = 0; //�t���[��y�T�C�Y
		self->name = name; //�t���[���̖���
		self->mode = mode; //�q�t���[�����c����=0,������=1
		self->gap = gap; //�q�t���[���ԓ��m�̌���(px�P��)
		self->length = length; //�S�t���[���������l�T�C�Y���̎��t���[���̃T�C�Y
		self->lock_lenght = lock_length; //���݂̎��t���[���̒���(mode = 0�Ȃ�c��, mode = 1�Ȃ牡��)�̌Œ��on/off
		self->index = 0;
		//�e�t���[�������w��̏ꍇ�A�ȉ��̏������Ȃ�
		if (parent == nullptr) {
			return;
		}
		self->parent->childs.push_back(self); //�e�t���[���Ɏ��t���[���ǉ��o�^
		self->index = self->parent->num_child;//���t���[�����̎��t���[���̊����ԍ�(=0,1,2,3,...)
		self->parent->num_child += 1; //�e�t���[�����X�V
		return;
	}
	int GetFrameHight(frame *f) { //���[�t���[���̏����T�C�Y����S�t���[���̏���y�T�C�Y�擾
		int sum = 0;
		//���t���[���̏��L����S�Ă̎q�t���[���̏������擾
		for (int i = 0; i < f->num_child; i++) {
			if (f->childs[i]->num_child == 0) {
				sum += f->childs[i]->length;
			}else{
				//�q�t���[�����q�t���[�����������Ă����ꍇ�A�ċA����
				sum += GetFrameHight(f->childs[i]);
			}
		}
		sum += f->gap * (f->num_child + 1); //�q�t���[���ԓ��m�̌��Ԃ��܂߂��l���Z�o
		return sum; //�t���[���̍�����Ԃ�
	}
	bool MakeFramePos(frame *f) { //�t���[���ʒu�Z�o
		for (int i = 0; i < f->num_child; i++) {
			if (f->childs[i]->num_child == 0) {

			}else{

			}
		}
		/*
		if (f->child == NULL) { //���t���[�����q�t���[���������Ă��邩�m�F
			return 1;
		}
		int x = f->x;
		bool mode = f->mode; //���t���[���̎q�t���[��������@�擾
		int num_x = f->pos.left; //���t���[���̍����W����q�t���[���z�u
		int num_y = f->pos.top; //���t���[���̏���W����q�t���[���z�u
		int num = 0; //�q�t���[���̐��J�E���g�p�ϐ�
		frame *last; //�q�t���[����1�Â������ϐ�
		last = f->child; //���t���[���̍ŏ��̎q�t���[�����
		if (mode) { //���t���[���̎q�t���[���������тł���Ύq�t���[���̌��J�E���g
			while (true) {
				num += 1;
				if (last->next == NULL) { //���t���[�����Ȃ���΃��[�v�I��
					break;
				}
				last = last->next; //���̃t���[���Ɉړ�
			}
			x /= num; //�t���[���̉��������T�C�Y
		}
		last = f->child; //���t���[���̍ŏ��̎q�t���[�����
		while (true) {
			last->x = x; //�t���[�������擾
			last->pos.left = num_x; //�q�t���[���̍����W�擾
			last->pos.top = num_y; //�q�t���[���̏���W�擾
			last->pos.right = last->pos.left + last->x; //�q�t���[���̉E���W�Z�o
			last->pos.bottom = last->pos.top + last->y; //�q�t���[���̉����W�Z�o
			if (last->child != NULL) { //�q�t���[���Ɏq�t���[��������΂���̈ʒu���o
				MakeFramePos(last);
			}
			if (last->next != NULL) { //���t���[���������
				if (!mode) { //�c���тł����
					num_y = last->pos.bottom; //���t���[���̏���W�X�V
				}
				else { //�����тł����
					num_x = last->pos.right; //���t���[���̍����W�X�V
				}
				last = last->next; //���̃t���[���Ɉړ�
			}
			else { //���t���[�����Ȃ���΃��[�v�I��
				break;
			}
		}
		return 0;
		*/
	}
	void resize(frame *f, RECT size) {
		f->pos.left = size.left;
		f->pos.top = size.top;
		f->x = size.right - size.left;
		MakeFramePos(f);
	}
};

//�p�����[�^�[�̒l��t���[���ϐ��Ǘ��N���X
class Parameteres {
public:
	//�R���X�g���N�g
	Frames frames; //�t���[������N���X

				   //�ϐ�������
	VSTParameteres *p_value; //�p�����[�^�[�l�ϐ��Q�|�C���^
	VSTParameteresFrames p_frame; //�p�����[�^�[�t���[���ϐ��Q
	VSTParameteresLength p_length; //�p�����[�^�[�\���͈͕ϐ��Q

								   //�֐��錾
	Parameteres() { //�S�p�����[�^�[���̃t���[���쐬
					//�t���[������
		frames.add(NULL, &p_frame.root, 0, 0); //���[�g�t���[������
		frames.add(&p_frame.root, &p_frame.tone, 0, 0); //���F�ݒ�t���[������
		frames.add(&p_frame.tone, &p_frame.make_auto, 0, 100); //�����ŉ��F�𐶐����邩
		frames.add(&p_frame.tone, &p_frame.raw_wave_para, 0, 0); //���L�C���f���g�𑩂˂�
		frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, 0, 100); //���g�`�f�[�^�̎g�p�����邩�ǂ���
		frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, 0, 100); //���g�`�̎g�p�����g�`(�t�@�C���}�b�s���O�ɂ��邩��(�L�E�ցE�M))
		frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, 0, 100); //��ʒu(�蓮�ύX��)
		frames.add(&p_frame.tone, &p_frame.tone_para, 0, 0); //���L�C���f���g�̃t���[���𑩂˂�
		frames.add(&p_frame.tone_para, &p_frame.change_natural, 0, 100); //�����ω������R�ɂ��邩
		frames.add(&p_frame.tone_para, &p_frame.overtones, 0, 100); //�{���O���tor���X�y�N�g���O���t
		frames.add(&p_frame.tone_para, &p_frame.iovertones, 0, 100); //�{���̐�
		frames.add(&p_frame.tone_para, &p_frame.wave_type, 0, 100); //��{�g�`�̌`
		frames.add(&p_frame.tone_para, &p_frame.hostpar, 1, 0); //���L�C���f���g�̃t���[���𑩂˂�
		frames.add(&p_frame.hostpar, &p_frame.vol, 0, 100); //����
		frames.add(&p_frame.hostpar, &p_frame.pitch, 0, 100); //����(IDI�l�ɉ��Z)
		frames.add(&p_frame.tone, &p_frame.wave_limit, 0, 100); //�g�`�̐�Βl�̏��
		frames.add(&p_frame.tone, &p_frame.outwave, 0, 100); //�o�͔g�`
		frames.add(&p_frame.root, &p_frame.fade, 0, 0); //�t�F�[�h�ݒ�t���[������
		frames.add(&p_frame.fade, &p_frame.use_string_mode, 0, 100); //�����[�h�̎g�p�����邩�ǂ���
		frames.add(&p_frame.fade, &p_frame.use_fade_change, 0, 100); //�Ȃ߂炩�ȉ���,���ʂ̕ω����g�p���邩�ǂ���(2�����ɉ����o���Ȃ�)
		frames.add(&p_frame.fade, &p_frame.fadein, 1, 0);
		frames.add(&p_frame.fadein, &p_frame.fadein_vol, 0, 100);
		frames.add(&p_frame.fadein, &p_frame.fadein_pitch, 0, 0);
		frames.add(&p_frame.fade, &p_frame.fadeout, 1, 0);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, 0, 100);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, 0, 0);
		frames.add(&p_frame.fade, &p_frame.fadechange, 1, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, 0, 100);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, 0, 0);
		frames.GetFrameHight(&p_frame.root);
		//�p�����[�^�[�\���͈͐ݒ�
		p_length.rawwave.left = 0.0f;
		p_length.rawwave.right = 1.0f;
		p_length.rawwave.top = -1.0f;
		p_length.rawwave.bottom = 1.0f;
	}
};

/*
//�t���[������N���X
class Frames {
public:
//�֐��錾
void add(frame *parent, frame *self, bool mode, int size) { //�t���[���̒ǉ�
self->parent = parent; //�e�t���[���̃|�C���^
self->child = NULL; //��ԏ��߂̎q�t���[���̃|�C���^
self->next = NULL; //���t���[���̃|�C���^
self->mode = mode; //0:�q�t���[�����c���уt���[��,1:�q�t���[����������
self->pos.left = 0; //�t���[�������W
self->pos.top = 0; //�t���[������W
self->pos.right = 0; //�t���[���E���W
self->pos.bottom = 0; //�t���[�������W
self->y = size; //�t���[���̍���
if (parent == NULL) { //�e�t���[�����w��̏ꍇ��L�̒l�̂܂܂�OK
return;
}
if (parent->child == NULL) { //�e�t���[�����q�t���[���������Ă��Ȃ������ꍇ
parent->child = self; //�e�t���[���Ɉ�ԍŏ��̎q�t���[���o�^
}else{ //�e�t���[�����q�t���[���������Ă�����
//�e�t���[���̎q�t���[�����ԍŌ�Ɏ��t���[���o�^
frame *last = parent->child;
while (last->next != NULL) {
last = last->next;
}
last->next = self;
}
return;
}
int GetFrameHight(frame *f) { //���[�t���[���̃T�C�Y����S�t���[����y�T�C�Y�擾
bool mode = f->mode; //���t���[���̎q�t���[��������@�擾
int num = 0; //���t���[���̍�������p�ϐ�
if (f->child == NULL) { //�w�肳�ꂽ�t���[�����q�t���[���������Ȃ������ꍇ
return 0; //������0�ɂȂ�
}
frame *last = f->child; //�e�t���[���̑S�q�t���[����1�Â���p�ϐ�
while (true) {
if (last->child != NULL) { //���t���[���Ɏq�t���[�������݂����
last->y = GetFrameHight(last); //�q�t���[����y�T�C�Y�擾
}
num += last->y; //��L�Ŏ擾�����q�t���[���̍��������Z
if (!mode) { //���t���[���̎q�t���[�����c���тł����
if (last->next != NULL) { //���t���[�������݂����ꍇ
last = last->next;//���̎q�t���[���Ɉړ�
}else { //���t���[�����Ȃ���΃��[�v�I��
break;
}
}else { //�����тł���΃��[�v�I��
break;
}
}
return num; //�t���[���̍�����Ԃ�
}
bool MakeFramePos(frame *f) { //�t���[���ʒu�Z�o
if (f->child == NULL) { //���t���[�����q�t���[���������Ă��邩�m�F
return 1;
}
int x = f->x;
bool mode = f->mode; //���t���[���̎q�t���[��������@�擾
int num_x = f->pos.left; //���t���[���̍����W����q�t���[���z�u
int num_y = f->pos.top; //���t���[���̏���W����q�t���[���z�u
int num = 0; //�q�t���[���̐��J�E���g�p�ϐ�
frame *last; //�q�t���[����1�Â������ϐ�
last = f->child; //���t���[���̍ŏ��̎q�t���[�����
if (mode) { //���t���[���̎q�t���[���������тł���Ύq�t���[���̌��J�E���g
while (true) {
num += 1;
if (last->next == NULL) { //���t���[�����Ȃ���΃��[�v�I��
break;
}
last = last->next; //���̃t���[���Ɉړ�
}
x /= num; //�t���[���̉��������T�C�Y
}
last = f->child; //���t���[���̍ŏ��̎q�t���[�����
while (true) {
last->x = x; //�t���[�������擾
last->pos.left = num_x; //�q�t���[���̍����W�擾
last->pos.top = num_y; //�q�t���[���̏���W�擾
last->pos.right = last->pos.left + last->x; //�q�t���[���̉E���W�Z�o
last->pos.bottom = last->pos.top + last->y; //�q�t���[���̉����W�Z�o
if (last->child != NULL) { //�q�t���[���Ɏq�t���[��������΂���̈ʒu���o
MakeFramePos(last);
}
if (last->next != NULL) { //���t���[���������
if (!mode) { //�c���тł����
num_y = last->pos.bottom; //���t���[���̏���W�X�V
}
else { //�����тł����
num_x = last->pos.right; //���t���[���̍����W�X�V
}
last = last->next; //���̃t���[���Ɉړ�
}else { //���t���[�����Ȃ���΃��[�v�I��
break;
}
}
return 0;
}
void resize(frame *f, RECT size) {
f->pos.left = size.left;
f->pos.top = size.top;
f->x = size.right - size.left;
MakeFramePos(f);
}
};

//�p�����[�^�[�̒l��t���[���ϐ��Ǘ��N���X
class Parameteres {
public:
//�R���X�g���N�g
Frames frames; //�t���[������N���X

//�ϐ�������
VSTParameteres *p_value; //�p�����[�^�[�l�ϐ��Q�|�C���^
VSTParameteresFrames p_frame; //�p�����[�^�[�t���[���ϐ��Q
VSTParameteresLength p_length; //�p�����[�^�[�\���͈͕ϐ��Q

//�֐��錾
Parameteres() { //�S�p�����[�^�[���̃t���[���쐬
//�t���[������
frames.add(NULL, &p_frame.root, 0, 0); //���[�g�t���[������
frames.add(&p_frame.root, &p_frame.tone, 0, 0); //���F�ݒ�t���[������
frames.add(&p_frame.tone, &p_frame.make_auto, 0, 100); //�����ŉ��F�𐶐����邩
frames.add(&p_frame.tone, &p_frame.raw_wave_para, 0, 0); //���L�C���f���g�𑩂˂�
frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, 0, 100); //���g�`�f�[�^�̎g�p�����邩�ǂ���
frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, 0, 100); //���g�`�̎g�p�����g�`(�t�@�C���}�b�s���O�ɂ��邩��(�L�E�ցE�M))
frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, 0, 100); //��ʒu(�蓮�ύX��)
frames.add(&p_frame.tone, &p_frame.tone_para, 0, 0); //���L�C���f���g�̃t���[���𑩂˂�
frames.add(&p_frame.tone_para, &p_frame.change_natural, 0, 100); //�����ω������R�ɂ��邩
frames.add(&p_frame.tone_para, &p_frame.overtones, 0, 100); //�{���O���tor���X�y�N�g���O���t
frames.add(&p_frame.tone_para, &p_frame.iovertones, 0, 100); //�{���̐�
frames.add(&p_frame.tone_para, &p_frame.wave_type, 0, 100); //��{�g�`�̌`
frames.add(&p_frame.tone_para, &p_frame.hostpar, 1, 0); //���L�C���f���g�̃t���[���𑩂˂�
frames.add(&p_frame.hostpar, &p_frame.vol, 0, 100); //����
frames.add(&p_frame.hostpar, &p_frame.pitch, 0, 100); //����(IDI�l�ɉ��Z)
frames.add(&p_frame.tone, &p_frame.wave_limit, 0, 100); //�g�`�̐�Βl�̏��
frames.add(&p_frame.tone, &p_frame.outwave, 0, 100); //�o�͔g�`
frames.add(&p_frame.root, &p_frame.fade, 0, 0); //�t�F�[�h�ݒ�t���[������
frames.add(&p_frame.fade, &p_frame.use_string_mode, 0, 100); //�����[�h�̎g�p�����邩�ǂ���
frames.add(&p_frame.fade, &p_frame.use_fade_change, 0, 100); //�Ȃ߂炩�ȉ���,���ʂ̕ω����g�p���邩�ǂ���(2�����ɉ����o���Ȃ�)
frames.add(&p_frame.fade, &p_frame.fadein, 1, 0);
frames.add(&p_frame.fadein, &p_frame.fadein_vol, 0, 100);
frames.add(&p_frame.fadein, &p_frame.fadein_pitch, 0, 0);
frames.add(&p_frame.fade, &p_frame.fadeout, 1, 0);
frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, 0, 100);
frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, 0, 0);
frames.add(&p_frame.fade, &p_frame.fadechange, 1, 0);
frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, 0, 100);
frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, 0, 0);
frames.GetFrameHight(&p_frame.root);
//�p�����[�^�[�\���͈͐ݒ�
p_length.rawwave.left = 0.0f;
p_length.rawwave.right = 1.0f;
p_length.rawwave.top = -1.0f;
p_length.rawwave.bottom = 1.0f;
}
};
*/

//GUI�N���X
class GUI {
public:
	//�֐��錾
	float percent(float a, float a_min, float a_max, float b_min, float b_max) { //�P�ʕϊ��֐�
		float b;
		b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
		return b;
	}
	//�w�i������
	void reset() {
		ofBackground(30, 30, 30);
	}
	//���̃{�b�N�X�`��֐�
	void LIneBox(RECT pos) {
		ofLine(pos.left, pos.top, pos.right, pos.top);
		ofLine(pos.right, pos.top, pos.right, pos.bottom);
		ofLine(pos.right, pos.bottom, pos.left, pos.bottom);
		ofLine(pos.left, pos.bottom, pos.left, pos.top);
	}
	//�t���[���̋��E���`��
	void FrameLine(frame *root) {
		frame *last = root->child;
		while (true) {
			if (last->child != NULL) {
				FrameLine(last);
			}
			ofSetColor(0, 128, 198, 40);
			ofSetLineWidth(1);
			LIneBox(last->pos);
			if (last->next != NULL) {
				last = last->next;
			}
			else {
				break;
			}
		}
	}
	void rawwave(frame *f, RECTF len, float *samples, int num_sample, float *allrawwave, int num_allrawwave, POINT rlen) { //���g�`�f�[�^�̎g�p�����I��

	}
	void loop() { //���t���[���Ăяo���֐�

	}
	/*
	�ǉ��\��̊֐�
	�X�C�b�`�֐�
	�X���C�h�ϒ�R�֐�
	�e�O���t�p�����[�^�[�֐�
	*/
	//�R���X�g���N�g

};

//�`��N���X
class Draw {
public:
	//�ϐ��錾
	float fps;
	RECT win_size;

	//�R���X�g���N�g
	Parameteres para;
	GUI gui;

	//�֐��錾
	bool resize(WINDOW_INFO win_info) { //�`��̈�ύX
		win_size.right = win_info.size.x;
		win_size.bottom = win_info.size.y;
		win_size.left = 0;
		win_size.top = 0;
		fps = win_info.fps;
		para.frames.resize(&para.p_frame.root, win_size);
		return 0;
	}
	void loop() { //���[�v���ɌĂяo���֐�
				  //��ʏ�����
		gui.reset();
		//GUI�N���X�̖��t���[���Ăяo���֐�
		gui.loop();
		//�t���[���̋��E���`��
		gui.FrameLine(&para.p_frame.root);
		//�e�p�����[�^�[�`��

	}
};

//�S�Ă̏������s���N���X
class AllProcess {
public:
	//�ϐ�������
	WINDOW_INFO win_info;

	//�R���X�g���N�g
	ShareMem sm; //���L�������N���X
	Draw draw; //�`��N���X

			   //�֐��錾
	bool init(LPSTR cmd, WINDOW_INFO win_info2) { //�����������s�֐�
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
		draw.para.p_value = &sm.smd->para;
		return 0;
	}
	bool loop() { //���[�v���ɌĂяo���֐�
				  //win_info�̃E�B���h�E�T�C�Y�X�V
		GetWindowRect(win_info.hwnd, &win_info.window);
		win_info.size.x = win_info.window.right - win_info.window.left;
		win_info.size.y = win_info.window.bottom - win_info.window.top;
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
		return 0;
	}
	//�I��������
	~AllProcess() {
		if (sm.smd != nullptr) {
			sm.smd->Ready = nullptr;
		}
	}
};

class ofApp : public ofBaseApp {

public:
	//�R���X�g���N�^
	ofApp(int argc, LPSTR arg[]); //�R���\�[�������擾

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	WINDOW_INFO win_info; //�E�B���h�E���ێ��ϐ�������

						  //�R���X�g���N�g
	AllProcess app;

	//���s������������
	LPSTR lpCmdLine;
};
