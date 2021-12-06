//=============================================================================
//
// �r���{�[�h [billboard.h]
// ����T�q
//2021/12/1			�t�@�C���쐬
//=============================================================================
#pragma once
#include "main.h"
#include "mesh.h"

//***********************************************************
//�\���̒�`
//***********************************************************
struct TBillboard {
	XMFLOAT3	m_pos;			//�ʒu
	XMFLOAT2	m_size;			//�T�C�Y
	bool		m_use;			//�g�p���Ă��邩�ǂ���
	int			m_anim;			//�A�j���[�V�����Đ��g
	int			m_count;		//�A�j���[�V�����t���[����
};

//*****�萔��`*****
#define MAX_BILLBOARD		100

class Billboard {
public:
	Billboard();
	~Billboard();
	void Update();
	void Draw();
	int Start(XMFLOAT3 pos, XMFLOAT2 size);

	HRESULT MakeVertex(ID3D11Device* pDevice);
private:
	TBillboard	m_billboard[MAX_BILLBOARD];		//�r���{�[�h�̏��
	MATERIAL	m_material;						//�}�e���A��
};