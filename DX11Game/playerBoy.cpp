//=============================================================================
//
// �v���C���[�j�̎q [playerBoy.cpp]
// ����T�q
//=============================================================================
#include "playerBoy.h"
#include "input.h"
#include "bsphere.h"
#include "DWBox.h"
#include "shadow.h"
#include "Sound.h"
#include "effect.h"
#include "break.h"
#include "torch.h"

//*****�񋓌^*****
enum DIR { RIGHT, LEFT };

enum BOY_ANIM { STOP, WALK, JUMP, PUSH_BOY, CARRY_BOY, UP, DOWN, MAX_BOY_ANIM, };

//*****�萔��`*****
#define PLAYER_BOY_STOP_MODEL_PATH			"data/model/boy_stopping.fbx"
#define PLAYER_BOY_WALKING_MODEL_PATH		"data/model/boy_walking.fbx"
#define PLAYER_BOY_JUMP_MODEL_PATH			"data/model/boy_jumping.fbx"
#define PLAYER_BOY_PUSH_MODEL_PATH			"data/model/boy_pushing.fbx"
#define PLAYER_BOY_CARRY_MODEL_PATH			"data/model/boy_carrying.fbx"
#define PLAYER_BOY_UP_MODEL_PATH			"data/model/boy_up.fbx"
#define PLAYER_BOY_DOWN_MODEL_PATH			"data/model/boy_down.fbx"

#define PLAYER_BOY_JUMP_ANIM_TIME			(60)
#define PLAYER_BOY_UP_ANIM_TIME				(60)
#define PLAYER_BOY_DOWN_ANIM_TIME			(60)

#define	PLAYER_BOY_VALUE_MOVE	(0.10f)		// �ړ����x
#define	PLAYER_BOY_RATE_MOVE		(0.20f)		// �ړ������W��
#define	PLAYER_BOY_VALUE_ROTATE	(9.0f)		// ��]���x
#define	PLAYER_BOY_RATE_ROTATE	(0.20f)		// ��]�����W��

#define PLAYER_BOY_COLLISION_SIZE_X		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Y		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Z		2.5f

#define PLAYER_BOY_COLLISION_SIZE_RAD	4.0f

#define JUMP_POWER		(1.4f)
#define JUMP_WHILE		(26)
#define JUMP_TIME		(50)
#define GRAVITY_BOY		(1.0f)	// �d��
#define RESIST_X		(0.85f)

#define UP_TIME			(1.0f)

//*****�\����*****
static std::string g_BoyAnimFile[] = {
	PLAYER_BOY_STOP_MODEL_PATH,
	PLAYER_BOY_WALKING_MODEL_PATH,
	PLAYER_BOY_JUMP_MODEL_PATH,
	PLAYER_BOY_PUSH_MODEL_PATH,	
	PLAYER_BOY_CARRY_MODEL_PATH,
	PLAYER_BOY_UP_MODEL_PATH,	
	PLAYER_BOY_DOWN_MODEL_PATH,
};

//*****�O���[�o���ϐ�*****
XMFLOAT3 g_oldBoyPos;
static int g_nowHand;
static int timeJudge; // 0:�ߋ�,1:����
static int g_nJumpCnt;
static int g_nNoJumpTime;
float g_fUpTime;
static Effect* g_effect;
static Break* g_break;
static Torch* g_torch;

//==============================================================
//�ݽ�׸�
//==============================================================
Player_Boy::Player_Boy()

{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_pos = XMFLOAT3(-90.0f, -45.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bJump = false;
	m_bLand = false;
	m_bHave = false;
	m_collision = false;
	m_nHand = 9999;
	g_nowHand = 9999;
	timeJudge = 0;
	g_nJumpCnt = -1;
	g_fUpTime = 0.0f;

	m_pad = GetJoyState(0);

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, PLAYER_BOY_STOP_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}
	for(int i = 0; i < MAX_BOY_ANIM; i++)
		m_modelSub[i].Load(pDevice, pDeviceContext, g_BoyAnimFile[i]);

	//���E������
	m_nSphere = CreateBSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), PLAYER_BOY_COLLISION_SIZE_RAD, m_mtxWorld);

	// �ۉe�̐���
	m_nShadow = CreateShadow(m_pos, 16.0f);
}
//==============================================================
//�޽�׸�
//==============================================================
Player_Boy::~Player_Boy() {
	// ���f���̉��
	//SetAnim(STOP);
	//m_model.Release();
	for (int i = 0; i < MAX_BOY_ANIM; i++)
		m_modelSub[i].Release();
	//���E�����
	ReleaseBSphere(m_nSphere);
	//�ۉe�J��
	ReleaseShadow(m_nShadow);
}
//==============================================================
//�X�V
//==============================================================
void Player_Boy::Update() {
	g_oldBoyPos = m_pos;
	m_nAnimTime--;
	g_nNoJumpTime--;
	//g_nJumpCnt--;

	m_pad = GetJoyState(0);

	// �W�����v�̐ݒ�
	if (g_nJumpCnt > 0)
	{
		OBJECT_INFO hantei = CollisionOldMap(XMFLOAT2(m_pos.x, m_pos.y + 10.0f), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
		if (hantei.m_nCategory > 0)
		{
			m_pos.y -= 2.0f;
			g_nJumpCnt = -1;
			m_bJump = false;
			return;
		}
		else
		{
			m_move.y += JUMP_POWER;
			m_bJump = true;
			m_bLand = false;
		}

		g_nJumpCnt--;
	}

	// �ҋ@�A�j���[�V�����ɐݒ�
	m_nAnim = STOP;

	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	XMFLOAT3 oldPos = m_pos;
	if (GetKeyPress(VK_LEFT)|| (GetJoyDpadLeft(0)/*��*/)){
		m_dir = LEFT;
			// ���ړ�
			m_nAnim = WALK;
			if (m_bLand == true)
				CSound::Play(BGM_WALK);
			m_move.x -= SinDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;

			m_rotDest.y = rotCamera.y + 90.0f;
			g_torch->StartTorch(XMFLOAT3(m_pos.x, m_pos.y + 30, m_pos.z), XMFLOAT2(50.0f, 50.0f));

	}else if (GetKeyPress(VK_RIGHT) || GetJoyDpadRight(0)/*�E*/) {
		m_dir = RIGHT;
			// �E�ړ�
			m_nAnim = WALK;
			if(m_bLand == true)
				CSound::Play(BGM_WALK);
			m_move.x -= SinDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;

			m_rotDest.y = rotCamera.y - 90.0f;
	}
	if (GetKeyTrigger(VK_UP) || (GetJoyDpadUp(0) && GetJoyCount() != 0))	//��Ƀp�b�h�̏���͂������Ă��邩��
	{
		// �W�����v
		if (m_bJump == false)
		{
			m_nAnim = JUMP;
			CSound::Stop(BGM_WALK);
			CSound::Play(SE_JUMP);
			m_move.y += GRAVITY_BOY * 3;
			m_bJump = true;
			m_bLand = false;
			g_nJumpCnt = JUMP_WHILE;
			g_nNoJumpTime = JUMP_TIME;
		}
	}

	if (m_nAnim == STOP)
		CSound::Stop(BGM_WALK);

	// �d��
	m_move.y -= GRAVITY_BOY;
	if (m_bJump)
	{
		m_move.x *= RESIST_X;
	}

	

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = m_rotDest.y - m_rot.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	m_rot.y += fDiffRotY * PLAYER_BOY_RATE_ROTATE;
	if (m_rot.y >= 180.0f) {
		m_rot.y -= 360.0f;
	}
	if (m_rot.y < -180.0f) {
		m_rot.y += 360.0f;
	}

	// �ʒu�ړ�
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	// �ړ��ʂɊ�����������
	m_move.x += (0.0f - m_move.x) * PLAYER_BOY_RATE_MOVE;
	m_move.y += (0.0f - m_move.y) * PLAYER_BOY_RATE_MOVE;
	m_move.z += (0.0f - m_move.z) * PLAYER_BOY_RATE_MOVE;

	if (m_pos.x < -100.0f) {
		m_pos.x = -100.0f;
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
		m_bJump = false;
		m_bLand = true;
	}
	if (m_pos.y > 80.0f) {
		m_pos.y = 80.0f;
	}

	// �����蔻��
	std::vector<OBJECT_INFO> collision = WalkCollisionOldMap(XMFLOAT2(m_pos.x, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
	std::vector<OBJECT_INFO>::iterator it = collision.begin();
	while (it != collision.end())
	{
		if (it->m_nCategory > 0)
		{
			if (it->m_bOnBox == true)
			{
				if (!m_bLand)
					m_pos.x = g_oldBoyPos.x;
				m_bLand = true;
				m_bJump = false;
				m_pos.y = g_oldBoyPos.y;
				m_move.y = 0.0f;
			}
			else
			{
				CollisionSide(it);
			}
		}

		it++;
	}

	//�U���̓����蔻��
	if (GetKeyPress(VK_SPACE)|| GetJoyTrigger(0, JOYBUTTON2))
	{
     		/*��*/OBJECT_INFO object = CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
		if(object.m_nCategory == BREAK)
		{
			CSound::Play(SE_BREAK);
			GetWoodBox()->Destroy(object.m_nObject);
		}
		object = CollisionNowMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
		if (object.m_nCategory == BREAK)
			GetWoodBox()->Destroy(object.m_nObject);
		g_break->StartBreak(XMFLOAT3(m_pos.x + 8, m_pos.y + 4, m_pos.z), XMFLOAT2(20.0f, 20.0f));
	}



	// �I�u�W�F�N�g������
	if ((GetKeyPress(VK_A) || GetJoyTrigger(0, JOYBUTTON1)) && !m_bHave)
	{
		if (CollisionOldMap(XMFLOAT2(m_pos.x + 0.1f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory == CARRY) {
			m_nAnim = UP;
			CSound::Play(SE_UP);
			m_nAnimTime = PLAYER_BOY_UP_ANIM_TIME;
			m_nHand = CollisionOldMap(XMFLOAT2(m_pos.x + 0.1f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
			g_nowHand = CollisionNowMap(XMFLOAT2(m_pos.x + 0.1f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
			g_effect->StartEffect(XMFLOAT3(m_pos.x + 8, m_pos.y + 4, m_pos.z), XMFLOAT2(50.0f, 50.0f));
			m_bHave = true;
			g_torch->StartTorch(XMFLOAT3(m_pos.x, m_pos.y + 30, m_pos.z), XMFLOAT2(50.0f, 50.0f));
		}
		if (CollisionOldMap(XMFLOAT2(m_pos.x - 0.1f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory == CARRY) {
			m_nAnim = UP;
			CSound::Play(SE_UP);
			m_nAnimTime = PLAYER_BOY_UP_ANIM_TIME;
			m_nHand = CollisionOldMap(XMFLOAT2(m_pos.x - 0.1f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
			g_nowHand = CollisionNowMap(XMFLOAT2(m_pos.x - 0.1f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
			g_effect->StartEffect(XMFLOAT3(m_pos.x + 8, m_pos.y + 4, m_pos.z), XMFLOAT2(50.0f, 50.0f));
			m_bHave = true;
		}
	}
	// �I�u�W�F�N�g�����
	if ((GetKeyPress(VK_S) || GetJoyTrigger(0, JOYBUTTON1)) &&m_bHave)
	{
		m_nAnim = DOWN;
		CSound::Play(SE_DOWN);

		if (GetObjectInfo(g_nowHand, CARRY).m_nCategory == CARRY)
		{
			XMFLOAT3 aPos = GetBox()->GetPos(m_nHand);
			XMFLOAT2 aSize = GetBox()->GetSize();
			float fWidth;
			if (m_dir == RIGHT)
				fWidth = 5.0f;
			else if (m_dir == LEFT)
				fWidth = -5.0f;

			bool aFlg = GetDWBox()->Collision(XMFLOAT2(aPos.x + fWidth, aPos.y), aSize);
			if (aFlg) {
				XMFLOAT3 bPos = GetBox()->GetPos(g_nowHand);
				SetObject(g_nowHand, GetHalfBox()->CreateOldNow(XMFLOAT3(aPos.x - fWidth, bPos.y, aPos.z), 1));
				GetBox()->Destroy(g_nowHand);
			}
		}

		m_nHand = 9999;
		if (GetObjectInfo(g_nowHand, CARRY).m_nCategory == CARRY)
			GetBox()->SetOldBoxPos(g_nowHand);
		else
			GetHalfBox()->SetOldHalfBoxPos(g_nowHand);
		g_nowHand = 9999;
		m_bHave = false;
		g_fUpTime = 0.0f;
	}

	// ���������ꏏ�Ɉړ�
	if (m_nHand != 9999)
	{
		if (g_fUpTime < 20.0f)
			g_fUpTime += UP_TIME;
		
		m_move.y += g_fUpTime;
		GetBox()->SetBoxPos(m_nHand, m_pos, m_move, 0);   // �ߋ��̍��W�𔽉f
		m_move.y -= g_fUpTime;

		if (g_nowHand != 9999)
		{
			if (GetObjectInfo(g_nowHand, CARRY).m_nCategory == CARRY)
				GetBox()->SetBoxPos(g_nowHand, m_pos, m_move, 1); // �����̍��W���ꎞ�ۑ�
			else
				GetHalfBox()->SetHalfBoxPos(g_nowHand, m_pos, m_move, 1);
		}
	}

	// �A�j���[�V�����X�V
	SetAnim(m_nAnim);

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot.x),
		XMConvertToRadians(m_rot.y), XMConvertToRadians(m_rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);

	//���E���ړ�
	MoveBSphere(m_nSphere, m_mtxWorld);

	//�ۉe�ړ�
	MoveShadow(m_nShadow, m_pos);

	//PrintDebugProc("��x:%2fy:%2fz:%2f", m_pos.x, m_pos.y, m_pos.z);
}
//==============================================================
//�`��
//==============================================================
void Player_Boy::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	m_model.Draw(pDC, m_mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	m_model.Draw(pDC, m_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//==============================================================
//�j�̎q���W�擾
//==============================================================
XMFLOAT3 Player_Boy::GetBoyPos() {
	return m_pos;
}

//==============================================================
//�j�̎q�ړ��ʎ擾
//==============================================================
XMFLOAT3 Player_Boy::GetBoyMove() {
	return m_move;
}

//==============================================================
//�j�̎q�̎������擾
//==============================================================
int Player_Boy::GetBoyHand() {
	return m_nHand;
}

//==============================================================
//�j�̎q�̃A�j���[�V�����ݒ�
//==============================================================
void Player_Boy::SetAnim(int nAnim)
{
	if (m_bHave && m_nAnimNow == CARRY_BOY)
		if (nAnim != DOWN)
			return;

	if (m_nAnimNow == UP)
		if (m_nAnimTime <= 0)
			nAnim = CARRY_BOY;

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
		m_nAnimTime = PLAYER_BOY_JUMP_ANIM_TIME;
		m_nAnimNow = JUMP;
		break;
	case PUSH_BOY:
		m_model = m_modelSub[nAnim];
		m_nAnimNow = PUSH_BOY;
		break;
	case CARRY_BOY:
		m_model = m_modelSub[nAnim];
		m_nAnimNow = CARRY_BOY;
		break;
	case UP:
		m_model = m_modelSub[nAnim];
		m_nAnimNow = UP;
		break;
	case DOWN:
		m_model = m_modelSub[nAnim];
		m_nAnimTime = PLAYER_BOY_DOWN_ANIM_TIME;
		m_nAnimNow = DOWN;
		break;
	}
}
// =========================================================
// �j�̎q�����̂������Ă��邩�̃t���O��n��
// =========================================================
bool Player_Boy::GetHaveFlg() {
	return m_bHave;
}

//==============================================================
//�j�̎q�̉��̓����蔻��
//==============================================================
void Player_Boy::CollisionSide(std::vector<OBJECT_INFO>::iterator it)
{
	switch (it->m_nCategory)
	{
	case NORMAL:
		if (GetDWBox()->GetPos(it->m_nObject).x > m_pos.x)
			m_pos.x = g_oldBoyPos.x - COLLISION_SIDE_LONG;
		else if (GetDWBox()->GetPos(it->m_nObject).x < m_pos.x)
			m_pos.x = g_oldBoyPos.x + COLLISION_SIDE_LONG;
	case BREAK:
		if (GetWoodBox()->GetPos(it->m_nObject).x > m_pos.x)
			m_pos.x = g_oldBoyPos.x - COLLISION_SIDE_LONG;
		else if (GetWoodBox()->GetPos(it->m_nObject).x < m_pos.x)
			m_pos.x = g_oldBoyPos.x + COLLISION_SIDE_LONG;
	case CARRY:
		if (GetBox()->GetPos(it->m_nObject).x > m_pos.x)
			m_pos.x = g_oldBoyPos.x - COLLISION_SIDE_LONG;
		else if (GetBox()->GetPos(it->m_nObject).x < m_pos.x)
			m_pos.x = g_oldBoyPos.x + COLLISION_SIDE_LONG;
	}
}