//=============================================================================
//
// 3D�I�u�W�F�N�g [object_3D.h]
// ����T�q
//2021/11/30			�t�@�C���쐬
//=============================================================================
#pragma once
#include "main.h"
#include "AssimpModel.h"

class Object_3D {
public:
	Object_3D();
	~Object_3D();
	void Update();
	void Draw();
private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT3		m_pos;		// ���݂̈ʒu
	XMFLOAT3		m_rot;		// ���݂̌���
	XMFLOAT3		m_rotDest;	// �ړI�̌���
	XMFLOAT3		m_move;		// �ړ���

	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X

	int				m_nCube;		//�����̔ԍ�
};
