//=============================================================================
//
// �v���C���[�j�̎q [playerBoy.cpp]
// ����T�q
//=============================================================================
#include "playerBoy.h"
#include "input.h"
#include "map.h"
#include "bsphere.h"


//*****�񋓌^*****
enum DIR { RIGHT, LEFT };

//*****�萔��`*****
#define PLAYER_BOY_MODEL_PATH			"data/model/slime001.fbx"

#define	PLAYER_BOY_VALUE_MOVE	(0.15f)		// �ړ����x
#define	PLAYER_BOY_RATE_MOVE		(0.20f)		// �ړ������W��
#define	PLAYER_BOY_VALUE_ROTATE	(9.0f)		// ��]���x
#define	PLAYER_BOY_RATE_ROTATE	(0.20f)		// ��]�����W��

#define PLAYER_BOY_COLLISION_SIZE_X		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Y		4.0f
#define PLAYER_BOY_COLLISION_SIZE_Z		2.5f

#define PLAYER_BOY_COLLISION_SIZE_RAD	4.0f

#define JUMP_POWER		(23.0f)
#define GRAVITY_BOY		(2.0f)	// �d��
#define RESIST_X		(0.7f)

//*****�O���[�o���ϐ�*****
XMFLOAT3 g_oldBoyPos;

//==============================================================
//�ݽ�׸�
//==============================================================
Player_Boy::Player_Boy()

{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_pos = XMFLOAT3(-100.0f, -45.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bJump = false;
	m_bLand = false;
	m_nHund = 9999;

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, PLAYER_BOY_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}
	//���E������
	m_nSphere = CreateBSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), PLAYER_BOY_COLLISION_SIZE_RAD, m_mtxWorld);
}
//==============================================================
//�޽�׸�
//==============================================================
Player_Boy::~Player_Boy() {
	// ���f���̉��
	m_model.Release();
	//���E�����
	ReleaseBSphere(m_nSphere);
}
//==============================================================
//�X�V
//==============================================================
void Player_Boy::Update() {
	g_oldBoyPos = m_pos;

	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (GetKeyPress(VK_LEFT)) {
		m_dir = LEFT;
			// ���ړ�
			m_move.x -= SinDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;

			m_rotDest.y = rotCamera.y + 90.0f;

	}else if (GetKeyPress(VK_RIGHT)) {
		m_dir = RIGHT;
			// �E�ړ�
			m_move.x -= SinDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;

			m_rotDest.y = rotCamera.y - 90.0f;
	}
	if (GetKeyTrigger(VK_UP))
	{
		if (!m_bJump)
		{
			m_move.y += JUMP_POWER;
			m_bJump = true;
		}
	}

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

	// �����蔻��
	OBJECT_INFO collision = CollisionOldMap(XMFLOAT2(m_pos.x, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y));
	if (collision.m_nCategory > 0)
	{
		if (m_bLand == true && collision.m_bOnBox == true)
			m_pos.y = g_oldBoyPos.y;
		else if (m_bLand == true)
			m_pos.x = g_oldBoyPos.x;
	}
	//----�n�`�Ƃ̓����蔻��----
	if (CheckField())
	{	//������ꍇ�̏���
		m_move.y = 0.0f;
		m_bJump = false;
		m_bLand = true;
	}
	else
	{
		if (m_bLand)
		{
			m_bJump = true;
			m_bLand = false;
		}
	}

	//�U���̓����蔻��
	if (GetKeyPress(VK_SPACE))
	{
		/*��*/int num = CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		GetBox()->Destroy(num);
		num = CollisionNowMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		GetBox()->Destroy(num);
	}
	// �I�u�W�F�N�g������
	if (GetKeyPress(VK_A))
	{
		if (CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory > 0)
			m_nHund = CollisionOldMap(XMFLOAT2(m_pos.x + 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;
		if (CollisionOldMap(XMFLOAT2(m_pos.x - 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory > 0)
			m_nHund = CollisionOldMap(XMFLOAT2(m_pos.x - 4.0f, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nObject;	
	}
	// �I�u�W�F�N�g�����
	if (GetKeyPress(VK_S))
	{
		m_nHund = 9999;
	}

	// ���������ꏏ�Ɉړ�
	GetBox()->SetBoxPos(m_nHund, m_move);

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
//�j�̎q�̓����蔻��
//==============================================================
bool Player_Boy::CheckField()
{
	Box* pBox = GetBox();
	OBJECT_INFO* pOldMap = GetMap(1);

	for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++, pOldMap++) {
		switch (pOldMap->m_nCategory) {
		case 0:
			break;
		case NORMAL:
			if (!pBox->GetState(pOldMap->m_nObject))
			{
				break;
			}
			XMFLOAT3 boxPos = pBox->GetPos(pOldMap->m_nObject);
			if (m_pos.x <= boxPos.x - 8.0f) continue;
			if (boxPos.x + 8.0f <= m_pos.x) continue;

			if (m_pos.y >= boxPos.y + 18.0f && g_oldBoyPos.y <= boxPos.y + 18.0f)
			{
				m_pos.y = boxPos.y + 18.0f;
				return true;
			}
			else if (m_pos.y <= boxPos.y - 5.0f && g_oldBoyPos.y >= boxPos.y - 5.0f)
			{
				m_pos.y = boxPos.y - 5.0f;
				m_move.y = 0.0f;
			}
			break;
		}
	}
}