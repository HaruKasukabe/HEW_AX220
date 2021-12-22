//===================================================
//		マップセレクト[mapselect.cpp]
//菅野佑斗
//=====================================================
#include "mapselect.h"
#include "fade.h"
#include "debugproc.h"
#include "input.h"
#include "bg.h"
#include "Texture.h"
#include "polygon.h"


#define MAPS_POS_X		300.0f
#define MAPS_POS_Y		0.0f
#define MAPS_SIZE_X		SCREEN_WIDTH
#define MAPS_SIZE_Y		SCREEN_HEIGHT

// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;

int  g_map=1;


HRESULT InitMapSelect()
{
	HRESULT hr = MB_OK;
		ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	//hr = CreateTextureFromFile(pDevice, BG_TEXTURE_PATH_MAP, &g_pTexture);

	g_map = 1;

	return hr;
}
void UninitMapSelect()
{
	// 背景テクスチャ解放
	SAFE_RELEASE(g_pTexture);


}
void UpdateMapSelect()
{
	if (g_map<5 )
	{
		if (GetKeyTrigger(VK_UP))
		{
			g_map += 1;
		}
	}

	if(g_map > 1)
	{
		if (GetKeyTrigger(VK_DOWN))
		{
			g_map -= 1;
		}

	}

	if (GetKeyPress(VK_F3))
	{
		StartFadeOut(SCENE_STAGE);
	}

	if (GetKeyPress(VK_RETURN))
	{
		switch (g_map)
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


	PrintDebugProc("ﾏｯﾌﾟｾﾚｸﾄ:%d\n", g_map);
}
void DrawMapSelect()
{
//	ID3D11DeviceContext* pDC = GetDeviceContext();
	//SetPolygonSize(MAPS_SIZE_X, MAPS_SIZE_Y);
	//SetPolygonPos(MAPS_POS_X, MAPS_POS_Y);
	//SetPolygonTexture(g_pTexture);
	//SetPolygonUV(0.0f, 0.0f);
	//DrawPolygon(pDC);

}
