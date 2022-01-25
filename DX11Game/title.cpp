// 背景 [title.cpp]
#include "title.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "sceneGame.h"

// マクロ定義
#define TITLE_TEXTURE_PATH		L"data/texture/title.png"
#define TITLE_TEXTURE_PATH_2	L"data/texture/back_tower1.png"

#define TITLE_POS_X		(0.0f)
#define TITLE_POS_Y		(200.0f)
#define TITLE_SIZE_X	(440.0f)
#define TITLE_SIZE_Y	(120.0f)

#define TITLE_MOVE_SPEED	(0.4f)

// 構造体
static LPCWSTR g_TitleTex[] = {
	TITLE_TEXTURE_PATH,
	TITLE_TEXTURE_PATH_2,
};

//グローバル変数
bool g_bMoveDir;

//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Title::Title()
	:m_pos(XMFLOAT2(TITLE_POS_X, TITLE_POS_Y))
	,m_size(XMFLOAT2(TITLE_SIZE_X, TITLE_SIZE_Y))
{
	//テクスチャ読み込み
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for(int i = 0; i < MAX_TITLE_TEXTURE; i++)
		hr = CreateTextureFromFile(pDevice, g_TitleTex[i], &m_pTexture[i]);

	g_bMoveDir = true;
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Title::~Title() {
	//テクスチャ解放
	for (int i = 0; i < MAX_TITLE_TEXTURE; i++)
		SAFE_RELEASE(m_pTexture[i]);
}
//==============================================================
//更新
//==============================================================
void Title::Update() {
	if (m_pos.y < TITLE_POS_Y - 20.0f)
		g_bMoveDir = true;
	else if (m_pos.y > TITLE_POS_Y + 20.0f)
		g_bMoveDir = false;

	if (g_bMoveDir == false)
		m_pos.y -= TITLE_MOVE_SPEED;
	else
		m_pos.y += TITLE_MOVE_SPEED;
}
//==============================================================
//描画
//==============================================================
void Title::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 今の描画
	SetBlendState(BS_ALPHABLEND);
	SetPolygonSize(m_size.x, m_size.y);
	SetPolygonPos(m_pos.x, m_pos.y);
	SetPolygonTexture(m_pTexture[0]);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);
	SetBlendState(BS_NONE);
}
