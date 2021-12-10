//=============================================================================
//
// �v���C���[���̎q [playerGirl.cpp]
// ����T�q
//=============================================================================
#include "playerGirl.h"
#include "playerBoy.h"
#include "sceneGame.h"
#include "input.h"
#include "collision.h"
#include "map.h"

//*****�萔��`*****
#define PLAYER_BOY_MODEL_PATH			"data/model/dog.x"

#define	PLAYER_BOY_VALUE_MOVE	(0.10f)		// �ړ����x
#define	PLAYER_BOY_RATE_MOVE	(0.20f)		// �ړ������W��
#define	PLAYER_BOY_VALUE_ROTATE	(9.0f)		// ��]���x
#define	PLAYER_BOY_RATE_ROTATE	(0.20f)		// ��]�����W��

#define PLAYER_BOY_COLLISION_SIZE_X		2.5f
#define PLAYER_BOY_COLLISION_SIZE_Y		2.5f
#define PLAYER_BOY_COLLISION_SIZE_Z		2.5f

#define PLAYER_BOY_COLLISION_SIZE_RAD	2.5f

#define GRAVITY	(0.4f)	// �d��

//*****�O���[�o���ϐ�*****

XMFLOAT3 g_BoyPos; // �j�̎q�̍��W

//==============================================================
//�ݽ�׸�
//==============================================================
Player_Girl::Player_Girl()

{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_pos = XMFLOAT3(-100.0f, -45.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BoyPos = XMFLOAT3(0.0f, 0.0f, 0.0f);


	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, PLAYER_BOY_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}

	//m_nCube = GetCube()->Create(XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(5.0f,5.0f,5.0f),m_mtxWorld);
}
//==============================================================
//�޽�׸�
//==============================================================
Player_Girl::~Player_Girl() {
	// ���f���̉��
	m_model.Release();
	//�����̉��
	//GetCube()->Release(m_nCube);
}
//==============================================================
//�X�V
//==============================================================
void Player_Girl::Update() {
	XMFLOAT3 oldPos = m_pos;

	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	// �j�̎q�̍��W���擾
	g_BoyPos = GetOld()->GetPlayerBoy()->GetBoyPos();
	//if (g_BoyPos.x <= m_pos.x) {

		// ���ړ�
		//m_move.x -= SinDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;
		//m_move.z -= CosDeg(rotCamera.y + 90.0f) * PLAYER_BOY_VALUE_MOVE;

		//m_rotDest.y = rotCamera.y + 90.0f;

	//}
	//else if (g_BoyPos.x > m_pos.x) {

		// �E�ړ�
		m_move.x -= SinDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;
		m_move.z -= CosDeg(rotCamera.y - 90.0f) * PLAYER_BOY_VALUE_MOVE;

		m_rotDest.y = rotCamera.y - 90.0f;
	//}

	// �d��
	m_move.y -= GRAVITY;



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
	}
	if (m_pos.y > 80.0f) {
		m_pos.y = 80.0f;
	}

	if (CollisionNowMap(XMFLOAT2(m_pos.x, m_pos.y), XMFLOAT2(PLAYER_BOY_COLLISION_SIZE_X, PLAYER_BOY_COLLISION_SIZE_Y)).m_nCategory > 0)
	{
		m_pos = oldPos;
	}


	if (GetKeyPress(VK_RETURN)) {
		// ���Z�b�g
		m_pos = XMFLOAT3(0.0f, -50.0f, 0.0f);
		m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

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

	//�����̈ړ�
	//GetCube()->Move(m_nCube,m_mtxWorld);
	/*�e�X�g*/

	//�����蔻��
}
//==============================================================
//�`��
//==============================================================
void Player_Girl::Draw() {
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
//���̎q�̈ʒu�擾
//==============================================================
XMFLOAT3 Player_Girl::GetPos()
{
	return m_pos;
}
//==============================================================
//���̎q�̈ʒu�ݒ�
//==============================================================
void Player_Girl::SetPos(XMFLOAT3 pos)
{
	m_pos = pos;
}
