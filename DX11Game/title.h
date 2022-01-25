// �^�C�g���`�� [title.h]
#pragma once
#include "main.h"

//// �֐��v���g�^�C�v
//HRESULT InitBG();	// ������
//void UninitBG();	// �I������
//void UpdateBG();	// �X�V
//void DrawBG();		// �`��

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