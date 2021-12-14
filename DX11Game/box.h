//===================================================
//		��[box.h]
//����T�q
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"


//*****************************************************************************
// �\����
//*****************************************************************************
struct TBox {
	bool		m_use;		// �g�p���Ă��邩�ǂ���
	XMFLOAT3	m_pos;		// ���S���W(���f�����W�n)
	bool		m_state;	//���	true:�󂳂�ĂȂ�,false:�󂳂�Ă���
	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	int			m_nTime;	// �����ߋ���
};

//*****�}�N����`*****
#define MAX_BOX		(100)

class Box {
public:
	Box();
	~Box();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos);

	void Release(int num);

	bool Destroy(int num);
	void Move(int num, XMFLOAT3 pos);

	XMFLOAT3 GetPos(int num);
	XMFLOAT2 GetSize();

	bool GetState(int num);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime);

private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X
	TBox			m_box[MAX_BOX];		//���̏��
};
