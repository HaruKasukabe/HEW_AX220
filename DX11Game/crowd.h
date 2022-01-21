// �_�`�� [crowd.h]
#pragma once
#include "main.h"

//// �֐��v���g�^�C�v
//HRESULT InitBG();	// ������
//void UninitBG();	// �I������
//void UpdateBG();	// �X�V
//void DrawBG();		// �`��

#define MAX_CROWD_TEXTURE	(4)

class Crowd {
public:
	Crowd();
	 ~Crowd();
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
	ID3D11ShaderResourceView* m_pTexture[MAX_CROWD_TEXTURE];
};