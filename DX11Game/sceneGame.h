//===================================================
//		ゲームシーン[sceneGame.h]
//小楠裕子
//=====================================================
#pragma once
#include "main.h"
#include "old.h"
#include "now.h"

HRESULT InitSceneGame();
void UninitSceneGame();
void UpdateSceneGame();
void DrawSceneGame();

Old* GetOld();
Now* GetNow();