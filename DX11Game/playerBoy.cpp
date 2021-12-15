//=============================================================================
//
// プレイヤー男の子 [playerBoy.cpp]
// 小楠裕子
//=============================================================================
#include "playerBoy.h"
#include "input.h"
#include "map.h"
#include "bsphere.h"
#include "explosion.h"

//*****列挙型*****
enum DIR { RIGHT, LEFT };

//*****定数定義*****
#define PLAYER_BOY_MODEL_PATH			"data/model/slime001.fbx"

#define	PLAYER_BOY_VALUE_MOVE	(0.15f)		// 移動速度
#define	PLAYER_BOY_RATE_MOVE		(0.20f)		// 移動慣性係数
#define	PLAYER_BOY_VALUE_ROTATE	(9.0f)		// 回転速度
#define	PLAYER_BOY_RATE_ROTATE	(0.20f)		// 回転慣性係数

#define PLAYER_BOY_COLLISION_SIZE_X		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Y		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Z		2.5f

#define PLAYER_BOY_COLLISION_SIZE_RAD	4.0f


//*****グローバル変数*****


//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Player_Boy::Player_Boy()

{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	m_pos = XMFLOAT3(-100.0f, -45.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, PLAYER_BOY_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
	}
	//境界球生成
	m_nSphere = CreateBSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), PLAYER_BOY_COLLISION_SIZE_RAD, m_mtxWorld);
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Player_Boy::~Player_Boy() {
	// モデルの解放
	m_model.Release();
	//境界球解放
	ReleaseBSphere(m_nSphere);
}
//==============================================================
//更新
//==============================================================
void Player_Boy::Update() {
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	XMFLOAT3 oldPos = m_pos;
	if (GetKeyPress(VK_LEFT)) {
		m_dir = LEFT;
			// 左移動
			m_move.x -= SinDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;

			m_rotDest.y = rotCamera.y + 90.0f;

	}else if (GetKeyPress(VK_RIGHT)) {
		m_dir = RIGHT;
			// 右移動
			m_move.x -= SinDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;
			//エフェクト
			StartExplosion(XMFLOAT3(m_pos.x, m_pos.y, m_pos.z), XMFLOAT2(30.0f, 30.0f));

			

			m_rotDest.y = rotCamera.y - 90.0f;
	}


	

	// 目的の角度までの差分
	float fDiffRotY = m_rotDest.y - m_rot.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	m_rot.y += fDiffRotY * PLAYER_BOY_RATE_ROTATE;
	if (m_rot.y >= 180.0f) {
		m_rot.y -= 360.0f;
	}
	if (m_rot.y < -180.0f) {
		m_rot.y += 360.0f;
	}

	// 位置移動
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;



	// 移動量に慣性をかける
	m_move.x += (0.0f - m_move.x) * PLAYER_BOY_RATE_MOVE;
	m_move.y += (0.0f - m_move.y) * PLAYER_BOY_RATE_MOVE;
	m_move.z += (0.0f - m_move.z) * PLAYER_BOY_RATE_MOVE;

	if (m_pos.x < -310.0f) {
		m_pos.x = -310.0f;
	}
	if (m_pos.x > 310.0f) {
		m_pos.x = 310.0f;
	}
	if (m_pos.z < -310.0f) {
		m_pos.z = -310.0f;
	}
	if (m_pos.z > 310.0f) {
		m_pos.z = 310.0f;
	}
	if (m_pos.y < -45.0f) {
		m_pos.y = -45.0f;
	}
	if (m_pos.y > 80.0f) {
		m_pos.y = 80.0f;
	}
	//当たり判定
	if (CollisionOldMap(XMFLOAT2(m_pos.x,m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory > 0) 
	{
		m_pos = oldPos;
	}
	//攻撃の当たり判定
	if (GetKeyPress(VK_SPACE))
	{
		/*仮*/int num = CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		GetBox()->Destroy(num);
		num = CollisionNowMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		GetBox()->Destroy(num);
	}


	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot.x),
		XMConvertToRadians(m_rot.y), XMConvertToRadians(m_rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);

	//境界球移動
	MoveBSphere(m_nSphere, m_mtxWorld);


}
//==============================================================
//描画
//==============================================================
void Player_Boy::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	m_model.Draw(pDC, m_mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	m_model.Draw(pDC, m_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
}

//==============================================================
//男の子座標取得
//==============================================================
XMFLOAT3 Player_Boy::GetBoyPos() {
	return m_pos;
}

//==============================================================
//男の子移動量取得
//==============================================================
XMFLOAT3 Player_Boy::GetBoyMove() {
	return m_move;
}