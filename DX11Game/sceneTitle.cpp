//===================================================
//		�^�C�g���V�[��[sceneTitle.cpp]
//����T�q
//2021/11/30	�t�@�C���쐬
//=====================================================
#include "sceneTitle.h"
#include "fade.h"
#include "bg.h"
#include "input.h"
#include "cloud.h"
#include "title.h"

//*****�O���[�o���ϐ�*****
static BG* g_pBG;		//�w�i
Cloud* g_pCloud;		//�_
Title* g_pTitle;		//�^�C�g��

//=============================
//		������
//=============================
HRESULT InitSceneTitle() {
	HRESULT hr = MB_OK;


	// �w�i������
	g_pBG = new BG;

	// �_������
	g_pCloud = new Cloud;

	// �^�C�g��������
	g_pTitle = new Title;

	return hr;
}

//=============================
//		�I��
//=============================
void UninitSceneTitle() {
	
	// �w�i�I������
	delete g_pBG;

	// �_�I������
	delete g_pCloud;

	// �^�C�g���I������
	delete g_pTitle;
}

//=============================
//		�X�V
//=============================
void UpdateSceneTitle() {
	

	// �w�i�X�V
	g_pBG->Update(5);

	// �_�X�V
	g_pCloud->Update();

	// �^�C�g���X�V
	g_pTitle->Update();

	/*if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_SAMPLE);
	}*/
	if (GetKeyPress(VK_F2)) {
		StartFadeOut(SCENE_GAME);
	}

	if (GetKeyPress(VK_RETURN))
	{
		StartFadeOut(SCENE_MAP);

	}

	
}

//=============================
//		�`��
//=============================
void DrawSceneTitle() {
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �w�i�`��
	g_pBG->Draw();

	// �_�`��
	g_pCloud->Draw();

	// �^�C�g���`��
	g_pTitle->Draw();
}