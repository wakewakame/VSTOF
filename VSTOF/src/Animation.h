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
	void set_fps(double frame_rate);
	//�A�j���[�V�����z��̒���
	int get_len();
	//�A�j���[�V�����ϐ��̒ǉ�
	void add(double length, char effect_type, bool *sw);
	//���t���[���Ăяo���֐�
	void loop();
};