//========================================================
//
//��������[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

struct TBreak
{
	XMFLOAT3	pos;		//�ʒu
	XMFLOAT2	size;		//�T�C�Y
	bool		use;		//�g�p���Ă��邩�ǂ���
	int			anim;		//�A�j���[�V�����Đ��ʒu
	int			count;		//�A�j���[�V�����t���[����

};

//********************************************************
//�v���g�^�C�v�錾
//********************************************************
class Break
{
public:
	Break();
	~Break();
	void UpdateBreak(void);
	void DrawBreak(void);

	int StartBreak(XMFLOAT3 pos, XMFLOAT2 size);
private:
	
};


//HRESULT InitBreak(void);
//void UninitBreak(void);

