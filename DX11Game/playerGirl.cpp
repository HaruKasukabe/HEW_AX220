//=============================================================================
//
// プレイヤー女の子 [playerGirl.cpp]
// 小楠裕子
//=============================================================================
#include "playerGirl.h"
#include "playerBoy.h"
#include "sceneGame.h"
#include "input.h"
#include "collision.h"
#include "map.h"

//*****定数定義*****
#define PLAYER_BOY_MODEL_PATH			"data/model/girl_walking.fbx"


#define	PLAYER_BOY_VALUE_MOVE	(0.05f)		// 移動速度
#define	PLAYER_BOY_RATE_MOVE	(0.20f)		// 移動慣性係数
#define	PLAYER_BOY_VALUE_ROTATE	(9.0f)		// 回転速度
#define	PLAYER_BOY_RATE_ROTATE	(0.20f)		// 回転慣性係数

#define PLAYER_BOY_COLLISION_SIZE_X		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Y		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Z		2.5f

#define PLAYER_BOY_COLLISION_SIZE_RAD	4.0f

#define GRAVITY_GIRL	(2.0f)	// 重力

//*****グローバル変数*****

XMFLOAT3 g_BoyPos; // 男の子の座標
XMFLOAT3 g_oldGirlPos; // 前の座標

//==============================================================
//ｺﾝｽﾄﾗｸﾀ
//==============================================================
Player_Girl::Player_Girl()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	m_pos = XMFLOAT3(-100.0f, -45.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BoyPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_oldGirlPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bLand = false;


	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, PLAYER_BOY_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
	}

	//m_nCube = GetCube()->Create(XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(5.0f,5.0f,5.0f),m_mtxWorld);
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Player_Girl::~Player_Girl() {
	// モデルの解放
	m_model.Release();
	//立方体解放
	//GetCube()->Release(m_nCube);
}
//==============================================================
//更新
//==============================================================
void Player_Girl::Update() {
	g_oldGirlPos = m_pos;

	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	// 男の子の座標を取得
	g_BoyPos = GetOld()->GetPlayerBoy()->GetBoyPos();

	// 右移動
	m_move.x -= SinDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;
	m_move.z -= CosDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;

	m_rotDest.y = rotCamera.y - 90.0f;

	// 重力
	m_move.y -= GRAVITY_GIRL;


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
		m_move.y = 0.0f;
		m_bLand = true;
	}
	if (m_pos.y > 80.0f) {
		m_pos.y = 80.0f;
	}

	// 当たり判定
	OBJECT_INFO collision = CollisionNowMap(XMFLOAT2(m_pos.x, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
	if (collision.m_nCategory > 0)
	{
		if (m_bLand == true && collision.m_bOnBox == true)
			m_pos.y = g_oldGirlPos.y;
		else if (m_bLand == true)
			m_pos.x = g_oldGirlPos.x;
	}
	//----地形との当たり判定----
	if (CheckField())
	{	//乗った場合の処理
		m_move.y = 0.0f;
		m_bLand = true;
	}
	else
	{
		if (m_bLand)
		{
			m_bLand = false;
		}
	}

	if (GetKeyPress(VK_RETURN)) {
		// リセット
		m_pos = XMFLOAT3(-100.0f, -45.0f, 0.0f);
		m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
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

	//立方体移動
	//GetCube()->Move(m_nCube,m_mtxWorld);
	/*テスト*/

	//当たり判定
}
//==============================================================
//描画
//==============================================================
void Player_Girl::Draw() {
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
//女の子の位置取得
//==============================================================
XMFLOAT3 Player_Girl::GetGirlPos()
{
	return m_pos;
}
XMFLOAT3 Player_Girl::GetGirlMove()
{
	return m_move;
}
//==============================================================
//女の子の位置設定
//==============================================================
void Player_Girl::SetGirlPos(XMFLOAT3 pos)
{
	if (m_pos.y < pos.y)	
	{
		m_move.y += GRAVITY_GIRL + 3.0f;
	}
	//m_pos = pos;
}

//==============================================================
//女の子の当たり判定
//==============================================================
bool Player_Girl::CheckField()
{
	Box* pBox = GetBox();
	OBJECT_INFO* pNowMap = GetMap(0);

	XMFLOAT3 boxPos;
	for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++, pNowMap++) {
		switch (pNowMap->m_nCategory) {
		case 0:
			break;
		case NORMAL:
			if (!pBox->GetState(pNowMap->m_nObject))
			{
				break;
			}
			boxPos = pBox->GetPos(pNowMap->m_nObject);
			if (m_pos.x <= boxPos.x - 8.0f) continue;
			if (boxPos.x + 8.0f <= m_pos.x) continue;

			if (m_pos.y >= boxPos.y + 18.0f && g_oldGirlPos.y <= boxPos.y + 18.0f)
			{
				m_pos.y = boxPos.y + 18.0f;
				return true;
			}
			else if (m_pos.y <= boxPos.y - 5.0f && g_oldGirlPos.y >= boxPos.y - 5.0f)
			{
				m_pos.y = boxPos.y - 5.0f;
				m_move.y = 0.0f;
			}
			break;
		case CARRY:
			if (!pBox->GetState(pNowMap->m_nObject))
			{
				break;
			}
			boxPos = pBox->GetPos(pNowMap->m_nObject);
			if (m_pos.x <= boxPos.x - 8.0f) continue;
			if (boxPos.x + 8.0f <= m_pos.x) continue;

			if (m_pos.y >= boxPos.y + 18.0f && g_oldGirlPos.y <= boxPos.y + 18.0f)
			{
				m_pos.y = boxPos.y + 18.0f;
				return true;
			}
			else if (m_pos.y <= boxPos.y - 5.0f && g_oldGirlPos.y >= boxPos.y - 5.0f)
			{
				m_pos.y = boxPos.y - 5.0f;
				m_move.y = 0.0f;
			}
			break;
		}
	}
}