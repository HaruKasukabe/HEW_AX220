//========================================================
//
//爆発処理[explosion.h]
//
//========================================================
#pragma once

#include "main.h"

//********************************************************
//プロトタイプ宣言
//********************************************************
HRESULT InitDashEF(void);
void UninitDashEF(void);
void UpdateDashEF(void);
void DrawDashEF(void);

int StartDashEF(XMFLOAT3 pos, XMFLOAT2 size);