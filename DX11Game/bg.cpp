// �w�i [bg.cpp]
#include "bg.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "sceneGame.h"

// �}�N����`
#define BG_TEXTURE_PATH_SAMPLE		L"data/texture/sky001.jpg"
#define BG_TEXTURE_PATH_TITLE		L"data/texture/field000.jpg"
#define BG_TEXTURE_PATH_GAME_NOW	L"data/texture/back2.png"
#define BG_TEXTURE_PATH_GAME_OLD	L"data/texture/back1.png"
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_SIZE_X		SCREEN_WIDTH
#define BG_SIZE_Y		SCREEN_HEIGHT

#define BG_POS_Y_NOW	(180.0f)
#define BG_POS_Y_OLD	(-BG_POS_Y_NOW)
#define BG_SCROLL_SPEED	(1.5f)

// �\����
static LPCWSTR g_BgTex[] = {
	BG_TEXTURE_PATH_GAME_NOW,
	BG_TEXTURE_PATH_GAME_OLD,
};

//�O���[�o���ϐ�
XMFLOAT3 g_girlMove;
XMFLOAT3 g_boyMove;

//==============================================================
//�ݽ�׸�
//==============================================================
BG::BG()
	:m_pos(XMFLOAT2(BG_POS_X, BG_POS_Y))
	,m_posNow(XMFLOAT2(BG_POS_X, BG_POS_Y_NOW))
	,m_posNow2(XMFLOAT2(BG_SIZE_X - 10.0f, BG_POS_Y_NOW))
	,m_posOld(XMFLOAT2(BG_POS_X, BG_POS_Y_OLD))
	,m_posOld2(XMFLOAT2(BG_SIZE_X - 10.0f, BG_POS_Y_OLD))
	,m_size(XMFLOAT2(BG_SIZE_X, BG_SIZE_Y))
	,m_sizeGame(XMFLOAT2(BG_SIZE_X, BG_SIZE_Y / 2))
{
	//�e�N�X�`���ǂݍ���
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	switch (GetScene()) {
	case SCENE_TITLE:
		hr = CreateTextureFromFile(pDevice, BG_TEXTURE_PATH_TITLE, &m_pTexture[0]);
		break;
	case SCENE_GAME:
		for(int i = 0; i < 2; i++)
			hr = CreateTextureFromFile(pDevice, g_BgTex[i], &m_pTexture[i]);
		break;
	case SCENE_RESULT:
		//hr = CreateTextureFromFile(pDevice, BG_TEXTURE_PATH_RESULT, &m_pTexture);
		break;
	case SCENE_SAMPLE:
		hr = CreateTextureFromFile(pDevice, BG_TEXTURE_PATH_SAMPLE, &m_pTexture[0]);
	}
}
//==============================================================
//�޽�׸�
//==============================================================
BG::~BG() {
	//�e�N�X�`�����
	switch (GetScene())
	{
	default:
		SAFE_RELEASE(m_pTexture[0]);
		break;
	case SCENE_GAME:
		for (int i = 0; i < 2; i++)
			SAFE_RELEASE(m_pTexture[i]);
		break;
	}
}
//==============================================================
//�X�V
//==============================================================
void BG::Update(int nTime) {
	switch (GetScene())
	{
	case SCENE_GAME:
		// ���̎q�̈ړ��ʂ��擾
		g_girlMove = GetNow()->GetPlayerGirl()->GetGirlMove();
		// �j�̎q�̈ړ��ʂ��擾
		g_boyMove = GetOld()->GetPlayerBoy()->GetBoyMove();

		switch (nTime)
		{
		case 0:
			// �w�i�̍��W�����Z�b�g
			if (m_posNow.x <= -BG_SIZE_X)
				m_posNow.x = BG_SIZE_X - 10.0f;
			if (m_posNow2.x <= -BG_SIZE_X)
				m_posNow2.x = BG_SIZE_X - 10.0f;

			m_posNow.x -= g_girlMove.x * BG_SCROLL_SPEED;
			m_posNow2.x -= g_girlMove.x * BG_SCROLL_SPEED;
			break;
		case 1:
			// �w�i�̍��W�����Z�b�g
			if (m_posOld.x <= -BG_SIZE_X)
				m_posOld.x = BG_SIZE_X - 10.0f;
			if (m_posOld2.x <= -BG_SIZE_X)
				m_posOld2.x = BG_SIZE_X - 10.0f;

			m_posOld.x -= g_boyMove.x * BG_SCROLL_SPEED;
			m_posOld2.x -= g_boyMove.x * BG_SCROLL_SPEED;
			break;
		}
		break;
	default:
		break;
	}
}
//==============================================================
//�`��
//==============================================================
void BG::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	switch (GetScene())
	{
	default:
		SetPolygonSize(m_size.x, m_size.y);
		SetPolygonPos(m_pos.x, m_pos.y);
		SetPolygonTexture(m_pTexture[0]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		break;

	case SCENE_GAME:
		// ���̕`��
		SetPolygonSize(m_sizeGame.x, m_sizeGame.y);
		SetPolygonPos(m_posNow.x, m_posNow.y);
		SetPolygonTexture(m_pTexture[0]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		SetPolygonSize(m_sizeGame.x, m_sizeGame.y);
		SetPolygonPos(m_posNow2.x, m_posNow2.y);
		SetPolygonTexture(m_pTexture[0]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);

		// �ߋ��̔w�i�`��
		SetPolygonSize(m_sizeGame.x, m_sizeGame.y);
		SetPolygonPos(m_posOld.x, m_posOld.y);
		SetPolygonTexture(m_pTexture[1]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		SetPolygonSize(m_sizeGame.x, m_sizeGame.y);
		SetPolygonPos(m_posOld2.x, m_posOld2.y);
		SetPolygonTexture(m_pTexture[1]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		break;
	}
}
