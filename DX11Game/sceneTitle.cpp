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
	

	// ”wŒiXV
	g_pBG->Update(5);

	// ‰_XV
	//g_pCrowd->Update(5);

	/*if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_SAMPLE);
	}*/
	if (GetKeyPress(VK_F2)) {
		StartFadeOut(SCENE_GAME);
	}

	if (GetKeyPress(VK_RETURN))
	{
		StartFadeOut(SCENE_MAP);

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