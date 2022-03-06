//===================================================
//		タイトルシーン[sceneTitle.cpp]
//小楠裕子
//2021/11/30	ファイル作成
//=====================================================
#include "sceneTitle.h"
#include "fade.h"
#include "bg.h"
#include "input.h"
#include "UserGuide.h"
#include "title.h"
#include "cloud.h"
#include "Sound.h"

//*****グローバル変数*****
static BG* g_pBG;		//背景
Cloud* g_pCloud;		//雲
Title* g_pTitle;		//タイトル
bool GetUserFlg;

//=============================
//		初期化
//=============================
HRESULT InitSceneTitle() {
	HRESULT hr = MB_OK;


	// 背景初期化
	g_pBG = new BG;

	// 雲初期化
	g_pCloud = new Cloud;
	GetUserFlg = false;
	// タイトル初期化
	g_pTitle = new Title;

	//サウンド初期化
	CSound::Init();
	CSound::Play(BGM_TITLE);

	return hr;
}

//=============================
//		終了
//=============================
void UninitSceneTitle() {
	
	// 背景終了処理
	delete g_pBG;

	// 雲終了処理
	delete g_pCloud;

	// タイトル終了処理
	delete g_pTitle;

	//サウンド終了
	CSound::Stop(BGM_TITLE);
	CSound::Fin();
}

//=============================
//		更新
//=============================
void UpdateSceneTitle() {
	
	bool GuideFlg = GetUserGuideFlg();

	// 背景更新
	g_pBG->Update(5);

	// 雲更新
	g_pCloud->Update();

	// タイトル更新
	g_pTitle->Update();

	/*if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_SAMPLE);
	}*/
	if (!GuideFlg)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, JOYBUTTON1)) 
		{
			CSound::Play(SE_DECIDE);
			StartFadeOut(SCENE_STAGE);
		}

		if (GetKeyPress(VK_F3))// 後々これをチュートリアルとゲームの切り替えに使う（Returnで）
		{
			StartFadeOut(SCENE_MAP);

		}
	}
	if (GetKeyTrigger(VK_M) || GetJoyTrigger(0, JOYBUTTON3))
	{
		SetUserGuideFlg(true);
		GuideFlg = true;
	}

	CSound::Update();
}

//=============================
//		描画
//=============================
void DrawSceneTitle() {
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// 背景描画
	g_pBG->Draw();

	// 雲描画
	g_pCloud->Draw();

	// タイトル描画
	g_pTitle->Draw();
}