//========================================================
//
//�r���{�[�h�e[bullet.h]
//
//========================================================
#pragma once

#include "main.h"

//********************************************************
//�v���g�^�C�v�錾
//********************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int FireBullet(XMFLOAT3 pos, XMFLOAT3 dir);