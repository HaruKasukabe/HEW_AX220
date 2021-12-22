//画面遷移[scene.h]
#pragma once
#include "main.h"

//定数定義
enum EScene {
	SCENE_NONE = 0,		//未設定
	SCENE_TITLE,		//タイトル画面
	SCENE_GAME,			//ゲーム画面
	SCENE_RESULT,		//リザルト画面
	SCENE_SAMPLE,
	SCENE_MAP,
	SCENE_STAGE,
	
	MAX_SCENE			//シーン総数
};

//関数プロトタイプ
HRESULT InitScene();			//初期化
void UninitScene();				//終了処理
void UpdateScene();				//更新
void DrawScene();				//描画

void SetScene(EScene eScene);	//切り替え

EScene GetScene();				//シーン取得