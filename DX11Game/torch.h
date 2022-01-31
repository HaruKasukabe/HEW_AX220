//========================================================
//
//��������[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

struct TTorch
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
class Torch
{
public:
	Torch();
	~Torch();
	void UpdateTorch(void);
	void DrawTorch(void);

	int StartTorch(XMFLOAT3 pos, XMFLOAT2 size);
private:

};


//HRESULT InitTorch(void);
//void UninitTorch(void);

