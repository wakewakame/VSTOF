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

//UI�̃A�j���[�V�������ʊǗ��N���X�̐݌v����
/*
��̗̂��ꃁ��
�A�j���[�V�����N���X�Ƀt���[���ϐ��z���
vector�ō��A������e�t���[���̕ێ�����
�ǉ�����B�錾���͂ǂ̃t���[����
�A�j���[�V�����ϐ��z��̔z�񒷂�0
�eUI�`��֐���������Q�Ƃ��A�z��̒�����
����Ȃ��ꍇ�A�z����g�����A
���߂�ꂽ�l�ɏ�����
����Ă���ꍇ�͕��u
�A�j���[�V�����ϐ��̒��g�́A
time{
double t; //fps�Ɉˑ����Ȃ����Ԏ��ϐ�(0.0<=t<=1.0)
char val; //�ω��O���t�̎��
}
val = 0 -> �ω��͂��̂܂�
val = 1 -> y=x^2
val = 2 -> y=-(x-1)^2+1
val = 3 -> y=3*(x^2)-2*(x^3) <-�O���֐��̂��

�܂��A�A�j���[�V�������̕ω��O���t���e��
�������֐����p��
�܂��A�R���X�g���N�^�̈����Ȃǂ�fps�������Ɏ��A
fps���擾
*/

//GUI�N���X�̕ύX���ׂ��_
/*
Draw�N���X�ŊeGUI�p�[�c��z�u���A
���Ƃ�GUI�N���X�̃��[�v�֐���
Draw�N���X�Ŗ��t���[���Ăяo��������
�ǂ��悤�ɂ���B
�������邱�Ƃɂ��A�A�j���[�V�����ϐ��Ȃǂ�
�Ǘ����ȒP���킩��₷���Ȃ�ׁB
*/

//�h��Ԃ��̎d�l�ύX�ɂ���
/*
f->size.x<num_sample�̎��͓h��Ԃ���������
�w��͈͓��̑��p�`�`��
A->B
 ��
percent�ϊ�
```
float *samples; //�g�`�z��
int height; // �`�捂��
for(int j = 0; j < width; j++){
	percent(
		i,
		0,
		width,
		(int)samples[index],
		(int)samples[index+1]
	);
}
```
*/