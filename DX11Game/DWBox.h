//===================================================
//	�����Ȃ���[DWBox.h]
//  ����y�s
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"
#include "box.h"

//*****�}�N����`*****
#define MAX_DWBOX		(100)

class DWBox  : public  Box{
public:
	DWBox();
	~DWBox();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos, int nCat);

	void Release(int num);

	TBox* GetBox();
	XMFLOAT3 GetPos(int num);
	XMFLOAT2 GetSize(int num);

	bool GetState(int num);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime/*,XMFLOAT3 scl*/);

private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X
	TBox			m_box[MAX_DWBOX];		//�����Ȃ����̏��

	MATERIAL m_material;
};
