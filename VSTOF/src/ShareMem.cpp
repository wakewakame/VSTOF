#include "ShareMem.h"

bool ShareMem::Open(LPCTSTR MapName) { //���L�������A�N�Z�X�֐�
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

ShareMem::~ShareMem() { //���L�������N���[�Y
	UnmapViewOfFile(smd); //���L�������f�[�^�|�C���^�J��
}