//=============================================================================
//
// �G�@���� [enemy.cpp]
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
// �}�N����`
//*****************************************************************************
#define MODEL_ENEMY			"data/model/hikouki_green.x"

#define	VALUE_MOVE_ENEMY	(0.40f)		// �ړ����x
#define	RATE_MOVE_ENEMY		(0.20f)		// �ړ������W��
#define	VALUE_ROTATE_ENEMY	(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY	(0.20f)		// ��]�����W��

#define MAX_ENEMY			(10)		// �G�@�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TEnemy {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X

	int			m_nShadow;	// �ۉe�ԍ�
	int			m_nBSphere;	//���E���ԍ�

	bool		m_use;		//�g�p�t���O

	int m_nCube;
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel	g_model;			// ���f��
static TEnemy		g_enemy[MAX_ENEMY];	// �G�@���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_enemy[i].m_pos = XMFLOAT3(rand() % 620 - 310.0f,
									20.0f,
									rand() % 620 - 310.0f);
		g_enemy[i].m_rot = XMFLOAT3(0.0f, rand() % 360 - 180.0f, 0.0f);
		g_enemy[i].m_rotDest = g_enemy[i].m_rot;
		g_enemy[i].m_move = XMFLOAT3(
			-SinDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(g_enemy[i].m_rot.y) * VALUE_MOVE_ENEMY);

		// �ۉe�̐���
		g_enemy[i].m_nShadow = CreateShadow(g_enemy[i].m_pos, 12.0f);
		//���E���̐���
		//g_enemy[i].m_nBSphere = CreateBSphere(XMFLOAT3(0.0f,0.0f,0.0f), 5.0f, g_enemy[i].m_mtxWorld);

		//�����̂̐���
		g_enemy[i].m_nCube = GetCube()->Create(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(5.0f, 5.0f, 5.0f), g_enemy[i].m_mtxWorld);

		g_enemy[i].m_use = true;
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		// �ۉe�̉��
		ReleaseShadow(g_enemy[i].m_nShadow);
		//���E���̉��
		ReleaseBSphere(g_enemy[i].m_nBSphere);

		//�����̂̉��
		GetCube()->Release(g_enemy[i].m_nCube);
		
		g_enemy[i].m_use = false;
	}

	// ���f���̉��
	g_model.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		// �ړ�
		g_enemy[i].m_pos.x += g_enemy[i].m_move.x;
		g_enemy[i].m_pos.y += g_enemy[i].m_move.y;
		g_enemy[i].m_pos.z += g_enemy[i].m_move.z;

		// �ǂɂԂ�����
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

		// �ړI�̊p�x�܂ł̍���
		float fDiffRotY = g_enemy[i].m_rotDest.y - g_enemy[i].m_rot.y;
		if (fDiffRotY >= 180.0f) {
			fDiffRotY -= 360.0f;
		}
		if (fDiffRotY < -180.0f) {
			fDiffRotY += 360.0f;
		}

		// �ړI�̊p�x�܂Ŋ�����������
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

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemy[i].m_rot.x),
			XMConvertToRadians(g_enemy[i].m_rot.y),
			XMConvertToRadians(g_enemy[i].m_rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
			g_enemy[i].m_pos.x,
			g_enemy[i].m_pos.y,
			g_enemy[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemy[i].m_mtxWorld, mtxWorld);

		// �ۉe�̈ړ�
		MoveShadow(g_enemy[i].m_nShadow, g_enemy[i].m_pos);
		//���E���̈ړ�
		MoveBSphere(g_enemy[i].m_nBSphere, g_enemy[i].m_mtxWorld);

		//�����̂̈ړ�
		GetCube()->Move(g_enemy[i].m_nCube,g_enemy[i].m_mtxWorld);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (!g_enemy[i].m_use) {
			continue;
		}
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
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
