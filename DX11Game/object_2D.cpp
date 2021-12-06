//=============================================================================
//
// �QD�I�u�W�F�N�g [object_2D.cpp]
// ����T�q
//2021/11/30			�t�@�C���쐬
//=============================================================================
#include "object_2D.h"
#include "Texture.h"
#include "polygon.h"

//*****�萔��`*****
#define OBJECT_2D_POS_X		0.0f		//�ʒuX
#define OBJECT_2D_POS_Y		0.0f		//�ʒuY
#define OBJECT_2D_SIZE_X	100.0f		//����
#define OBJECT_2D_SIZE_Y	100.0f		//�c��

#define OBJECT_2D_TEXTURE_PATH		L"data/texture/field000.jpg"

//*****�O���[�o���ϐ�*****


//==============================================================
//�ݽ�׸�
//==============================================================
Object_2D::Object_2D() 
	:m_pos(XMFLOAT2(OBJECT_2D_POS_X, OBJECT_2D_POS_Y))
	,m_size(XMFLOAT2(OBJECT_2D_SIZE_X,OBJECT_2D_SIZE_Y))
{
	//�e�N�X�`���ǂݍ���
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	hr = CreateTextureFromFile(pDevice, OBJECT_2D_TEXTURE_PATH, &m_pTexture);
}
//==============================================================
//�޽�׸�
//==============================================================
Object_2D::~Object_2D() {
	//�e�N�X�`�����
	SAFE_RELEASE(m_pTexture);
}
//==============================================================
//�X�V
//==============================================================
void Object_2D::Update() {

}
//==============================================================
//�`��
//==============================================================
void Object_2D::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(m_size.x,m_size.y);
	SetPolygonPos(m_pos.x,m_pos.y);
	SetPolygonTexture(m_pTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	//���ɖ߂�
}