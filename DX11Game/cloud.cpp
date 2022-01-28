// 雲 [cloud.cpp]
#include "cloud.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "sceneGame.h"
#include <time.h>

// マクロ定義
#define CLOUD_TEXTURE_PATH		L"data/texture/cloud.png"
#define CLOUD_TEXTURE_PATH_2	L"data/texture/cloud2.png"
#define CLOUD_TEXTURE_PATH_3	L"data/texture/cloud3.png"
#define CLOUD_TEXTURE_PATH_4	L"data/texture/cloud4.png"

#define CLOUD_POS_X		-600.0f
#define CLOUD_POS_Y		220.0f
#define CLOUD_POS_2_X	-900.0f
#define CLOUD_POS_2_Y	200.0f
#define CLOUD_POS_3_X	-1200.0f
#define CLOUD_POS_3_Y	160.0f
#define CLOUD_POS_4_X	-1500.0f
#define CLOUD_POS_4_Y	180.0f

#define CLOUD_SIZE		(180.0f)
#define CLOUD_SIZE_2	(120.0f)
#define CLOUD_SIZE_3	(240.0f)
#define CLOUD_SIZE_4	(260.0f)

#define CLOUD_END		(800.0f)

#define CLOUD_SCROLL_SPEED		(0.5f)
#define CLOUD_SCROLL_SPEED_2	(0.8f)
#define CLOUD_SCROLL_SPEED_3	(1.3f)
#define CLOUD_SCROLL_SPEED_4	(1.8f)

// 構造体
static LPCWSTR g_CloudTex[] = {
	CLOUD_TEXTURE_PATH,
	CLOUD_TEXTURE_PATH_2,
	CLOUD_TEXTURE_PATH_3,
	CLOUD_TEXTURE_PATH_4,
};

//グローバル変数


//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Cloud::Cloud()
	:m_pos(XMFLOAT2(CLOUD_POS_X, CLOUD_POS_Y))
	,m_pos2(XMFLOAT2(CLOUD_POS_2_X, CLOUD_POS_2_Y))
	,m_pos3(XMFLOAT2(CLOUD_POS_3_X, CLOUD_POS_3_Y))
	,m_pos4(XMFLOAT2(CLOUD_POS_4_X, CLOUD_POS_4_Y))
	,m_size(XMFLOAT2(CLOUD_SIZE, CLOUD_SIZE))
	,m_size2(XMFLOAT2(CLOUD_SIZE_2, CLOUD_SIZE_2))
	,m_size3(XMFLOAT2(CLOUD_SIZE_3, CLOUD_SIZE_3))
	,m_size4(XMFLOAT2(CLOUD_SIZE_4, CLOUD_SIZE_4))
{
	//テクスチャ読み込み
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for(int i = 0; i < MAX_CLOUD_TEXTURE; i++)
		hr = CreateTextureFromFile(pDevice, g_CloudTex[i], &m_pTexture[i]);
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Cloud::~Cloud() {
	//テクスチャ解放
	for (int i = 0; i < MAX_CLOUD_TEXTURE; i++)
		SAFE_RELEASE(m_pTexture[i]);
}
//==============================================================
//更新
//==============================================================
void Cloud::Update() {
	srand((unsigned int)(time)(NULL));

		// 背景の座標をリセット
	if (m_pos.x > CLOUD_END)
	{
		m_pos.x = CLOUD_POS_X - 10.0f;
		m_pos.y = rand() % 180;
	}
	if (m_pos2.x > CLOUD_END)
	{
		m_pos2.x = CLOUD_POS_2_X - 10.0f;
		m_pos2.y = rand() % 240 + 180;
	}

	m_pos.x += CLOUD_SCROLL_SPEED;
	m_pos2.x += CLOUD_SCROLL_SPEED;

	if (m_pos3.x > CLOUD_END)
	{
		m_pos3.x = CLOUD_POS_3_X - 10.0f;
		m_pos3.y = rand() % 320 + 240;
	}
	if (m_pos4.x > CLOUD_END)
	{
		m_pos4.x = CLOUD_POS_4_X - 10.0f;
		m_pos4.y = rand() % 400 + 320;
	}

	m_pos3.x += CLOUD_SCROLL_SPEED;
	m_pos4.x += CLOUD_SCROLL_SPEED;
}
//==============================================================
//描画
//==============================================================
void Cloud::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetBlendState(BS_ALPHABLEND);

	SetPolygonSize(m_size.x, m_size.y);
	SetPolygonPos(m_pos.x, m_pos.y);
	SetPolygonTexture(m_pTexture[0]);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	SetPolygonSize(m_size2.x, m_size2.y);
	SetPolygonPos(m_pos2.x, m_pos2.y);
	SetPolygonTexture(m_pTexture[1]);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);
	
	SetPolygonSize(m_size3.x, m_size3.y);
	SetPolygonPos(m_pos3.x, m_pos3.y);
	SetPolygonTexture(m_pTexture[2]);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	SetPolygonSize(m_size4.x, m_size4.y);
	SetPolygonPos(m_pos4.x, m_pos4.y);
	SetPolygonTexture(m_pTexture[3]);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	SetBlendState(BS_NONE);
}
