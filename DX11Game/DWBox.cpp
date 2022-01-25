//===================================================
//		��[box.cpp]
//����T�q
//=====================================================
#include "DWBox.h"
#include "Texture.h"
#include "Shader.h"
#include "bsphere.h"
#include "sceneGame.h"
#include "map.h"
#include "collision.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define DWBOX_MODEL_PATH	"data/model/Quadrock.fbx"
#define DWBOX_NOW_TEXTURE_PATH	"data/texture/rock2_color.jpg"
#define DWBOX_OLD_TEXTURE_PATH	"data/texture/rock_color.jpg"

#define MAX_DWBOX_TEXTURE	(2)

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define DWBOX_COLLISION_SIZE_X	6.0f // scl��1�̂Ƃ��̐ݒ�
#define DWBOX_COLLISION_SIZE_Y	10.0f // scl��1�̂Ƃ��̐ݒ�

//*********************************************************
//�\���̒�`
//*********************************************************
const char* g_DWBoxTexture[MAX_DWBOX_TEXTURE]
{
	DWBOX_NOW_TEXTURE_PATH,
	DWBOX_OLD_TEXTURE_PATH,
};

//*********************************************************
//�O���[�o���ϐ�
//*********************************************************
MESH g_dwboxMesh;
static TAssimpMaterial g_dwBoxMaterial[MAX_DWBOX_TEXTURE];

//=============================
//		�ݽ�׸�
//=============================
DWBox::DWBox() {

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < MAX_BOX; ++i) {
		m_box[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_box[i].m_oldPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_box[i].m_state = true;
		m_box[i].m_use = false;
		m_box[i].m_scl = XMFLOAT3(1.0f, 1.0f, 1.0f);//�f�t�H���g�ݒ�
		m_box[i].m_collision = XMFLOAT2(DWBOX_COLLISION_SIZE_X, DWBOX_COLLISION_SIZE_Y);
	}

	g_dwboxMesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_dwboxMesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_dwboxMesh.pMaterial = &m_material;

	// ���f���f�[�^�̓ǂݍ���
	if (!m_model.Load(pDevice, pDeviceContext, DWBOX_MODEL_PATH)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	}
	// �e�N�X�`���̓ǂݍ���
	HRESULT hr;
	for (int i = 0; i < MAX_DWBOX_TEXTURE; i++)
	{
		hr = CreateTextureFromFile(pDevice, g_DWBoxTexture[i], &g_dwBoxMaterial[i].pTexture);
		if (FAILED(hr))
		{
			MessageBoxA(GetMainWnd(), "�e�N�X�`���ǂݍ��݃G���[", "���@�I�̃e�N�X�`��", MB_OK);
		}
	}
	m_model.SetMaterial(&g_dwBoxMaterial[0]);


	XMStoreFloat4x4(&g_dwboxMesh.mtxTexture, XMMatrixIdentity());

}

//=============================
//		�޽�׸�
//=============================
DWBox::~DWBox() {
	// ���f���̉��
	m_model.Release();
	ReleaseMesh(&g_dwboxMesh);
}
//=============================
//		�X�V
//=============================
void DWBox::Update() {
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// ���b�V���X�V
	UpdateMesh(&g_dwboxMesh);

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
void DWBox::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	for (int i = 0; i < MAX_BOX; ++i)
	{
		//���g�p�Ȃ�X�L�b�v
		if (!m_box[i].m_use) {
			continue;
		}

		// �s����������`��
		m_model.Draw(pDC, m_box[i].m_mtxWorld, eOpacityOnly);
		DrawMesh(pDC, &g_dwboxMesh);
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
void DWBox::Draw(int num) {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	//���g�p�Ȃ�`�悵�Ȃ�
	if (!m_box[num].m_use) {
		return;
	}
	//�j�󂳂�Ă�����`�悵�Ȃ�
	if (!m_box[num].m_state) {
		return;
	}

	//���Ԃɉ����ăe�N�X�`����ύX
	if (m_box[num].m_nTime == 0)
		m_model.SetMaterial(&g_dwBoxMaterial[0]);
	else
		m_model.SetMaterial(&g_dwBoxMaterial[1]);

	// �s����������`��
	m_model.Draw(pDC, m_box[num].m_mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	m_model.Draw(pDC, m_box[num].m_mtxWorld, eTransparentOnly);
	DrawMesh(pDC, &g_dwboxMesh);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//=============================
//	������ ���� : ���f�����W�A�T�C�Y�A���[���h�}�g���b�N�X
//=============================
int DWBox::Create(XMFLOAT3 pos, int nCat, int nTime) {
	TBox* pDWBox = m_box;
	for (int i = 0; i < MAX_BOX; ++i, ++pDWBox) {
		if (pDWBox->m_use) continue;
		pDWBox->m_pos = pos;
		pDWBox->m_oldPos = pos;
		pDWBox->m_state = true;
		pDWBox->m_use = true;
		pDWBox->m_nCat = nCat;
		pDWBox->m_nTime = nTime;

		return i;
	}
	return -1;
}

//=============================
//	�����	���� :�L���[�u�ԍ�
//=============================
void DWBox::Release(int num) {
	if (num < 0 || num >= MAX_BOX)
		return;
	m_box[num].m_use = false;
}

//=============================
//	���̏��@�擾
//=============================
TBox* DWBox::GetBox()
{
	return m_box;
}

//=============================
//	���@���W�擾
//=============================
XMFLOAT3 DWBox::GetPos(int num) {
	return m_box[num].m_pos;
}

////=============================
////	���@���W�ݒ�
////=============================
//void DWBox::SetDWBoxPos(int num, XMFLOAT3 pos, int time) {
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
XMFLOAT2 DWBox::GetSize(int num) {
	return m_box[num].m_collision;
}
//=============================
//	���@��Ԏ擾
//=============================
bool DWBox::GetState(int num) {
	return m_box[num].m_state;
}

//===============================================
//		���Ɖߋ��𕪂����`��(0�����A1���ߋ�)
//===============================================
void DWBox::DrawOldNow(int nTime) {
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
int DWBox::CreateOldNow(XMFLOAT3 pos, int nTime/*, XMFLOAT3 scl*/) {
	TBox* pDWBox = m_box;
	for (int i = 0; i < MAX_BOX; ++i, ++pDWBox) {
		if (pDWBox->m_use) continue;
		pDWBox->m_pos = pos;
		pDWBox->m_oldPos = pos;
		pDWBox->m_state = true;
		pDWBox->m_use = true;
		pDWBox->m_nTime = nTime;
		// pDWBox->m_scl = scl; �X�P�[���̐ݒ�
		pDWBox->m_collision = XMFLOAT2(DWBOX_COLLISION_SIZE_X*pDWBox->m_scl.x, DWBOX_COLLISION_SIZE_Y*pDWBox->m_scl.y);
		return i;
	}
	return -1;
}

//=======================================
//	�����蔻��
//=======================================
bool DWBox::Collision(XMFLOAT2 pos, XMFLOAT2 size) {
	for (int i = 0; i < MAX_DWBOX; ++i) {
		if (!m_box[i].m_use) {
			break;
		}
		if (CollisionRect(pos, size, XMFLOAT2(m_box[i].m_pos.x, m_box[i].m_pos.y), m_box[i].m_collision)) {/*�o�O�͂�����*/
			return true;
		}
	}
	return false;
}