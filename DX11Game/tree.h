//=============================================================================
//
// �؏��� [tree.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TREE				(256)	// �؍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TTree {
	bool		m_use;		// �g�p���Ă��邩�ǂ���
	XMFLOAT3	m_pos;		// �ʒu
	XMFLOAT4X4  m_mtxWorld; // ���[���h�}�g���b�N�X
	XMFLOAT3    m_scl;      // �؂̃X�P�[��
	XMFLOAT2    m_collision;// �����蔻��
	int         m_nTime;    // �ߋ���������
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Tree{
public:
	Tree();
	~Tree();
	void Update();
	void Draw();
	void Draw(int num);
	TTree* GetTree();
	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime);

private:
	CAssimpModel m_model;          // ���f��
	CAssimpModel m_sModel;
	XMFLOAT4X4   m_mtxWorld;       // ���[���h�}�g���b�N�X
	TTree        m_tree[MAX_TREE]; // �؂̏��

	MATERIAL    m_material;
	MATERIAL    m_seedMaterial;

};