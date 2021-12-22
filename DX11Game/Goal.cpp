//=============================================================================
//
//  ゴール[Goal.h]
// 武井遥都
//=============================================================================
#include "Goal.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "now.h"

// マクロ定義
#define GOAL_TEXTURE_PATH	L"data/texture/気弾6.jpg"
#define GOAL_POS_X		0.0f
#define GOAL_POS_Y		0.0f
#define GOAL_SIZE_X		SCREEN_WIDTH / 2
#define GOAL_SIZE_Y		SCREEN_HEIGHT / 2


//// グローバル変数
static bool g_bUse;


//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Goal::Goal()
	:m_pos(XMFLOAT2(GOAL_POS_X, GOAL_POS_Y))
	, m_size(XMFLOAT2(GOAL_SIZE_X, GOAL_SIZE_Y))
{
	//テクスチャ読み込み
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	hr = CreateTextureFromFile(pDevice, GOAL_TEXTURE_PATH, &m_pTexture);

	g_bUse = false;
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Goal::~Goal() {
	//テクスチャ解放
	SAFE_RELEASE(m_pTexture);
}
//==============================================================
//更新
//==============================================================
void Goal::Update(float fX) {
	if (fX >= 108.0f)
		g_bUse = true;
}
//==============================================================
//描画
//==============================================================
void Goal::Draw() {
	if (g_bUse)
	{
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetPolygonSize(m_size.x, m_size.y);
		SetPolygonPos(m_pos.x, m_pos.y);
		SetPolygonTexture(m_pTexture);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
	}
}
