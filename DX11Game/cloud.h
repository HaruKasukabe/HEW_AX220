// 雲描画 [cloud.h]
#pragma once
#include "main.h"

//// 関数プロトタイプ
//HRESULT InitBG();	// 初期化
//void UninitBG();	// 終了処理
//void UpdateBG();	// 更新
//void DrawBG();		// 描画

#define MAX_CLOUD_TEXTURE	(4)

class Cloud {
public:
	Cloud();
	 ~Cloud();
	void Update();
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_pos2;
	XMFLOAT2	m_pos3;
	XMFLOAT2	m_pos4;
	XMFLOAT2	m_size;
	XMFLOAT2	m_size2;
	XMFLOAT2	m_size3;
	XMFLOAT2	m_size4;
	ID3D11ShaderResourceView* m_pTexture[MAX_CLOUD_TEXTURE];
};