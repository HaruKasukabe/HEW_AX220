//===================================================
//		�ؔ�[WoodBox.cpp]
//������
//=====================================================
#include "WoodBox.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define WOOD_BOX_MODEL_PATH			"data/model/WoodBox.fbx"
#define WOOD_BOX_NOW_TEXTURE_PATH	"data/texture/�ؔ�.jpg"
#define WOOD_BOX_OLD_TEXTURE_PATH	"data/texture/�ؔ�.jpg"

#define MAX_WOOD_BOX_TEXTURE		(2)

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define WOOD_BOX_COLLISION_SIZE_X	4.0f
#define WOOD_BOX_COLLISION_SIZE_Y	10.0f
#define WOOD_BOX_GRAVITY				0.15f

#define BOY_HUND_LONG			10.0f

//*********************************************************
//�\���̒�`
//*********************************************************
const char* g_woodBoxTexture[MAX_WOOD_BOX_TEXTURE]
{
	WOOD_BOX_NOW_TEXTURE_PATH,
	WOOD_BOX_OLD_TEXTURE_PATH,
};

//*********************************************************
//�O���[�o���ϐ�
//*********************************************************
MESH g_woodBoxMesh;
static TAssimpMaterial g_woodBoxMaterial[MAX_WOOD_BOX_TEXTURE];

//=============================
//		�ݽ�׸�
//=============================
WoodBox::WoodBox(){

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < MAX_WOOD_BOX; ++i) {
		m_woodBox[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_woodBox[i].m_oldPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_woodBox[i].m_state = true;
		m_woodBox[i].m_use = false;
	}

	g_woodBoxMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_woodBoxMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scl = XMFLOAT3(9.0f, 36.0f, 6.0f);

	// �}�e���A���̏����ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_woodBoxMesh.pMaterial = &m_material;

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, WOOD_BOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}

	// �e�N�X�`���̓ǂݍ���
	HRESULT hr;
	for (int i = 0; i < MAX_WOOD_BOX_TEXTURE; i++)
	{
		hr = CreateTextureFromFile(pDevice, g_woodBoxTexture[i], &g_woodBoxMaterial[i].pTexture);
		if (FAILED(hr))
		{
			MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "�ؔ��̃e�N�X�`��", MB_OK);
		}
	}
	m_model.SetMaterial(&g_woodBoxMaterial[0]);


	XMStoreFloat4x4(&g_woodBoxMesh.mtxTexture, XMMatrixIdentity());

}

//=============================
//		�޽�׸�
//=============================
WoodBox::~WoodBox() {
	// ���f���̉��
	m_model.Release();
	ReleaseMesh(&g_woodBoxMesh);
}
//=============================
//		�X�V
//=============================
void WoodBox::Update() {
	XMMATRIX mtxWorld,mtxScl,mtxTranslate;
	// ���b�V���X�V
	UpdateMesh(&g_woodBoxMesh);

	for (int i = 0; i < MAX_WOOD_BOX; ++i) 
	{
		//���g�p�Ȃ�X�L�b�v
		if (!m_woodBox[i].m_use) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_scl.x, m_scl.y, m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_woodBox[i].m_pos.x, m_woodBox[i].m_pos.y, m_woodBox[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_woodBox[i].m_mtxWorld, mtxWorld);


	}
	
}

//=============================
//		�`��
//=============================
void WoodBox::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_WOOD_BOX; ++i)
	{
		//���g�p�Ȃ�X�L�b�v
		if (!m_woodBox[i].m_use) {
			continue;
		}

		// �s����������`��
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_woodBoxMesh);
		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//=============================
//		�`��
//=============================
void WoodBox::Draw(int num) {
	ID3D11DeviceContext* pDC = GetDeviceContext();
		//���g�p�Ȃ�`�悵�Ȃ�
		if (!m_woodBox[num].m_use) {
			return;
		}
		//�j�󂳂�Ă�����`�悵�Ȃ�
		if (!m_woodBox[num].m_state) {
			return;
		}

		//���Ԃɉ����ăe�N�X�`����ύX
		if (m_woodBox[num].m_nTime == 0)
			m_model.SetMaterial(&g_woodBoxMaterial[0]);
		else
			m_model.SetMaterial(&g_woodBoxMaterial[1]);

		// �s����������`��
		m_model.Draw(pDC, m_woodBox[num].m_mtxWorld, eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_woodBox[num].m_mtxWorld, eTransparentOnly);
		DrawMesh(pDC, &g_woodBoxMesh);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//=============================
//	������ ���� : ���f�����W�A�T�C�Y�A���[���h�}�g���b�N�X
//=============================
int WoodBox::Create(XMFLOAT3 pos, int nCat, int nTime) {
	TBox* pWoodBox = m_woodBox;
	for (int i = 0; i < MAX_WOOD_BOX; ++i, ++pWoodBox) {
		if (pWoodBox->m_use) continue;
		pWoodBox->m_pos = pos;
		pWoodBox->m_oldPos = pos;
		pWoodBox->m_state = true;
		pWoodBox->m_use = true;
		pWoodBox->m_nCat = nCat;
		pWoodBox->m_nTime = nTime;

		return i;
	}
	return -1;
}

//=============================
//	�����	���� :�L���[�u�ԍ�
//=============================
void WoodBox::Release(int num) {
	if (num < 0 || num >= MAX_WOOD_BOX)
		return;
	m_woodBox[num].m_use = false;
}

//=============================
//	���@�j��
//=============================
bool WoodBox::Destroy(int num) {
	if (num < 0 || num >= MAX_WOOD_BOX)
		return false;
	if (!m_woodBox[num].m_nCat == CARRY)
		return false;
	m_woodBox[num].m_state = false;
	return true;
}

//=============================
//	���̏��@�擾
//=============================
TBox* WoodBox::GetWoodBox()
{
	return m_woodBox;
}

//=============================
//	���@���W�擾
//=============================
XMFLOAT3 WoodBox::GetPos(int num) {
	return m_woodBox[num].m_pos;
}

//=============================
//	���@���W�ݒ�
//=============================
void WoodBox::SetWoodBoxPos(int num, XMFLOAT3 pos, XMFLOAT3 move, int time) {
	XMFLOAT3 boyPos = GetOld()->GetBoyPos();
	if (!(m_woodBox[num].m_nCat == CARRY))
		return;

	// �ߋ��p
	if (time == 0){
		if (move.x > 0.0f)
			m_woodBox[num].m_pos.x = boyPos.x + BOY_HUND_LONG;
		else if (move.x < 0.0f)
			m_woodBox[num].m_pos.x = boyPos.x - BOY_HUND_LONG;

		if (!(boyPos.y - m_woodBox[num].m_pos.y >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_pos.y <= -BOY_HUND_LONG))
			m_woodBox[num].m_pos.y += move.y;
		if (m_woodBox[num].m_pos.y > pos.y + 20.0f)
			m_woodBox[num].m_pos.y = pos.y + 20.0f;
		if (!(boyPos.z - m_woodBox[num].m_pos.z >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_pos.z <= -BOY_HUND_LONG))
			m_woodBox[num].m_pos.z += move.z;
	}
	// �����p
	if (time == 1) {
		if (move.x > 0.0f)
			m_woodBox[num].m_oldPos.x = boyPos.x + BOY_HUND_LONG;
		else if (move.x < 0.0f)
			m_woodBox[num].m_oldPos.x = boyPos.x - BOY_HUND_LONG;

		if (!(boyPos.y - m_woodBox[num].m_oldPos.y >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_oldPos.y <= -BOY_HUND_LONG))
			m_woodBox[num].m_oldPos.y += move.y;
		if (!(boyPos.z - m_woodBox[num].m_oldPos.z >= BOY_HUND_LONG || boyPos.y - m_woodBox[num].m_oldPos.z <= -BOY_HUND_LONG))
			m_woodBox[num].m_oldPos.z += move.z;
	}
#ifndef TAKEI_HARUTO
	PrintDebugProc("ο�ݻ�ˮ�x:%2f,y:%2f,z:%2f\n", m_woodBox[num].m_pos.x, m_woodBox[num].m_pos.y, m_woodBox[num].m_pos.z);
	PrintDebugProc("�ݴ���ˮ�x:%2f,y:%2f,z:%2f\n", 
		m_woodBox[num].m_oldPos.x, m_woodBox[num].m_oldPos.y, m_woodBox[num].m_oldPos.z);
#endif
}

//=============================
//	���@���W���f
//=============================
void WoodBox::SetOldWoodBoxPos(int num) {
	if (!m_woodBox[num].m_nCat == CARRY)	// ������NORMAL�ɂ��Ă���܂�(�{���Ȃ�MOVE)
		return;

	m_woodBox[num].m_pos.x = m_woodBox[num].m_oldPos.x;
	m_woodBox[num].m_pos.y = m_woodBox[num].m_oldPos.y;
	m_woodBox[num].m_pos.z = m_woodBox[num].m_oldPos.z;

}

//=============================
//	���@�T�C�Y�擾
//=============================
XMFLOAT2 WoodBox::GetSize() {
	return XMFLOAT2(WOOD_BOX_COLLISION_SIZE_X, WOOD_BOX_COLLISION_SIZE_Y);
}

//=============================
//	���@��Ԏ擾
//=============================
bool WoodBox::GetState(int num) {
	return m_woodBox[num].m_state;
}

//===============================================
//		���Ɖߋ��𕪂����`��(0�����A1���ߋ�)
//===============================================
void WoodBox::DrawOldNow(int nTime) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_WOOD_BOX; ++i)
	{
		if (!m_woodBox[i].m_use) {
			continue;
		}
		if (!m_woodBox[i].m_nTime == nTime)
		{
			continue;
		}
		// �s����������`��
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_woodBox[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//=======================================
//	�L���[�u����(0�����A1���ߋ�)
//=======================================
int WoodBox::CreateOldNow(XMFLOAT3 pos, int nTime) {
	TBox* pWoodBox = m_woodBox;
	for (int i = 0; i < MAX_WOOD_BOX; ++i, ++pWoodBox) {
		if (pWoodBox->m_use) continue;
		pWoodBox->m_pos = pos;
		pWoodBox->m_oldPos = pos;
		pWoodBox->m_state = true;
		pWoodBox->m_use = true;
		pWoodBox->m_nTime = nTime;
		return i;
	}
	return -1;
}

//=======================================
//	�d�͐ݒ�
//=======================================
void WoodBox::SetGravity(int nObject, int nPat)
{
	if(GetOld()->GetPlayerBoy()->GetBoyHand() != nObject)
		if(!(m_woodBox[nObject].m_pos.y <= -49.0f))
			m_woodBox[nObject].m_pos.y -= WOOD_BOX_GRAVITY;
}