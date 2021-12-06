//========================================================
//
//���G�t�F�N�g[fire.cpp]
//
//========================================================
#include "fire.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"
#include "input.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define TEXTURE_FILENAME	L"data/texture/effect000.jpg"	//�e�N�X�`���t�@�C����
#define M_DIFFUSE			XMFLOAT4(1.0f,0.2f,0.6f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_FIRE		100

#define FIRE_ANIM_COUNT	15			//��R�}������̕\����

//***********************************************************
//�\���̒�`
//***********************************************************
struct TFire {
	XMFLOAT3	pos;		//�ʒu
	float		size;		//�T�C�Y
	bool		use;		//�g�p���Ă��邩�ǂ���
	int			count;		//�A�j���[�V�����t���[����
	XMFLOAT4X4	world;		// ���[���h �}�g���b�N�X
};

//*************************************************************
//�O���[�o���ϐ�
//*************************************************************
static MESH		g_mesh;			//���b�V�����
static MATERIAL	g_material;		//�}�e���A��
static TFire	g_fire[MAX_FIRE];		//�������

//**************************************************************
//�v���g�^�C�v�錾
//**************************************************************
static HRESULT MakeVertexFire(ID3D11Device* pDevice);

//==============================================================
//������
//==============================================================
HRESULT InitFire(void) {
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//���_���̍쐬
	hr = MakeVertexFire(pDevice);
	if (FAILED(hr)) {
		return hr;
	}

	//�}�e���A���̐ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	//�e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr)) {
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//������񏉊���
	for (int i = 0; i < MAX_FIRE; ++i) {
		g_fire[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_fire[i].size = 10.0f;
		g_fire[i].use = false;
		g_fire[i].count = FIRE_ANIM_COUNT;
	}
	
	return hr;
}

//==============================================================
//�I������
//==============================================================
void UninitFire(void) {
	ReleaseMesh(&g_mesh);
}

//==============================================================
//�X�V
//==============================================================
void UpdateFire(void) {
	/*if (GetKeyPress(VK_F)) {
		StartFire(XMFLOAT3(0.0f, 50.0f, 0.0f), XMFLOAT2(10.0f, 10.0f));
	}*/
	TFire* pFire = g_fire;
	for (int i = 0; i < MAX_FIRE; ++i, ++pFire) {
		//���g�p�Ȃ�X�L�b�v
		if (!pFire->use) {
			continue;
		}
		//�J�E���^�X�V
		--pFire->count;
		if (pFire->count <= 0) {
			pFire->use = false;	//����
		}
	}
}

//==============================================================
//�`��
//==============================================================
void DrawFire(void) {
	
	ID3D11DeviceContext* pDC = GetDeviceContext();
	CLight::Get()->SetDisable();
	SetBlendState(BS_ADDITIVE);	// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	TFire* pFire = g_fire;
	for (int i = 0; i < MAX_FIRE; ++i, ++pFire) {
		if (!pFire->use) {
			continue;
		}
		// ���S���W���ړ�
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, XMVector3TransformCoord(
			XMLoadFloat3(&pFire->pos),
			XMLoadFloat4x4(&pFire->world)));
		// �T�C�Y�𔽉f
		g_mesh.mtxWorld._11 = g_mesh.mtxWorld._22 =
			g_mesh.mtxWorld._33 = pFire->size;
		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pos.x;
		g_mesh.mtxWorld._42 = pos.y;
		g_mesh.mtxWorld._43 = pos.z;
		//�s�����x�ݒ�
		g_material.Diffuse.w = pFire->count * 0.06f;
		// �`��
		DrawMesh(pDC, &g_mesh);
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

		

//==============================================================
//�����J�n�@���� : ���f�����W�A�T�C�Y�A���[���h�}�g���b�N�X
//==============================================================
int StartFire(XMFLOAT3 pos, float size, XMFLOAT4X4 world) {
	TFire* pFire = g_fire;
	for (int i = 0; i < MAX_FIRE; ++i, ++pFire) {
		//�g�p���Ȃ�X�L�b�v
		if (pFire->use) {
			continue;
		}
		pFire->pos = pos;
		pFire->size = size;
		pFire->count = FIRE_ANIM_COUNT;
		pFire->world = world;
		pFire->use = true;
		return i;		//��������(0�`�����ԍ��j
	}
	return -1;			//�������Ȃ�����
}

//==============================================================
//���_���̍쐬
//==============================================================
HRESULT MakeVertexFire(ID3D11Device* pDevice) {
	//�ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	//���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-0.5f,0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	//�ꎞ�I�ȃC���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];

	//�C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	//���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	//�ꎞ�I�Ȓ��_�z��E�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}