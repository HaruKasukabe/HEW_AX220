//=============================================================================
//
// �v���C���[�j�̎q [playerBoy.h]
// ����T�q
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"
#include "input.h"
#include "map.h"

#define COLLISION_SIDE_LONG	(0.0f)

class Player_Boy {
public:
	Player_Boy();
	~Player_Boy();
	void Update();
	void Draw();
	XMFLOAT3 GetBoyPos();
	XMFLOAT3 GetBoyMove();
	int GetBoyHand();

	void SetAnim(int);

	bool GetHaveFlg();
	void CollisionSide(std::vector<OBJECT_INFO>::iterator);
private:
	CAssimpModel	m_model;	//���f��
	CAssimpModel	m_modelSub[7];	//���f��(�A�j���[�V�����p)
	XMFLOAT3		m_pos;		// ���݂̈ʒu
	XMFLOAT3		m_rot;		// ���݂̌���
	XMFLOAT3		m_rotDest;	// �ړI�̌���
	XMFLOAT3		m_move;		// �ړ���
	int				m_dir;		//
	bool			m_bJump;	// �W�����v���t���O
	bool			m_bLand;	// ���n�E�ڒn�t���O
	bool            m_collision; // ���ɂ������Ă���
	bool            m_bHave;    // �A�C�e�������t���O

	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X

	int				m_nSphere;		//���E���ԍ�
	int				m_nShadow;	// �ۉe�ԍ�

	int				m_nHand;	// ������

	int				m_nAnim;		// �A�j���[�V����
	int				m_nAnimNow;		// ���݂̃A�j���[�V����
	int				m_nAnimTime;	// �A�j���[�V�����̎���

	JOYINFOEX *m_pad;
};
