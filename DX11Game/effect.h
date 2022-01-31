//========================================================
//
//��������[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

struct TEffect
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
class Effect
{
public:
	Effect();
	~Effect();
	void UpdateEffect(void);
	void DrawEffect(void);

	int StartEffect(XMFLOAT3 pos, XMFLOAT2 size);
private:

};


//HRESULT InitEffect(void);
//void UninitEffect(void);

