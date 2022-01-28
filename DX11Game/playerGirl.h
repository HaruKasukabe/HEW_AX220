//=============================================================================
//
// �v���C���[���̎q [playerGirl.h]
// ����T�q
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"
#include "map.h"

class Player_Girl {
public:
	Player_Girl();
	~Player_Girl();
	void Update();
	void Draw();

	XMFLOAT3 GetGirlPos();
	XMFLOAT3 GetGirlMove();
	void SetGirlPos(XMFLOAT3);

	bool CheckField();

	void SetAnim(int);
	void CollisionSide(std::vector<OBJECT_INFO>::iterator);
private:
	CAssimpModel	m_model;	//���f��
	CAssimpModel	m_modelSub[3];	//���f��(�A�j���[�V�����p)
	XMFLOAT3		m_pos;		// ���݂̈ʒu
	XMFLOAT3		m_rot;		// ���݂̌���
	XMFLOAT3		m_rotDest;	// �ړI�̌���
	XMFLOAT3		m_move;		// �ړ���
	XMFLOAT3		m_scl;		//�I�u�W�F�N�g�̃X�P�[��
	bool			m_bLand;	// ���n�E�ڒn�t���O
	bool			m_bJump;	// �W�����v�\

	int				m_nShadow;	// �ۉe�ԍ�

	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X

	int				m_nCube;		//�����̔ԍ�

	int				m_nAnim;		// �A�j���[�V����
	int				m_nAnimNow;		// ���݂̃A�j���[�V����
	int				m_nAnimTime;	// �A�j���[�V�����̎���
};
