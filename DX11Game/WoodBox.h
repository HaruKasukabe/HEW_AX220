//===================================================
//		�ؔ�[WoodBox.h]
//������
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"
#include "box.h"

//*****�}�N����`*****
#define MAX_WOOD_BOX		(100)

class WoodBox : public Box{
public:
	WoodBox();
	~WoodBox();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos, int nCat, int nTime);

	void Release(int num);

	bool Destroy(int num);

	TBox* GetWoodBox();
	XMFLOAT3 GetPos(int num);
	void SetWoodBoxPos(int num, XMFLOAT3 pos, XMFLOAT3 move, int time);
	void SetOldWoodBoxPos(int num);
	XMFLOAT2 GetSize();

	bool GetState(int num);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime);

	void SetGravity(int, int);

private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X
	TBox			m_woodBox[MAX_WOOD_BOX];		//���̏��
	XMFLOAT3		m_scl;		//�I�u�W�F�N�g�̃X�P�[��

	MATERIAL m_material;
};
