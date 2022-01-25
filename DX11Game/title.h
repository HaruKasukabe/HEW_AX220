// タイトル描画 [title.h]
#pragma once
#include "main.h"

//// 関数プロトタイプ
//HRESULT InitBG();	// 初期化
//void UninitBG();	// 終了処理
//void UpdateBG();	// 更新
//void DrawBG();		// 描画

#define MAX_TITLE_TEXTURE	(2)

class Title {
public:
	Title();
	 ~Title();
	void Update();
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_size;
	ID3D11ShaderResourceView* m_pTexture[MAX_TITLE_TEXTURE];
};