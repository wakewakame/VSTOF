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
	FBO();
	//�f�X�g���N�^
	~FBO();
	//�t���[���o�b�t�@�̒ǉ�
	int add(int x, int y);
	//�J���[�t���[���o�b�t�@�̐؂�ւ�(index=-1�Ȃ烌���_�[�o�b�t�@)
	void change_c(int index);
	//�A���t�@�t���[���o�b�t�@�̐؂�ւ�(index=-1�Ȃ烌���_�[�o�b�t�@)
	void change_a(int index);
	//�w��J���[�o�b�t�@�`��
	void draw_c(int x, int y, int index);
	//�t���[���T�C�Y�擾�֐�
	POINT get_size(int index);
};