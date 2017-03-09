#pragma once

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

	//�c���[�\����CUI�`��(DEGUG�p)
	void draw_node_name(frame *f) {
		std::string tree = "[frame nodes tree viewer]\n";
		std::vector<int> gap;
		tree = draw_node_name(f, gap, tree);
		std::cout << tree << std::endl;
	}
	string draw_node_name(frame *f, std::vector<int> gap, std::string tree) {
		bool pop_back = true;
		if (f->parent != nullptr) {
			if (f->parent->parent != nullptr) {
				if (f->parent->parent->childs[f->parent->parent->childs.size() - 1] == f->parent) {
					if (gap.size() >= 2) {
						gap[gap.size() - 2] += gap[gap.size() - 1] + 2;
						gap.pop_back();
						pop_back = false;
					}
				}
			}
		}
		if (f->parent != nullptr) {
			tree += "\n";
			for (int i = 0; i < gap.size(); i++) {
				for (int j = 0; j < gap[i]; j++) {
					tree += " ";
				}
				if (i == (gap.size() - 1)) {
					break;
				}
				tree += "��";
			}
			if (f->parent->childs[f->parent->childs.size() - 1] == f) {
				tree += "��";
			}else {
				tree += "��";
			}
		}
		tree += "[";
		tree += f->name;
		tree += "]";
		gap.push_back(0);
		for (int i = 0; i < f->childs.size(); i++) {
			tree = draw_node_name(f->childs[i], gap, tree);
		}
		if (pop_back) {
			gap.pop_back();
		}
		return tree;
	}
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