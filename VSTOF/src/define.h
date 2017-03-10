#pragma once

#include "ofMain.h"
#include "windows.h"
#include <vector>
#include <string>

//���F���
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

struct Dimension {
	bool on_limit_min; //�p�����[�^�̉���̍ŏ��l���L�����ǂ���
	bool on_limit_max; //�p�����[�^�̉���̍ő�l���L�����ǂ���
	float limit_min; //�p�����[�^�̉���̍ŏ��l
	float limit_max; //�p�����[�^�̉���̍ő�l
	int min_index; //�O���t�̕`���̍ŏ��l�p�����[�^�̃C���f�b�N�X
	int max_index; //�O���t�̕`���̍ő�l�p�����[�^�̃C���f�b�N�X
	std::vector<float> val; //�p�����[�^�̒l�̔z��
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
};

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

//���[�e�B���e�B�N���X
class Utility {
public:
	//�����ϊ��֐�(�߂�l:�ϊ���,����1~5:�ϊ��O�̒l,�ϊ��O�̍ŏ��l,�ϊ��O�̍ő�l,�ϊ���̍ŏ��l,�ϊ���̍ő�l)
	static double Proportion(double i, double i_mix, double i_max, double j_min, double j_max) {
		double j;
		j = j_min + (i - i_mix) * ((j_max - j_min) / (i_max - i_mix));
		return j;
	}
	static float Proportion(float i, float i_mix, float i_max, float j_min, float j_max) {
		float j;
		j = int(Proportion(double(i), double(i_mix), double(i_max), double(j_min), double(j_max)));
		return j;
	}
	static int Proportion(int i, int i_mix, int i_max, int j_min, int j_max) {
		int j;
		j = int(Proportion(double(i), double(i_mix), double(i_max), double(j_min), double(j_max)));
		return j;
	}
};

//�O���t��̍��W
struct GraphPos {
	int x;
	float y;
};

//�E�B���h�E��̍��W
struct WindowPos {
	int x;
	int y;
};

//�O���t�̃v���p�e�B
struct graph {
	float *samples; //�g�`�{��
	int num_sample; //�g�`�̃T���v����
	int start_index; //�`��̈�̎n�߂̔g�`�̃T���v���̃C���f�b�N�X
	int end_index; //�`��̈�̏I���̔g�`�̃T���v���̃C���f�b�N�X
	float zero_index_val; //�g�`�̃T���v����0�̒n�_�̃O���t��̒P�ʂ�x���W
	float last_index_val; //�g�`�̃T���v�����ŏI�n�_�̃O���t��̒P�ʂ�x���W
	float val_min; //�`��̈�̃T���v���l�̍ŏ��l
	float val_max; //�`��̈�̃T���v���l�̍ő�l
	float val_lim_min; //�`��̈�̃T���v���l�̍ŏ��l�̉���̍ŏ��l
	float val_lim_max; //�`��̈�̃T���v���l�̍ő�l�̉���̍ő�l
};

