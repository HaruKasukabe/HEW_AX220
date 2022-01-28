//=============================================================================
//
// モデル処理 [model.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "model.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "bsphere.h"
#include "enemy.h"
#include "fire.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_PLANE			"data/model/hikouki_white.x"

#define	VALUE_MOVE_MODEL	(0.50f)		// 移動速度
#define	RATE_MOVE_MODEL		(0.20f)		// 移動慣性係数
#define	VALUE_ROTATE_MODEL	(9.0f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.20f)		// 回転慣性係数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model;		// モデル

static XMFLOAT3		g_posModel;		// 現在の位置
static XMFLOAT3		g_rotModel;		// 現在の向き
static XMFLOAT3		g_rotDestModel;	// 目的の向き
static XMFLOAT3		g_moveModel;	// 移動量

static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス

static int			g_nShadow;		// 丸影番号

static int			g_nBSphere;		//境界球番号

bool				g_MoveFlag;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	g_posModel = XMFLOAT3(0.0f, 20.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_PLANE)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
		return E_FAIL;
	}

	// 丸影の生成
	g_nShadow = CreateShadow(g_posModel, 12.0f);
	//境界球の生成
	//g_nBSphere = CreateBSphere(XMFLOAT3(0.0f,0.0f,0.0f), 5.0f, g_mtxWorld);

	g_MoveFlag = false;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	// 丸影の解放
	ReleaseShadow(g_nShadow);
	//境界球の解放
	ReleaseBSphere(g_nBSphere);
	// モデルの解放
	g_model.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	g_MoveFlag = false;
	if (GetKeyPress(VK_LEFT)) {
		g_MoveFlag = true;
		if (GetKeyPress(VK_UP)) {
			// 左前移動
			g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 135.0f;
		} else if (GetKeyPress(VK_DOWN)) {
			// 左後移動
			g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 45.0f;
		} else {
			// 左移動
			g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + 90.0f;
		}
	} else if (GetKeyPress(VK_RIGHT)) {
		g_MoveFlag = true;
		if (GetKeyPress(VK_UP)) {
			// 右前移動
			g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 135.0f;
		} else if (GetKeyPress(VK_DOWN)) {
			// 右後移動
			g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 45.0f;
		} else {
			// 右移動
			g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - 90.0f;
		}
	} else if (GetKeyPress(VK_UP)) {
		g_MoveFlag = true;
		// 前移動
		g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = 180.0f + rotCamera.y;
	} else if (GetKeyPress(VK_DOWN)) {
		g_MoveFlag = true;
		// 後移動
		g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = rotCamera.y;
	}

	if (GetKeyPress(VK_I)) {
		g_MoveFlag = true;
		g_moveModel.y += VALUE_MOVE_MODEL;
	}
	if (GetKeyPress(VK_K)) {
		g_MoveFlag = true;
		g_moveModel.y -= VALUE_MOVE_MODEL;
	}

	if (GetKeyPress(VK_LSHIFT)) {
		// 左回転
		g_rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (g_rotDestModel.y < -180.0f) {
			g_rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_RSHIFT)) {
		// 右回転
		g_rotDestModel.y += VALUE_ROTATE_MODEL;
		if (g_rotDestModel.y >= 180.0f) {
			g_rotDestModel.y -= 360.0f;
		}
	}

	// 目的の角度までの差分
	float fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	g_rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (g_rotModel.y >= 180.0f) {
		g_rotModel.y -= 360.0f;
	}
	if (g_rotModel.y < -180.0f) {
		g_rotModel.y += 360.0f;
	}

	// 位置移動
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	g_posModel.z += g_moveModel.z;

	

	// 移動量に慣性をかける
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_MODEL;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_MODEL;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_MODEL;

	if (g_posModel.x < -310.0f) {
		g_posModel.x = -310.0f;
	}
	if (g_posModel.x > 310.0f) {
		g_posModel.x = 310.0f;
	}
	if (g_posModel.z < -310.0f) {
		g_posModel.z = -310.0f;
	}
	if (g_posModel.z > 310.0f) {
		g_posModel.z = 310.0f;
	}
	if (g_posModel.y < 0.0f) {
		g_posModel.y = 0.0f;
	}
	if (g_posModel.y > 80.0f) {
		g_posModel.y = 80.0f;
	}

	if (GetKeyPress(VK_RETURN)) {
		// リセット
		g_posModel = XMFLOAT3(0.0f, 20.0f, 0.0f);
		g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
		XMConvertToRadians(g_rotModel.y), XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// 丸影の移動
	MoveShadow(g_nShadow, g_posModel);

	//境界球の移動
	MoveBSphere(g_nBSphere, g_mtxWorld);

	//敵との当たり判定
	CollisionEnemy(g_posModel, 5.0f);

	//弾発射
	if (GetKeyRepeat(VK_SPACE)) {
		FireBullet(g_posModel, XMFLOAT3(-g_mtxWorld._31, -g_mtxWorld._32, -g_mtxWorld._33));
	}

	if (g_MoveFlag) {
		StartFire(XMFLOAT3(0.0f, 0.0f, 8.0f), 10.0f, g_mtxWorld);
	}

	//PrintDebugProc("[ﾋｺｳｷ ｲﾁ : (%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	//PrintDebugProc("[ﾋｺｳｷ ﾑｷ : (%f) < ﾓｸﾃｷ ｲﾁ:(%f) >]\n", g_rotModel.y, g_rotDestModel.y);
	//PrintDebugProc("\n");

	PrintDebugProc("*** ﾋｺｳｷ ｿｳｻ ***\n");
	PrintDebugProc("ﾏｴ   ｲﾄﾞｳ : \x1e\n");//↑
	PrintDebugProc("ｳｼﾛ  ｲﾄﾞｳ : \x1f\n");//↓
	PrintDebugProc("ﾋﾀﾞﾘ ｲﾄﾞｳ : \x1d\n");//←
	PrintDebugProc("ﾐｷﾞ  ｲﾄﾞｳ : \x1c\n");//→
	PrintDebugProc("ｼﾞｮｳｼｮｳ   : I\n");
	PrintDebugProc("ｶｺｳ       : K\n");
	PrintDebugProc("ﾋﾀﾞﾘ ｾﾝｶｲ : LSHIFT\n");
	PrintDebugProc("ﾐｷﾞ  ｾﾝｶｲ : RSHIFT\n");
	PrintDebugProc("\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
}

//=============================================================================
// 位置取得
//=============================================================================
XMFLOAT3& GetModelPos()
{
	return g_posModel;
}
