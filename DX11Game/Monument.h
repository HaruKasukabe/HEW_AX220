//=============================================================================
//
// �S�[���̐Δ� [Monument.h]
// Author : ���� �y�s
//
//=============================================================================
#pragma once
#include "main.h"
#include "mesh.h"
#include "AssimpModel.h"

//********************************************************
// �\����
//********************************************************
struct Sekihi {
	bool m_use;
	XMFLOAT3 m_pos;
	XMFLOAT4X4 m_mtxWorld;
	XMFLOAT3 m_scl;
	int m_nTime;
};

// *******�}�N����`*********
#define MAX_MONUMENT (2)

class Monument {
public:
	Monument();
	~Monument();
	void Update();
	void Draw(int nTime);
	int Create(XMFLOAT3 pos, int nTime);
	void Release(int num);

private:
	CAssimpModel m_model;
	XMFLOAT4X4 m_mtxWorld;
	Sekihi m_monument[MAX_MONUMENT];

	MATERIAL m_material;
};