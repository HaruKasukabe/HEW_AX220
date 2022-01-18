//=============================================================================
//
// �؏��� [tree.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "tree.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_TREE      "data/model/Tree.fbx"
#define	TEXTURE_TREE	"data/texture/tree_color.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MODEL_SEED      "data/model/seed.fbx"
#define TEXTURE_SEED    "data/texture/seed_color.jpg"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define TREE_COLLISION_SIZE_X 4.0f
#define TREE_COLLISION_SIZE_Y 4.0f

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_treeMesh;				// ���b�V�����
static MESH             g_seedMesh;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Tree::Tree()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �؂̏�񏉊���
	for (int i = 0; i < MAX_TREE; ++i){
		m_tree[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_tree[i].m_use = false;
		m_tree[i].m_scl = XMFLOAT3(0.5f, 1.0f, 0.5f);
		m_tree[i].m_collision = XMFLOAT2(TREE_COLLISION_SIZE_X * m_tree[i].m_scl.x, TREE_COLLISION_SIZE_Y * m_tree[i].m_scl.y);
		m_tree[i].m_nTime = -1;
	}

	g_treeMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_treeMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_seedMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_seedMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_treeMesh.pMaterial = &m_material;

	m_seedMaterial.Diffuse = M_DIFFUSE;
	m_seedMaterial.Ambient = M_AMBIENT;
	m_seedMaterial.Specular = M_SPECULAR;
	m_seedMaterial.Power = M_POWER;
	m_seedMaterial.Emissive = M_EMISSIVE;
	g_treeMesh.pMaterial = &m_seedMaterial;

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, MODEL_TREE)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "�؂̃��f��", MB_OK);
	}

	// �e�N�X�`���̓ǂݍ���
	static TAssimpMaterial material;
	HRESULT hr = CreateTextureFromFile(pDevice, TEXTURE_TREE, &material.pTexture);
	if (FAILED(hr)) {
		MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "�؂̃e�N�X�`��", MB_OK);
	}
	m_model.SetMaterial(&material);

	XMStoreFloat4x4(&g_treeMesh.mtxTexture, XMMatrixIdentity());

	// ���f���f�[�^�̓ǂݍ���
	if (!m_sModel.Load(pDevice, pDeviceContext, MODEL_SEED)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "��̃��f��", MB_OK);
	}

	// �e�N�X�`���̓ǂݍ���
	static TAssimpMaterial material_Seed;
	hr = CreateTextureFromFile(pDevice, TEXTURE_SEED, &material_Seed.pTexture);
	if (FAILED(hr)) {
		MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "�؂̃e�N�X�`��", MB_OK);
	}
	m_sModel.SetMaterial(&material_Seed);

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Tree::~Tree()
{
	// ���f���̊J��
	m_model.Release();
	ReleaseMesh(&g_treeMesh);
	m_sModel.Release();
	ReleaseMesh(&g_seedMesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void Tree::Update()
{
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// ���b�V���X�V
	UpdateMesh(&g_treeMesh);
	UpdateMesh(&g_seedMesh);

	for (int i = 0; i < MAX_TREE; ++i) {
		// ���g�p�Ȃ�X�L�b�v
		if (!m_tree[i].m_use) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_tree[i].m_scl.x, m_tree[i].m_scl.y, m_tree[i].m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_tree[i].m_pos.x, m_tree[i].m_pos.y, m_tree[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_tree[i].m_mtxWorld, mtxWorld);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Tree::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_TREE; ++i) {
		// ���g�p�Ȃ�X�L�b�v
		if (!m_tree[i].m_use) {
			continue;
		}

		// �s�����ȕ������쐬
		m_model.Draw(pDC, m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_treeMesh);
		// �������ȕ������쐬
		SetBlendState(BS_ALPHABLEND); // ���u�����h�L��
		SetZWrite(false);             // Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_tree[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);              // Z�o�b�t�@�X�V
		SetBlendState(BS_NONE);       // ���u�����h����
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Tree::Draw(int num)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// ���g�p�Ȃ�X�L�b�v
	if (!m_tree[num].m_use) {
		return;
	}

	// �s�����ȕ������쐬
	m_model.Draw(pDC, m_mtxWorld, eOpacityOnly);
	DrawMesh(pDC, &g_treeMesh);
	// �������ȕ������쐬
	SetBlendState(BS_ALPHABLEND); // ���u�����h�L��
	SetZWrite(false);             // Z�o�b�t�@�X�V���Ȃ�
	m_model.Draw(pDC, m_tree[num].m_mtxWorld, eTransparentOnly);
	SetZWrite(true);              // Z�o�b�t�@�X�V
	SetBlendState(BS_NONE);       // ���u�����h����
}

// ========================
// ��  ���擾
// ========================
TTree* Tree::GetTree()
{
	return m_tree;
}

// ========================
// �� �ߋ������킯�`��
// ========================
void Tree::DrawOldNow(int n_Time)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_TREE; ++i) 
	{
		if (!m_tree[i].m_use) {
			continue;
		}
		if (m_tree[i].m_nTime != n_Time) {
			continue;
		}
		if (m_tree[i].m_nTime == 0) {
			// �s����������`��
			m_model.Draw(pDC, m_tree[i].m_mtxWorld, eOpacityOnly);

			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
			m_model.Draw(pDC, m_tree[i].m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}
		if (m_tree[i].m_nTime == 1) {
			// �s����������`��
			m_sModel.Draw(pDC, m_tree[i].m_mtxWorld, eOpacityOnly);

			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
			m_sModel.Draw(pDC, m_tree[i].m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}
	}
}

// ========================
// ��  ����(0�����A1���ߋ�)
// ========================
int Tree::CreateOldNow(XMFLOAT3 pos, int n_Time)
{
	TTree* pTree = m_tree;
	for (int i = 0; i < MAX_TREE; ++i, ++pTree)
	{
		if (pTree->m_use) continue;
		pTree->m_pos = pos;
		pTree->m_use = true;
		pTree->m_nTime = n_Time;
		if (n_Time == 1)
		{
			pTree->m_scl = XMFLOAT3(10.0f,10.0f,10.0f);
		}
		return i;
	}
	return -1;
}