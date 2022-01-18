//=============================================================================
//
// �S�[���̐Δ�[Monument.cpp]
// Author : ���� �y�s
//
//=============================================================================
#include "Monument.h"
#include "Texture.h"
#include "Shader.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define BOX_MODEL_PATH	"data/model/sekihi.fbx"
#define BOX_TEXTURE_PATH "data/texture/sekihi_color.jpg"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

//*********************************************************
//�O���[�o���ϐ�
//*********************************************************
MESH g_monumentMesh;

//==========================
// �R���X�g���N�^
//==========================
Monument::Monument(){

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_MONUMENT; ++i) {
		m_monument[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_monument[i].m_use = false;
		m_monument[i].m_scl = XMFLOAT3(1.5f, 2.0f, 1.0f);
		m_monument[i].m_nTime = -1;
	}

	g_monumentMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_monumentMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_monumentMesh.pMaterial = &m_material;

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, BOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "���j�������gModel", MB_OK);
	}
	// �e�N�X�`���̓ǂݍ���
	static TAssimpMaterial material;
	HRESULT hr = CreateTextureFromFile(pDevice, BOX_TEXTURE_PATH, &material.pTexture);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "���j�������g�̃e�N�X�`��", MB_OK);
	}
	m_model.SetMaterial(&material);


	XMStoreFloat4x4(&g_monumentMesh.mtxTexture, XMMatrixIdentity());
}

//=============================
//		�޽�׸�
//=============================
Monument::~Monument() {
	// ���f���̉��
	m_model.Release();
	ReleaseMesh(&g_monumentMesh);
}
//=============================
//		�X�V
//=============================
void Monument::Update() {
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// ���b�V���X�V
	UpdateMesh(&g_monumentMesh);

	for (int i = 0; i < MAX_MONUMENT; ++i)
	{
		//���g�p�Ȃ�X�L�b�v
		if (!m_monument[i].m_use) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_monument[i].m_scl.x, m_monument[i].m_scl.y, m_monument[i].m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_monument[i].m_pos.x, m_monument[i].m_pos.y, m_monument[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_monument[i].m_mtxWorld, mtxWorld);


	}

}

//===============================================
//		�`��(0�����A1���ߋ�)
//===============================================
void Monument::Draw(int nTime) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_MONUMENT; ++i)
	{
		if (!m_monument[i].m_use) {
			continue;
		}
		if (!m_monument[i].m_nTime == nTime)
		{
			continue;
		}
		// �s����������`��
		m_model.Draw(pDC, m_monument[i].m_mtxWorld, eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_monument[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//=======================================
//	���j�������g����(0�����A1���ߋ�)
//=======================================
int Monument::Create(XMFLOAT3 pos, int nTime) {
	Sekihi* pMonument = m_monument;
	for (int i = 0; i < MAX_MONUMENT; ++i, ++pMonument) {
		if (pMonument->m_use) continue;
		pMonument->m_pos = pos;
		pMonument->m_use = true;
		pMonument->m_nTime = nTime;
		return i;
	}
	return -1;
}

//================================
//	�ƭ��ĉ��	���� :�L���[�u�ԍ�
//================================
void Monument::Release(int num){
	if (num < 0 || num >= MAX_MONUMENT) {
		return;
	}
	m_monument[num].m_use = false;
}