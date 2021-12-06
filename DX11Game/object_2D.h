//=============================================================================
//
// �QD�I�u�W�F�N�g [object_2D.h]
// ����T�q
//2021/11/30			�t�@�C���쐬
//=============================================================================
#pragma once
#include "main.h"


class Object_2D {
public:
	Object_2D();
	~Object_2D();
	void Update();
	void Draw();
private:
	XMFLOAT2	m_pos;
	XMFLOAT2	m_size;
	ID3D11ShaderResourceView* m_pTexture;
};