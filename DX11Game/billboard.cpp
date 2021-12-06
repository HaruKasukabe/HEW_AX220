//=============================================================================
//
// �r���{�[�h [billboard.cpp]
// ����T�q
//2021/12/1			�t�@�C���쐬
//=============================================================================
#include "billboard.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "input.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define BILLBOARD_TEXTURE_PATH	L"data/texture/explosion000.png"	//�e�N�X�`���t�@�C����
#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define BILLBOARD_TEXTURE_SPLIT_X	8			//�e�N�X�`����������
#define BILLBOARD_TEXTURE_SPLIT_Y	1			//�e�N�X�`���������c
#define BILLBOARD_ANIM_COUNT		4			//��R�}������̕\����

//*********************************************************
//�O���[�o���ϐ�
//*********************************************************
static MESH		g_mesh;							//���b�V�����

//==============================================================
//�ݽ�׸�
//==============================================================
Billboard::Billboard() {
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	//���_���̍쐬
	hr = MakeVertex(pDevice);

	//�}�e���A���̐ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &m_material;

	//�e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, BILLBOARD_TEXTURE_PATH, &g_mesh.pTexture);

	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	//�r���{�[�h��񏉊���
	for (int i = 0; i < MAX_BILLBOARD; ++i) {
		m_billboard[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_billboard[i].m_size = XMFLOAT2(10.0f, 10.0f);
		m_billboard[i].m_use = false;
		m_billboard[i].m_anim = 0;
		m_billboard[i].m_count = BILLBOARD_ANIM_COUNT;
	}
}

//==============================================================
//�޽�׸�
//==============================================================
Billboard::~Billboard() {
	ReleaseMesh(&g_mesh);
}

//==============================================================
//�X�V
//==============================================================
void Billboard::Update() {
	if (GetKeyRelease(VK_SPACE)) {
		Start(XMFLOAT3(0.0f, 50.0f, 0.0f), XMFLOAT2(100.0f, 100.0f));
	}


	TBillboard* pBillboard = m_billboard;
	for (int i = 0; i < MAX_BILLBOARD; ++i, ++pBillboard) {
		//���g�p�Ȃ�X�L�b�v
		if (!pBillboard->m_use) {
			continue;
		}

		//�A�j���[�V����
		//�J�E���^�X�V
		--pBillboard->m_count;
		if (pBillboard->m_count <= 0) {
			//�A�j���[�V�����X�V
			++pBillboard->m_anim;
			if (pBillboard->m_anim >= BILLBOARD_TEXTURE_SPLIT_X * BILLBOARD_TEXTURE_SPLIT_Y) {
				pBillboard->m_use = false;	//����
				continue;
			}
			//�J�E���^������
			pBillboard->m_count = BILLBOARD_ANIM_COUNT;
		}

		
	}
}

//==============================================================
//�`��
//==============================================================
void Billboard::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();		//��������
	SetBlendState(BS_ALPHABLEND);		//���u�����f�B���O�L��
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	TBillboard* pBillboard = m_billboard;
	for (int i = 0; i < MAX_BILLBOARD; ++i, ++pBillboard) {
		//���g�p�Ȃ�X�L�b�v
		if (!pBillboard->m_use) {
			continue;
		}
		//�e�N�X�`���}�g���b�N�X�X�V
		XMMATRIX mtxTex = XMMatrixScaling(1.0f / BILLBOARD_TEXTURE_SPLIT_X, 1.0f / BILLBOARD_TEXTURE_SPLIT_Y, 1.0f);
		int u = pBillboard->m_anim % BILLBOARD_TEXTURE_SPLIT_X;
		int v = pBillboard->m_anim / BILLBOARD_TEXTURE_SPLIT_X;
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation((float)u / BILLBOARD_TEXTURE_SPLIT_X, (float)v / BILLBOARD_TEXTURE_SPLIT_X, 0.0f));
		XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTex);
		//�r���[�s��̉�]�����̓]�u�s���ݒ�(+�g�k)
		g_mesh.mtxWorld._11 = mView._11 * pBillboard->m_size.x;
		g_mesh.mtxWorld._12 = mView._21 * pBillboard->m_size.x;
		g_mesh.mtxWorld._13 = mView._31 * pBillboard->m_size.x;
		g_mesh.mtxWorld._14 = 0.0f;
		g_mesh.mtxWorld._21 = mView._12 * pBillboard->m_size.y;
		g_mesh.mtxWorld._22 = mView._22 * pBillboard->m_size.y;
		g_mesh.mtxWorld._23 = mView._32 * pBillboard->m_size.y;
		g_mesh.mtxWorld._24 = 0.0f;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		g_mesh.mtxWorld._34 = 0.0f;
		//�ʒu�𔽉f
		g_mesh.mtxWorld._41 = pBillboard->m_pos.x;
		g_mesh.mtxWorld._42 = pBillboard->m_pos.y;
		g_mesh.mtxWorld._43 = pBillboard->m_pos.z;
		g_mesh.mtxWorld._44 = 1.0f;
		//�`��
		DrawMesh(pDC, &g_mesh);
	}
	SetBlendState(BS_NONE);		//���u�����f�B���O����
	CLight::Get()->SetEnable();	//�����L��
}

//==============================================================
//����	���� : ���[���h���W,�T�C�Y
//==============================================================
int Billboard::Start(XMFLOAT3 pos, XMFLOAT2 size) {
	TBillboard* pBillboard = m_billboard;
	for (int i = 0; i < MAX_BILLBOARD; ++i, ++pBillboard) {
		//�g�p���Ȃ�X�L�b�v
		if (pBillboard->m_use) {
			continue;
		}
		pBillboard->m_pos = pos;
		pBillboard->m_size = size;
		pBillboard->m_anim = 0;
		pBillboard->m_count = BILLBOARD_ANIM_COUNT;
		pBillboard->m_use = true;
		return i;		//��������
	}
	return -1;			//�����ł��Ȃ�����
}

//==============================================================
//���_���̍쐬
//==============================================================
HRESULT Billboard::MakeVertex(ID3D11Device* pDevice) {
	//�ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	//���_�z��̒��g�𖄂߂�
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
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