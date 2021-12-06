//=============================================================================
//
// �v���C���[�j�̎q [playerBoy.h]
// ����T�q
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"

class Player_Boy {
public:
	Player_Boy();
	~Player_Boy();
	void Update();
	void Draw();
	XMFLOAT3 GetBoyPos();
private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT3		m_pos;		// ���݂̈ʒu
	XMFLOAT3		m_rot;		// ���݂̌���
	XMFLOAT3		m_rotDest;	// �ړI�̌���
	XMFLOAT3		m_move;		// �ړ���
	int				m_dir;		//
	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X

	int				m_nSphere;		//���E���ԍ�
};
