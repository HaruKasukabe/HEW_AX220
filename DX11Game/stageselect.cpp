//===================================================
//		ステージセレクト[stageselect.cpp]
//菅野佑斗
//=====================================================
#include "stageselect.h"
#include "fade.h"
#include "input.h"
#include "debugproc.h"

int g_stage=1;

HRESULT InitStageSelect()
{
	HRESULT hr = MB_OK;

	g_stage = 1;

	return hr;
}
void UninitStageSelect()
{

}
void UpdateStageSelect()
{
	if (g_stage < 5)
	{
		if (GetKeyTrigger(VK_UP))
		{
			g_stage += 1;
		}
	}
	if(g_stage > 1)
	{
		if (GetKeyTrigger(VK_DOWN))
		{
			g_stage -= 1;
		}

	}
	if (GetKeyPress(VK_RETURN))
	{
		switch (g_stage)
		{
		case 1:
			StartFadeOut(SCENE_TITLE);
			break;
		case 2:
			StartFadeOut(SCENE_STAGE);
			break;
		case 3:
			StartFadeOut(SCENE_GAME);
			break;
		case 4:
			StartFadeOut(SCENE_RESULT);
			break;
		case 5:
			StartFadeOut(SCENE_SAMPLE);
			break;

		default:
			break;
		};
	}


	

	PrintDebugProc("ｽﾃｰｼﾞｾﾚｸﾄ:%d\n", g_stage);

}
void DrawStageSelect()
{

}
