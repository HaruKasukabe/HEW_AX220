//===================================================
//		�n�[�t�u���b�N[HalfBox.cpp]
//����T�q
//=====================================================
#include "HalfBox.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define HALFBOX_MODEL_PATH	"data/model/box001.x"
#define HALFBOX_TEXTURE_PATH "data/texture/�K�i.jpg"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define HALFBOX_COLLISION_SIZE_X	4.0f // scl��1�̂Ƃ��̐ݒ�
#define HALFBOX_COLLISION_SIZE_Y	2.0f // scl��1�̂Ƃ��̐ݒ�

//*********************************************************
//�O���[�o���ϐ�
//*********************************************************
MESH g_halfboxMesh;

//=============================
//		�ݽ�׸�
//=============================
HalfBox::HalfBox() {

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < MAX_BOX; ++i) {
		m_box[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_box[i].m_oldPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_box[i].m_state = true;
		m_box[i].m_use = false;
		m_box[i].m_scl = XMFLOAT3(1.0f, 1.0f, 1.0f);//�f�t�H���g�ݒ�
		m_box[i].m_collision = XMFLOAT2(HALFBOX_COLLISION_SIZE_X, HALFBOX_COLLISION_SIZE_Y);
	}

	g_halfboxMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_halfboxMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_halfboxMesh.pMaterial = &m_material;

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, HALFBOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}
	// �e�N�X�`���̓ǂݍ���
	static TAssimpMaterial material;
	HRESULT hr = CreateTextureFromFile(pDevice, HALFBOX_TEXTURE_PATH, &material.pTexture);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "���������I�̃e�N�X�`��", MB_OK);
	}
	m_model.SetMaterial(&material);


	XMStoreFloat4x4(&g_halfboxMesh.mtxTexture, XMMatrixIdentity());

}

//=============================
//		�޽�׸�
//=============================
HalfBox::~HalfBox() {
	// ���f���̉��
	m_model.Release();
	ReleaseMesh(&g_halfboxMesh);
}
//=============================
//		�X�V
//=============================
void HalfBox::Update() {
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// ���b�V���X�V
	UpdateMesh(&g_halfboxMesh);

	for (int i = 0; i < MAX_BOX; ++i)
	{
		//���g�p�Ȃ�X�L�b�v
		if (!m_box[i].m_use) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_box[i].m_scl.x, m_box[i].m_scl.y, m_box[i].m_scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_box[i].m_pos.x, m_box[i].m_pos.y, m_box[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_box[i].m_mtxWorld, mtxWorld);


	}

}

//=============================
//		�`��
//=============================
void HalfBox::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_BOX; ++i)
	{
		//���g�p�Ȃ�X�L�b�v
		if (!m_box[i].m_use) {
			continue;
		}

		// �s����������`��
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_halfboxMesh);
		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//=============================
//		�`��
//=============================
void HalfBox::Draw(int num) {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	//���g�p�Ȃ�`�悵�Ȃ�
	if (!m_box[num].m_use) {
		return;
	}
	//�j�󂳂�Ă�����`�悵�Ȃ�
	if (!m_box[num].m_state) {
		return;
	}
	// �s����������`��
	m_model.Draw(pDC, m_box[num].m_mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	m_model.Draw(pDC, m_box[num].m_mtxWorld, eTransparentOnly);
	DrawMesh(pDC, &g_halfboxMesh);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//=============================
//	������ ���� : ���f�����W�A�T�C�Y�A���[���h�}�g���b�N�X
//=============================
int HalfBox::Create(XMFLOAT3 pos, int nCat) {
	TBox* pHALFBox = m_box;
	for (int i = 0; i < MAX_BOX; ++i, ++pHALFBox) {
		if (pHALFBox->m_use) continue;
		pHALFBox->m_pos = pos;
		pHALFBox->m_oldPos = pos;
		pHALFBox->m_state = true;
		pHALFBox->m_use = true;
		pHALFBox->m_nCat = nCat;

		return i;
	}
	return -1;
}

//=============================
//	�����	���� :�L���[�u�ԍ�
//=============================
void HalfBox::Release(int num) {
	if (num < 0 || num >= MAX_BOX)
		return;
	m_box[num].m_use = false;
}

//=============================
//	���̏��@�擾
//=============================
TBox* HalfBox::GetBox()
{
	return m_box;
}

//=============================
//	���@���W�擾
//=============================
XMFLOAT3 HalfBox::GetPos(int num) {
	return m_box[num].m_pos;
}

////=============================
////	���@���W�ݒ�
////=============================
//void HALFBox::SetHALFBoxPos(int num, XMFLOAT3 pos, int time) {
//	XMFLOAT3 boyPos = GetOld()->GetBoyPos();
//	if (!m_box[num].m_nCat == CARRY)
//		return;
//
//	// �ߋ��p
//	if (time == 0) {
//		if (pos.x > 0.0f)
//			m_box[num].m_pos.x = boyPos.x + BOY_HUND_LONG;
//		else if (pos.x < 0.0f)
//			m_box[num].m_pos.x = boyPos.x - BOY_HUND_LONG;
//
//		if (!(boyPos.y - m_box[num].m_pos.y >= BOY_HUND_LONG || boyPos.y - m_box[num].m_pos.y <= -BOY_HUND_LONG))
//			m_box[num].m_pos.y += pos.y;
//		if (!(boyPos.z - m_box[num].m_pos.z >= BOY_HUND_LONG || boyPos.y - m_box[num].m_pos.z <= -BOY_HUND_LONG))
//			m_box[num].m_pos.z += pos.z;
//	}
//	// �����p
//	if (time == 1) {
//		if (pos.x > 0.0f)
//			m_box[num].m_oldPos.x = boyPos.x + BOY_HUND_LONG;
//		else if (pos.x < 0.0f)
//			m_box[num].m_oldPos.x = boyPos.x - BOY_HUND_LONG;
//
//		if (!(boyPos.y - m_box[num].m_oldPos.y >= BOY_HUND_LONG || boyPos.y - m_box[num].m_oldPos.y <= -BOY_HUND_LONG))
//			m_box[num].m_oldPos.y += pos.y;
//		if (!(boyPos.z - m_box[num].m_oldPos.z >= BOY_HUND_LONG || boyPos.y - m_box[num].m_oldPos.z <= -BOY_HUND_LONG))
//			m_box[num].m_oldPos.z += pos.z;
//	}
//#ifndef TAKEI_HARUTO
//	PrintDebugProc("ο�ݻ�ˮ�x:%2f,y:%2f,z:%2f\n", m_box[num].m_pos.x, m_box[num].m_pos.y, m_box[num].m_pos.z);
//	PrintDebugProc("�ݴ���ˮ�x:%2f,y:%2f,z:%2f\n",
//		m_box[num].m_oldPos.x, m_box[num].m_oldPos.y, m_box[num].m_oldPos.z);
//#endif
//}

//=============================
//	���@�T�C�Y�擾
//=============================
XMFLOAT2 HalfBox::GetSize(int num) {
	return m_box[num].m_collision;
}
//=============================
//	���@��Ԏ擾
//=============================
bool HalfBox::GetState(int num) {
	return m_box[num].m_state;
}

//===============================================
//		���Ɖߋ��𕪂����`��(0�����A1���ߋ�)
//===============================================
void HalfBox::DrawOldNow(int nTime) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_BOX; ++i)
	{
		if (!m_box[i].m_use) {
			continue;
		}
		if (!m_box[i].m_nTime == nTime)
		{
			continue;
		}
		// �s����������`��
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//=======================================
//	�L���[�u����(0�����A1���ߋ�)
//=======================================
int HalfBox::CreateOldNow(XMFLOAT3 pos, int nTime/*, XMFLOAT3 scl*/) {
	TBox* pHALFBox = m_box;
	for (int i = 0; i < MAX_BOX; ++i, ++pHALFBox) {
		if (pHALFBox->m_use) continue;
		pHALFBox->m_pos = pos;
		pHALFBox->m_oldPos = pos;
		pHALFBox->m_state = true;
		pHALFBox->m_use = true;
		pHALFBox->m_nTime = nTime;
		// pHALFBox->m_scl = scl; �X�P�[���̐ݒ�
		pHALFBox->m_collision = XMFLOAT2(HALFBOX_COLLISION_SIZE_X*pHALFBox->m_scl.x, HALFBOX_COLLISION_SIZE_Y*pHALFBox->m_scl.y);

		return i;
	}
	return -1;
}