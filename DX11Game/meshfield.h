//=============================================================================
//
// ���b�V���n�ʏ��� [meshfield.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "mesh.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
//void UninitMeshField(void);
//void UpdateMeshField(void);
//void DrawMeshField(void);

class MeshField {
public:
	MeshField(int nNumBlockX, int nNumBlockZ,
		float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ,XMFLOAT3 pos);
	~MeshField();
	void Update();
	void Draw();

	HRESULT MakeVertex(ID3D11Device* pDevice,
		int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ);
private:
	MATERIAL						m_material;				// �}�e���A��

};
