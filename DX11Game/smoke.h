//========================================================
//
//���G�t�F�N�g[smoke.h]
//
//========================================================
#pragma once

#include "main.h"

//********************************************************
//�v���g�^�C�v�錾
//********************************************************
HRESULT InitSmoke(void);
void UninitSmoke(void);
void UpdateSmoke(void);
void DrawSmoke(void);

int StartSmoke(XMFLOAT3 pos, XMFLOAT2 size);