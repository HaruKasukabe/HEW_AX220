//=============================================================================
//
// プレイヤー男の子 [playerBoy.cpp]
// 小楠裕子
//=============================================================================
#include "playerBoy.h"
#include "input.h"
#include "map.h"
#include "bsphere.h"


//*****列挙型*****
enum DIR { RIGHT, LEFT };

//*****定数定義*****
#define PLAYER_BOY_MODEL_PATH			"data/model/boy_idol.fbx"


#define	PLAYER_BOY_VALUE_MOVE	(0.15f)		// 移動速度
#define	PLAYER_BOY_RATE_MOVE		(0.20f)		// 移動慣性係数
#define	PLAYER_BOY_VALUE_ROTATE	(9.0f)		// 回転速度
#define	PLAYER_BOY_RATE_ROTATE	(0.20f)		// 回転慣性係数

#define PLAYER_BOY_COLLISION_SIZE_X		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Y		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Z		2.5f

#define PLAYER_BOY_COLLISION_SIZE_RAD	4.0f

#define JUMP_POWER		(12.0f)
#define JUMP_WHILE		(15)
#define GRAVITY_BOY		(1.0f)	// 重力
#define RESIST_X		(0.7f)

//*****グローバル変数*****
XMFLOAT3 g_oldBoyPos;
static int g_nowHand;
static int timeJudge; // 0:過去,1:未来
static int g_nJumpCnt;

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
	m_bJump = false;
	m_bLand = false;
	m_nHand = 9999;
	g_nowHand = 9999;
	timeJudge = 0;
	g_nJumpCnt = 0;

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
	g_oldBoyPos = m_pos;
	g_nJumpCnt--;

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

			m_rotDest.y = rotCamera.y - 90.0f;
	}
	if (GetKeyTrigger(VK_UP))
	{
		// ジャンプ
		if (g_nJumpCnt < 0)
		{
			m_move.y += JUMP_POWER;
			m_bJump = true;
			g_nJumpCnt = JUMP_WHILE;
		}
	}

	// 重力
	m_move.y -= GRAVITY_BOY;
	if (m_bJump)
	{
		m_move.x *= RESIST_X;
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
		m_move.y = 0.0f;
		m_bJump = false;
		m_bLand = true;
	}
	if (m_pos.y > 80.0f) {
		m_pos.y = 80.0f;
	}

	// 当たり判定
	OBJECT_INFO collision = CollisionOldMap(XMFLOAT2(m_pos.x, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
	if (collision.m_nCategory > 0)
	{
		if (m_bLand == true && collision.m_bOnBox == true)
			m_pos.y = g_oldBoyPos.y;
		else if (m_bLand == true)
			m_pos.x = g_oldBoyPos.x;
	}
	//----地形との当たり判定----
	if (CheckField())
	{	//乗った場合の処理
		m_move.y = 0.0f;
		m_bJump = false;
		m_bLand = true;
	}
	else
	{
		if (!m_bLand)
		{
			m_bJump = true;
			m_bLand = false;
		}
	}

	//攻撃の当たり判定
	if (GetKeyPress(VK_SPACE)||GetKeyPress(JOYSTICKID1))
	{
		/*仮*/OBJECT_INFO object = CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
		if(object.m_nCategory == BREAK)
			GetBox()->Destroy(object.m_nObject);
		object = CollisionNowMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
		if (object.m_nCategory == BREAK)
			GetBox()->Destroy(object.m_nObject);
	}



	// オブジェクトを持つ
	if (GetKeyPress(VK_A))
	{
		if (CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory == CARRY) {
			m_nHand = CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
			g_nowHand = CollisionNowMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		}
		if (CollisionOldMap(XMFLOAT2(m_pos.x - 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory == CARRY) {
			m_nHand = CollisionOldMap(XMFLOAT2(m_pos.x - 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
			g_nowHand = CollisionNowMap(XMFLOAT2(m_pos.x - 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		}
	}
	// オブジェクトを放す
	if (GetKeyPress(VK_S))
	{
		m_nHand = 9999;
		GetBox()->SetOldBoxPos(g_nowHand);
		g_nowHand = 9999;
	}

	// 持ち物を一緒に移動
	GetBox()->SetBoxPos(m_nHand, m_move, 0);   // 過去の座標を反映
	GetBox()->SetBoxPos(g_nowHand, m_move, 1); // 未来の座標を一時保存


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

//==============================================================
//男の子の当たり判定
//==============================================================
bool Player_Boy::CheckField()
{
	Box* pBox = GetBox();
	OBJECT_INFO* pOldMap = GetMap(1);

	XMFLOAT3 boxPos;
	for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++, pOldMap++) {
		switch (pOldMap->m_nCategory) {
		case 0:
			break;
		default:
			if (!pBox->GetState(pOldMap->m_nObject))
			{
				break;
			}
			boxPos = pBox->GetPos(pOldMap->m_nObject);
			if (m_pos.x <= boxPos.x - 6.0f) continue;
			if (boxPos.x + 6.0f <= m_pos.x) continue;

			if (m_pos.y >= boxPos.y + 6.0f && g_oldBoyPos.y <= boxPos.y + 6.0f)
			{
				m_pos.y = boxPos.y + 18.0f;
				return true;
			}
			else if (m_pos.y <= boxPos.y - 5.0f && g_oldBoyPos.y >= boxPos.y - 5.0f)
			{
				m_pos.y = boxPos.y - 5.0f;
				m_move.y = 0.0f;
				return false;
			}
			break;
		}
	}
}