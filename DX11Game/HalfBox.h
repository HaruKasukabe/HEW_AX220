//===================================================
//	�n�[�t�u���b�N[HalfBox.h]
//  ����y�s
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"
#include "box.h"
#include "DWBox.h"

//*****�}�N����`*****
#define MAX_HALFBOX		(100)

class HalfBox : public  Box {
public:
	HalfBox();
	~HalfBox();
	void Update();
	void Draw();
	void Draw(int num);

	int Create(XMFLOAT3 pos, int nCat);

	void Release(int num);

	TBox* GetBox();
	XMFLOAT3 GetPos(int num);
	XMFLOAT2 GetSize(int num);

	bool GetState(int num);
	bool GetUse(int num);

	void DrawOldNow(int nTime);
	int CreateOldNow(XMFLOAT3 pos, int nTime/*,XMFLOAT3 scl*/);

	bool CollisionHalfBox(XMFLOAT2 pos, XMFLOAT2 size);

	bool CheckHalfBox(XMFLOAT3 pos);

	void SetHalfBoxPos(int num, XMFLOAT3 pos, XMFLOAT3 move, int time);
	void SetOldHalfBoxPos(int num);
private:
	CAssimpModel	m_model;	//���f��
	XMFLOAT4X4		m_mtxWorld;		// ���[���h�}�g���b�N�X
	TBox			m_box[MAX_HALFBOX];		//�����Ȃ����̏��

	MATERIAL m_material;
};
