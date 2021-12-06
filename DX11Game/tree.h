//=============================================================================
//
// 木処理 [tree.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TREE				(256)	// 木最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TTree {
	XMFLOAT3	pos;		// 位置
	XMFLOAT4	col;		// 色
	float		width;		// 幅
	float		height;		// 高さ
	int			idxShadow;	// 影ID
	bool		use;		// 使用しているかどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);

int SetTree(XMFLOAT3 pos, float width, float height, XMFLOAT4 col);
