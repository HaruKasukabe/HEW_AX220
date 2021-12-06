//===================================================
//		������[cube.h]
//����T�q
//2021/12/1	�t�@�C���쐬
//2021/12/2	Create,Move,Release�֐��ǉ�
//=====================================================
#pragma once
#include "main.h"
#include "mesh.h"



//*****************************************************************************
// �\����
//*****************************************************************************
struct TCube {
	bool		m_use;		// �g�p���Ă��邩�ǂ���
	XMFLOAT3	m_pos;		// ���S���W(���f�����W�n)
	XMFLOAT4X4	m_world;	// ���[���h �}�g���b�N�X
	XMFLOAT3	m_size;		//�T�C�Y
	XMFLOAT3	m_rot;		//��]
};

//*****�}�N����`*****
#define MAX_CUBE		(100)

class Cube {
public:
	Cube();
	~Cube();
	void Update();
	void Draw();

	int Create(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT4X4 world);
	void Move(int nCube, XMFLOAT4X4 world);
	void Release(int nCube);

	HRESULT MakeVertex(ID3D11Device* pDevice);
private:
	MATERIAL		m_material;			// �}�e���A��
	TCube			m_cube[MAX_CUBE];	//�L���[�u�̏��
	bool			m_visible;			// �\��ON/OFF
};
