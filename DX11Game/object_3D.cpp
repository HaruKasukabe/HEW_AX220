//=============================================================================
//
// 3D�I�u�W�F�N�g [object_3D.cpp]
// ����T�q
//2021/11/30			�t�@�C���쐬
//=============================================================================
#include "object_3D.h"
#include "input.h"
#include "sceneSample.h"
#include "enemy.h"

//*****�萔��`*****
#define OBJECT_3D_MODEL_PATH			"data/model/hikouki_white.x"

#define	OBJECT_3D_VALUE_MOVE	(0.50f)		// �ړ����x
#define	OBJECT_3D_RATE_MOVE		(0.20f)		// �ړ������W��
#define	OBJECT_3D_VALUE_ROTATE	(9.0f)		// ��]���x
#define	OBJECT_3D_RATE_ROTATE	(0.20f)		// ��]�����W��

#define OBJECT_3D_COLLISION_SIZE_X		2.5f
#define OBJECT_3D_COLLISION_SIZE_Y		2.5f
#define OBJECT_3D_COLLISION_SIZE_Z		2.5f

#define OBJECT_3D_COLLISION_SIZE_RAD	2.5f


//*****�O���[�o���ϐ�*****


//==============================================================
//�ݽ�׸�
//==============================================================
Object_3D::Object_3D()

{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_pos = XMFLOAT3(0.0f, 20.0f, 0.0f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDest = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, OBJECT_3D_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}

	m_nCube = GetCube()->Create(XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(5.0f,5.0f,5.0f),m_mtxWorld);
}
//==============================================================
//�޽�׸�
//==============================================================
Object_3D::~Object_3D() {
	// ���f���̉��
	m_model.Release();
	//�����̉��
	GetCube()->Release(m_nCube);
}
//==============================================================
//�X�V
//==============================================================
void Object_3D::Update() {
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	if (GetKeyPress(VK_LEFT)) {
		if (GetKeyPress(VK_UP)) {
			// ���O�ړ�
			m_move.x -= SinDeg(rotCamera.y + 135.0f) * OBJECT_3D_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y + 135.0f) * OBJECT_3D_VALUE_MOVE;

			m_rotDest.y = rotCamera.y + 135.0f;
		}
		else if (GetKeyPress(VK_DOWN)) {
			// ����ړ�
			m_move.x -= SinDeg(rotCamera.y + 45.0f) * OBJECT_3D_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y + 45.0f) * OBJECT_3D_VALUE_MOVE;

			m_rotDest.y = rotCamera.y + 45.0f;
		}
		else {
			// ���ړ�
			m_move.x -= SinDeg(rotCamera.y + 90.0f) * OBJECT_3D_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y + 90.0f) * OBJECT_3D_VALUE_MOVE;

			m_rotDest.y = rotCamera.y + 90.0f;
		}
	}
	else if (GetKeyPress(VK_RIGHT)) {

		if (GetKeyPress(VK_UP)) {
			// �E�O�ړ�
			m_move.x -= SinDeg(rotCamera.y - 135.0f) * OBJECT_3D_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y - 135.0f) * OBJECT_3D_VALUE_MOVE;

			m_rotDest.y = rotCamera.y - 135.0f;
		}
		else if (GetKeyPress(VK_DOWN)) {
			// �E��ړ�
			m_move.x -= SinDeg(rotCamera.y - 45.0f) * OBJECT_3D_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y - 45.0f) * OBJECT_3D_VALUE_MOVE;

			m_rotDest.y = rotCamera.y - 45.0f;
		}
		else {
			// �E�ړ�
			m_move.x -= SinDeg(rotCamera.y - 90.0f) * OBJECT_3D_VALUE_MOVE;
			m_move.z -= CosDeg(rotCamera.y - 90.0f) * OBJECT_3D_VALUE_MOVE;

			m_rotDest.y = rotCamera.y - 90.0f;
		}
	}
	else if (GetKeyPress(VK_UP)) {

		// �O�ړ�
		m_move.x -= SinDeg(180.0f + rotCamera.y) * OBJECT_3D_VALUE_MOVE;
		m_move.z -= CosDeg(180.0f + rotCamera.y) * OBJECT_3D_VALUE_MOVE;

		m_rotDest.y = 180.0f + rotCamera.y;
	}
	else if (GetKeyPress(VK_DOWN)) {

		// ��ړ�
		m_move.x -= SinDeg(rotCamera.y) * OBJECT_3D_VALUE_MOVE;
		m_move.z -= CosDeg(rotCamera.y) * OBJECT_3D_VALUE_MOVE;

		m_rotDest.y = rotCamera.y;
	}

	if (GetKeyPress(VK_I)) {

		m_move.y += OBJECT_3D_VALUE_MOVE;
	}
	if (GetKeyPress(VK_K)) {

		m_move.y -= OBJECT_3D_VALUE_MOVE;
	}

	if (GetKeyPress(VK_LSHIFT)) {
		// ����]
		m_rotDest.y -= OBJECT_3D_VALUE_ROTATE;
		if (m_rotDest.y < -180.0f) {
			m_rotDest.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_RSHIFT)) {
		// �E��]
		m_rotDest.y += OBJECT_3D_VALUE_ROTATE;
		if (m_rotDest.y >= 180.0f) {
			m_rotDest.y -= 360.0f;
		}
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
	m_rot.y += fDiffRotY * OBJECT_3D_RATE_ROTATE;
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
	m_move.x += (0.0f - m_move.x) * OBJECT_3D_RATE_MOVE;
	m_move.y += (0.0f - m_move.y) * OBJECT_3D_RATE_MOVE;
	m_move.z += (0.0f - m_move.z) * OBJECT_3D_RATE_MOVE;

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
	if (m_pos.y < 0.0f) {
		m_pos.y = 0.0f;
	}
	if (m_pos.y > 80.0f) {
		m_pos.y = 80.0f;
	}

	if (GetKeyPress(VK_RETURN)) {
		// ���Z�b�g
		m_pos = XMFLOAT3(0.0f, 20.0f, 0.0f);
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
	GetCube()->Move(m_nCube,m_mtxWorld);

	//�����蔻��
	CollisionEnemy(m_pos, XMFLOAT3(OBJECT_3D_COLLISION_SIZE_X, OBJECT_3D_COLLISION_SIZE_Y, OBJECT_3D_COLLISION_SIZE_Z));
}
//==============================================================
//�`��
//==============================================================
void Object_3D::Draw() {
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