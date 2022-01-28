//===================================================
//		操作説明[UserGuide.cpp]
//武井遥都
//2022/1/25	ファイル作成
//=====================================================
#include "UserGuide.h"
#include "fade.h"
#include "bg.h"
#include "Texture.h"
#include "polygon.h"
#include "input.h"
#include "mesh.h"

// ***************************************
// マクロ定義
// ***************************************
#define KEYBOARD_TEXTURE_PATH L"data/texture/Guide_K.png"
#define CONTROLLER_TEXTURE_PATH L"data/texture/Guide_C.png"
#define USERGUIDE_POS XMFLOAT2(0.0f, 0.0f)
#define MAX_TEXTURE (2)

// ****************************************
// グローバル変数
// ****************************************
static MESH	g_UserGuideMesh;					// 構造体
static ID3D11ShaderResourceView* g_UserGuideTexture[MAX_TEXTURE];
static int g_UserPadTimer;
static bool bUserFlg; // 画面を表示しているか
static int g_DispNum; // 表示画面の数字

//=============================
//		初期化
//=============================
HRESULT InitUserGuide() {
	HRESULT hr = MB_OK;
	ID3D11Device* pDevice = GetDevice();

	bUserFlg = false;
	g_DispNum = 0;

	hr = CreateTextureFromFile(pDevice, KEYBOARD_TEXTURE_PATH, &g_UserGuideTexture[0]);
	if (FAILED(hr))
		return hr;
	hr = CreateTextureFromFile(pDevice, CONTROLLER_TEXTURE_PATH, &g_UserGuideTexture[1]);
	if (FAILED(hr))
		return hr;



	return hr;
}

//=============================
//		終了
//=============================
void UninitUserGuide() {
	// メッシュ開放
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		SAFE_RELEASE(g_UserGuideTexture[i]);
	}
	g_DispNum = 0;
}

//=============================
//		更新
//=============================
void UpdateUserGuide() {
	if (GetKeyPress(VK_X) || GetJoyButton(0, JOYBUTTON3))
	{
		// Xキーを押したら操作説明の画面を閉じる
		bUserFlg = false;
	}
	if (g_DispNum == 0 && (GetJoyDpadRight(0) || GetKeyPress(VK_RIGHT)))
	{
		// キーボード表示のとき右を押したらｺﾝﾄﾛｰﾗｰを表示
		g_DispNum = 1;
	}
	if (g_DispNum == 1 && (GetJoyDpadLeft(0) || GetKeyPress(VK_LEFT)))
	{
		// キーボード表示のとき右を押したらｺﾝﾄﾛｰﾗｰを表示
		g_DispNum = 0;
	}

	if (g_DispNum > 1 || g_DispNum < 0)
	{
		g_DispNum = 0;
	}
}

//=============================
//		描画
//=============================
void DrawUserGuide() {
	if (bUserFlg) {
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetBlendState(BS_ALPHABLEND);
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(USERGUIDE_POS.x,USERGUIDE_POS.y);
		SetPolygonAngle(0.0f);
		SetPolygonTexture(g_UserGuideTexture[g_DispNum]);
		DrawPolygon(pDC);
		SetBlendState(BS_NONE);
	}
}

//=============================
//	描画フラグを設定
//=============================
void SetUserGuideFlg(bool Flg)
{
	bUserFlg = Flg;
}

//=============================
//	描画フラグを渡す
//=============================
bool GetUserGuideFlg()
{
	return bUserFlg;
}