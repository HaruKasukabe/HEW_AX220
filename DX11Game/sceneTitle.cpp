//===================================================
//		タイトルシーン[sceneTitle.cpp]
//小楠裕子
//2021/11/30	ファイル作成
//=====================================================
#include "sceneTitle.h"
#include "fade.h"
#include "bg.h"
#include "input.h"
#include "crowd.h"
#include "UserGuide.h"


//*****グローバル変数*****
static BG* g_pBG;		//背景
Crowd* g_pCrowd;		//雲


//=============================
//		初期化
//=============================
HRESULT InitSceneTitle() {
	HRESULT hr = MB_OK;


	// 背景初期化
	g_pBG = new BG;

	// 雲初期化
	//g_pCrowd = new Crowd;


	return hr;
}

//=============================
//		終了
//=============================
void UninitSceneTitle() {
	
	// 背景終了処理
	delete g_pBG;

	// 雲終了処理
	//delete g_pCrowd;
}

//=============================
//		更新
//=============================
void UpdateSceneTitle() {
	
	bool GuideFlg = GetUserGuideFlg();

	// 背景更新
	g_pBG->Update(5);

	// 雲更新
	//g_pCrowd->Update(5);

	/*if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_SAMPLE);
	}*/
	if (!GuideFlg)
	{
		if (GetKeyPress(VK_RETURN) || GetJoyTrigger(0, JOYBUTTON1)) {
			StartFadeOut(SCENE_GAME);
		}

		if (GetKeyPress(VK_F3))// 後々これをチュートリアルとゲームの切り替えに使う（Returnで）
		{
			StartFadeOut(SCENE_MAP);

		}
	}
	if (GetKeyPress(VK_M) || GetJoyButton(0, JOYBUTTON3))
	{
		SetUserGuideFlg(true);
		GuideFlg = true;
	}

	
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
	//g_pCrowd->Draw();
}