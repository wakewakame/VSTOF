#include "Placement.h"

Parameteres::Parameteres() { //�S�p�����[�^�[���̃t���[���쐬
				//�t���[������
				//frames.add(frame *parent, frame *self, std::string name, int length, bool lock)
	frames.add(nullptr, &p_frame.root, "root", 0, 0);
	frames.add(&p_frame.root, &p_frame.all, "all", 0, 0);
	frames.add(&p_frame.root, &p_frame.scroll, "scroll", 16, 1);
	frames.add(&p_frame.all, &p_frame.tone, "tone", 0, 0); //���F�ݒ�t���[������
	frames.add(&p_frame.tone, &p_frame.make_auto, "make_auto", 200, 0); //�����ŉ��F�𐶐����邩
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

Parameteres::~Parameteres() {
	frames.all_dispose(&p_frame.root);
}

bool Draw::resize(WINDOW_INFO win_info) { //�`��̈�ύX
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

void Draw::loop() { //���[�v���ɌĂяo���֐�
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
		gui.volume_gui(&para.p_frame.rawwave, &b);
	}
	//���t���[���Ăяo���֐�
	gui.loop();
}