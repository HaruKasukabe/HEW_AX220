//=============================================================================
//
// UI [ui.h]
// ����T�q
//2021/11/30			�t�@�C���쐬
//=============================================================================
#pragma once
#include "main.h"


class UI {
public:
	UI();
	~UI();
	void Update();
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_size;
	ID3D11ShaderResourceView* m_pTexture;
};
