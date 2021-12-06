//=============================================================================
//
// UI [ui.cpp]
// ����T�q
//2021/11/30			�t�@�C���쐬
//=============================================================================
#include "ui.h"
#include "Texture.h"
#include "polygon.h"

//*****�萔��`*****
#define UI_POS_X		500.0f		//�ʒuX
#define UI_POS_Y		500.0f		//�ʒuY
#define UI_SIZE_X	100.0f			//����
#define UI_SIZE_Y	100.0f			//�c��
#define UI_TEXTURE_PATH		L"data/texture/number.png"
#define UI_TEXTURE_SPLIT_X	5		//�e�N�X�`����������
#define UI_TEXTURE_SPLIT_Y	5		//�e�N�X�`���������c
#define UI_FRAME			12		//�\������g

//*****�O���[�o���ϐ�*****


//==============================================================
//�ݽ�׸�
//==============================================================
UI::UI()
	:m_pos(XMFLOAT2(UI_POS_X, UI_POS_Y))
	, m_size(XMFLOAT2(UI_SIZE_X, UI_SIZE_Y))
{
	//�e�N�X�`���ǂݍ���
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	hr = CreateTextureFromFile(pDevice, UI_TEXTURE_PATH, &m_pTexture);
}
//==============================================================
//�޽�׸�
//==============================================================
UI::~UI() {
	//�e�N�X�`�����
	SAFE_RELEASE(m_pTexture);
}
//==============================================================
//�X�V
//==============================================================
void UI::Update() {

}
//==============================================================
//�`��
//==============================================================
void UI::Draw() {
	SetBlendState(BS_ALPHABLEND);	// ���u�����h
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(m_size.x, m_size.y);
	SetPolygonPos(m_pos.x, m_pos.y);
	SetPolygonFrameSize(1.0f / UI_TEXTURE_SPLIT_X,1.0f/ UI_TEXTURE_SPLIT_Y);
	SetPolygonUV((UI_FRAME % UI_TEXTURE_SPLIT_X) / (float)UI_TEXTURE_SPLIT_X,
		(UI_FRAME / UI_TEXTURE_SPLIT_X) / (float)UI_TEXTURE_SPLIT_Y);
	SetPolygonTexture(m_pTexture);
	DrawPolygon(pDC);

	//���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

	SetBlendState(BS_NONE);	// ���u�����h����
	SetZWrite(true);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
}