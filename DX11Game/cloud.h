// �_�`�� [cloud.h]
#pragma once
#include "main.h"

//// �֐��v���g�^�C�v
//HRESULT InitBG();	// ������
//void UninitBG();	// �I������
//void UpdateBG();	// �X�V
//void DrawBG();		// �`��

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