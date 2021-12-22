// 背景描画 [bg.h]
#pragma once
#include "main.h"

//// 関数プロトタイプ
//HRESULT InitBG();	// 初期化
//void UninitBG();	// 終了処理
//void UpdateBG();	// 更新
//void DrawBG();		// 描画


class BG {
public:
	BG();
	 ~BG();
	void Update();
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_pos2;
	XMFLOAT2	m_size;
	ID3D11ShaderResourceView* m_pTexture;
};