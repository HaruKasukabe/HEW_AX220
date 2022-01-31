//===================================================
//		ステージセレクト[stageselect.cpp]
//菅野佑斗
//=====================================================
#include "stageselect.h"
#include "Texture.h"
#include "polygon.h"
#include "fade.h"
#include "input.h"
#include "debugproc.h"
#include "mapData.h"
#include "bg.h"
#include "cloud.h"
#include "Sound.h"

// マクロ定義
#define TEXTURE_PATH_STAGE_SELECT		L"data/texture/select.png"

#define STAGE_SELECT_BG_POS_X		-30.0f
#define STAGE_SELECT_BG_POS_Y		90.0f
#define STAGE_SELECT_BG_SIZE_X		SCREEN_WIDTH / 2
#define STAGE_SELECT_BG_SIZE_Y		SCREEN_HEIGHT / 2

#define STAGE_SELECT_BG_POS_Y_2		160.0f

//グローバル変数
static int g_stage;
ID3D11ShaderResourceView* g_stageSelectTexture;
static BG* g_pBG2;		//背景
Cloud* g_pCloud2;		//雲

HRESULT InitStageSelect()
{
	HRESULT hr = MB_OK;
	ID3D11Device* pDevice = GetDevice();

	// 背景初期化
	g_pBG2 = new BG;

	// 雲初期化
	g_pCloud2 = new Cloud;

	//サウンド初期化
	CSound::Init();

	hr = CreateTextureFromFile(pDevice, TEXTURE_PATH_STAGE_SELECT, &g_stageSelectTexture);

	g_stage = 0;

	return hr;
}
void UninitStageSelect()
{
	// テクスチャ解放
	SAFE_RELEASE(g_stageSelectTexture);

	// 背景終了処理
	delete g_pBG2;

	// 雲終了処理
	delete g_pCloud2;

	//サウンド終了
	CSound::Fin();
}
void UpdateStageSelect()
{
	// 雲更新
	g_pCloud2->Update();

	if (g_stage < 1)
	{
		if (GetKeyTrigger(VK_UP))
		{
			CSound::Play(SE_SELECT);
			g_stage += 1;
		}
	}
	if(g_stage > 0)
	{
		if (GetKeyTrigger(VK_DOWN))
		{
			CSound::Play(SE_SELECT);
			g_stage -= 1;
		}

	}
	if (GetKeyTrigger(VK_RETURN))
	{
		CSound::Play(SE_DECIDE);

		StartFadeOut(SCENE_GAME);
	}

	CSound::Update();

	PrintDebugProc("ｽﾃｰｼﾞｾﾚｸﾄ:%d\n", g_stage);
}
void DrawStageSelect()
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// 背景描画
	g_pBG2->Draw();

	// 雲描画
	g_pCloud2->Draw();

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetBlendState(BS_ALPHABLEND);
	switch (g_stage)
	{
	case STAGE_TUTORIAL:
		SetPolygonSize(STAGE_SELECT_BG_SIZE_X, STAGE_SELECT_BG_SIZE_Y);
		SetPolygonPos(STAGE_SELECT_BG_POS_X, STAGE_SELECT_BG_POS_Y);
		SetPolygonTexture(g_stageSelectTexture);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
		break;
	case STAGE_1:
		SetPolygonSize(STAGE_SELECT_BG_SIZE_X, STAGE_SELECT_BG_SIZE_Y);
		SetPolygonPos(STAGE_SELECT_BG_POS_X, STAGE_SELECT_BG_POS_Y_2);
		SetPolygonTexture(g_stageSelectTexture);
		SetPolygonUV(0.0f, 0.0f);
		DrawPolygon(pDC);
	}
	SetBlendState(BS_NONE);
}

int GetStage()
{
	return g_stage;
}