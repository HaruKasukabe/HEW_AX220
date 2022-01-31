//========================================================
//
//���j����[effect.cpp]
//
//========================================================
#include "effect.h"
#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define TEXTURE_FILENAME	L"data/texture/mark.png"	//�e�N�X�`���t�@�C����
#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_EFFECT		50

#define EFFECT_FRAME_X	4			//���t���[����
#define EFFECT_FRAME_Y	5			//�c�t���[����
#define EFFECT_ANIM_COUNT	4			//��R�}������̕\����

//*************************************************************
//�O���[�o���ϐ�
//*************************************************************
static MESH		g_mesh;			//���b�V�����
static MATERIAL	g_material;		//�}�e���A��
static TEffect	g_effect[MAX_EFFECT];		//�������

//**************************************************************
//�v���g�^�C�v�錾
//**************************************************************
static HRESULT MakeVertexEffect(ID3D11Device* pDevice);

//==============================================================
//������
//==============================================================
Effect::Effect()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	HRESULT hr;

	//���_���̍쐬
	hr = MakeVertexEffect(pDevice);

	//�}�e���A���̐ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	//�e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
		
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	for (int i = 0; i < MAX_EFFECT; ++i) {
		g_effect[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_effect[i].size = XMFLOAT2(10.0f, 10.0f);
		g_effect[i].use = false;
		g_effect[i].anim = 0;
		g_effect[i].count = EFFECT_ANIM_COUNT;
	}



}

Effect::~Effect()
{
	ReleaseMesh(&g_mesh);

}

/*HRESULT InitEffect(void) {
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//���_���̍쐬
	hr = MakeVertexEffect(pDevice);
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
	for (int i = 0; i < MAX_EFFECT; ++i) {
		g_effect[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_effect[i].size = XMFLOAT2(10.0f,10.0f);
		g_effect[i].use = false;
		g_effect[i].anim = 0;
		g_effect[i].count = EFFECT_ANIM_COUNT;
	}
	
	return hr;
}
*/
//==============================================================
//�I������
//==============================================================
/*void UninitEffect(void) {
	ReleaseMesh(&g_mesh);
}
*/
//==============================================================
//�X�V
//==============================================================
void Effect::UpdateEffect(void) {
	TEffect* pEffect = g_effect;
	for (int i = 0; i < MAX_EFFECT; ++i, ++pEffect) {
		//���g�p�Ȃ�X�L�b�v
		if (!pEffect->use) {
			continue;
		}
		//�J�E���^�X�V
		--pEffect->count;
		if (pEffect->count <= 0) {
			//�A�j���[�V�����X�V
			++pEffect->anim;
			if (pEffect->anim >= EFFECT_FRAME_X * EFFECT_FRAME_Y) {
				pEffect->use = false;	//����
				continue;
			}
			//�J�E���^������
			pEffect->count = EFFECT_ANIM_COUNT;
		}
	}
}

//==============================================================
//�`��
//==============================================================
void Effect::DrawEffect(void) {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();		//��������
	SetBlendState(BS_ALPHABLEND);		//���u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TEffect* pEffect = g_effect;
	for (int i = 0; i < MAX_EFFECT; ++i, ++pEffect) {
		//���g�p�Ȃ�X�L�b�v
		if (!pEffect->use) {
			continue;
		}
		//�e�N�X�`���}�g���b�N�X�X�V
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / EFFECT_FRAME_X, 1.0f / EFFECT_FRAME_Y, 1.0f);
		int u = pEffect->anim % EFFECT_FRAME_X;
		int v = pEffect->anim / EFFECT_FRAME_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / EFFECT_FRAME_X, (float)v / EFFECT_FRAME_Y, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//�r���[�s��̉�]�����̓]�u�s���ݒ�(+�g�k)
		g_mesh.mtxWorld._11 = mView._11 * pEffect->size.x;
		g_mesh.mtxWorld._12 = mView._21 * pEffect->size.x;
		g_mesh.mtxWorld._13 = mView._31 * pEffect->size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pEffect->size.y;
		g_mesh.mtxWorld._22 = mView._22 * pEffect->size.y;
		g_mesh.mtxWorld._23 = mView._32 * pEffect->size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//�ʒu�𔽉f
		g_mesh.mtxWorld._41 = pEffect->pos.x;
		g_mesh.mtxWorld._42 = pEffect->pos.y;
		g_mesh.mtxWorld._43 = pEffect->pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//�`��
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		//���u�����f�B���O����
	CLight::Get()->SetEnable();	//�����L��
}

//==============================================================
//�����J�n
//==============================================================
int Effect::StartEffect(XMFLOAT3 pos, XMFLOAT2 size) {
	TEffect* pEffect = g_effect;
	for (int i = 0; i < MAX_EFFECT; ++i, ++pEffect) {
		//�g�p���Ȃ�X�L�b�v
		if (pEffect->use) {
			continue;
		}
		pEffect->pos = pos;
		pEffect->size = size;
		pEffect->anim = 0;
		pEffect->count = EFFECT_ANIM_COUNT;
		pEffect->use = true;
		return i;		//��������(0�`�����ԍ��j
	}
	return -1;			//�������Ȃ�����
}

//==============================================================
//���_���̍쐬
//==============================================================
HRESULT MakeVertexEffect(ID3D11Device* pDevice) {
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
