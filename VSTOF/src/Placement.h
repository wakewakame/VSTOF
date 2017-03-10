#pragma once

#include "Debug.h"
#include "define.h"
#include "Frame.h"
#include "ShareMem.h"
#include "GUI.h"

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

//�p�����[�^�[�̒l��t���[���ϐ��Ǘ��N���X
class Parameteres {
public:
	//�R���X�g���N�g
	Frames frames; //�t���[������N���X

	//�ϐ�������
	VSTParameteres *p_value; //�p�����[�^�[�l�ϐ��Q�|�C���^
	VSTParameteresFrames p_frame; //�p�����[�^�[�t���[���ϐ��Q

	//�֐��錾
	//�R���X�g���N�^
	Parameteres();
	//�f�X�g���N�^
	~Parameteres();
};

//�`��N���X
class Draw {
public:
	//�ϐ��錾
	float fps;
	RECT win_size;
	bool a; //�f�o�b�O�p
	float b; //�f�o�b�O�p

	//�R���X�g���N�g
	Parameteres para;
	GUI gui;

	//�֐��錾
	bool resize(WINDOW_INFO win_info);
	void loop();
};