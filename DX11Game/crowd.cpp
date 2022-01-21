// 雲 [crowd.cpp]
#include "crowd.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "sceneGame.h"

// マクロ定義
#define CROWD_TEXTURE_PATH		L"data/texture/back_tower2.png"
#define CROWD_TEXTURE_PATH_2	L"data/texture/back_tower1.png"
#define CROWD_TEXTURE_PATH_3	L"data/texture/back_dungeon.png"
#define CROWD_TEXTURE_PATH_4	L"data/texture/back_sky.png"

#define CROWD_POS_X		0.0f
#define CROWD_POS_Y		0.0f
#define CROWD_SIZE_X	(120.0f)
#define CROWD_SIZE_Y	(120.0f)

#define CROWD_POS_Y_NOW	(180.0f)
#define CROWD_POS_Y_OLD	(-CROWD_POS_Y_NOW)
#define CROWD_SCROLL_SPEED	(1.5f)

// 構造体
static LPCWSTR g_CrowdTex[] = {
	CROWD_TEXTURE_PATH,
	CROWD_TEXTURE_PATH_2,
	CROWD_TEXTURE_PATH_3,
	CROWD_TEXTURE_PATH_4,
};

//グローバル変数


//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Crowd::Crowd()
	:m_pos(XMFLOAT2(CROWD_POS_X, CROWD_POS_Y))
	,m_posNow(XMFLOAT2(CROWD_POS_X, CROWD_POS_Y_NOW))
	,m_posNow2(XMFLOAT2(CROWD_SIZE_X - 10.0f, CROWD_POS_Y_NOW))
	,m_posOld(XMFLOAT2(CROWD_POS_X, CROWD_POS_Y_OLD))
	,m_posOld2(XMFLOAT2(CROWD_SIZE_X - 10.0f, CROWD_POS_Y_OLD))
	,m_size(XMFLOAT2(CROWD_SIZE_X, CROWD_SIZE_Y))
	,m_sizeGame(XMFLOAT2(CROWD_SIZE_X, CROWD_SIZE_Y / 2))
{
	//テクスチャ読み込み
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for(int i = 0; i < MAX_CROWD_TEXTURE; i++)
		hr = CreateTextureFromFile(pDevice, g_CrowdTex[i], &m_pTexture[i]);
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Crowd::~Crowd() {
	//テクスチャ解放
	for (int i = 0; i < MAX_CROWD_TEXTURE; i++)
		SAFE_RELEASE(m_pTexture[i]);
}
//==============================================================
//更新
//==============================================================
void Crowd::Update(int nTime) {
	switch (GetScene())
	{
	case SCENE_GAME:
		switch (nTime)
		{
		case 0:
			// 背景の座標をリセット
			if (m_posNow.x <= -CROWD_SIZE_X)
				m_posNow.x = CROWD_SIZE_X - 10.0f;
			if (m_posNow2.x <= -CROWD_SIZE_X)
				m_posNow2.x = CROWD_SIZE_X - 10.0f;

			m_posNow.x += CROWD_SCROLL_SPEED;
			m_posNow2.x += CROWD_SCROLL_SPEED;
			break;
		case 1:
			// 背景の座標をリセット
			if (m_posOld.x <= -CROWD_SIZE_X)
				m_posOld.x = CROWD_SIZE_X - 10.0f;
			if (m_posOld2.x <= -CROWD_SIZE_X)
				m_posOld2.x = CROWD_SIZE_X - 10.0f;

			m_posOld.x += CROWD_SCROLL_SPEED;
			m_posOld2.x += CROWD_SCROLL_SPEED;
			break;
		}
		break;
	default:
		break;
	}
}
//==============================================================
//描画
//==============================================================
void Crowd::Draw() {
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
		// 今の描画
		SetPolygonSize(m_sizeGame.x, m_sizeGame.y);
		SetPolygonPos(CROWD_POS_X, CROWD_POS_Y_NOW);
		SetPolygonTexture(m_pTexture[2]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		SetBlendState(BS_ALPHABLEND);
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
		SetBlendState(BS_NONE);

		// 過去の背景描画
		SetPolygonSize(m_sizeGame.x, m_sizeGame.y);
		SetPolygonPos(CROWD_POS_X, CROWD_POS_Y_OLD);
		SetPolygonTexture(m_pTexture[3]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		SetBlendState(BS_ALPHABLEND);
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
		SetBlendState(BS_NONE);
		break;
	}
}
