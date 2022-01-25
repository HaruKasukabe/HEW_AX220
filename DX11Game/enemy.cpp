//=============================================================================
//
// 敵機処理 [enemy.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "enemy.h"
#include "main.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bsphere.h"
#include "collision.h"
#include "explosion.h"
#include "sceneSample.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_ENEMY			"data/model/hikouki_green.x"

#define	VALUE_MOVE_ENEMY	(0.40f)		// 移動速度
#define	RATE_MOVE_ENEMY		(0.20f)		// 移動慣性係数
#define	VALUE_ROTATE_ENEMY	(7.0f)		// 回転速度
#define	RATE_ROTATE_ENEMY	(0.20f)		// 回転慣性係数

#define MAX_ENEMY			(10)		// 敵機最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TEnemy {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス

	int			m_nShadow;	// 丸影番号
	int			m_nBSphere;	//境界球番号

	bool		m_use;		//使用フラグ

	int m_nCube;
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model;			// モデル
static TEnemy		g_enemy[MAX_ENEMY];	// 敵機情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// 位置・回転・スケールの初期設定
		g_enemy[i].m_pos = XMFLOAT3(rand() % 620 - 310.0f,
									20.0f,
									rand() % 620 - 310.0f);
		g_enemy[i].m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_enemy[i].m_rotDest = g_enemy[i].m_rot;
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY);

		// 丸影の生成
		g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 12.0f);
		//境界球の生成
		//g_enemy[i].m_nBSphere = CreateBSphere(XMFLOAT3(0.0f,0.0f,0.0f), 5.0f, g_enemy[i].m_mtxWorld);

		//立方体の生成
		g_enemy[i].m_nCube = GetCube()->Create(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(5.0f, 5.0f, 5.0f), g_enemy[i].m_mtxWorld);

		g_enemy[i].m_use = true;
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		// 丸影の解放
		ReleaseShadow(g_enemy[i].m_nShadow);
		//境界球の解放
		ReleaseBSphere(g_enemy[i].m_nBSphere);

		//立方体の解放
		GetCube()->Release(g_enemy[i].m_nCube);
		
		g_enemy[i].m_use = false;
	}

	// モデルの解放
	g_model.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		// 移動
		g_enemy[i].m_pos.x += g_enemy[i].m_move.x;
		g_enemy[i].m_pos.y += g_enemy[i].m_move.y;
		g_enemy[i].m_pos.z += g_enemy[i].m_move.z;

		// 壁にぶつかった
		bool lr = false, fb = false;
		if (g_enemy[i].m_pos.x < -310.0f) {
			g_enemy[i].m_pos.x = -310.0f;
			lr = true;
		}
		if (g_enemy[i].m_pos.x > 310.0f) {
			g_enemy[i].m_pos.x = 310.0f;
			lr = true;
		}
		if (g_enemy[i].m_pos.z < -310.0f) {
			g_enemy[i].m_pos.z = -310.0f;
			fb = true;
		}
		if (g_enemy[i].m_pos.z > 310.0f) {
			g_enemy[i].m_pos.z = 310.0f;
			fb = true;
		}
		if (g_enemy[i].m_pos.y < 0.0f) {
			g_enemy[i].m_pos.y = 0.0f;
		}
		if (g_enemy[i].m_pos.y > 80.0f) {
			g_enemy[i].m_pos.y = 80.0f;
		}
		if (fabsf(g_enemy[i].m_rot.y - g_enemy[i].m_rotDest.y) < 0.0001f) {
			if (lr) {
				g_enemy[i].m_move.x *= -1.0f;
			}
			if (fb) {
				g_enemy[i].m_move.z *= -1.0f;
			}
			if (lr || fb) {
				g_enemy[i].m_rotDest.y = XMConvertToDegrees(atan2f(-g_enemy[i].m_move.x, -g_enemy[i].m_move.z));
			}
		}

		// 目的の角度までの差分
		float fDiffRotY = g_enemy[i].m_rotDest.y - g_enemy[i].m_rot.y;
		if (fDiffRotY >= 180.0f) {
			fDiffRotY -= 360.0f;
		}
		if (fDiffRotY < -180.0f) {
			fDiffRotY += 360.0f;
		}

		// 目的の角度まで慣性をかける
		g_enemy[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
		if (g_enemy[i].m_rot.y >= 180.0f) {
			g_enemy[i].m_rot.y -= 360.0f;
		}
		if (g_enemy[i].m_rot.y < -180.0f) {
			g_enemy[i].m_rot.y += 360.0f;
		}
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY);

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemy[i].m_rot.x),
			XMConvertToRadians(g_enemy[i].m_rot.y),
			XMConvertToRadians(g_enemy[i].m_rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(
			g_enemy[i].m_pos.x,
			g_enemy[i].m_pos.y,
			g_enemy[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_enemy[i].m_mtxWorld, mtxWorld);

		// 丸影の移動
		MoveShadow(g_enemy[i].m_nShadow, g_enemy[i].m_pos);
		//境界球の移動
		MoveBSphere(g_enemy[i].m_nBSphere, g_enemy[i].m_mtxWorld);

		//立方体の移動
		GetCube()->Move(g_enemy[i].m_nCube,g_enemy[i].m_mtxWorld);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
}

bool CollisionEnemy(XMFLOAT3 pos, float radius) {

	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		if (CollisionSphere(pos, radius, g_enemy[i].m_pos, 5.0f)) {
			StartExplosion(g_enemy[i].m_pos, XMFLOAT2(50.0f, 50.0f));
			ReleaseBSphere(g_enemy[i].m_nBSphere);
			ReleaseShadow(g_enemy[i].m_nShadow);
			g_enemy[i].m_use = false;
			return true;
		}
	}
	return false;
}

bool CollisionEnemy(XMFLOAT3 pos, XMFLOAT3 size) {
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		if (CollisionAABB(pos, size, g_enemy[i].m_pos, XMFLOAT3(2.5f,2.5f,2.5f))) {
			StartExplosion(g_enemy[i].m_pos, XMFLOAT2(50.0f, 50.0f));
			ReleaseBSphere(g_enemy[i].m_nBSphere);
			ReleaseShadow(g_enemy[i].m_nShadow);
			GetCube()->Release(g_enemy[i].m_nCube);
			g_enemy[i].m_use = false;
			return true;
		}
	}
	return false;
}
