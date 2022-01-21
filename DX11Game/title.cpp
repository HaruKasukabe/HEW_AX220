// �w�i [title.cpp]
#include "title.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "sceneGame.h"

// �}�N����`
#define TITLE_TEXTURE_PATH		L"data/texture/back_tower2.png"
#define TITLE_TEXTURE_PATH_2	L"data/texture/back_tower1.png"

#define TITLE_POS_X		(SCREEN_WIDTH / 2)
#define TITLE_POS_Y		(SCREEN_HEIGHT / 3)
#define TITLE_SIZE_X	(120.0f)
#define TITLE_SIZE_Y	(120.0f)

#define TITLE_MOVE_SPEED	(1.5f)

// �\����
static LPCWSTR g_TitleTex[] = {
	TITLE_TEXTURE_PATH,
	TITLE_TEXTURE_PATH_2,
};

//�O���[�o���ϐ�
bool g_bMoveDir;

//==============================================================
//�ݽ�׸�
//==============================================================
Title::Title()
	:m_pos(XMFLOAT2(TITLE_POS_X, TITLE_POS_Y))
	,m_size(XMFLOAT2(TITLE_SIZE_X, TITLE_SIZE_Y))
{
	//�e�N�X�`���ǂݍ���
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for(int i = 0; i < MAX_TITLE_TEXTURE; i++)
		hr = CreateTextureFromFile(pDevice, g_TitleTex[i], &m_pTexture[i]);

	g_bMoveDir = true;
}
//==============================================================
//�޽�׸�
//==============================================================
Title::~Title() {
	//�e�N�X�`�����
	for (int i = 0; i < MAX_TITLE_TEXTURE; i++)
		SAFE_RELEASE(m_pTexture[i]);
}
//==============================================================
//�X�V
//==============================================================
void Title::Update(int nTime) {
	if (m_pos.y < TITLE_POS_Y - 180.0f)
		g_bMoveDir = true;
	else if (m_pos.y > TITLE_POS_Y + 180.0f)
		g_bMoveDir = false;

	if (g_bMoveDir == false)
		m_pos.y -= TITLE_MOVE_SPEED;
	else
		m_pos.y += TITLE_MOVE_SPEED;
}
//==============================================================
//�`��
//==============================================================
void Title::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// ���̕`��
	SetPolygonSize(m_size.x, m_size.y);
	SetPolygonPos(m_pos.x, m_pos.y);
	SetPolygonTexture(m_pTexture[0]);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);
}
