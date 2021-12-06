//===================================================
//		������[cube.cpp]
//����T�q
//2021/12/1	�t�@�C���쐬
//2021 / 12 / 2	Create, Move, Release�֐��ǉ�
//=====================================================
#include "cube.h"
#include "Texture.h"
#include "Shader.h"

//*********************************************************
//�}�N����`
//*********************************************************
#define CUBE_TEXTURE_PATH	/*L"data/texture/explosion000.png"*/	//�e�N�X�`���t�@�C����
#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

//*********************************************************
//�O���[�o���ϐ�
//*********************************************************
static MESH		g_mesh;							//���b�V�����

//=============================
//		�ݽ�׸�
//=============================
Cube::Cube(){
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

	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	for (int i = 0; i < MAX_CUBE; i++) {
		//���[���h�}�g���b�N�X������
		XMStoreFloat4x4(&m_cube[i].m_world, XMMatrixIdentity());
		m_cube[i].m_use = false;
	}

	m_visible = true;

}

//=============================
//		�޽�׸�
//=============================
Cube::~Cube() {
	ReleaseMesh(&g_mesh);
}
//=============================
//		�X�V
//=============================
void Cube::Update() {
}

//=============================
//		�`��
//=============================
void Cube::Draw() {
	if (!m_visible) return;
	ID3D11DeviceContext* pDC = GetDeviceContext();
	TCube* pCube = m_cube;
	for (int i = 0; i < MAX_CUBE; ++i,++pCube) {
		if (!pCube->m_use) {
			continue;
		}
		// ���S���W���ړ�
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, XMVector3TransformCoord(
			XMLoadFloat3(&pCube->m_pos),
			XMLoadFloat4x4(&pCube->m_world)));
		// �T�C�Y�𔽉f
		g_mesh.mtxWorld._11 = pCube->m_size.x;
		g_mesh.mtxWorld._22 = pCube->m_size.y;
		g_mesh.mtxWorld._33 = pCube->m_size.z;
		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pos.x;
		g_mesh.mtxWorld._42 = pos.y;
		g_mesh.mtxWorld._43 = pos.z;
		// �`��
		DrawMesh(pDC, &g_mesh);
	}

}
//=============================
//	�L���[�u���� ���� : ���f�����W�A�T�C�Y�A���[���h�}�g���b�N�X
//=============================
int Cube::Create(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT4X4 world) {
	TCube* pCube = m_cube;
	for (int i = 0; i < MAX_CUBE; ++i, ++pCube) {
		if (pCube->m_use) continue;
		pCube->m_pos = pos;
		pCube->m_size = size;
		pCube->m_world = world;
		pCube->m_use = true;
		return i;
	}
	return -1;
}
//=============================
//	�L���[�u�ړ��@���� : �L���[�u�ԍ��A���[���h�}�g���b�N�X
//=============================
void Cube::Move(int nCube, XMFLOAT4X4 world) {
	if (nCube < 0 || nCube >= MAX_CUBE)
		return;
	m_cube[nCube].m_world = world;
}
//=============================
//	�L���[�u���	���� :�L���[�u�ԍ�
//=============================
void Cube::Release(int nCube) {
	if (nCube < 0 || nCube >= MAX_CUBE)
		return;
	m_cube[nCube].m_use = false;
}

//=============================
//		���_���̍쐬
//=============================
HRESULT Cube::MakeVertex(ID3D11Device* pDevice) {
	//�ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 24;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	//���_�z��̒��g�𖄂߂�  ��O
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, -0.5f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, -0.5f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//��
	pVertexWk[4].vtx = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	pVertexWk[5].vtx = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	pVertexWk[6].vtx = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	pVertexWk[7].vtx = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	pVertexWk[4].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	pVertexWk[5].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	pVertexWk[6].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	pVertexWk[7].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	//��
	pVertexWk[8].vtx = XMFLOAT3(0.5f, 0.5f, 0.5f);
	pVertexWk[9].vtx = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	pVertexWk[10].vtx = XMFLOAT3(0.5f, -0.5f, 0.5f);
	pVertexWk[11].vtx = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	pVertexWk[8].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
	pVertexWk[9].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
	pVertexWk[10].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
	pVertexWk[11].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//�E
	pVertexWk[12].vtx = XMFLOAT3(0.5f, 0.5f, -0.5f);
	pVertexWk[13].vtx = XMFLOAT3(0.5f, 0.5f, 0.5f);
	pVertexWk[14].vtx = XMFLOAT3(0.5f, -0.5f, -0.5f);
	pVertexWk[15].vtx = XMFLOAT3(0.5f, -0.5f, 0.5f);
	pVertexWk[12].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	pVertexWk[13].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	pVertexWk[14].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	pVertexWk[15].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//��
	pVertexWk[16].vtx = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	pVertexWk[17].vtx = XMFLOAT3(0.5f, 0.5f, 0.5f);
	pVertexWk[18].vtx = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	pVertexWk[19].vtx = XMFLOAT3(0.5f, 0.5f, -0.5f);
	pVertexWk[16].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[17].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[18].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertexWk[19].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//��
	pVertexWk[20].vtx = XMFLOAT3(0.5f, -0.5f, 0.5f);
	pVertexWk[21].vtx = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	pVertexWk[22].vtx = XMFLOAT3(0.5f, -0.5f, -0.5f);
	pVertexWk[23].vtx = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	pVertexWk[20].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
	pVertexWk[21].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
	pVertexWk[22].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
	pVertexWk[23].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);

	for (int i = 0; i < g_mesh.nNumVertex; ++i) {
		pVertexWk[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		switch (i % 4) {
		case 0:
			pVertexWk[i].tex = XMFLOAT2(0.0f, 0.0f);
			break;
		case 1:
			pVertexWk[i].tex = XMFLOAT2(1.0f, 0.0f);
			break;
		case 2:
			pVertexWk[i].tex = XMFLOAT2(0.0f, 1.0f);
			break;
		case 3: 
			pVertexWk[i].tex = XMFLOAT2(1.0f, 1.0f);
			break;
		}
	}
	//�ꎞ�I�ȃC���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = 24;
	int* pIndexWk = new int[24];

	//�C���f�b�N�X�z��̒��g�𖄂߂�
	for (int i = 0; i < g_mesh.nNumIndex; ++i) {
		pIndexWk[i] = i;
	}

	//���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	//�ꎞ�I�Ȓ��_�z��E�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}