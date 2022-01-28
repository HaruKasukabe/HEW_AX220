//===================================================
//		�ߋ�[old.cpp]
//����T�q
//=====================================================
#include "old.h"
#include "map.h"
#include "bsphere.h"
#include "shadow.h"


//=============================
//		�ݽ�׸�
//=============================
Old::Old() {
	// �ۉe������
	InitShadow();
	//̨���ޏ�����
	m_pMeshField = new MeshField(8, 1, 80.0f, 80.0f, 1.0f, 1.0f, XMFLOAT3(0.0f,-50.0f,0.0f));
	//�j�̎q������
	m_pPlayerBoy = new Player_Boy;
	//���E��������
	InitBSphere();
}

//=============================
//		�޽�׸�
//=============================
Old::~Old(){
	//̨���ޏI��
	delete m_pMeshField;
	//�j�̎q�I��
	delete m_pPlayerBoy;
	//���E���I��
	UninitBSphere();
	// �ۉe�I������
	UninitShadow();
}

//=============================
//		�X�V
//=============================
void Old::Update(){
	//̨���ލX�V
	m_pMeshField->Update();
	//�j�̎q�X�V
	m_pPlayerBoy->Update();
	//���E���X�V
	UpdateBSphere();
}

//=============================
//		�`��
//=============================
void Old::Draw(){
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	// �ۉe�`��
	DrawShadow();


	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	//̨���ޕ`��
	m_pMeshField->Draw();
	//�j�̎q�`��
	m_pPlayerBoy->Draw();
	//�}�b�v�`��
	DrawOldMap();
	//���E���`��
	DrawBSphere();
	// �ۉe�`��
	DrawShadow();
}


//=============================
//		�j�̎q�擾
//=============================
Player_Boy* Old::GetPlayerBoy() {
	return m_pPlayerBoy;
}

// �v���C���[�̍��W���l��
XMFLOAT3 Old::GetBoyPos()
{
	return m_pPlayerBoy->GetBoyPos();
}