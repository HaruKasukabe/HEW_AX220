//===================================================
//		�^�C�g���V�[��[sceneTitle.cpp]
//����T�q
//2021/11/30	�t�@�C���쐬
//=====================================================
#include "sceneTitle.h"
#include "fade.h"
#include "bg.h"
#include "input.h"
#include "UserGuide.h"
#include "title.h"
#include "cloud.h"
#include "Sound.h"

//*****�O���[�o���ϐ�*****
static BG* g_pBG;		//�w�i
Cloud* g_pCloud;		//�_
Title* g_pTitle;		//�^�C�g��
bool GetUserFlg;

//=============================
//		������
//=============================
HRESULT InitSceneTitle() {
	HRESULT hr = MB_OK;


	// �w�i������
	g_pBG = new BG;

	// �_������
	g_pCloud = new Cloud;
	GetUserFlg = false;
	// �^�C�g��������
	g_pTitle = new Title;

	//�T�E���h������
	CSound::Init();
	CSound::Play(BGM_TITLE);

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

	//�T�E���h�I��
	CSound::Stop(BGM_TITLE);
	CSound::Fin();
}

//=============================
//		�X�V
//=============================
void UpdateSceneTitle() {
	
	bool GuideFlg = GetUserGuideFlg();

	// �w�i�X�V
	g_pBG->Update(5);

	// �_�X�V
	g_pCloud->Update();

	// �^�C�g���X�V
	g_pTitle->Update();

	/*if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_SAMPLE);
	}*/
	if (!GuideFlg)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, JOYBUTTON1)) 
		{
			CSound::Play(SE_DECIDE);
			StartFadeOut(SCENE_STAGE);
		}

		if (GetKeyPress(VK_F3))// ��X������`���[�g���A���ƃQ�[���̐؂�ւ��Ɏg���iReturn�Łj
		{
			StartFadeOut(SCENE_MAP);

		}
	}
	if (GetKeyTrigger(VK_M) || GetJoyTrigger(0, JOYBUTTON3))
	{
		SetUserGuideFlg(true);
		GuideFlg = true;
	}

	CSound::Update();
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