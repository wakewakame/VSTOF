//���L�������\����
struct ShareMemData {
	HWND Ready; //GUI�N���C�A���g���N�������TRUE�ɂ���(���d�N������̂���)
	HWND Host; //�`���n���h��
	VSTParameteres para; //VST�̃p�����[�^�[�Q
};

//���L�������A�N�Z�X�N���X
class ShareMem {
public:
	//�ϐ�������
	HANDLE MapHandle = nullptr; //�}�b�v�̃n���h��
	ShareMemData *smd = nullptr; //���L�������f�[�^�|�C���^
	int ShareMemSize = sizeof(ShareMemData); //�}�b�v�̃T�C�Y

											 //�֐��錾
	bool Open(LPCTSTR MapName);
};