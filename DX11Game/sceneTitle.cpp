//===================================================
//		ƒ^ƒCƒgƒ‹ƒV[ƒ“[sceneTitle.cpp]
//¬“í—TŽq
//2021/11/30	ƒtƒ@ƒCƒ‹ì¬
//=====================================================
#include "sceneTitle.h"
#include "fade.h"
#include "bg.h"
#include "input.h"
#include "crowd.h"
#include "UserGuide.h"


//*****ƒOƒ[ƒoƒ‹•Ï”*****
static BG* g_pBG;		//”wŒi
Crowd* g_pCrowd;		//‰_


//=============================
//		‰Šú‰»
//=============================
HRESULT InitSceneTitle() {
	HRESULT hr = MB_OK;


	// ”wŒi‰Šú‰»
	g_pBG = new BG;

	// ‰_‰Šú‰»
	//g_pCrowd = new Crowd;


	return hr;
}

//=============================
//		I—¹
//=============================
void UninitSceneTitle() {
	
	// ”wŒiI—¹ˆ—
	delete g_pBG;

	// ‰_I—¹ˆ—
	//delete g_pCrowd;
}

//=============================
//		XV
//=============================
void UpdateSceneTitle() {
	
	bool GuideFlg = GetUserGuideFlg();

	// ”wŒiXV
	g_pBG->Update(5);

	// ‰_XV
	//g_pCrowd->Update(5);

	/*if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_SAMPLE);
	}*/
	if (!GuideFlg)
	{
		if (GetKeyPress(VK_RETURN) || GetJoyTrigger(0, JOYBUTTON1)) {
			StartFadeOut(SCENE_GAME);
		}

		if (GetKeyPress(VK_F3))// ŒãX‚±‚ê‚ðƒ`ƒ…[ƒgƒŠƒAƒ‹‚ÆƒQ[ƒ€‚ÌØ‚è‘Ö‚¦‚ÉŽg‚¤iReturn‚Åj
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
//		•`‰æ
//=============================
void DrawSceneTitle() {
	// Zƒoƒbƒtƒ@–³Œø(Zƒ`ƒFƒbƒN–³&ZXV–³)
	SetZBuffer(false);

	// ”wŒi•`‰æ
	g_pBG->Draw();

	// ‰_•`‰æ
	//g_pCrowd->Draw();
}