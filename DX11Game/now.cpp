//===================================================
//		����[now.cpp]
//����T�q
//=====================================================
#include "now.h"
#include "map.h"

//=============================
//		�ݽ�׸�
//=============================
Now::Now() {
	//̨���ޏ�����
	m_pMeshField = new MeshField(8, 1, 80.0f, 80.0f, 1.0f, 1.0f, XMFLOAT3(0.0f,-50.0f,0.0f));
	//���̎q������
	m_pPlayerGirl = new Player_Girl;
}

//=============================
//		�޽�׸�
//=============================
Now::~Now(){
	//̨���ޏI��
	delete m_pMeshField;
	//���̎q�I��
	delete m_pPlayerGirl;
}

//=============================
//		�X�V
//=============================
void Now::Update(){
	//̨���ލX�V
	m_pMeshField->Update();
	//���̎q�X�V
	m_pPlayerGirl->Update();
}

//=============================
//		�`��
//=============================
void Now::Draw(){
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);



	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	//̨���ޕ`��
	m_pMeshField->Draw();
	//���̎q�`��
	m_pPlayerGirl->Draw();
	//�}�b�v�`��
	DrawNowMap();
}