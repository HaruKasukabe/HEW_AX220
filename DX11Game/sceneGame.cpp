//===================================================
//		�Q�[���V�[��[sceneGame.cpp]
//����T�q
//=====================================================
#include "sceneGame.h"
#include "fade.h"
#include "Sound.h"
#include "old.h"
#include "now.h"
#include "input.h"
//#include "box.h"
#include "map.h"
#include "gimmick.h"

//*****�O���[�o���ϐ�*****
static Old* g_pOld;		//�ߋ�
static Now* g_pNow;		//����
//static Box* g_pBox;		//��
static Gimmick* g_pGimmick;	//�M�~�b�N

const float FRAME_BUFFER_W = SCREEN_WIDTH;   //�t���[���o�b�t�@�̕��B
const float FRAME_BUFFER_H = SCREEN_HEIGHT;   //�t���[���o�b�t�@�̍����B
ID3D11DeviceContext* d3dDeviceContext;   //D3D11�f�o�C�X�R���e�L�X�g�A�������ς݂Ƃ���
D3D11_VIEWPORT viewPorts[2];   //�����r���[�|�[�g�A��������f���̕`��O�ɐݒ肷��
D3D11_VIEWPORT viewPortsReset;   //�����r���[�|�[�g�A��������f���̕`��O�ɐݒ肷��

//=============================
//		������
//=============================
HRESULT InitSceneGame() {
	HRESULT hr = MB_OK;
	//�ߋ�������
	g_pOld = new Old;
	//���ݏ�����
	g_pNow = new Now;
	//�M�~�b�N������
	g_pGimmick = new Gimmick;

	//��������
	//g_pBox = new Box;
	//g_pBox->Create(XMFLOAT3(0.0f, -50.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, -30.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, -10.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, 10.0f, 0.0f));
	//g_pBox->Create(XMFLOAT3(0.0f, 30.0f, 0.0f));

	//���ꂪ����
	viewPorts[0].Width = FRAME_BUFFER_W;   //��ʂ̉��T�C�Y
	viewPorts[0].Height = FRAME_BUFFER_H / 2;   //��ʂ̏c�T�C�Y
	viewPorts[0].TopLeftX = 0;   //��ʍ����x���W
	viewPorts[0].TopLeftY = 0;   //��ʍ����y���W
	viewPorts[0].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
	viewPorts[0].MaxDepth = 1.0f;   //�[�x�l�̍ő�l

	//���ꂪ�����
	viewPorts[1].Width = FRAME_BUFFER_W;   //��ʂ̉��T�C�Y
	viewPorts[1].Height = FRAME_BUFFER_H / 2;   //��ʂ̏c�T�C�Y
	viewPorts[1].TopLeftX = 0;   //��ʍ����x���W
	viewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;   //��ʍ����y���W
	viewPorts[1].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
	viewPorts[1].MaxDepth = 1.0f;   //�[�x�l�̍ő�l

	viewPortsReset.Width = FRAME_BUFFER_W;   //��ʂ̉��T�C�Y
	viewPortsReset.Height = FRAME_BUFFER_H;   //��ʂ̏c�T�C�Y

	//�}�b�v������
	InitMap();

	CSound::Init();
	CSound::Play(BGM_000);
	return hr;
}

//=============================
//		�I��
//=============================
void UninitSceneGame() {
	//�ߋ��I��
	delete g_pOld;
	//���ݏI��
	delete g_pNow;
	//�M�~�b�N�I��
	delete g_pGimmick;

	//�}�b�v�I��
	UninitMap();

	CSound::Stop(BGM_000);
	CSound::Fin();
	
}

//=============================
//		�X�V
//=============================
void UpdateSceneGame() {
	//�ߋ��X�V
	g_pOld->Update();

	//���ݍX�V
	g_pNow->Update();

	//���X�V
	//g_pBox->Update();
	//�}�b�v�X�V
	UpdateMap();

	//�M�~�b�N�X�V
	g_pGimmick->Update(g_pOld->GetBoyPos());

	if (GetKeyPress(VK_F1)) {
		StartFadeOut(SCENE_TITLE);
	}



	CSound::Update();
}

//=============================
//		�`��
//=============================
void DrawSceneGame() {
	d3dDeviceContext = GetDeviceContext();

		//�r���[�|�[�g��ݒ�@����
		d3dDeviceContext->RSSetViewports(1, &viewPorts[0]);
		//���`��
		g_pNow->Draw();
		g_pGimmick->NowDraw();

		//�r���[�|�[�g��ݒ�@�����
		d3dDeviceContext->RSSetViewports(1, &viewPorts[1]);
		//�ߋ��`��
		g_pOld->Draw();
		g_pGimmick->OldDraw();


		//g_pBox->Draw();

	//�r���[�|�[�g�̐ݒ�����ɖ߂�
	d3dDeviceContext->RSSetViewports(1, &viewPortsReset);
}

//=============================
//		�ߋ��擾
//=============================
Old* GetOld() {
	return g_pOld;
}

//=============================
//		���ݎ擾
//=============================
Now* GetNow() {
	return g_pNow;
}




