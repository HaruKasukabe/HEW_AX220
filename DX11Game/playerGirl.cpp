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

//*****列挙型*****
enum GIRL_ANIM { STOP, WALK, JUMP, MAX_GIRL_ANIM, };

//*****定数定義*****
#define PLAYER_GIRL_STOP_MODEL_PATH			"data/model/girl_stopping.fbx"
#define PLAYER_GIRL_WALKING_MODEL_PATH		"data/model/girl_walking.fbx"
#define PLAYER_GIRL_JUMP_MODEL_PATH			"data/model/girl_jumping.fbx"

#define PLAYER_GIRL_JUMP_ANIM_TIME	(60)

#define	PLAYER_GIRL_VALUE_MOVE	(0.05f)		// 移動速度
#define	PLAYER_GIRL_RATE_MOVE	(0.20f)		// 移動慣性係数
#define	PLAYER_GIRL_VALUE_ROTATE	(9.0f)		// 回転速度
#define	PLAYER_GIRL_RATE_ROTATE	(0.20f)		// 回転慣性係数

#define PLAYER_GIRL_COLLISION_SIZE_X		4.0f
#define PLAYER_GIRL_COLLISION_SIZE_Y		4.0f
#define PLAYER_GIRL_COLLISION_SIZE_Z		2.5f

#define PLAYER_GIRL_COLLISION_SIZE_RAD	4.0f

#define GRAVITY_GIRL	(2.0f)	// 重力

//*****構造体*****
static std::string g_GirlAnimFile[] = {
	PLAYER_GIRL_STOP_MODEL_PATH,
	PLAYER_GIRL_WALKING_MODEL_PATH,
	PLAYER_GIRL_JUMP_MODEL_PATH,
};

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
	m_scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_oldGirlPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bLand = false;


	// モデルデータの読み込み
	if (!m_model.Load(pDevice, pDeviceContext, PLAYER_GIRL_STOP_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
	}
	for (int i = 0; i < MAX_GIRL_ANIM; i++)
		m_modelSub[i].Load(pDevice, pDeviceContext, g_GirlAnimFile[i]);

	//m_nCube = GetCube()->Create(XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(5.0f,5.0f,5.0f),m_mtxWorld);
}
//==============================================================
//ﾃﾞｽﾄﾗｸﾀ
//==============================================================
Player_Girl::~Player_Girl() {
	// モデルの解放
	SetAnim(STOP);
	m_model.Release();
	for (int i = 1; i < MAX_GIRL_ANIM; i++)
		m_modelSub[i].Release();
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
	m_nAnim = WALK;
	m_move.x -= SinDeg(rotCamera.y - 90.0f) * PLAYER_GIRL_VALUE_MOVE;
	m_move.z -= CosDeg(rotCamera.y - 90.0f) * PLAYER_GIRL_VALUE_MOVE;


	m_rotDest.y = rotCamera.y - 90.0f;

	// 重力
	m_move.y -= GRAVITY_GIRL;
	if (!m_bLand)
	{
		m_move.x *= 0.9f;
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
	m_rot.y += fDiffRotY * PLAYER_GIRL_RATE_ROTATE;
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
	m_move.x += (0.0f - m_move.x) * PLAYER_GIRL_RATE_MOVE;
	m_move.y += (0.0f - m_move.y) * PLAYER_GIRL_RATE_MOVE;
	m_move.z += (0.0f - m_move.z) * PLAYER_GIRL_RATE_MOVE;

	if (m_pos.x < -310.0f) {
		m_pos.x = -310.0f;
	}
	if (m_pos.x > MAP_END) {
		m_pos.x = MAP_END;
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
	std::vector<OBJECT_INFO> collision = WalkCollisionNowMap(XMFLOAT2(m_pos.x, m_pos.y), XMFLOAT2(PLAYER_GIRL_COLLISION_SIZE_X, PLAYER_GIRL_COLLISION_SIZE_Y));
	std::vector<OBJECT_INFO>::iterator it = collision.begin();
	while (it != collision.end())
	{
		//if (it->m_nCategory == 3)
		//{
		//	m_pos.y += 15.0f;
		//}
		if (it->m_bOnBox == true)
		{
			m_nAnim = WALK;
			m_bLand = true;
			m_pos.y = g_oldGirlPos.y;
		}
		else
		{
			m_nAnim = STOP;
			m_pos.x = g_oldGirlPos.x;
		}
		it++;
	}

	if (GetKeyPress(VK_RETURN)) {
		// リセット
		m_pos = XMFLOAT3(-100.0f, -45.0f, 0.0f);
		m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// アニメーション更新
	SetAnim(m_nAnim);

	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	mtxScl = XMMatrixScaling(m_scl.x, m_scl.y, m_scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

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
	DWBox* pDWBox = GetDWBox();
	OBJECT_INFO* pNowMap = GetMap(0);

	XMFLOAT3 boxPos;
	for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++, pNowMap++) {
		switch (pNowMap->m_nCategory) {
		case 0:
			break;
		case NORMAL:
			if (!pDWBox->GetState(pNowMap->m_nObject))
			{
				break;
			}
			boxPos = pDWBox->GetPos(pNowMap->m_nObject);
			if (m_pos.x <= boxPos.x - 2.0f) continue;
			if (boxPos.x + 2.0f <= m_pos.x) continue;

			if (m_pos.y >= boxPos.y + 6.0f && g_oldGirlPos.y <= boxPos.y + 6.0f)
			{
				m_pos.y = boxPos.y + 18.0f;
				return true;
			}
			break;
		default:
			if (!pBox->GetState(pNowMap->m_nObject))
			{
				break;
			}
			boxPos = pBox->GetPos(pNowMap->m_nObject);
			if (m_pos.x <= boxPos.x - 2.0f) continue;
			if (boxPos.x + 2.0f <= m_pos.x) continue;

			if (m_pos.y >= boxPos.y + 6.0f && g_oldGirlPos.y <= boxPos.y + 6.0f)
			{
				m_pos.y = boxPos.y + 18.0f;
				return true;
			}
			break;
		}
	}
}

//==============================================================
//女の子のアニメーション設定
//==============================================================
void Player_Girl::SetAnim(int nAnim)
{
	switch (nAnim)
	{
	case STOP:
		if (m_nAnimTime <= 0)
		{
			m_model = m_modelSub[nAnim];
			m_nAnimNow = STOP;
		}
		break;
	case WALK:
		if (m_nAnimTime <= 0)
		{
			m_model = m_modelSub[nAnim];
			m_nAnimNow = WALK;
		}
		break;
	case JUMP:
		m_model = m_modelSub[nAnim];
		m_nAnimTime = PLAYER_GIRL_JUMP_ANIM_TIME;
		m_nAnimNow = JUMP;
		break;
	}
}