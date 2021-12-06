//========================================================
//
//���G�t�F�N�g[smoke.cpp]
//
//========================================================
#include "smoke.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"
#include "input.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define TEXTURE_FILENAME	L"data/texture/smoke000.png"	//�e�N�X�`���t�@�C����
#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_SMOKE		1000

#define SMOKE_ANIM_COUNT	60			//��R�}������̕\����

//***********************************************************
//�\���̒�`
//***********************************************************
struct TSmoke {
	XMFLOAT3	pos;		//�ʒu
	XMFLOAT2	size;		//�T�C�Y
	bool		use;		//�g�p���Ă��邩�ǂ���
	int			count;		//�A�j���[�V�����t���[����
};

//*************************************************************
//�O���[�o���ϐ�
//*************************************************************
static MESH		g_mesh;			//���b�V�����
static MATERIAL	g_material;		//�}�e���A��
static TSmoke	g_smoke[MAX_SMOKE];		//���G�t�F�N�g���

//**************************************************************
//�v���g�^�C�v�錾
//**************************************************************
static HRESULT MakeVertexSmoke(ID3D11Device* pDevice);

//==============================================================
//������
//==============================================================
HRESULT InitSmoke(void) {
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//���_���̍쐬
	hr = MakeVertexSmoke(pDevice);
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
	for (int i = 0; i < MAX_SMOKE; ++i) {
		g_smoke[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_smoke[i].size = XMFLOAT2(10.0f,10.0f);
		g_smoke[i].use = false;
		g_smoke[i].count = SMOKE_ANIM_COUNT;
	}
	
	return hr;
}

//==============================================================
//�I������
//==============================================================
void UninitSmoke(void) {
	ReleaseMesh(&g_mesh);
}

//==============================================================
//�X�V
//==============================================================
void UpdateSmoke(void) {
	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		//���g�p�Ȃ�X�L�b�v
		if (!pSmoke->use) {
			continue;
		}
		//�J�E���^�X�V
		--pSmoke->count;
		if (pSmoke->count <= 0) {
			pSmoke->use = false;	//����
		}
		//�T�C�Y�X�V
		pSmoke->size.x += 0.3f;
		pSmoke->size.y += 0.1f;
		//�ʒu�X�V
		pSmoke->pos.y += 0.5f;
	}
}

//==============================================================
//�`��
//==============================================================
void DrawSmoke(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();		//��������
	SetBlendState(BS_ALPHABLEND);		//���u�����h
	SetZWrite(false);
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		//���g�p�Ȃ�X�L�b�v
		if (!pSmoke->use) {
			continue;
		}
		//�e�N�X�`���}�g���b�N�X�X�V
		//�r���[�s��̉�]�����̓]�u�s���ݒ�(+�g�k)
		g_mesh.mtxWorld._11 = mView._11 * pSmoke->size.x;
		g_mesh.mtxWorld._12 = mView._21 * pSmoke->size.x;
		g_mesh.mtxWorld._13 = mView._31 * pSmoke->size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pSmoke->size.y;
		g_mesh.mtxWorld._22 = mView._22 * pSmoke->size.y;
		g_mesh.mtxWorld._23 = mView._32 * pSmoke->size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//�ʒu�𔽉f
		g_mesh.mtxWorld._41 = pSmoke->pos.x;
		g_mesh.mtxWorld._42 = pSmoke->pos.y;
		g_mesh.mtxWorld._43 = pSmoke->pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//�s�����x�ݒ�
		g_material.Diffuse.w = 0.3f - (0.005f * (SMOKE_ANIM_COUNT - pSmoke->count));
		if (g_material.Diffuse.w < 0.0f) {
			g_material.Diffuse.w = 0.0f;
		}
		//�`��
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		//���u�����h����
	CLight::Get()->SetEnable();	//�����L��
	SetZWrite(true);
}

//==============================================================
//�J�n
//==============================================================
int StartSmoke(XMFLOAT3 pos, XMFLOAT2 size) {
	TSmoke* pSmoke = g_smoke;
	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
		//�g�p���Ȃ�X�L�b�v
		if (pSmoke->use) {
			continue;
		}
		pSmoke->pos = pos;
		pSmoke->size = size;
		pSmoke->count = SMOKE_ANIM_COUNT;
		pSmoke->use = true;
		return i;		//��������(0�`�����ԍ��j
	}
	return -1;			//�������Ȃ�����
}

//==============================================================
//���_���̍쐬
//==============================================================
HRESULT MakeVertexSmoke(ID3D11Device* pDevice) {
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