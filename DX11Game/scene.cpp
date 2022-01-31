//画面遷移[scene.cpp]
#include "scene.h"
#include "fade.h"
#include "sceneSample.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "mapselect.h"
#include "stageselect.h"
#include "UserGuide.h"

//グローバル変数
static EScene g_eScene = SCENE_NONE;

//初期化
HRESULT InitScene() {
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE);		//最初はタイトル画面
	InitFade();
	InitUserGuide();
	return hr;
}

//終了処理
void UninitScene() {
	SetScene(SCENE_NONE);		//現在の画面を終了
	UninitFade();
	UninitUserGuide();
}

//更新処理
void UpdateScene() {
	switch (g_eScene) {
	case SCENE_TITLE:		//タイトル画面
		UpdateSceneTitle();
		break;
	case SCENE_MAP:
		UpdateMapSelect();
		break;
	case SCENE_STAGE:
		UpdateStageSelect();
		break;
	case SCENE_GAME:		//ゲーム画面
		UpdateSceneGame();
		break;
	case SCENE_RESULT:	//リザルト画面
		//UpdateResult();
		break;
	case SCENE_SAMPLE:	//サンプルシーン
		UpdateSceneSample();
		break;
	}
	UpdateUserGuide();
	UpdateFade();
}

//描画処理
void DrawScene() {
	switch (g_eScene) {
	case SCENE_TITLE:		//タイトル画面
		DrawSceneTitle();
		break;
	case SCENE_STAGE:
		DrawStageSelect();
		break;
	case SCENE_GAME:		//ゲーム画面
		DrawSceneGame();
		break;
	case SCENE_RESULT:		//リザルト画面
		//DrawResult();
		break;
	case SCENE_SAMPLE:		//サンプルシーン
		DrawSceneSample();
		break;
	}
	DrawUserGuide();
	DrawFade();
}

//切り替え
void SetScene(EScene eScene) {
	//現在の画面を終了
	switch (g_eScene) {
		case SCENE_TITLE:			//タイトル画面
			UninitSceneTitle();
			break;
		case SCENE_STAGE:
			UninitStageSelect();
			break;
		case SCENE_GAME:			//ゲーム画面
			UninitSceneGame();
			break;
		case SCENE_RESULT:			//リザルト画面
			//UninitResult();
			break;
		case SCENE_SAMPLE:			//サンプルシーン
			UninitSceneSample();
			break;
	}

	//画面を入れ替え
	g_eScene = eScene;
	//次の画面を初期化
	switch (g_eScene) {
		case SCENE_TITLE:			//タイトル画面
			InitSceneTitle();
			break;
		case SCENE_STAGE:
			InitStageSelect();
			break;
		case SCENE_GAME:			//ゲーム画面
			InitSceneGame();
			break;
		case SCENE_RESULT:			//リザルト画面
			//InitResult();
			break;
		case SCENE_SAMPLE:			//サンプルシーン
			InitSceneSample();
			break;
	}
}

EScene GetScene() {
	return g_eScene;
}