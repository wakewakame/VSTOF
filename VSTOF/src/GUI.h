//�C���N���[�h
#include "ofMain.h"
#include "Frame.h"
#include "win_event.h"

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

//GUI�N���X
class GUI {
public:
	//---�ϐ��錾---//
	double move; //�ėp�A�j���[�V�����ϐ�
	double fps; //�t���[�����[�g

	//---�R���X�g���N�g---//
	WIN_EVENT win_event;
	Frames frames;
	UI_DESIGN ui;

	//---�֐��錾---//
	//�P�ʕϊ��֐�
	float percent(float a, float a_min, float a_max, float b_min, float b_max);
	int percent(int a, int a_min, int a_max, int b_min, int b_max);
	//�w�i������
	void reset();
	//�t���[�����[�g�w��
	void set_fps(double frame_rate);
	//���̃{�b�N�X�`��֐�
	void LineBox(RECT pos, int thick);
	//�t���[���̋��E���`��
	void FrameLine(frame *root);
	//�t���[���̖��̕`��
	void FrameName(frame *root);
	//�P�ʐ��`��֐�(�P�ʂ�`�悷���,���̖{���̒P�ʂł̒���,�`�悷��Ƃ��̐��̎n�܂�n�_�̒l,�`�悷��Ƃ��̐��̏I���̒n�_�̒l,�P�ʐ��̐�)
	void unit_line(RECT line, float length, float start, float finish, int num);
	//�O���t���W����E�B���h�E���W�ɕϊ�����֐�
	GraphPos conversion(WindowPos);
	//�O���t���UI(�t���[��,�g�`�z��,�T���v����,�`�惂�[�h)
	void wave_graph(frame *f, graph g, bool mode);
	//�p�����[�^�N���X���O���t�v���p�e�B�N���X�ɕϊ�����֐�
	graph c_graph(float *samples, GraphPara *param);
	//�O���t�ɒP�ʐ�������֐�
	void graph_unitline(frame *f, graph g, char *x, char *y);
	//�}�E�X�ŏ��������\�ȃO���t�`��֐�
	void wave_draw(frame *f, graph g, bool mode, bool active);
	//�g��k���\�ȃO���t�`��֐�
	void wave_gui(frame *f, float *samples, int num_sample, bool mode);
	//�{�����[�����UI
	void volume(frame *f, float *val);
	//�{�����[��UI
	void volume_gui(frame *f, float *val);
	//�p�����[�^�J�[�\���`��֐�
	void cursor(GraphPara *param, int index, char mode);
	//�X�C�b�`UI
	void sw(frame *f, bool *sw);
	//���t���[���Ăяo���֐�
	void loop();
	/*
	�ǉ��\��̊֐�
	�X�C�b�`�֐�
	�X���C�h�ϒ�R�֐�
	�e�O���t�p�����[�^�[�֐�
	ttf,otf�̎��̂ŕ�����`��֐�
	*/
	//�R���X�g���N�g

};