//========================================================
//
//���G�t�F�N�g[fire.h]
//
//========================================================
#pragma once

#include "main.h"

//********************************************************
//�v���g�^�C�v�錾
//********************************************************
HRESULT InitFire(void);
void UninitFire(void);
void UpdateFire(void);
void DrawFire(void);

int StartFire(XMFLOAT3 pos, float size, XMFLOAT4X4 world);