// 背景 [tutorial.cpp]
#include "tutorial.h"
#include "scene.h"
#include "Texture.h"
#include "polygon.h"
#include "sceneGame.h"
#include "UserGuide.h"
#include "Sound.h"

// マクロ定義
#define TUTORIAL_TEXTURE_PATH		L"data/texture/tutorial_carry.png"
#define TUTORIAL_TEXTURE_PATH_2		L"data/texture/tutorial_break.png"
#define TUTORIAL_TEXTURE_PATH_3     L"data/texture/tutorial_goal.png"

#define TUTORIAL_POS_X		(0.0f)
#define TUTORIAL_POS_Y		(200.0f)
#define TUTORIAL_SIZE_X		(440.0f)
#define TUTORIAL_SIZE_Y		(360.0f)

#define TUTORIAL_BOY_POS_X		(-30.0f)
#define TUTORIAL_BOY_POS_X_2	(50.0f)
#define TUTORIAL_BOY_POS_X_3	(100.0f)

// 構造体
static LPCWSTR g_TutorialTex[] = {
	TUTORIAL_TEXTURE_PATH,
	TUTORIAL_TEXTURE_PATH_2,
	TUTORIAL_TEXTURE_PATH_3,
};

//グローバル変数
bool g_bTutorial[MAX_TUTORIAL_TEXTURE];
int g_nNum;

//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Tutorial::Tutorial()
	:m_pos(XMFLOAT2(TUTORIAL_POS_X, TUTORIAL_POS_Y))
	, m_size(XMFLOAT2(TUTORIAL_SIZE_X, TUTORIAL_SIZE_Y))
{
	//テクスチャ読み込み
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for (int i = 0; i < MAX_TUTORIAL_TEXTURE; i++)
		hr = CreateTextureFromFile(pDevice, g_TutorialTex[i], &m_pTexture[i]);

	for (int i = 0; i < MAX_TUTORIAL_TEXTURE; i++)
		g_bTutorial[i] = false;
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Tutorial::~Tutorial() {
	//テクスチャ解放
	for (int i = 0; i < MAX_TUTORIAL_TEXTURE; i++)
		SAFE_RELEASE(m_pTexture[i]);
}
//==============================================================
//更新
//==============================================================
void Tutorial::Update(XMFLOAT3 boyPos) {
	if (boyPos.x >= TUTORIAL_BOY_POS_X && g_bTutorial[0] == false)
	{
		CSound::Play(SE_TUTORIAL);
		g_bTutorial[0] = true;
		g_nNum = 0;
	}
	if (boyPos.x >= TUTORIAL_BOY_POS_X_2 && g_bTutorial[1] == false)
	{
		CSound::Play(SE_TUTORIAL);
		g_bTutorial[1] = true;
		g_nNum = 1;
	}
	if (boyPos.x >= TUTORIAL_BOY_POS_X_3 && g_bTutorial[2] == false)
	{
		CSound::Play(SE_TUTORIAL);
		g_bTutorial[2] = true;
		g_nNum = 2;
	}
}
//==============================================================
//描画
//==============================================================
void Tutorial::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetBlendState(BS_NONE);
	for (int i = 0; i < MAX_TUTORIAL_TEXTURE; i++)
	{
		if (!g_bTutorial[i] || g_nNum != i)
			continue;

		SetPolygonSize(m_size.x, m_size.y);
		SetPolygonPos(m_pos.x, m_pos.y);
		SetPolygonTexture(m_pTexture[i]);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
	}
	SetBlendState(BS_NONE);
}
