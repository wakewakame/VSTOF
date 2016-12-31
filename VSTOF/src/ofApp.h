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
	bool lock; //�e�q�t���[���̒���(mode=0�Ȃ�c��,mode=1�Ȃ牡��)�̌Œ�on/off
	int lock_length; //�Œ�T�C�Y�̑S�q�t���[���ƑSgap�̘a(���[�t���[����0����)
};
//�p�����[�^�[�l�\����
struct VSTParameteresFrames {
	frame root;
	frame all;
	frame scroll;
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

//���[�e�B���e�B�N���X
class Utility {
public:
	//�����ϊ��֐�(�߂�l:�ϊ���,����1~5:�ϊ��O�̒l,�ϊ��O�̍ŏ��l,�ϊ��O�̍ő�l,�ϊ���̍ŏ��l,�ϊ���̍ő�l)
	static double Proportion(double i,double i_mix,double i_max,double j_min,double j_max) {
		double j;
		j = j_min + (i - i_mix) * ((j_max - j_min) / (i_max - i_mix));
		return j;
	}
	static int Proportion(int i, int i_mix, int i_max, int j_min, int j_max) {
		int j;
		j = int(Proportion(double(i), double(i_mix), double(i_max), double(j_min), double(j_max)));
		return j;
	}
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
	//�t���[���ǉ��֐�(����:�e�t���[��(�Ȃ����nullptr),���t���[��,�t���[����,�q�t���[���̔z����@(0�Ȃ�c,1�Ȃ牡),�q�t���[���Ԃ̊Ԋu,���t���[���̒���(mode=0�Ȃ獂��,1�Ȃ畝),���t���[�����Œ�T�C�Y�ɂ���on/off)
	void add(frame *parent, frame *self, std::string name, int length, bool lock) {
		self->parent = parent; //�e�t���[���̃|�C���^
		self->num_child = 0; //�q�t���[���̐�������
		self->pos.left = 0; //�t���[�������W
		self->pos.top = 0; //�t���[������W
		self->pos.right = 0; //�t���[���E���W
		self->pos.bottom = 0; //�t���[�������W
		self->size = {0,0}; //�t���[���T�C�Y
		self->name = name; //�t���[���̖���
		self->mode = 0; //�q�t���[�����c����=0,������=1
		self->gap = 0; //�q�t���[���ԓ��m�̌���(px�P��)
		self->length = length; //�S�t���[���������l�T�C�Y���̎��t���[���̃T�C�Y
		self->lock = lock; //���݂̎��t���[���̒���(mode = 0�Ȃ�c��, mode = 1�Ȃ牡��)�̌Œ��on/off
		self->index = 0;
		self->lock_length = 0; //�Œ�T�C�Y�̑S�q�t���[���ƑSgap�̘a(���[�t���[����0����)
		//�e�t���[�������w��̏ꍇ�A�ȉ��̏������Ȃ�
		if (parent == nullptr) {
			return;
		}
		self->parent->childs.push_back(self); //�e�t���[���Ɏ��t���[���ǉ��o�^
		self->index = self->parent->num_child;//���t���[�����̎��t���[���̊����ԍ�(=0,1,2,3,...)
		self->parent->num_child += 1; //�e�t���[�����X�V
		return;
	}
	void set_parent(frame *self, bool mode, int gap) {
		self->mode = mode; //�q�t���[���������тȂ�0,�c���тȂ�1
		self->gap = gap; //���t���[���Ԃ̌��ԃT�C�Y(px�P��)
		return;
	}
	//�S�t���[���̓o�^�������ɁA���[�t���[������S�e�t���[����length���擾�֐�
	void get_length(frame *f) {
		if (f->num_child != 0) {
			f->length = 0; //�S�q�t���[���ƑSgap�̘a
			f->lock_length = 0; //�Œ�T�C�Y�̑S�q�t���[���ƑSgap�̘a(���[�t���[����0����)
			//�q�t���[���̌������[�v
			for (int i = 0; i < f->num_child; i++) {
				//�q�t���[�����q�t���[���������Ă����ꍇ�A�ċA
				if (f->childs[i]->num_child != 0) {
					get_length(f->childs[i]);
				}
				//�e�t���[���̒����Ɏq�t���[���̒����𑫂��Ă���
				f->length += f->childs[i]->length;
				//�����q�t���[�������[�t���[���ŁA�Œ�T�C�Y�t���[���̏ꍇ
				if (f->childs[i]->lock) {
					f->lock_length += f->childs[i]->length;
				}
			}
			//�Sgap�𑫂�
			f->length += f->gap * (f->num_child + 1);
			f->lock_length += f->gap * (f->num_child + 1);
		}
		return;
	}
	//�S�t���[���̈ʒu�Z�o(����1,2,3:���t���[���̃|�C���^,���t���[����`�悷��ʒu,0�Ȃ璷���͏����l�̂܂�)
	void resize(frame *f, RECT pos) {
		f->size.x = pos.right - pos.left;
		f->size.y = pos.bottom - pos.top;
		f->pos = pos;
		//�q�t���[��������΁A�q�t���[���̔z�u�����Ă���
		if (f->num_child != 0) {
			if (f->mode) {
				//�����т̏ꍇ
				//�q�t���[�������[�v
				for (int i = 0; i < f->num_child; i++) {
					//i�ڂ̎q�t���[���ʒu�ϐ�������
					RECT child_pos;
					//i�ڂ̎q�t���[���ʒu�Z�o
					//�㉺�ʒu
					child_pos.top = f->pos.top + f->gap;
					child_pos.bottom = f->pos.bottom - f->gap;
					//���ʒu
					if (i == 0) {
						child_pos.left = f->pos.left + f->gap;
					}else{
						child_pos.left = f->childs[i-1]->pos.right + f->gap;
					}
					//�E�ʒu
					if (f->childs[i]->lock) {
						//�Œ�T�C�Y�t���[���̏ꍇ�͊����ϊ��s�v
						child_pos.right =
							child_pos.left +
							f->childs[i]->length;
					}else{
						//��Œ�T�C�Y�t���[���̏ꍇ�͊����v�Z
						child_pos.right =
							child_pos.left +
							Utility::Proportion(
								f->childs[i]->length,
								0,
								f->length - f->lock_length,
								0,
								f->size.x - f->lock_length
							);
					}
					//�q�t���[���ʒu�ݒ�
					resize(f->childs[i], child_pos);
				}
			}else{
				//�c���т̏ꍇ
				//�q�t���[�������[�v
				for (int i = 0; i < f->num_child; i++) {
					//i�ڂ̎q�t���[���ʒu�ϐ�������
					RECT child_pos;
					//i�ڂ̎q�t���[���ʒu�Z�o
					//���E�ʒu
					child_pos.left = f->pos.left + f->gap;
					child_pos.right = f->pos.right - f->gap;
					//��ʒu
					if (i == 0) {
						child_pos.top = f->pos.top + f->gap;
					}else{
						child_pos.top = f->childs[i-1]->pos.bottom + f->gap;
					}
					//���ʒu
					if (f->childs[i]->lock) {
						//�Œ�T�C�Y�t���[��or�X�N���[���t���[���̏ꍇ�͊����ϊ��s�v
						child_pos.bottom =
							child_pos.top +
							f->childs[i]->length;
					}else{
						//��Œ�T�C�Y�t���[���̏ꍇ�͊����v�Z
						child_pos.bottom =
							child_pos.top +
							Utility::Proportion(
								f->childs[i]->length,
								0,
								f->length - f->lock_length,
								0,
								f->size.y - f->lock_length
							);
					}
					//�q�t���[���ʒu�ݒ�
					resize(f->childs[i], child_pos);
				}
			}
		}
		return;
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
		//frames.add(frame *parent, frame *self, std::string name, int length, bool lock)
		frames.add(nullptr, &p_frame.root, "root", 0, 0);
		frames.add(&p_frame.root, &p_frame.all, "all", 0, 0);
		frames.add(&p_frame.root, &p_frame.scroll, "scroll", 16, 1);
		frames.add(&p_frame.all, &p_frame.tone, "tone", 0, 0); //���F�ݒ�t���[������
		frames.add(&p_frame.tone, &p_frame.make_auto, "make_auto", 100, 0); //�����ŉ��F�𐶐����邩
		frames.add(&p_frame.tone, &p_frame.raw_wave_para, "raw_wave_para", 0, 0); //���L�C���f���g�𑩂˂�
		frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, "use_rawwave", 100, 0); //���g�`�f�[�^�̎g�p�����邩�ǂ���
		frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, "rawwave", 100, 0); //���g�`�̎g�p�����g�`(�t�@�C���}�b�s���O�ɂ��邩��(�L�E�ցE�M))
		frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, "base_pitch", 100, 0); //��ʒu(�蓮�ύX��)
		frames.add(&p_frame.tone, &p_frame.tone_para, "tone_para", 0, 0); //���L�C���f���g�̃t���[���𑩂˂�
		frames.add(&p_frame.tone_para, &p_frame.change_natural, "change_natural", 100, 0); //�����ω������R�ɂ��邩
		frames.add(&p_frame.tone_para, &p_frame.overtones, "overtones", 100, 0); //�{���O���tor���X�y�N�g���O���t
		frames.add(&p_frame.tone_para, &p_frame.iovertones, "iovertones", 100, 0); //�{���̐�
		frames.add(&p_frame.tone_para, &p_frame.wave_type, "wave_type", 100, 0); //��{�g�`�̌`
		frames.add(&p_frame.tone_para, &p_frame.hostpar, "hostpar", 0, 0); //���L�C���f���g�̃t���[���𑩂˂�
		frames.add(&p_frame.hostpar, &p_frame.vol, "vol", 100, 0); //����
		frames.add(&p_frame.hostpar, &p_frame.pitch, "pitch", 100, 0); //����(IDI�l�ɉ��Z)
		frames.add(&p_frame.tone, &p_frame.wave_limit, "wave_limit", 100, 0); //�g�`�̐�Βl�̏��
		frames.add(&p_frame.tone, &p_frame.outwave, "outwave", 100, 0); //�o�͔g�`
		frames.add(&p_frame.all, &p_frame.fade, "fade", 0, 0); //�t�F�[�h�ݒ�t���[������
		frames.add(&p_frame.fade, &p_frame.use_string_mode, "use_string_mode", 100, 0); //�����[�h�̎g�p�����邩�ǂ���
		frames.add(&p_frame.fade, &p_frame.use_fade_change, "use_fade_change", 100, 0); //�Ȃ߂炩�ȉ���,���ʂ̕ω����g�p���邩�ǂ���(2�����ɉ����o���Ȃ�)
		frames.add(&p_frame.fade, &p_frame.fadein, "fadein", 0, 0);
		frames.add(&p_frame.fadein, &p_frame.fadein_vol, "fadein_vol", 100, 0);
		frames.add(&p_frame.fadein, &p_frame.fadein_pitch, "fadein_pitch", 100, 0);
		frames.add(&p_frame.fade, &p_frame.fadeout, "fadeout", 0, 0);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, "fadeout_vol", 100, 0);
		frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, "fadeout_pitch" ,100, 0);
		frames.add(&p_frame.fade, &p_frame.fadechange, "fadechange", 0, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, "fadechange_vol", 100, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, "fadechange_pitch", 100, 0);

		//set_parent(frame *self, bool mode, int gap)
		frames.set_parent(&p_frame.root, 1, 0);
		frames.set_parent(&p_frame.hostpar, 1, 0);
		frames.set_parent(&p_frame.fadein, 1, 0);
		frames.set_parent(&p_frame.fadeout, 1, 0);
		frames.set_parent(&p_frame.fadechange, 1, 0);
		frames.set_parent(&p_frame.fadechange, 1, 0);
		
		frames.get_length(&p_frame.root); //�S�t���[����length���擾
	}
};

//�E�B���h�E�C�x���g�����N���X
class WIN_EVENT {
public:
	//�ϐ��錾
	HWND hwnd; //���ݎw�肳��Ă���E�B���h�E�n���h��
	HWND m_hwnd; //�}�E�X���̍őO�ʃE�B���h�E�n���h��
	POINT mouse_pos; //�}�E�X���W
	MSG msg; //�E�B���h�E���b�Z�[�W����ϐ�

	//�֐��錾
	//�E�B���h�E�n���h���ݒ�֐�
	void SetHwnd(HWND h) {
		hwnd = h;
		return;
	}
	//�}�E�X�����E�B���h�E�𑀍삵�Ă��邩�ǂ����̔���
	bool GetMouseOnWindow() {
		GetMousePos();
		return 0;
	}
	//�}�E�X���w��͈͓��𑀍삵�Ă��邩�ǂ����̔���
	bool GetMouseOnRect() {
		GetMousePos();
		return 0;
	}
	//�}�E�X�z�C�[���ω��ʎ擾�֐�
	int GetScroll() {

		return 0;
	}
	//�}�E�X���W�擾�֐�
	void GetMousePos() {
		GetMessage(&msg, NULL, 0, 0);
		//mouse_pos = ; //�}�E�X���W���
		return;
	}
};

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
		ofSetLineWidth(1.0f);
		ofSetColor(255, 255, 255, 240);
		//���t���[���`��
		LIneBox(root->pos);
		//���t���[���Ɏq�t���[��������ΑS���`��
		if (root->num_child != 0) {
			for (int i = 0; i < root->num_child; i++) {
				FrameLine(root->childs[i]);
			}
		}
	}
	//�t���[���̖��̕`��
	void FrameName(frame *root) {
		ofSetColor(255, 255, 255, 255);
		//���t���[�����q�t���[���������Ă��ꍇ
		if (root->num_child != 0) {
			//���t���[���Ɏq�t���[��������ΑS���`��
			if (root->num_child != 0) {
				for (int i = 0; i < root->num_child; i++) {
					FrameName(root->childs[i]);
				}
			}
		}else{
			//�q�t���[���������Ă��Ȃ��ꍇ
			ofDrawBitmapString(root->name, root->pos.left + 4, root->pos.top + 12);
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
		//win_info�̃E�B���h�E�T�C�Y�X�V
		GetClientRect(win_info.hwnd, &win_info.window);
		win_info.size.x = win_info.window.right - win_info.window.left;
		win_info.size.y = win_info.window.bottom - win_info.window.top;
		para.p_frame.root.pos.left = 0;
		para.p_frame.root.pos.top = 0;
		para.p_frame.root.pos.right = win_info.size.x;
		para.p_frame.root.pos.bottom = win_info.size.y;
		fps = win_info.fps;
		para.frames.resize(&para.p_frame.root, para.p_frame.root.pos); //�S�t���[���̎����z�u
		return 0;
	}
	void loop() { //���[�v���ɌĂяo���֐�
		//��ʏ�����
		gui.reset();
		//GUI�N���X�̖��t���[���Ăяo���֐�
		gui.loop();
		//�t���[���̋��E���`��
		gui.FrameLine(&para.p_frame.root);
		//�t���[���̖��̕`��
		gui.FrameName(&para.p_frame.root);
		//�e�p�����[�^�[�`��

	}
};

//�S�Ă̏������s���N���X
class AllProcess {
public:
	//�ϐ�������
	WINDOW_INFO win_info; //�E�B���h�E���ێ��ϐ�������

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
		}else {
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
