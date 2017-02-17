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
//�A�j���[�V�����ϐ��Ǘ��N���X
class Animation {
private:
	//�ϐ��錾
	double fps; //�t���[�����[�g
	std::vector<bool> motion; //�A�j���[�V�������J�n����Ă����1,����ӊO��0
	std::vector<double> len; //�A�j���[�V�������鎞��(�P�ʂ�sec)
	std::vector<double> t; //fps�Ɉˑ����Ȃ��o�ߎ���(0.0<=t<=len)
	std::vector<char> type; //�A�j���[�V�����̕ω��̎��(0<=val)
	std::vector<bool*> swp; //sw=0�Ȃ�t��0�ɋ߂Â��A�t�Ȃ�t
	std::vector<bool> bsw; //�O�t���[���̂Ƃ���sw1

public:
	//�ϐ��錾
	std::vector<double> m; //���[�V�������W(��{�I�� 0.0<=m<=1.0)
	std::vector<double> p; //(p/1.0)=(t/len)

						   //�֐��錾
						   //fps�w��
	void set_fps(double frame_rate) {
		fps = frame_rate;
	}
	//�A�j���[�V�����z��̒���
	int get_len() {
		return t.size();
	}
	//�A�j���[�V�����ϐ��̒ǉ�
	void add(double length, char effect_type, bool *sw) {
		motion.push_back(0);
		len.push_back(length);
		t.push_back(0.0);
		m.push_back(0.0);
		p.push_back(0.0);
		type.push_back(effect_type);
		swp.push_back(sw);
		bsw.push_back(*sw);
	}
	//���t���[���Ăяo���֐�
	void loop() {
		for (int i = 0; i < t.size(); i++) {
			//�O�t���[���Ƃ̒l���Ⴄ�ꍇ�A�A�j���[�V�����J�n
			if (*swp[i] != bsw[i]) {
				motion[i] = 1;
				bsw[i] = *swp[i];
			}
			//�A�j���[�V��������
			if ((motion[i]) || (type[i] == 4)) {
				//�o�ߎ��Ԍv�Z
				if (*swp[i]) {
					t[i] += (1.0 / fps);
					if (t[i] >= len[i]) {
						t[i] = len[i];
						motion[i] = 0;
					}
				}
				else {
					t[i] -= (1.0 / fps);
					if (t[i] <= 0.0) {
						t[i] = 0.0;
						motion[i] = 0;
					}
				}
				//���[�V�����v�Z
				p[i] = t[i] / len[i];
				switch (type[i]) {
				case 0:
					//y=x
					m[i] = p[i];
					break;
				case 1:
					//y=x^2
					m[i] = pow(p[i], 2);
					break;
				case 2:
					//y=-(x-1)^2+1
					m[i] = 1.0 - pow(p[i] - 1.0, 2);
					break;
				case 3:
					//y=3*(x^2)-2*(x^3)
					//x=0��y���ɏ��l0�ƂȂ�Ax=1��y���ɑ�l1�ƂȂ�O���֐�
					m[i] = 3 * pow(p[i], 2) - 2 * pow(p[i], 3);
					break;
				case 4:
					//�w�莞�Ԃ�0.0-1.0��ԃ��[�v
					if (motion[i] == 0) {
						motion[i] = 1;
						t[i] = 0.0;
					}
					m[i] = p[i];
					break;
				}
			}
		}
	}
};
//�t���[���o�b�t�@�N���X
class FBO {
public:
	/*
	///����///
	�A���t�@�t���[���o�b�t�@�͕K�v�ȂƂ�������������
	*/
	//�t���[���o�b�t�@�N���X�̔z��
	std::vector<ofFbo> color;
	std::vector<ofFbo> alpha;
	//���݂̃t���[���o�b�t�@�̌�
	int num;
	//���ݎg�p���Ă���t���[���o�b�t�@�̃C���f�b�N�X(-1�Ȃ烌���_�[�o�b�t�@)
	int now_index;
	//���t���[���o�b�t�@�N���X�̃R���X�g���N�g
	ofFbo fbo_;
	//�R���X�g���N�^
	FBO() {
		now_index = -1;
		num = 0;
	}
	//�f�X�g���N�^
	~FBO() {
		if (now_index != -1) {
			color[now_index].end();
			alpha[now_index].end();
		}
	}
	//�t���[���o�b�t�@�̒ǉ�
	int add(int x, int y) {
		//�J���[�t���[���o�b�t�@
		color.push_back(fbo_); //�z��ɃJ���[�t���[���o�b�t�@�N���X�ǉ�
		color[num].allocate(x, y, GL_RGBA); //�J���[�t���[���o�b�t�@����
		color[num].begin();
		ofClear(255, 255, 255, 0); //�J���[�t���[���o�b�t�@������
		color[num].end();
		//�A���t�@�t���[���o�b�t�@
		alpha.push_back(fbo_); //�z��ɃA���t�@�t���[���o�b�t�@�N���X�ǉ�
		alpha[num].allocate(x, y, GL_ALPHA); //�A���t�@�t���[���o�b�t�@����
		alpha[num].begin();
		ofClear(0, 0, 0, 255); //�A���t�@�t���[���o�b�t�@������
		alpha[num].end();
		num += 1;
		return num - 1;
	}
	//�J���[�t���[���o�b�t�@�̐؂�ւ�(index=-1�Ȃ烌���_�[�o�b�t�@)
	void change_c(int index) {
		if (now_index != -1) {
			color[now_index].end();
		}
		if (index != -1) {
			color[index].begin();
		}
		now_index = index;
	}
	//�A���t�@�t���[���o�b�t�@�̐؂�ւ�(index=-1�Ȃ烌���_�[�o�b�t�@)
	void change_a(int index) {
		if (now_index != -1) {
			alpha[now_index].end();
			//�J���[�t���[���o�b�t�@�Ƀ}�X�N�K��
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
	//�w��J���[�o�b�t�@�`��
	void draw_c(int x, int y, int index) {
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);
		color[index].draw(x, y);
	}
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
	std::vector<void*> data; //�ėp�z��(�N���X�̃|�C���^�Ȃ�)
	/*
	Animation animation; //�A�j���[�V�����ϐ��z��
	FBO fbo; //�o�b�t�@�t���[���z��
	*/
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
	static double Proportion(double i, double i_mix, double i_max, double j_min, double j_max) {
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
		self->size = { 0,0 }; //�t���[���T�C�Y
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
					}
					else {
						child_pos.left = f->childs[i - 1]->pos.right + f->gap;
					}
					//�E�ʒu
					if (f->childs[i]->lock) {
						//�Œ�T�C�Y�t���[���̏ꍇ�͊����ϊ��s�v
						child_pos.right =
							child_pos.left +
							f->childs[i]->length;
					}
					else {
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
			}
			else {
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
					}
					else {
						child_pos.top = f->childs[i - 1]->pos.bottom + f->gap;
					}
					//���ʒu
					if (f->childs[i]->lock) {
						//�Œ�T�C�Y�t���[��or�X�N���[���t���[���̏ꍇ�͊����ϊ��s�v
						child_pos.bottom =
							child_pos.top +
							f->childs[i]->length;
					}
					else {
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
	//�w��t���[���ȉ��̂��ׂẴt���[����dispose���s
	void all_dispose(frame *f) {
		dispose(f);
		for (int i = 0; i < f->childs.size(); i++) {
			all_dispose(f->childs[i]);
		}
	}
	//�t���[������new�����N���X�����ׂ�delete����֐�
	void dispose(frame *f) {
		for (int i = 0; i < f->data.size(); i++) {
			delete f->data[i];
		}
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
		frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, "fadeout_pitch", 100, 0);
		frames.add(&p_frame.fade, &p_frame.fadechange, "fadechange", 0, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, "fadechange_vol", 100, 0);
		frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, "fadechange_pitch", 100, 0);

		//set_parent(frame *self, bool mode, int gap)
		frames.set_parent(&p_frame.root, 1, 2);
		frames.set_parent(&p_frame.hostpar, 1, 2);
		frames.set_parent(&p_frame.fadein, 1, 2);
		frames.set_parent(&p_frame.fadeout, 1, 2);
		frames.set_parent(&p_frame.fadechange, 1, 2);
		frames.set_parent(&p_frame.fadechange, 1, 2);

		frames.get_length(&p_frame.root); //�S�t���[����length���擾
	}
	//�f�X�g���N�^
	~Parameteres() {
		frames.all_dispose(&p_frame.root);
	}
};

//�E�B���h�E�C�x���g�����N���X
class WIN_EVENT {
public:
	//�ϐ��錾
	POINT mouse; //���݂̃}�E�X���W
	bool l_click; //���݂̍��N���b�N���(������Ă�����1)
	bool b_l_click; //�O�t���[����l_click�ϐ��̓��e

					//�֐��錾
					//�w��RECT���ɑ��݂��邩�ǂ���
	bool in(RECT area) {
		if (
			(area.left <= mouse.x) &&
			(area.top <= mouse.y) &&
			(area.right >= mouse.x) &&
			(area.bottom >= mouse.y)
			) {
			return 1;
		}
		return 0;
	}
	//�}�E�X�����N���b�N���ꂽ���ǂ���
	bool get_l_click() {
		return (l_click == 1 && b_l_click == 0);
	}
	//�w��RECT���Ń}�E�X���N���b�N���ꂽ���ǂ���
	bool l_click_in(RECT area) {
		return (in(area) && get_l_click());
	}
	//���t���[���Ăяo���֐�
	void loop() {
		b_l_click = l_click;
	}
};

//GUI�N���X
class GUI {
public:
	//�ϐ��錾
	double move; //�ėp�A�j���[�V�����ϐ�
	double fps; //�t���[�����[�g

	//�R���X�g���N�g
	WIN_EVENT win_event;
	Frames frames;

	//�֐��錾
	//�P�ʕϊ��֐�
	float percent(float a, float a_min, float a_max, float b_min, float b_max) {
		float b;
		b = b_min + (b_max - b_min)*((a - a_min) / (a_max - a_min));
		return b;
	}
	int percent(int a, int a_min, int a_max, int b_min, int b_max) {
		return (int)percent((float)a, (float)a_min, (float)a_max, (float)b_min, (float)b_max);
	}
	//�w�i������
	void reset() {
		ofBackground(30, 30, 30);
	}
	//�t���[�����[�g�w��
	void set_fps(double frame_rate) {
		fps = frame_rate;
	}
	//���̃{�b�N�X�`��֐�
	void LineBox(RECT pos, int thick) {
		ofRect(pos.left, pos.top, pos.right - pos.left, thick); //��
		ofRect(pos.right - thick, pos.top + thick, thick, (pos.bottom - pos.top) - 2 * thick); //�E
		ofRect(pos.left, pos.bottom - thick, pos.right - pos.left, thick); //��
		ofRect(pos.left, pos.top + thick, thick, (pos.bottom - pos.top) - 2 * thick); //��
	}
	//�t���[���̋��E���`��
	void FrameLine(frame *root) {
		ofSetColor(255, 255, 255, 255);
		//���t���[���`��
		LineBox(root->pos, 1);
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
		}
		else {
			//�q�t���[���������Ă��Ȃ��ꍇ
			ofDrawBitmapString(root->name, root->pos.left + 4, root->pos.top + 12);
		}
	}
	//�O���t���UI(�t���[��,�g�`�z��,�T���v����,�`�惂�[�h)
	void rawwave(frame *f, float *samples, int num_sample, char mode) {
		//�`��������̂��߁A�s�N�Z���̐��ɍ��킹�ĕ`��
		int index = 0; //�z�񂩂�l���Q�Ƃ���Ƃ��̃C���f�b�N�X
		float percentage = 0.0f;
		float height; //���`�悷��g�̍���
		float b_height; //�O�t���[����height
		for (int i = 0; i < f->size.x; i++) {
			percentage = percent((float)i, 0.0f, (float)f->size.x, 0.0f, (float)num_sample - 1); //�ꎞ���
			index = (int)percentage; //�g�`�z��̃C���f�b�N�X�Z�o
			percentage = percentage - (float)index; //int,float�̐�����K�����̐��ɂȂ�
													//(�O�̈�)index+1>=num_sample�ɂȂ����烋�[�v�E�o
			if (index + 1 >= num_sample) {
				break;
			}
			//���`�悷��g�̍���
			height =
				percent(samples[index], 0.0f, 1.0f, 0.0f, (float)f->size.y * 0.7f) * (1.0f - percentage) +
				percent(samples[index + 1], 0.0f, 1.0f, 0.0f, (float)f->size.y * 0.7f) * (percentage);
			//f->size.x<num_sample�̎��͓h��Ԃ���������
			if (i % 2 == 0) {
				ofSetColor(255, 255, 255, 255);
				switch (mode) {
				case 0: //�h��Ԃ��Ȃ�
					break;
				case 1: //y=0.0�𒆐S�ɓh��Ԃ�
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f - height,
						1,
						height
					);
					break;
				case 2: //�g�`���牺��h��Ԃ�
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f - height,
						1,
						f->size.y / 2.0f + height
					);
					break;
				case 3: //������C���`��
					ofSetColor(255, 0, 0, 255);
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f - f->size.y * 0.7f / 2.0f,
						1,
						1
					);
					ofRect(
						f->pos.left + i,
						f->pos.bottom - f->size.y / 2.0f + f->size.y * 0.7f / 2.0f,
						1,
						1
					);
					break;
				}
			}
			if (i > 0) {
				ofSetColor(255, 255, 255, 255);
				ofLine(
					f->pos.left + i - 1,
					f->pos.bottom - f->size.y / 2.0f - b_height,
					f->pos.left + i,
					f->pos.bottom - f->size.y / 2.0f - height
				);
			}
			b_height = height; //�O�t���[���̂Ƃ���height�擾�p
		}
	}
	//�g��k���\�ȃO���t�`��֐�
	void wave_gui(frame *f, float *samples, int num_sample, char mode) {
		if (f->data.size() == 0) {
			std::shared_ptr<frame> child(new frame);
			f->data.push_back(&child);
			frames.add(f, child.get(), "rawwave", 0, 0);
			f->childs[0]->pos.left = f->pos.left;
			f->childs[0]->pos.top = f->pos.top;
			f->childs[0]->pos.right = f->pos.right;
			f->childs[0]->pos.bottom = f->pos.bottom;
		}
		ofSetColor(255, 255, 255, 255);
		ofDrawBitmapString(((std::shared_ptr<frame>)(f->childs[0])).use_count(), f->pos.left + 30, f->pos.top + 30);
		rawwave(f->childs[0], samples, num_sample, mode);
	}
	//�p�����[�^�J�[�\���`��֐�
	void cursor(POINT pos,int size) {
		ofSetColor(255, 255, 255, 128);
		ofRect(pos.x-size/2,pos.y-size/2,size,size);
	}
	//�X�C�b�`UI
	void sw(frame *f, bool *sw) {
		//�A�j���[�V�����ϐ��m�F
		bool sw2[3];
		bool loop = 1;
		Animation *animation;
		FBO *fbo;
		if (f->data.size() == 0) {
			//�t���[���ɃN���X�ǉ�
			f->data.push_back(new Animation());
			f->data.push_back(new FBO());
			//�|�C���^���
			animation = (Animation*)f->data[0];
			fbo = (FBO*)f->data[1];
			//�N���X������
			animation->add(0.15, 3, &sw2[0]); //�}�E�X�����������Ƃ�1
			animation->add(0.15, 3, &sw2[1]); //�}�E�X�����������Ƃ�2
			animation->add(0.15, 3, sw); //�N���b�N���ꂽ�Ƃ�
			animation->add(1.0, 4, &loop);
			animation->set_fps(fps); //fps�w��
			fbo->add(60, 60);
		}else{
			animation = (Animation*)f->data[0];
			fbo = (FBO*)f->data[1];
		}
		//�t���[���o�b�t�@�ɕ`��
		fbo->change_c(0);
		ofSetColor(255, 255, 255, 255);
		ofRect(0, 0, 60, 60);
		fbo->change_c(-1);
		fbo->change_a(0);
		ofClear(0, 0, 0, 255);
		for (int i = 0; i < 60; i++) {
			for (int j = 0; j < 60; j++) {
				ofSetColor(0, 0, 0, 255 - (((i + j + (int)(40.0*animation->m[3])) / 20) % 2) * 128);
				ofRect(j, i, j + 1, i + 1);
			}
		}
		ofSetColor(0, 0, 0, 0);
		ofRect(20, 20, 20, 20);
		fbo->change_a(-1);
		//�X�C�b�`�C�x���g�m�F
		//�N���b�N���ꂽ�Ƃ�
		if (win_event.l_click_in({
			f->pos.left + 10,
			f->pos.top + 20,
			f->pos.left + 70,
			f->pos.top + 80
		})) {
			*sw = !(*sw);
		}
		//�}�E�X�����������Ƃ�
		sw2[0] = win_event.in({
			f->pos.left + 10,
			f->pos.top + 20,
			f->pos.left + 70,
			f->pos.top + 80
		});
		sw2[1] = (animation->p[0] >= 0.5);
		//�A�j���[�V�����m�F
		animation->loop();
		//�`��
		fbo->draw_c(f->pos.left + 10, f->pos.top + 20, 0);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(0, 128, 198, 255);
		move = 15.0*animation->m[0];
		ofRect(
			f->pos.left + 40 - (int)move,
			f->pos.top + 50 - (int)move,
			(int)move * 2,
			(int)move * 2
		);
		ofSetColor(255, 255, 255, 255);
		move = 15.0*animation->m[1];
		ofRect(
			f->pos.left + 40 - (int)move,
			f->pos.top + 50 - (int)move,
			(int)move * 2,
			(int)move * 2
		);
		ofSetColor(0, 128, 198, (int)(255.0*animation->p[1]));
		ofDrawBitmapString("click", f->pos.left + 21, f->pos.top + 53);
		ofSetColor(0, 128, 198, 255);
		move = 15.0*animation->m[2];
		ofRect(
			f->pos.left + 40 - (int)move,
			f->pos.top + 50 - (int)move,
			(int)move * 2,
			(int)move * 2
		);
	}
	//���t���[���Ăяo���֐�
	void loop() {
		win_event.loop();
	}
	/*
	�ǉ��\��̊֐�
	�X�C�b�`�֐�
	�X���C�h�ϒ�R�֐�
	�e�O���t�p�����[�^�[�֐�
	ttf,otf�̎��̂ŕ�����`��֐�
	*/
	//�R���X�g���N�g

};

//�`��N���X
class Draw {
public:
	//�ϐ��錾
	float fps;
	RECT win_size;
	bool a; //�f�o�b�O�p

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
		para.p_frame.all.pos.bottom = para.p_frame.all.length;
		para.frames.resize(&para.p_frame.all, para.p_frame.all.pos);
		gui.set_fps(fps);
		return 0;
	}
	void loop() { //���[�v���ɌĂяo���֐�
				  //��ʏ�����
		gui.reset();
		//�t���[���̋��E���`��
		gui.FrameLine(&para.p_frame.root);
		//�t���[���̖��̕`��
		gui.FrameName(&para.p_frame.root);
		//�e�p�����[�^�[�`��
		{
			gui.wave_gui(&para.p_frame.make_auto, para.p_value->outwave, para.p_value->noutwave, 0);
			gui.sw(&para.p_frame.raw_wave_para, &a);
		}
		//���t���[���Ăяo���֐�
		gui.loop();
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
	std::string title; //�E�B���h�E�^�C�g��������

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
		///debug///
		title = "";
		title += "fps:";
		title += std::to_string(ofGetFrameRate());
		SetWindowText(win_info.hwnd, (LPSTR)title.c_str());
		///debug///
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

	//FBO�R���X�g���N�g
	ofFbo fbo;

	//�E�B���h�E���ێ��ϐ�������
	WINDOW_INFO win_info;

	//�R���X�g���N�g
	AllProcess app;

	//���s������������
	LPSTR lpCmdLine;
};