//=============================================================================
//
// �v���C���[�j�̎q [playerBoy.h]
// ����T�q
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"
#include "input.h"

class Player_Boy {
public:
	Player_Boy();
	~Player_Boy();
	void Update();
	void Draw();
	XMFLOAT3 GetBoyPos();
	XMFLOAT3 GetBoyMove();
	int GetBoyHand();
	bool CheckField();
private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT3		m_pos;		// ���݂̈ʒu
	XMFLOAT3		m_rot;		// ���݂̌���
	XMFLOAT3		m_rotDest;	// �ړI�̌���
	XMFLOAT3		m_move;		// �ړ���
	int				m_dir;		//
	bool			m_bJump;	// �W�����v���t���O
	bool			m_bLand;	// ���n�E�ڒn�t���O
	bool            m_bHave;    // �A�C�e�������t���O

	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X

	int				m_nSphere;		//���E���ԍ�

	int				m_nHand;	// ������

	JOYINFOEX *m_pad;
};
