// 背景描画 [bg.h]
#pragma once
#include "main.h"

//// 関数プロトタイプ
//HRESULT InitBG();	// 初期化
//void UninitBG();	// 終了処理
//void UpdateBG();	// 更新
//void DrawBG();		// 描画

#define MAX_BG_TEXTURE	(4)

class BG {
public:
	BG();
	 ~BG();
	void Update(int);
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_posNow;
	XMFLOAT2	m_posNow2;
	XMFLOAT2	m_posOld;
	XMFLOAT2	m_posOld2;
	XMFLOAT2	m_size;
	XMFLOAT2	m_sizeGame;
	ID3D11ShaderResourceView* m_pTexture[MAX_BG_TEXTURE];
};